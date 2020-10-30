#include "stdafx.h"
#include "UIClient.h"
#include "UIProduct.h"
#include "UIWebBrowser.h"

namespace client {

	CControlUI* UIClient::CreateControl(LPCTSTR pstrClassName)
	{
		if (!sk::SafeCmpW(_T("CefWebBrowserUI"), pstrClassName, _tcslen(_T("CefWebBrowserUI"))))
		{
			//return new CefWebBrowserUI();
		}
		return nullptr;
	}

	LRESULT UIClient::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		UINT iKey = (UINT)wParam;
		if (iKey == VK_RETURN)
		{
#if 0
			if (StatusLogin())
			{
				auto pEditSearch = GetCtrl<CEditUI>(_TEXT("DEF_5B0F87A37DD07AC785551865A46C6158"));
				if (pEditSearch) {
					pEditSearch->Activate();
					pEditSearch->SetFocus();
				}
			}
#endif
			auto sk = 0;
		}

		bHandled = true;
		return 0;
	}

	void UIClient::AdjustWebUIPos()
	{
		if (!m_pWebBrowserUI)
			return;
		if (m_IsOpen.load())
		{
			auto pTitle = GetCtrl<CHorizontalLayoutUI>(_T("DEF_89D7FF20A3522FA886B4CF9C256035DB"));
			if (m_pWebUIParent && pTitle)
			{
				RECT rtWindow;
				GetWindowRect(GetHWND(), &rtWindow);
				RECT rt = m_pWebUIParent->GetRelativePos();
				SetWindowPos(
					m_pWebBrowserUI->GetHWND(),
					HWND_TOP,
					rtWindow.left + rt.left,
					rtWindow.top + rt.top + pTitle->GetHeight(),
					m_pWebUIParent->GetWidth(),
					m_pWebUIParent->GetHeight(),
					SWP_NOACTIVATE);
			}
		}
	}

	LRESULT UIClient::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg) 
		{
		case WM_HOTKEY:
		{
			auto idHotKey = (int)wParam; // identifier of hot key
			auto fuModifiers = (UINT)LOWORD(lParam); // key-modifier flags
			auto uVirtKey = (UINT)HIWORD(lParam); // virtual-key code

			if (uVirtKey == 'h' || uVirtKey == 'H')
			{
				if (IsWindowVisible(m_hWnd))
				{
					::ShowWindow(m_hWnd, SW_HIDE);

					if (m_pWebBrowserUI)
					{
						if (IsWindowVisible(m_pWebBrowserUI->GetHWND()))
						{
							m_pWebBrowserUI->ShowWindow(false);
						}
					}
				}
				else
				{
					::ShowWindow(m_hWnd, SW_SHOW);
					//::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREDRAW);		

					if (m_pWebBrowserUI)
					{			
						if (!IsWindowVisible(m_pWebBrowserUI->GetHWND()))
						{
							m_pWebBrowserUI->ShowWindow(true);
						}			
					}
				}
			}
			else if (uVirtKey == 'c' || uVirtKey == 'C')
			{
				OnUIClose();
			}
			else if (uVirtKey == 'l' || uVirtKey == 'L')
			{
				OnUIWebMoveToLeft();
			}
			else if (uVirtKey == 'r' || uVirtKey == 'R')
			{
				OnUIWebMoveToRight();
			}
			else if (uVirtKey == 'j' || uVirtKey == 'J')
			{
				if (m_pWebBrowserUI)
				{
					if (IsWindowVisible(m_pWebBrowserUI->GetHWND()))
					{
						m_pWebBrowserUI->ShowWindow(false);
					}
					else
					{
						m_pWebBrowserUI->ShowWindow(true);
					}
				}

			}
		}break;
		}///switch

		bHandled = FALSE;
		return 0UL;
	}

	LRESULT UIClient::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		auto mminfo = (MINMAXINFO*)lParam;
		mminfo->ptMaxPosition.x = 0;
		mminfo->ptMaxPosition.y = 0;
		auto width = ::GetSystemMetrics(SM_CXSCREEN);
		auto hegth = ::GetSystemMetrics(SM_CYSCREEN);

		APPBARDATA ad;
		ZeroMemory(&ad, sizeof APPBARDATA);
		ad.cbSize = sizeof APPBARDATA;
		::SHAppBarMessage(ABM_GETTASKBARPOS, &ad);
		switch (ad.uEdge) {
		case 0:
		case 2: {
			mminfo->ptMaxSize.x = width - (ad.rc.right - ad.rc.left);
			mminfo->ptMaxSize.y = hegth - 0;
		}break;
		case 1:
		case 3: {
			mminfo->ptMaxSize.x = width - 0;
			mminfo->ptMaxSize.y = hegth - (ad.rc.bottom - ad.rc.top);
		}break;
		}

		bHandled = false;
		return 0;
	}
	LRESULT UIClient::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(GetHWND());
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(sk::string(GetWindowClassName()).append(_TEXT(".xml")).c_str(), (LPCTSTR)0, this, &m_pm);
		ASSERT(pRoot && "Failed to parse 'ui.xml'");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);


		m_IsOpenT.store(true);
		m_Threads.emplace_back(
			[&]()
			{
#if 1
				auto other = ::OleInitialize(NULL);
				::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
				m_pWebBrowserUI = new UIWebBrowser();
				m_pWebBrowserUI->Create(NULL, _TEXT("新生联创"), UI_WNDSTYLE_FRAME, 0L);
				//m_pWebBrowserUI->ShowWindow(false);

				CPaintManagerUI::MessageLoop();


				::CoUninitialize();
				::OleUninitialize();
#else
				auto other = ::OleInitialize(NULL);
				::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
				
				CefMainArgs args(extern_global_hinstance);
				CefRefPtr<cef::App> app(new cef::App);
				int exitCode = CefExecuteProcess(args, app, NULL);
				CefSettings settings;
				CefInitialize(args, settings, app, NULL);


				m_pWebBrowserUI = new UIWebBrowser();
				m_pWebBrowserUI->Create(NULL, _TEXT("新生联创"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW);
				//m_pWebBrowserUI->ShowWindow(false);
				CefRunMessageLoop();
				//CPaintManagerUI::MessageLoop();
				CefShutdown();

				::CoUninitialize();
				::OleUninitialize();
#endif
			});


		InitWindow();
		this->Open();
		bHandled = TRUE;
		return 0;
	}

	void UIClient::InitWindow()
	{
		m_HotKeyShow = ::GlobalAddAtom(L"SKHotKeyShow");
		::RegisterHotKey(m_hWnd, m_HotKeyShow, MOD_CONTROL | MOD_ALT, int('H'));

		m_HotKeyUIWebShow = ::GlobalAddAtom(L"SKHotKeyShow");
		::RegisterHotKey(m_hWnd, m_HotKeyUIWebShow, MOD_CONTROL | MOD_ALT, int('J'));

		m_HotKeyClose = ::GlobalAddAtom(L"SKHotKeyClose");
		::RegisterHotKey(m_hWnd, m_HotKeyClose, MOD_CONTROL | MOD_ALT, int('C'));

		m_HotKeyLeftTo = ::GlobalAddAtom(L"SKHotKeyLeftTo");
		::RegisterHotKey(m_hWnd, m_HotKeyLeftTo, MOD_CONTROL | MOD_ALT, int('L'));

		m_HotKeyRightTo = ::GlobalAddAtom(L"SKHotKeyRightTo");
		::RegisterHotKey(m_hWnd, m_HotKeyRightTo, MOD_CONTROL | MOD_ALT, int('R'));

		m_pWndZoomIn->Create();
		::SetWindowPos(GetHWND(), nullptr, 0, 0, 500, 350, SWP_NOACTIVATE);
		CenterWindow();
		/*GetCtrl<CTreeViewUI>(_TEXT("DEF_252C209091A2AC3D370447CFC125F4E3"))->SelectItem(0);*/

		m_pWebUIParent = GetCtrl<CHorizontalLayoutUI>(_T("DEF_0C0EEFE9C1650882683439B4EFE6C4E3"));
		//auto pWebUI = GetCtrl<CWebBrowserUI>(_T("DEF_2730FCBC767AAD64BCB02BB1446EF61A"));
		//if (pWebUI) {
		//	sk::string targetUrl = _T("http://cn.bing.com");
		//	pWebUI->Navigate2(targetUrl.c_str());
		//}





	}

























	void UIClient::OnMinimized(CControlUI*)
	{
		::ShowWindow(GetHWND(), SW_SHOWMINIMIZED);
	}
	void UIClient::OnUIClose(CControlUI*)
	{
		m_pWebBrowserUI->Close();
		this->Close();
		m_pWndZoomIn->Close();
		NetApi<sk::network::INetworkApi>()->Stop();
		UIFrame::Close(IDOK);
		Global::SystemExit();
		::UnregisterHotKey(m_hWnd, m_HotKeyShow);
		::UnregisterHotKey(m_hWnd, m_HotKeyUIWebShow);
		::UnregisterHotKey(m_hWnd, m_HotKeyClose);
		::UnregisterHotKey(m_hWnd, m_HotKeyLeftTo);
		::UnregisterHotKey(m_hWnd, m_HotKeyRightTo);
	}
	void UIClient::OnUILogout(CControlUI* pCtrlUI)
	{
		NetApi<sk::network::INetworkApi>()->Stop();

		::SetWindowPos(GetHWND(), nullptr, 0, 0, 500, 350, SWP_NOACTIVATE);
		CenterWindow();

		GetCtrl<CTabLayoutUI>(_TEXT("DEF_A70C424EF50CEE83AB773256B8C50D41"))->SelectItem(0);

		{//! 清理UI控件相关数据
			auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
			if (pList) {
				pList->RemoveAll();
			}
		}

	}
	void UIClient::OnUISystemCaptureZoomIn(CControlUI* pCtrlUI)
	{
		WndZoomIn()->ShowImage(pCtrlUI->GetCache());
	}
	void UIClient::OnUISelectTargetServerAt(CControlUI* pCtrlUI)
	{		if (!pCtrlUI) return;
		auto pListTree = (CListUI*)pCtrlUI;
		auto nCurSel = pListTree->GetCurSel();
		if (nCurSel < 0) return;
		auto pSel = pListTree->GetItemAt(nCurSel);

		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit)
			return;
		pListEdit->Clear();
		pListEdit->NeedParentUpdate();
		pListEdit->SetTagUINT64(pSel->GetTagUINT64());
		pListEdit->Init();
		pListEdit->NeedParentUpdate();


		NetRequestSystemCon(0, pSel->GetTagUINT64(), "");
		NetRequestSystemStatusInfo(pSel->GetTagUINT64());
		NetRequestSystemCapture(pSel->GetTagUINT64());
	}
	void UIClient::OnUISystemCaptureUpdate()
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0 || pListEdit->GetCount() <= 0)
			return;
		pListEdit->Update(EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CAPTURE);
	}
	void UIClient::OnUISystemStatusUpdate()
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0 || pListEdit->GetCount() <= 0)
			return;
		pListEdit->Update(EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_STATUS);
	}
	void UIClient::OnUISystemConUpdate()
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0 || pListEdit->GetCount() <= 0)
			return;
		pListEdit->Update(EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON);
	}
	void UIClient::OnUISystemConProcessUpdate(const std::string& image)
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0 || pListEdit->GetCount() <= 0)
			return;
		pListEdit->Update(EnProductEditUIType::EN_PRODUCT_EDIT_UI_TYPE_SYSTEM_CON, image);
	}
	void UIClient::OnUIRefreshTargetSystemStatus(CControlUI* pCtrlUI)
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0)
			return;

		Global::SharedGet()->m_SyncObj.Request(
			NetSyncSession(sk::network::EnNetCmd::EN_NETCMD_100100210,
				5000,
				[this, pCtrlUI](const bool& result, const auto& head, const auto& pak)
				{
					if (result && !pak.empty())
					{
						auto pTargetUI = (IProductSystemStatusEditUI*)pCtrlUI->GetParent()->GetParent();
						if (pTargetUI)
						{
							pTargetUI->SetCache(pak);
							pTargetUI->Set();
						}
					}
					else
					{
						MsgBox(GetHWND(), _TEXT("目录服务器为空!"), _TEXT(""), MB_TOPMOST);
					}
				}), [&]()->auto {
					return Global::GetCore()->GetClient()->NetRequestSystemStatusInfo(pListEdit->GetTagUINT64());
				});
	}
	void UIClient::OnUIRefreshTargetSystemCapture(CControlUI* pCtrlUI)
	{
		auto pListEdit = GetCtrl<IProductControlPanelEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pListEdit || pListEdit->GetTagUINT64() <= 0)
			return;

		Global::SharedGet()->m_SyncObj.Request(
			NetSyncSession(sk::network::EnNetCmd::EN_NETCMD_100100220,
				5000,
				[this, pCtrlUI, pListEdit](const bool& result, const auto& head, const auto& pak)
				{
					if (result && !pak.empty())
					{
						auto pTargetUI = (IProductSystemCaptureEditUI*)pCtrlUI->GetParent()->GetParent();
						if (pTargetUI)
						{
							pTargetUI->SetTagUINT64(pListEdit->GetTagUINT64());
							pTargetUI->SetCache(pak);
							pTargetUI->Set();
						}
					}
					else
					{
						MsgBox(GetHWND(), _TEXT("目录服务器为空!"), _TEXT(""), MB_TOPMOST);
					}
				}), [&]()->auto {
					return Global::GetCore()->GetClient()->NetRequestSystemCapture(pListEdit->GetTagUINT64());
				});
	}
	void UIClient::OnUISystemClientObjsUpdate(shared::SystemStatusInfo* pSystemStatusInfo /*= nullptr*/)
	{
		if (Global::SharedGet()->m_SystemClientObjQ.empty())
			return;

		auto pList = GetCtrl<CListUI>(_TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391"));
		if (!pList)
			return;
		if (!pSystemStatusInfo)
		{
#if 0
			pList->RemoveAll();
			CDialogBuilder builder;
			Global::SharedGet()->m_SystemClientObjQ.iterate(
				[&](const auto& ID, auto& client)
				{
					auto pListLine = (IProductTreeNodeUI*)builder.Create(_TEXT("UIProductTreeNode.xml"));
					pList->Add(pListLine);
					pListLine->SetTagUINT64(client.ID);
					pListLine->SetSystemID(client.ID);
					pListLine->SetSystemHost("");
					pListLine->SetSystemStatus(false);
				});
			if (pList->GetCount()) { pList->SelectItem(0); }
#endif
		}
		else if (pList->GetCount())
		{
			auto at = (IProductTreeNodeUI*)pList->GetItemAtByTagUINT64(pSystemStatusInfo->ID);
			if (at)
			{
				at->SetSystemHost(pSystemStatusInfo->Hostname);
				at->SetSystemStatus(pSystemStatusInfo->Online);
				at->NeedParentUpdate();
			}
		}
	}

	void UIClient::OnUIRequestSystemClientObjs(CControlUI* pCtrlUI)
	{
		Global::SharedGet()->m_SyncObj.Request(
			NetSyncSession(sk::network::EnNetCmd::EN_NETCMD_100100205,
				10000,
				[this](const bool& result, const auto& head, const auto& pak)
				{
					if (result && !pak.empty())
					{
						std::vector<shared::ClientField> clients;
						sk::Helper::OneUnknownParseStruct<shared::ClientField>(pak.data(), pak.size(), clients);
						for (const auto& client : clients)
						{
							//Global::SharedGet()->m_SystemClientObjQ.push(client.ID, client);
						}

						if (Global::SharedGet()->m_SystemClientObjQ.empty())
							return;

						OnUISystemClientObjsUpdate();
					}
					else
					{
						MsgBox(GetHWND(), _TEXT("目录服务器为空!"), _TEXT(""), MB_TOPMOST);
					}
				}), []()->auto {
					return Global::GetCore()->GetClient()->NetRequestSystemClientObjs();
				});
	}


	void UIClient::OnUILogon(CControlUI* pCtrlUI)
	{

		Global::SharedGet()->m_SyncObj.Request(
			NetSyncSession(sk::network::EnNetCmd::EN_NETCMD_100000030,
				5000,
				[this](const bool& result, const auto& head, const auto& pak)
				{

					auto login_result = false;
					if (result && !pak.empty())
					{
						auto result = (shared::ClientField*)pak.data();
						if (result && SKAPIRES_SUCCESS(result->Result))
						{
							MINMAXINFO MMI = { 0 };
							::SendMessage(GetHWND(), WM_GETMINMAXINFO, 0, (LPARAM)&MMI);
							::SetWindowPos(GetHWND(), HWND_TOP, 0, 0, MMI.ptMaxSize.x, MMI.ptMaxSize.y, SWP_NOACTIVATE);
							CenterWindow();
							GetCtrl<CTabLayoutUI>(_TEXT("DEF_A70C424EF50CEE83AB773256B8C50D41"))->SelectItem(1);
							login_result = true;

							NetRequestSystemClientObjs();
						}
					}

					if (!login_result)
					{
						NetApi<sk::network::INetworkApi>()->Stop();
						MsgBox(GetHWND(), _TEXT("连接到服务器失败!"), _TEXT(""), MB_TOPMOST);
					}
				}), [&]()->auto {	return NetApi<sk::network::INetworkApi>()->Start(this); });
	}



	void UIClient::OnUICreateSystemConProcess(CControlUI* pCtrlUI)
	{
		auto pParentList = GetCtrl<CListUI>(_TEXT("DEF_D7D1A4B0B567CFF26BC4CEAE73748A9E"));
		if (!pParentList)
			return;
		CDialogBuilder builder;
		auto pSysProcessStatusEditCtrl = builder.Create<IProductSystemConProcessEditUI>(_TEXT("UISystemConProcess.xml"));
		pParentList->AddAt(pSysProcessStatusEditCtrl, 0);
		pSysProcessStatusEditCtrl->SetTagINT64(TypeToPtr(pParentList));
		pParentList->GetParent()->NeedParentUpdate();

		MsgBox(GetHWND(), _TEXT("创建新的进程节点完成!\n请完善'进程路径名'参数,修改才会生效."), _TEXT("提示"), MB_TOPMOST);
	}
	void UIClient::OnUIUpdateSystemConProcess(CControlUI* pCtrlUI)
	{

	}
	void UIClient::OnUIDeleteSystemConProcess(CControlUI* pCtrlUI)
	{
		auto pParentList = GetCtrl<CListUI>(_TEXT("DEF_D7D1A4B0B567CFF26BC4CEAE73748A9E"));
		if (!pParentList)
			return;
		if (pParentList->Remove(pCtrlUI->GetParent()->GetParent()->GetParent()))
		{
			pParentList->GetParent()->NeedParentUpdate();
		}
		else
		{
			MsgBox(GetHWND(), _TEXT("删除失败"), _TEXT("提示"), MB_TOPMOST);
		}
	}
	void UIClient::OnUISystemPoweroffProcess(CControlUI* pCtrlUI)
	{
		auto pSetupUI = GetCtrl<CListUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pSetupUI || pSetupUI->GetTagUINT64() <= 0)
			return;
		NetRequestSystemPoweroff(pSetupUI->GetTagUINT64());
	}
	void UIClient::OnUISystemRebootProcess(CControlUI* pCtrlUI)
	{
		auto pSetupUI = GetCtrl<CListUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pSetupUI || pSetupUI->GetTagUINT64() <= 0)
			return;
		NetRequestSystemReboot(pSetupUI->GetTagUINT64());
	}
	void UIClient::OnUISystemKillProcess(CControlUI* pCtrlUI)
	{
		auto self = (CContainerUI*)pCtrlUI->GetParent()->GetParent();
		auto target = self->GetSubCtrl<CControlUI>(_TEXT("DEF_7CE338A4E1C2372CF4D23C7041A8FFC3"));
		auto image = target->GetText();
		if (image.IsEmpty())
		{
			MsgBox(GetHWND(), _TEXT("不合法的映像名!"), _TEXT("错误"), MB_TOPMOST);
			return;
		}
		auto pSetupUI = GetCtrl<CListUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pSetupUI || pSetupUI->GetTagUINT64() <= 0)
			return;
		NetRequestSystemKillProcess(pSetupUI->GetTagUINT64(), sk::StringConvert::WStringToMBytes(image.GetData()));
	}
	void UIClient::OnUISaveSystemConChanged(CControlUI* pCtrlUI)
	{
		auto pSetupUI = GetCtrl<IProductSystemConEditUI>(_TEXT("DEF_770A32C71E22FBBDD2C8F330C683FD55"));
		if (!pSetupUI || pSetupUI->GetTagUINT64() <= 0)
			return;
		auto update = pSetupUI->Get();
		if (!update.empty())
		{
			auto controlpane = PtrToType(IProductControlPanelEditUI, pSetupUI->GetTagINT64());
			if (!controlpane)
				return;
			shared::SystemID ID = controlpane->GetTagUINT64();
			NetRequestSystemCon(1, ID, update);
		}
	}

	void UIClient::OnUIWebMoveToLeft()
	{
		m_pWebUIParent = GetCtrl<CHorizontalLayoutUI>(_T("DEF_0C0EEFE9C1650882683439B4EFE6C4E3"));

	}
	void UIClient::OnUIWebMoveToRight()
	{
		m_pWebUIParent = GetCtrl<CHorizontalLayoutUI>(_T("DEF_A238AC461A665414D74F8BCEB3F764E9"));
	}




	void UIClient::OnUIFx168NewStickerMD( sk::container::map<sk::packet, sk::packet>& pak)
	{

	}
	void UIClient::OnUIFx168NewStickerNews(const std::vector<sk::string>& news)
	{
		if (news.empty())
			return;

		auto pHL = GetCtrl<CHorizontalLayoutUI>(_T("DEF_C9D79D14CA66FAEA074806985064DF2A"));
		if (!pHL)
			return;
		pHL->RemoveAll();

		for (const auto& line : news)
		{
			auto pLabelNews = new CLabelUI();
			auto format = sk::Log::Format(
				LR"(width="{}" font="102" wordbreak="false" textcolor="#FF00008B" padding="0, 0, 0, 0")",
				line.size() * 15
			);
			pLabelNews->ApplyAttributeList(format.c_str());
			pLabelNews->SetText(line.c_str());
			pHL->Add(pLabelNews);
		}
		pHL->NeedParentUpdate();
	}
	void UIClient::OnUIFx168NewStickerUrl(const sk::string& url)
	{
		if(!url.empty())
			UIWebBrowserGet()->OpenSite(url);
	}


	void UIClient::Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
			if (msg.pSender->GetName() == _TEXT("DEF_5367D1B574F2DA1A8BE2CC5A709DA388")) //! 最小化
			{
				OnMinimized(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_C63FEB4ADE18128AE1C765D021677F64")) //! 关闭系统
			{
				OnUIClose(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_37C0AEB747179B19C2ABC7320C215DD9")) //! 退出登录
			{
				OnUILogout(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_95DCE79DA85B064019873074CC016F9D")) //! 登录
			{
				OnUILogon(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_AA940C6CDB6BEFC36FB6D931F852BCE8")) //! 刷新服务列表
			{
				OnUIRequestSystemClientObjs(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_35DC45D5DF129A0B7FD4260BAD0B6191")) //! 刷新目录服务状态
			{
				OnUIRefreshTargetSystemStatus(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_334483988B91996EEF43020DB1C25ADD")) //! 刷新目录服务截屏
			{
				OnUIRefreshTargetSystemCapture(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_8199818406345AC3B8C7D7AF03360488")) //! 截屏放大
			{
				OnUISystemCaptureZoomIn(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_F1E936816302422A7C646AC1334625DD")) //! 创建进程配置
			{
				OnUICreateSystemConProcess(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_21750B96C0ED3352B64788634677ADBB")) //! 删除进程配置
			{
				OnUIDeleteSystemConProcess(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_AA7E9DE6BD2ADCBB32C3D2F88ECAA2BB")) //! 关机
			{
				OnUISystemPoweroffProcess(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_DE51E231C869A02FE12F8FC040EB8DF9")) //! 重启
			{
				OnUISystemRebootProcess(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_F6A050B6CB63DF76C1EE83CA4BBA2678")) //! 保存修改全部配置
			{
				OnUISaveSystemConChanged(msg.pSender);
			}
			else if (msg.pSender->GetName() == _TEXT("DEF_C3695AAF4A250FB82462AF70A71C8C48"))//! 关闭进程
			{
				OnUISystemKillProcess(msg.pSender);
			}
		}///DUI_MSGTYPE_CLICK
		else if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			if (msg.pSender->GetName() == _TEXT("DEF_9CD12D041D24ED8D64E170F9A3A17391")) //! 选择指定服务
			{
				OnUISelectTargetServerAt(msg.pSender);
			}
		}///DUI_MSGTYPE_ITEMSELECT
		else if (msg.sType == DUI_MSGTYPE_ITEMRCLICK) {
			auto sk = 0;
		}











	}

}///namespace client