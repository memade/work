#include "stdafx.h"
#include "UIProduct.h"

namespace client {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IExchangeNodeUI::SetName(const sk::stringa& name)
	{
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_3D12BAF115FDB3F40B374B26890EC9E6"));
		if (pCtrl && !name.empty())
		{
			pCtrl->SetText(sk::StringConvert::MBytesToWString(name).c_str());
		}
	}
	void IExchangeNodeUI::SetValue(const sk::stringa& value)
	{
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_4AF6AE7A33133513C3DD75BAD2FA3F0A"));
		if (pCtrl && !value.empty())
		{
			pCtrl->SetText(sk::StringConvert::MBytesToWString(value).c_str());
		}
	}
	void IExchangeNodeUI::SetStatus(const sk::stringa& status)
	{
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_ECFDC30D874A8D21AC67CDC702CE0B87"));
		if (pCtrl && !status.empty())
		{
			pCtrl->SetText(sk::StringConvert::MBytesToWString(status).c_str());
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IQuotesNodeUI::IQuotesNodeUI()
	{
		CDialogBuilder Builder;
		auto pCtrl = Builder.Create(_T("UIQuotesNode.xml"));
		Add(pCtrl);
	}
	IQuotesNodeUI::~IQuotesNodeUI()
	{

	}
	void IQuotesNodeUI::AppendMD(const std::vector<NewstickerMD>& mds)
	{
		sk::string line;
		for (const auto& md : mds)
		{
		}
	}
	void IQuotesNodeUI::SetMD(const NewstickerMD& md)
	{
		auto pTradeCodeOnLineUI = GetSubCtrl<CLabelUI>(_T("DEF_373F9486BA64908828569980C6B38A1D"));
		if (pTradeCodeOnLineUI)
		{
			pTradeCodeOnLineUI->SetText(md.TradeCodeOnLine);
		}

		auto pTradePriceUI = GetSubCtrl<CLabelUI>(_T("DEF_B2909BEFD531DE4376EBCBBCD2F31E1C"));
		if (pTradePriceUI)
		{
			pTradePriceUI->SetText(md.TradePrice);
		}

		auto pPreClosePriceUI = GetSubCtrl<CLabelUI>(_T("DEF_B6F518C23C4A904F4460B521D730332C"));
		if (pPreClosePriceUI)
		{
			pPreClosePriceUI->SetText(md.PreClosePrice);
		}

		auto pUpPercentUI = GetSubCtrl<CLabelUI>(_T("DEF_B9424ABE686AC1A24F41DD1DF4D13BCB"));
		if (pUpPercentUI)
		{
			pUpPercentUI->SetText(md.UpPercent);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void INewstickerNews::Push(const sk::packet& key, const sk::string& text)
	{
		m_NewsQ.push(key, text);
	}
	void INewstickerNews::SetNews(const sk::string& text)
	{
		if (!text.empty())
		{
			auto pLabelNews = new CLabelUI();
			auto format = sk::Log::Format(
				LR"(text="{}" width="{}" font="102" wordbreak="false" textcolor="#FF00008B" padding="0, 0, 0, 0")",
				text.c_str(),
				text.size() * 20
			);
			pLabelNews->ApplyAttributeList(format.c_str());
			pLabelNews->NeedParentUpdate();
		}
	}

































	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductTreeNodeUI::SetSystemID(const shared::SystemID& ID) {
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_A342DF16ECC84D60E04A1ED50608896B"));
		if (pCtrl) {
			if (ID <= 0) {
				pCtrl->SetText(_TEXT("-"));
			}
			else {
				pCtrl->SetText(sk::Log::Format(_TEXT("{:X}"), ID).c_str());
			}
		}
	}

	void IProductTreeNodeUI::SetSystemHost(const sk::stringa& host)
	{
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_963EFA69E278CD1155AC76C4EBEC9376"));
		if (pCtrl) {
			if (host.empty()) {
				pCtrl->SetText(_TEXT("-"));
			}
			else {
				pCtrl->SetText(sk::StringConvert::MBytesToTString(host).c_str());
			}
		}

	}
	void IProductTreeNodeUI::SetSystemStatus(const bool& online)
	{
		auto pCtrl = GetSubCtrl<CLabelUI>(_TEXT("DEF_E2C2A58989612AE0F43D81B979A3BE98"));
		if (pCtrl) {
			if (online) {
				pCtrl->SetText(_TEXT("在线"));
			}
			else {
				pCtrl->SetText(_TEXT("离线"));
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductSystemStatusEditUI::Set()
	{
		if (GetCache().empty())
			return;
		auto pSystemStatusInfo = (shared::SystemStatusInfo*)GetCache().data();
		CEditUI* pEditAt = nullptr;
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_0B417F26054800790AE5A03236519D8C"))) != nullptr)
		{
			pEditAt->SetText(sk::Log::Format(_TEXT("{:X}"), pSystemStatusInfo->ID).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_2954FC3B70F676D731F40933A87C1860"))) != nullptr)
		{
			pEditAt->SetText(pSystemStatusInfo->Online ? _TEXT("在线") : _TEXT("离线"));
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_44B362B9F34B5E91AE0CD57584AE7FD9"))) != nullptr)
		{
			pEditAt->SetText(sk::StringConvert::MBytesToTString(pSystemStatusInfo->Hostname).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_3AF7C210AB6C7858460B999CAC72B304"))) != nullptr)
		{
			pEditAt->SetText(sk::StringConvert::MBytesToTString(pSystemStatusInfo->SystemVersion).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_CA8561BA838E7723A03E160AECEF58E5"))) != nullptr)
		{
			pEditAt->SetText(sk::StringConvert::MBytesToTString(pSystemStatusInfo->IPLocal).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_6E9A2D4C4B9F33E1F740A6822B715116"))) != nullptr)
		{
			pEditAt->SetText(sk::StringConvert::MBytesToTString(pSystemStatusInfo->IPNetwork).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_F9F6A48DBFF7EAAF8064EDD0F934D7E0"))) != nullptr)
		{
			pEditAt->SetText(tostring(pSystemStatusInfo->iCPUUsage).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_06391591FE8238B0623D2EBDDB82C894"))) != nullptr)
		{
			pEditAt->SetText(tostring(pSystemStatusInfo->iMemory).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_852B106025541586EFA726130545503D"))) != nullptr)
		{
			pEditAt->SetText(tostring(pSystemStatusInfo->dbDiskRead).c_str());
		}
		if ((pEditAt = GetSubCtrl<CEditUI>(_TEXT("DEF_3596D8C44A046C74EFAC21951C3947A9"))) != nullptr)
		{
			pEditAt->SetText(tostring(pSystemStatusInfo->dbDiskWrite).c_str());
		}
	}
	sk::packet IProductSystemStatusEditUI::Get() const
	{
		return GetCache();
	}
	void IProductSystemStatusEditUI::Update()
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return;
		Global::SharedGet()->m_SystemStatusInfoQ.search(
			parent->GetTagUINT64(),
			[&](const auto&, auto& info)
			{
				SetCache(sk::packet((char*)&info, shared::LENSYSTEMSTATUSINFO));
				Set();
			});
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductSystemCaptureEditUI::Set()
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return;
		if (GetCache().empty())
			return;
		auto pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_8199818406345AC3B8C7D7AF03360488"));
		if (!pTargetUI)
			return;
		pTargetUI->SetCache(sk::Log::Format("{}\\{:X}.jpg", Global::SetupGet()->CapturePath(), parent->GetTagUINT64()));
		pTargetUI->SetTagUINT64(parent->GetTagUINT64());
#if 0
		pTargetUI->SetBkImage(
			sk::StringConvert::MBytesToTString(pTargetUI->GetCache()).c_str(),
			GetCache());
#endif

	}
	sk::packet IProductSystemCaptureEditUI::Get() const
	{
		return GetCache();
	}
	void IProductSystemCaptureEditUI::Update()
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return;
		Global::SharedGet()->m_SystemCaptureCacheQ.search(
			parent->GetTagUINT64(),
			[&](const auto&, auto& cache)
			{
				SetCache(cache);
				Set();
			});
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductSystemConProcessEditUI::Set()
	{
		if (GetCache().empty())
			return;

		auto process = (shared::SystemProcessStatusInfo*)GetCache().data();

		CControlUI* pTargetUI = nullptr;
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_144756FA2BAD2DA622718D474333645B"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->PID).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_1F71B08AA72F14C97A11983A7E349D75"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->ThreadCount).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_A37D57F5BE0BC1BE209BCF3405593791"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->CPUUsedRate).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_874192EF6CD4D7582A7EF4B8605C6F42"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->MemoryUsed).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_326A4F23FB64BD3D888971A1093CED33"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->VirtualMemoryUsed).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_89E45EBF5B60723CFF49A52873515C61"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->HandleCount).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_5FCE2F8631DA80AFED757D3B42C98FAD"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->IOReadBytes).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_1FD7624DC4F5776DDD52E78F2861E681"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->IOWriteBytes).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_94CA84D7EB0E55CDBF8B2AC84923D4E1"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->IOReadCount).c_str());
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_F541FE5665E8ACE411036EC579670150"));
		if (pTargetUI)
		{
			pTargetUI->SetText(tostring(process->IOWriteCount).c_str());
		}
	}
	bool IProductSystemConProcessEditUI::Get(sk::packet& out)
	{
		out.clear();
		shared::SystemConProcess ProcessCon;

		CControlUI* pTargetUI = nullptr;

		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_BE1449555DD1D35CE39BBBF373D27C1E"));
		if (pTargetUI)
		{
			auto combo = (CComboUI*)pTargetUI;
			auto set = combo->GetItemAt(combo->GetCurSel());
			ProcessCon.Guard.interval = _tcstol(set->GetUserData().GetData(), nullptr, 10);
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_5CC3F114B29A14B43F5CE2D72A226357"));
		if (pTargetUI)
		{
			auto combo = (CComboUI*)pTargetUI;
			auto set = combo->GetItemAt(combo->GetCurSel());
			ProcessCon.Check.interval = _tcstol(set->GetUserData().GetData(), nullptr, 10);
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_A6EBAE422EA5C66FEA4149407D8C0ADE"));
		if (pTargetUI)
		{
			auto combo = (CComboUI*)pTargetUI;
			auto set = combo->GetItemAt(combo->GetCurSel());
			_tcstol(set->GetUserData().GetData(), nullptr, 10) ? ProcessCon.Guard.enable = true : ProcessCon.Guard.enable = false;
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_82F857FD535836B3B4EF0CB0C8D449F6"));
		if (pTargetUI)
		{
			auto combo = (CComboUI*)pTargetUI;
			auto set = combo->GetItemAt(combo->GetCurSel());
			_tcstol(set->GetUserData().GetData(), nullptr, 10) ? ProcessCon.Check.enable = true : ProcessCon.Check.enable = false;
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_15B9503C60DBE457E73139929BE94CD5"));
		if (pTargetUI)
		{
			auto duistr = pTargetUI->GetText();
			if (duistr.IsEmpty())
				return false;
			auto path = sk::StringConvert::WStringToMBytes(duistr.GetData());
			auto name = sk::Helper::GetFileNameByFullPath(path);
			if (name.empty())
				return false;
			sk::SafeCopyA(ProcessCon.Imagename, name.c_str(), _countof(ProcessCon.Imagename));
			sk::SafeCopyA(ProcessCon.Pathname, path.c_str(), _countof(ProcessCon.Pathname));
			out.append((char*)&ProcessCon, sizeof(shared::SystemConProcess));
		}

		return true;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductSystemConEditUI::Set()
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return;
		if (GetCache().empty())
			return;

		shared::SystemID outid;
		shared::SystemConObj outconobj;
		sk::container::set<shared::SystemConProcess> outpros;
		if (shared::SystemCon::DecodeBuf(GetCache(), outid, outconobj, outpros))
		{

			CControlUI* pTargetUI = nullptr;

			pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_D7D1A4B0B567CFF26BC4CEAE73748A9E"));
			if (pTargetUI)
			{
				pTargetUI->SetTagUINT64(GetTagUINT64());
				auto pListAt = (CListUI*)pTargetUI;
				pListAt->RemoveAll();

				CDialogBuilder builder;

				outpros.iterate(
					[&](shared::SystemConProcess& process)
					{
						auto pSysProcessStatusEditCtrl = builder.Create<IProductSystemConProcessEditUI>(_TEXT("UISystemConProcess.xml"));
						pListAt->Add(pSysProcessStatusEditCtrl);
						pSysProcessStatusEditCtrl->SetTagUINT64(pTargetUI->GetTagUINT64());
						pSysProcessStatusEditCtrl->SetTagINT64(TypeToPtr(pTargetUI));
						pSysProcessStatusEditCtrl->SetUserData(sk::StringConvert::MBytesToTString(process.Imagename).c_str());

						pTargetUI = pSysProcessStatusEditCtrl->GetSubCtrl<CControlUI>(_TEXT("DEF_15B9503C60DBE457E73139929BE94CD5"));
						if (pTargetUI)
						{
							pTargetUI->SetText(sk::StringConvert::MBytesToTString(process.Pathname).c_str());
						}
						pTargetUI = pSysProcessStatusEditCtrl->GetSubCtrl<CControlUI>(_TEXT("DEF_7CE338A4E1C2372CF4D23C7041A8FFC3"));
						if (pTargetUI)
						{
							pTargetUI->SetText(sk::StringConvert::MBytesToTString(process.Imagename).c_str());
						}
						pTargetUI = pSysProcessStatusEditCtrl->GetSubCtrl<CControlUI>(_TEXT("DEF_A6EBAE422EA5C66FEA4149407D8C0ADE"));
						if (pTargetUI)
						{
							auto pCombo = (CComboUI*)pTargetUI;
							if (process.Guard.enable)
							{
								for (int i = 0; i < pCombo->GetCount(); ++i)
								{
									auto at = pCombo->GetItemAt(i);
									if (_tcstol(at->GetUserData(), nullptr, 10) == 1)
									{
										pCombo->SelectItem(i);
										break;
									}
								}
							}
							else
							{
								for (int i = 0; i < pCombo->GetCount(); ++i)
								{
									auto at = pCombo->GetItemAt(i);
									if (_tcstol(at->GetUserData(), nullptr, 10) == 0)
									{
										pCombo->SelectItem(i);
										break;
									}
								}
							}
						}
						pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_82F857FD535836B3B4EF0CB0C8D449F6"));
						if (pTargetUI)
						{
							auto pCombo = (CComboUI*)pTargetUI;
							if (process.Check.enable)
							{
								for (int i = 0; i < pCombo->GetCount(); ++i)
								{
									auto at = pCombo->GetItemAt(i);
									if (_tcstol(at->GetUserData(), nullptr, 10) == 1)
									{
										pCombo->SelectItem(i);
										break;
									}
								}
							}
							else
							{
								for (int i = 0; i < pCombo->GetCount(); ++i)
								{
									auto at = pCombo->GetItemAt(i);
									if (_tcstol(at->GetUserData(), nullptr, 10) == 0)
									{
										pCombo->SelectItem(i);
										break;
									}
								}
							}
						}
						pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_BE1449555DD1D35CE39BBBF373D27C1E"));
						if (pTargetUI)
						{
							auto pCombo = (CComboUI*)pTargetUI;
							for (int i = 0; i < pCombo->GetCount(); ++i)
							{
								auto at = pCombo->GetItemAt(i);
								if (_tcstol(at->GetUserData(), nullptr, 10) == process.Guard.interval)
								{
									pCombo->SelectItem(i);
									break;
								}
							}
						}
						pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_5CC3F114B29A14B43F5CE2D72A226357"));
						if (pTargetUI)
						{
							auto pCombo = (CComboUI*)pTargetUI;
							for (int i = 0; i < pCombo->GetCount(); ++i)
							{
								auto at = pCombo->GetItemAt(i);
								if (_tcstol(at->GetUserData(), nullptr, 10) == process.Check.interval)
								{
									pCombo->SelectItem(i);
									break;
								}
							}
						}

					});
				pListAt->GetParent()->NeedParentUpdate();
			}

			pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_B265BAC1F43541FA0481981DC1888800"));
			if (pTargetUI)
			{
				auto pCombo = (CComboUI*)pTargetUI;
				for (int i = 0; i < pCombo->GetCount(); ++i)
				{
					auto at = pCombo->GetItemAt(i);
					if (_tcstol(at->GetUserData(), nullptr, 10) == outconobj.CaptureInterval)
					{
						pCombo->SelectItem(i);
						break;
					}
				}
			}
			pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_84D0DB74C249C3D9B30DAAEE7FBEA79E"));
			if (pTargetUI)
			{
				auto pCombo = (CComboUI*)pTargetUI;
				for (int i = 0; i < pCombo->GetCount(); ++i)
				{
					auto at = pCombo->GetItemAt(i);
					if (_tcstol(at->GetUserData(), nullptr, 10) == outconobj.ProcessInfoInterval)
					{
						pCombo->SelectItem(i);
						break;
					}
				}
			}
			pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_F4FE42727DEDAF7FEE069A84C0C84C98"));
			if (pTargetUI)
			{
				auto pCombo = (CComboUI*)pTargetUI;
				for (int i = 0; i < pCombo->GetCount(); ++i)
				{
					auto at = pCombo->GetItemAt(i);
					if (_tcstol(at->GetUserData(), nullptr, 10) == outconobj.SystemInfoInterval)
					{
						pCombo->SelectItem(i);
						break;
					}
				}
			}
		}
	}
	sk::packet IProductSystemConEditUI::Get()
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return "";
		shared::SystemID ID = parent->GetTagUINT64();
		shared::SystemConObj ConObj;
		sk::container::set<shared::SystemConProcess> Process;

		CControlUI* pTargetUI = nullptr;
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_B265BAC1F43541FA0481981DC1888800"));
		if (pTargetUI)
		{
			auto at = (CComboUI*)pTargetUI;
			auto set = at->GetItemAt(at->GetCurSel());
			ConObj.CaptureInterval = _tcstol(set->GetUserData().GetData(), nullptr, 10);
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_84D0DB74C249C3D9B30DAAEE7FBEA79E"));
		if (pTargetUI)
		{
			auto at = (CComboUI*)pTargetUI;
			auto set = at->GetItemAt(at->GetCurSel());
			ConObj.ProcessInfoInterval = _tcstol(set->GetUserData().GetData(), nullptr, 10);
		}
		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_F4FE42727DEDAF7FEE069A84C0C84C98"));
		if (pTargetUI)
		{
			auto at = (CComboUI*)pTargetUI;
			auto set = at->GetItemAt(at->GetCurSel());
			ConObj.SystemInfoInterval = _tcstol(set->GetUserData().GetData(), nullptr, 10);
		}


		pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_D7D1A4B0B567CFF26BC4CEAE73748A9E"));
		if (pTargetUI)
		{
			auto list = (CListUI*)pTargetUI;
			for (int i = 0; i < list->GetCount(); ++i)
			{
				auto conv = (IProductSystemConProcessEditUI*)list->GetItemAt(i);
				sk::packet at_get;
				if (conv->Get(at_get))
				{
					auto proc = (shared::SystemConProcess*)at_get.data();
					Process.push(*proc);
				}
			}
		}

		shared::SystemCon sysCon;
		sysCon.ID(ID);
		sysCon.process = Process;
		sysCon.Obj(ConObj);
		return sysCon.BufferGet();
	}
	void IProductSystemConEditUI::Update(const sk::packet& image)
	{
		auto parent = PtrToType(IProductControlPanelEditUI, GetTagINT64());
		if (!parent)
			return;
		if (image.empty())
		{
			Global::SharedGet()->m_SystemConCacheQ.search(
				parent->GetTagUINT64(),
				[&](const auto&, auto& cache)
				{
					SetCache(cache);
					Set();
				});
		}
		else
		{
			Global::SharedGet()->m_SystemProcessStatusInfoCacheQ.search(
				parent->GetTagUINT64(),
				[&](const auto&, auto& statuss)
				{
					statuss.search(
						image,
						[&](const auto&, auto& process)
						{
							auto pTargetUI = GetSubCtrl<CControlUI>(_TEXT("DEF_D7D1A4B0B567CFF26BC4CEAE73748A9E"));
							if (pTargetUI)
							{
								auto pListAt = (CListUI*)pTargetUI;
#if 0
								auto target = (IProductSystemConProcessEditUI*)pListAt->GetItemAtByUserData(sk::StringConvert::MBytesToTString(image).c_str());
								if (target)
								{
									target->SetCache(sk::packet((char*)&process, shared::LENSYSTEMPROCESSSTATUSINFO));
									target->Set();
								}
#endif
							}
						});
				});
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductControlPanelEditUI::Init()
	{
		CDialogBuilder builder;

		{
			auto pSysStatusEditCtrl = builder.Create<IProductSystemStatusEditUI>(_TEXT("UISystemStatus.xml"));
			Add(pSysStatusEditCtrl);
			pSysStatusEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_STATUS);
			pSysStatusEditCtrl->SetTagINT64(TypeToPtr(this));
		}

		{
			auto pSysCaptureEditCtrl = builder.Create<IProductSystemCaptureEditUI>(_TEXT("UISystemCapture.xml"));
			Add(pSysCaptureEditCtrl);
			pSysCaptureEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CAPTURE);
			pSysCaptureEditCtrl->SetTagINT64(TypeToPtr(this));
		}

		{
			auto pSysProcessEditCtrl = builder.Create<IProductSystemConEditUI>(_TEXT("UISystemCon.xml"));
			Add(pSysProcessEditCtrl);
			pSysProcessEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON);
			pSysProcessEditCtrl->SetTagINT64(TypeToPtr(this));
		}

	}
	void IProductControlPanelEditUI::Clear()
	{
		RemoveAll();
	}
	void IProductControlPanelEditUI::Update(const EnProductEditUIType& AtType, std::string addFlag)
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			auto NodeAt = GetItemAt(i);
			if (NodeAt && NodeAt->GetTagUINT64() == (UINT64)AtType)
			{
				switch (AtType)
				{
				case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_STATUS:
				{
					((IProductSystemStatusEditUI*)NodeAt)->Update();
				}break;
				case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CAPTURE:
				{
					((IProductSystemCaptureEditUI*)NodeAt)->Update();
				}break;
				case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON:
				{
					((IProductSystemConEditUI*)NodeAt)->Update(addFlag);
				}break;
				}///switch

				break;
			}
		}
	}

















	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductScreenShotEditUI::Set(const sk::packet& cache) {
#if 0
		auto pBtn = GetSubCtrl<CButtonUI>(_TEXT("DEF_8199818406345AC3B8C7D7AF03360488"));
		HGLOBAL hMemBmp = GlobalAlloc(GMEM_FIXED, cache.size());
		if (hMemBmp)
		{
			memcpy((void*)hMemBmp, cache.data(), cache.size());
			IStream* pStmBmp = NULL;
			if (S_OK == ::CreateStreamOnHGlobal(hMemBmp, FALSE, &pStmBmp))
			{

			}
			if (pStmBmp)
			{
				Gdiplus::Image* pImage = Gdiplus::Image::FromStream(pStmBmp);
				if (pImage)
				{
					CLSID encoderClsid;
					if (Global::GetEncoderClsid(L"image/jpeg", &encoderClsid))
					{
						//auto path = sk::Log::Format(L"{}\\{:X}.jpg", sk::StringConvert::MBytesToTString(Global::SetupGet()->CapturePath()), id);
						auto path = sk::Log::Format(L"{}\\B0100000000.jpg", sk::StringConvert::MBytesToTString(Global::SetupGet()->CapturePath()));
						DeleteFile(path.c_str());
						auto status = pImage->Save(path.c_str(), &encoderClsid);
						pBtn->SetBkImage(path.c_str());
					}
				}
				SK_DELETE_PTR(pImage);
			}
			pStmBmp->Release();
		}
#endif
	}
	sk::packet IProductScreenShotEditUI::Get() const {
		return GetCache();
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductEditUI::Parse() {
		sk::packet pak = GetCache();
		if (pak.empty()) return;

		CDialogBuilder builder;
		auto pSysProdEditCtrl = builder.Create<IProductScreenShotEditUI>(_TEXT("UISystemCapture.xml"));
		Add(pSysProdEditCtrl);
		pSysProdEditCtrl->Set(pak);
		pSysProdEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CAPTURE);

#if 0
		shared::SystemProduct sysProd;
		sk::container::set<shared::LPProduct> lpProds;
		sk::container::set<shared::MTFeederProduct> mtFeederProds;
		sk::container::set<shared::MTGatewayProduct> mtGatewayProds;
		if (shared::ProductCon::DeconBuffer(pak, sysProd, lpProds, mtFeederProds, mtGatewayProds)) {
			return;
		}

		CDialogBuilder builder;
		auto pSysProdEditCtrl = builder.Create<IProductSystemEditUI>(_TEXT("UIProductsEditSystem.xml"));
		Add(pSysProdEditCtrl);
		pSysProdEditCtrl->Set(sysProd);
		pSysProdEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM);

		lpProds.iterate(
			[&](auto lpProd)
			{
				auto pLPProdEditCtrl = builder.Create<IProductExchangeEditUI>(_TEXT("UIProductsEditExchange.xml"));
				Add(pLPProdEditCtrl);
				pLPProdEditCtrl->Set(lpProd);
				pLPProdEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_EXCHANGE);
			});

		mtGatewayProds.iterate(
			[&](auto& gatewayProd)
			{
				auto pGatewayProdEditCtrl = builder.Create<IProductMTGatewayEditUI>(_TEXT("UIProductsEditGateway.xml"));
				Add(pGatewayProdEditCtrl);
				pGatewayProdEditCtrl->Set(gatewayProd, lpProds);
				pGatewayProdEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_MTGATEWAY);

				mtFeederProds.iterate(
					[&](auto& feederProd, bool& itbreak)
					{
						if (gatewayProd.Symbol == feederProd.Symbol)
						{
							auto pFeederProdEditCtrl = builder.Create<IProductMTFeederEditUI>(_TEXT("UIProductsEditFeeder.xml"));
							Add(pFeederProdEditCtrl);
							pFeederProdEditCtrl->Set(feederProd);
							pFeederProdEditCtrl->SetTagUINT64((__int64)EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_MTFEEDER);
							itbreak = true;
						}
					});
			});
#endif
	}
	sk::packet IProductEditUI::Packet() {
		auto nLine = GetCount();
		if (nLine <= 0) return "";
#if 0
		shared::SystemProduct sysProd;
		sk::container::set<shared::LPProduct> lpProds;
		sk::container::set<shared::MTFeederProduct> mtFeederProds;
		sk::container::set<shared::MTGatewayProduct> mtGatewayProds;

		sk::packet pak;
		for (int i = 0; i < nLine; ++i) {
			auto pTargetLine = GetItemAt(i);
			if (!pTargetLine) continue;

			switch (EnProductEditUIType(GetItemAt(i)->GetTagUINT64())) {
			case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM: {
				auto pBusinessUI = (IProductSystemEditUI*)pTargetLine;
				auto sysProdObj = pBusinessUI->Get();
				if (sysProdObj) {
					sysProd = *sysProdObj;
				}
			}break;
			case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_EXCHANGE: {
				auto pBusinessUI = (IProductExchangeEditUI*)pTargetLine;
				auto lpProdObj = pBusinessUI->Get();
				if (lpProdObj) {
					lpProds.push(*lpProdObj);
				}
			}break;
			case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_MTFEEDER: {
				auto pBusinessUI = (IProductMTFeederEditUI*)pTargetLine;
				auto feederProdObj = pBusinessUI->Get();
				if (feederProdObj) {
					mtFeederProds.push(*feederProdObj);
				}
			}break;
			case EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_MTGATEWAY: {
				auto pBusinessUI = (IProductMTGatewayEditUI*)pTargetLine;
				auto gatewayProdObj = pBusinessUI->Get();
				if (gatewayProdObj) {
					mtGatewayProds.push(*gatewayProdObj);
				}
			}break;
			}///switch
		}

		if (sysProd.ID > 0) {
			shared::ProductCon prodConObj("", sysProd);
			lpProds.iterate([&](auto& prod) {prodConObj.EnconProd(prod); });
			mtGatewayProds.iterate(
				[&](shared::MTGatewayProduct& gatewayProd)
				{
					prodConObj.EnconProd(gatewayProd);
					mtFeederProds.iterate(
						[&](shared::MTFeederProduct& feederProd, auto& itbreak)
						{
							if (gatewayProd.Symbol == feederProd.Symbol)
							{
								feederProd.SpreadAsk = gatewayProd.SpreadAsk;
								feederProd.SpreadBid = gatewayProd.SpreadBid;
								feederProd.SpreadTotal = gatewayProd.SpreadTotal;
								prodConObj.EnconProd(feederProd);
								itbreak = true;
							}
						});
				});
			prodConObj.EnconEnd();
			pak = prodConObj.BufferGet();
			Global::SharedGet()->m_SystemSymbolConQ.search(
				sysProd.ID,
				[&](const auto&, auto& con)
				{
					if (!pak.compare(con))
					{
						//pak.clear();
					}
				});
		}
#endif
		return "";
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////







#if 0

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductSystemEditUI::Set(const shared::SystemProduct& sysProd) {
		SetCache(sk::packet((char*)&sysProd, shared::LENSYSTEMPRODUCT));
		GetSubCtrl<CEditUI>(_TEXT("DEF_8199818406345AC3B8C7D7AF03360488"))->SetText(sk::Log::Format(_TEXT("{:X}"), sysProd.ID).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_E4311B543C917BD2BDFA2FF6D3657B0E"))->SetText(sk::StringConvert::MBytesToTString(sysProd.Symbol.symbol).c_str());
	}
	std::shared_ptr<shared::SystemProduct> IProductSystemEditUI::Get() {
		auto cache = GetCache();
		auto pCache = (shared::SystemProduct*)cache.data();
		auto result = std::make_shared<shared::SystemProduct>(*pCache);
		return result;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductExchangeEditUI::Set(const shared::LPProduct& lpProd) {
		SetCache(sk::packet((char*)&lpProd, shared::LENLPPRODUCT));
		//!@ ID
		GetSubCtrl<CEditUI>(_TEXT("DEF_2B8769552A23837B2A80ABD17E6830A9"))->SetText(sk::Log::Format(_TEXT("{:X}"), lpProd.ID).c_str());
		//!@ 符号
		GetSubCtrl<CEditUI>(_TEXT("DEF_782FE71EADA80A6608D43FD6F5748E8E"))->SetText(sk::StringConvert::MBytesToTString(lpProd.Symbol.symbol).c_str());
		//!@ 本币
		GetSubCtrl<CEditUI>(_TEXT("DEF_F013433B32A84094D59D8182D9E8BAA0"))->SetText(sk::StringConvert::MBytesToTString(lpProd.QuotedCCY.currency).c_str());
		//!@ 合约大小
		GetSubCtrl<CEditUI>(_TEXT("DEF_F5DCB6944B77D5EF66CCA564C61441C8"))->SetText(sk::Log::Format(_TEXT("{}"), lpProd.ContractSize).c_str());
		//!@ 最小报价单位
		GetSubCtrl<CEditUI>(_TEXT("DEF_B75AAB7E5503C7B5718EE7CEB9711A0E"))->SetText(sk::Helper::FloatToStr(lpProd.TickSize, 6).c_str());
		//!@ 最小交易量
		GetSubCtrl<CEditUI>(_TEXT("DEF_E724368463111A712EF0BC8A28EA0068"))->SetText(sk::Helper::FloatToStr(lpProd.MinOrderSize, 6).c_str());
		//!@ 最大交易量
		GetSubCtrl<CEditUI>(_TEXT("DEF_A45A97E91EBCF634F3F22B1BA09AD978"))->SetText(sk::Helper::FloatToStr(lpProd.MaxOrderSize, 6).c_str());
		//!@ 交易量步长
		GetSubCtrl<CEditUI>(_TEXT("DEF_BBD2423C9D525B438688CB19AC078D62"))->SetText(sk::Helper::FloatToStr(lpProd.StepOrderSize, 6).c_str());
	}
	std::shared_ptr<shared::LPProduct> IProductExchangeEditUI::Get() {
		auto cache = GetCache();
		auto pCache = (shared::LPProduct*)cache.data();

		auto result = std::make_shared<shared::LPProduct>(*pCache);
		//!@ 合约大小
		result->ContractSize = std::stoi(GetSubCtrl<CEditUI>(_TEXT("DEF_F5DCB6944B77D5EF66CCA564C61441C8"))->GetText().GetData());
		//!@ 最小报价单位
		result->TickSize = std::stod(GetSubCtrl<CEditUI>(_TEXT("DEF_B75AAB7E5503C7B5718EE7CEB9711A0E"))->GetText().GetData());
		//!@ 最小交易量
		result->MinOrderSize = std::stod(GetSubCtrl<CEditUI>(_TEXT("DEF_E724368463111A712EF0BC8A28EA0068"))->GetText().GetData());
		//!@ 最大交易量
		result->MaxOrderSize = std::stod(GetSubCtrl<CEditUI>(_TEXT("DEF_A45A97E91EBCF634F3F22B1BA09AD978"))->GetText().GetData());
		//!@ 交易量步长
		result->StepOrderSize = std::stod(GetSubCtrl<CEditUI>(_TEXT("DEF_BBD2423C9D525B438688CB19AC078D62"))->GetText().GetData());
		return result;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductMTGatewayEditUI::Set(const shared::MTGatewayProduct& gatewayProds, sk::container::set<shared::LPProduct>& lpProds) {
		SetCache(sk::packet((char*)&gatewayProds, shared::LENMTGATEWAYPRODUCT));
		GetSubCtrl<CEditUI>(_TEXT("DEF_3E839BF7789BF0E6DFA24342111D0612"))->SetText(sk::Log::Format(_TEXT("{:X}"), gatewayProds.ID).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_B45EA3435AEEC80E041D2976445EF230"))->SetText(sk::StringConvert::MBytesToTString(gatewayProds.Symbol.symbol).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_8927CF51F002E0F94A944EF9763C6535"))->SetText(sk::Log::Format(_TEXT("{}"), gatewayProds.ContractSize).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->SetText(sk::Log::Format(_TEXT("{}"), gatewayProds.SpreadBid).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->SetText(sk::Log::Format(_TEXT("{}"), gatewayProds.SpreadAsk).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_DD0152AC9693302127869DFDB845EACD"))->SetText(sk::Log::Format(_TEXT("{}"), gatewayProds.SpreadTotal).c_str());
		GetSubCtrl<CEditUI>(_TEXT("DEF_F63588E183AF8A06F66118F7C1ECE253"))->SetText(sk::Log::Format(_TEXT("{}"), gatewayProds.AutoAdjustPositionSpreadRange).c_str());
		GetSubCtrl<COptionUI>(_TEXT("DEF_7F0D2E6D60A447C8B02AEC043D83BE10"))->SetGroup(tostring(gatewayProds.ID).c_str());
		GetSubCtrl<COptionUI>(_TEXT("DEF_11FD062045877D22EA88791731E5BD1E"))->SetGroup(tostring(gatewayProds.ID).c_str());
		if (gatewayProds.EnableTrade)
		{
			GetSubCtrl<COptionUI>(_TEXT("DEF_7F0D2E6D60A447C8B02AEC043D83BE10"))->Selected(true);
		}
		else
		{
			GetSubCtrl<COptionUI>(_TEXT("DEF_11FD062045877D22EA88791731E5BD1E"))->Selected(true);
		}

		auto pCombo = GetSubCtrl<CComboUI>(_TEXT("DEF_B2738C788AA1D1A64BB8E79CD7F60892"));
		if (pCombo) {
			lpProds.iterate(
				[&](auto& lpProd)
				{
					auto elem = new CListLabelElementUI();
					pCombo->Add(elem);
					elem->SetTagUINT64(shared::SystemIDType::ExchangeType(lpProd.ID));
					elem->SetText(sk::Log::Format(_TEXT("{:X}"), shared::SystemIDType::ExchangeType(lpProd.ID)).c_str());
				});
			for (int i = 0; i < pCombo->GetCount(); ++i)
			{
				if (pCombo->GetItemAt(i)->GetTagUINT64() == gatewayProds.AssignOrderLP)
				{
					pCombo->SelectItem(i);
					break;
				}
			}
		}
	}
	std::shared_ptr<shared::MTGatewayProduct> IProductMTGatewayEditUI::Get() {
		auto cache = GetCache();
		auto pCache = (shared::MTGatewayProduct*)cache.data();
		auto result = std::make_shared<shared::MTGatewayProduct>(*pCache);
		result->Symbol = sk::StringConvert::WStringToMBytes(GetSubCtrl<CEditUI>(_TEXT("DEF_B45EA3435AEEC80E041D2976445EF230"))->GetText().GetData()).c_str();
		result->SpreadBid = _ttoi(GetSubCtrl<CEditUI>(_TEXT("DEF_2BA0957EF0EA97174AB51EB0CEE81AA9"))->GetText().GetData());
		result->SpreadAsk = _ttoi(GetSubCtrl<CEditUI>(_TEXT("DEF_E7CD4A879AC2AF83E8BCC972FA9440DC"))->GetText().GetData());
		result->SpreadTotal = _ttoi(GetSubCtrl<CEditUI>(_TEXT("DEF_DD0152AC9693302127869DFDB845EACD"))->GetText().GetData());
		result->AutoAdjustPositionSpreadRange = _ttoi(GetSubCtrl<CEditUI>(_TEXT("DEF_F63588E183AF8A06F66118F7C1ECE253"))->GetText().GetData());
		if (GetSubCtrl<COptionUI>(_TEXT("DEF_7F0D2E6D60A447C8B02AEC043D83BE10"))->IsSelected()) {
			result->EnableTrade = true;
		}
		else {
			result->EnableTrade = false;
		}

		auto pCombo = GetSubCtrl<CComboUI>(_TEXT("DEF_B2738C788AA1D1A64BB8E79CD7F60892"));
		if (pCombo && pCombo->GetCount() > 0 && pCombo->GetCurSel() >= 0) {
			result->AssignOrderLP = pCombo->GetItemAt(pCombo->GetCurSel())->GetTagUINT64();
		}
		return result;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void IProductMTFeederEditUI::Set(const shared::MTFeederProduct& feederProds) {
		SetCache(sk::packet((char*)&feederProds, shared::LENMTFEEDERPRODUCT));
		GetSubCtrl<COptionUI>(_TEXT("DEF_ECACA9AE7F4420437A5DDBCD7CE73E00"))->SetGroup(tostring(feederProds.ID).c_str());
		GetSubCtrl<COptionUI>(_TEXT("DEF_634D8ADBFC97567E581EBC9FB889EBFE"))->SetGroup(tostring(feederProds.ID).c_str());
		if (feederProds.EnableQuote)
		{
			GetSubCtrl<COptionUI>(_TEXT("DEF_ECACA9AE7F4420437A5DDBCD7CE73E00"))->Selected(true);
		}
		else
		{
			GetSubCtrl<COptionUI>(_TEXT("DEF_634D8ADBFC97567E581EBC9FB889EBFE"))->Selected(true);
		}
	}
	std::shared_ptr<shared::MTFeederProduct> IProductMTFeederEditUI::Get() {
		auto cache = GetCache();
		auto pCache = (shared::MTFeederProduct*)cache.data();
		auto result = std::make_shared<shared::MTFeederProduct>(*pCache);
		if (GetSubCtrl<COptionUI>(_TEXT("DEF_ECACA9AE7F4420437A5DDBCD7CE73E00"))->IsSelected()) {
			result->EnableQuote = true;
		}
		else {
			result->EnableQuote = false;
		}
		return result;
	}

	IBridgeTradeRecordEditUI::IBridgeTradeRecordEditUI()
	{

	}
	IBridgeTradeRecordEditUI::~IBridgeTradeRecordEditUI()
	{

	}
	void IBridgeTradeRecordEditUI::Set(const shared::BridgeTradeRecord& record) {
		auto sk = 0;
	}
	std::shared_ptr<shared::BridgeTradeRecord> IBridgeTradeRecordEditUI::Get() {
		auto cache = GetCache();
		auto pCache = (shared::BridgeTradeRecord*)cache.data();
		auto result = std::make_shared<shared::BridgeTradeRecord>(*pCache);
		return result;
	}
#endif

}///namespace client