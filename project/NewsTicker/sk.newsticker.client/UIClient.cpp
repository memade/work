#include "stdafx.h"
#include "UIClient.h"
#include "UIProduct.h"

namespace client {

	UIClient::UIClient()
	{
		if (Init()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
	}

	UIClient::~UIClient()
	{
		UnInit();
	}

	int UIClient::Init()
	{
		m_pWndZoomIn = new PopupCaptureZoomIn(extern_global_hinstance, false);
		sk::intptr out = 0;
		Global::GetApi()->GetFunction<shared::tf_api_network_tcp_client_setup>(shared::api_network, shared::api_network_tcp_client_setup)(
			Global::SetupGet()->ServerAddr().c_str(),
			out);
		NetApi<sk::network::INetworkApi>(out);
		return out > 0 ? 0 : -1;
	}

	int UIClient::UnInit()
	{	
		SK_DELETE_PTR(m_pWndZoomIn);
		NetApi<sk::network::INetworkApi>()->Release();
		return 0;
	}

	int UIClient::Open()
	{
		if (m_IsOpen.load()) return 0;

		NetApi<sk::network::INetworkApi>()->Start(this);


		m_IsOpenT.store(true);
		m_Threads.emplace_back([this]() {ProcessUrlT(); });
		m_Threads.emplace_back([this]() {ProcessMDT(); });
		m_Threads.emplace_back([this]() {ProcessNewsT(); });








		m_Timer.Open();
		m_Timer.TimerAppend(500, this, OnTimer);
		m_Timer.TimerAppend(1000, this, OnTimer);
		m_IsOpen.store(true);
		return 0;
	}

	int UIClient::Close()
	{
		if (!m_IsOpen.load()) return 0;

		m_Timer.Close();

		m_IsOpenT.store(false);
		for (auto it = m_Threads.begin(); it != m_Threads.end(); ++it)
		{
			it->join();
		}
		m_Threads.clear();	
		m_pWndZoomIn->Close();

		NetApi<sk::network::INetworkApi>()->Stop();
		m_IsOpen.store(false);
		return 0;
	}

	void UIClient::ProcessMDT()
	{
		time_t time_update = 0;
		const time_t updateHz = 5000;

		const size_t move_step = 1;
		size_t move_total = 0;

		auto pHL = GetCtrl<CHorizontalLayoutUI>(_T("DEF_AA69119FF89905D7630875F7C8FE60D6"));
		if (!pHL)
			return;

		do {

			time_t time_now = sk::Helper::TickCountGet<std::chrono::milliseconds>();

			//! ±¨¼Û
			auto pJsonMapMD = Global::SharedGet()->m_NewstickerMDQ.pop();
			if (pJsonMapMD && !pJsonMapMD->empty())
			{
				NewstickerMD md;
				pJsonMapMD->iterate(
					[&](const auto& key, auto& val)
					{
						if (key == JsonKeyMD_Date)
						{
							sk::SafeCopyW(md.Date, sk::StringConvert::MBytesToWString(val).c_str(), _countof(md.Date));
						}
						else if (key == JsonKeyMD_TradeCodeOnLine)
						{
							sk::SafeCopyW(md.TradeCodeOnLine, sk::StringConvert::MBytesToWString(val).c_str(), _countof(md.TradeCodeOnLine));
						}
						else if (key == JsonKeyMD_TradePrice)
						{
							sk::SafeCopyW(md.TradePrice, sk::StringConvert::MBytesToWString(val).c_str(), _countof(md.TradePrice));
						}
						else if (key == JsonKeyMD_UpPercent)
						{
							sk::SafeCopyW(md.UpPercent, sk::StringConvert::MBytesToWString(val).c_str(), _countof(md.UpPercent));
						}
						else if (key == JsonKeyMD_PreClosePrice)
						{
							sk::SafeCopyW(md.PreClosePrice, sk::StringConvert::MBytesToWString(val).c_str(), _countof(md.PreClosePrice));
						}
					});

				if (md.TradeCodeOnLine[0])
				{
					Global::SharedGet()->m_NewstickerMDCacheQ.push(md.TradeCodeOnLine, md);
				}
			}



			if (time_update == 0 || time_now - time_update >= updateHz)
			{
				IQuotesNodeUI* pUI = new IQuotesNodeUI;
				sk::string mds;
				Global::SharedGet()->m_NewstickerMDCacheQ.iterate_clear(
					[&](const auto&, NewstickerMD& _md, auto& itbreak, auto& itclear)
					{
						auto comp = _tcstod(_md.TradePrice, nullptr) - _tcstod(_md.PreClosePrice, nullptr);
						sk::string arrow;
						if (!comp)
						{
							arrow = L"<c #BEBEBE><b>¡ö</b></c>";
						}
						else if (comp > 0)
						{
							arrow = L"<c #FF0000><b>¡ø</b></c>";
						}
						else
						{
							arrow = L"<c #00FF00><b>¨‹</b></c>";
						}

						mds.
							append(_md.TradeCodeOnLine).append(L"  ").
							append(_md.TradePrice).append(arrow).append(L"  ").
							append(sk::Log::Format(L"{:.2f}", comp)).append(L"  ").
							append(_md.UpPercent).append(L"%").append(L"          ");
						itclear = true;
					});

				if (!mds.empty() && !pHL->GetCount())
				{
					//pHL->RemoveAll();

					auto HL_width = pHL->GetWidth();
					auto HL_height = pHL->GetHeight();
					move_total = HL_width + mds.size() * 15;
					auto pLabelNews = new CLabelUI();
					auto format = sk::Log::Format(
						LR"(showhtml="true" float="true" pos="{},{},{},{}" font="103" wordbreak="false" textcolor="#FFFFFFFF" padding="0, 0, 0, 0")",
						HL_width,
						0,
						move_total,
						HL_height
					);
					pLabelNews->ApplyAttributeList(format.c_str());
					pLabelNews->SetText(mds.c_str());
					pHL->Add(pLabelNews);
					pHL->NeedParentUpdate();
				}

				time_update = time_now;
			}



			if (pHL->GetCount())
			{
				auto label = (CLabelUI*)pHL->GetItemAt(0);

				auto pos = label->GetRelativePos();
				auto alsdkfj = label->GetClientPos();
				auto klasdjflkadsf = label->GetX();
				auto alsdkfladsf = label->GetFixedXY();
				auto laksdjflaksdfjasdkjf = label->GetFixedWidth();

				SIZE moveto = label->GetFixedXY();
				moveto.cx -= move_step;
				label->SetFixedXY(moveto);
				label->NeedParentUpdate();

				move_total -= move_step;

				if (move_total <= 0)
				{
					pHL->RemoveAll();
				}
			}


			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (!m_IsOpenT.load())
				break;
		} while (1);
	}
	void UIClient::ProcessNewsT()
	{
		time_t time_update = 0;
		const time_t updateHz = 5000;

		const size_t move_step = 1;
		size_t move_total = 0;

		auto pHL = GetCtrl<CHorizontalLayoutUI>(_T("DEF_C9D79D14CA66FAEA074806985064DF2A"));
		if (!pHL)
			return;

		do {
			time_t time_now = sk::Helper::TickCountGet<std::chrono::milliseconds>();

			//! url
			auto pJsonMapNews = Global::SharedGet()->m_NewstickerNewsQ.pop();
			if (pJsonMapNews && !pJsonMapNews->empty())
			{
				pJsonMapNews->begin(
					[&](const auto&, auto& url)
					{
						Global::SharedGet()->m_NewstickerNewsCacheQ.push(url, sk::StringConvert::MBytesToWString(url));
					});
			}

			if (time_update == 0 || time_now - time_update >= updateHz)
			{
				sk::string news;
				Global::SharedGet()->m_NewstickerNewsCacheQ.iterate_clear(
					[&](const auto&, auto& _news, auto& itbreak, auto& itclear)
					{
						news.append(_news).append(L"");
						/*itclear = true;*/
					});

				if (!news.empty() && !pHL->GetCount())
				{
					//pHL->RemoveAll();

					auto HL_width = pHL->GetWidth();
					auto HL_height = pHL->GetHeight();
					move_total = HL_width + news.size() * 18;
					auto pLabelNews = new CTextUI();
					auto format = sk::Log::Format(
						LR"(float="true" pos="{},{},{},{}" font="102" textcolor="#FF00008B" padding="0, 0, 0, 0")",
						HL_width,
						HL_height/3,
						move_total,
						HL_height/3 *2 +2
					);
					pLabelNews->ApplyAttributeList(format.c_str());
					pLabelNews->SetText(news.c_str());
					pHL->Add(pLabelNews);
					pHL->NeedParentUpdate();
				}

				time_update = time_now;
			}



			if (pHL->GetCount())
			{
				auto label = (CLabelUI*)pHL->GetItemAt(0);

				auto pos = label->GetRelativePos();
				auto alsdkfj = label->GetClientPos();
				auto klasdjflkadsf = label->GetX();
				auto alsdkfladsf = label->GetFixedXY();
				auto laksdjflaksdfjasdkjf = label->GetFixedWidth();

				SIZE moveto = label->GetFixedXY();
				moveto.cx -= move_step;
				label->SetFixedXY(moveto);
				label->NeedParentUpdate();

				move_total -= move_step;

				if (move_total <= 0)
				{
					pHL->RemoveAll();
				}
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (!m_IsOpenT.load())
				break;
		} while (1);
	}

	void UIClient::ProcessUrlT()
	{
		time_t time_update = 0;
		const time_t updateHz = 20000;

		do {
			time_t time_now = sk::Helper::TickCountGet<std::chrono::milliseconds>();

			//! url
			auto pJsonMapUrl = Global::SharedGet()->m_NewstickerUrlQ.pop();
			if (pJsonMapUrl && !pJsonMapUrl->empty())
			{
				pJsonMapUrl->begin(
					[&](const auto&, auto& url)
					{
						Global::SharedGet()->m_NewstickerUrlCacheQ.push(url, sk::StringConvert::MBytesToWString(url));
					});
			}

			if (time_update == 0 || time_now - time_update >= updateHz)
			{
				if (!Global::SharedGet()->m_NewstickerUrlCacheQ.empty())
				{
					auto temp = Global::SharedGet()->m_NewstickerUrlCacheQ.pop();
					OnUIFx168NewStickerUrl(std::get<1>(*temp));
				}

				time_update = time_now;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (!m_IsOpenT.load())
				break;
		} while (1);
	}




	int UIClient::OnTimer(const sk::int64& _interval, const sk::intptr& _core_ptr, const sk::int64& _cur_time) {
		auto pClient = reinterpret_cast<UIClient*>(_core_ptr);

		if (_interval == 500)
		{
			pClient->AdjustWebUIPos();
		}

		if (_interval == 1000)
		{
			
		}

		return 0;
	}
}///namespace client