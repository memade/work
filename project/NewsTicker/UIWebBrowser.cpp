#include "stdafx.h"
#include "UIWebBrowser.h"

namespace client {

	UIWebBrowser::UIWebBrowser()
	{
		if (Init()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
	}

	UIWebBrowser::~UIWebBrowser()
	{
		UnInit();
	}

	int UIWebBrowser::Init()
	{
		return 0;
	}

	int UIWebBrowser::UnInit()
	{	
		return 0;
	}

	int UIWebBrowser::Open()
	{
		if (m_IsOpen.load()) return 0;


		m_IsOpen.store(true);
		return 0;
	}

	int UIWebBrowser::Close()
	{
		if (!m_IsOpen.load()) return 0;

		UIFrame::Close(IDOK);

		m_IsOpen.store(false);
		return 0;
	}



	CControlUI* UIWebBrowser::CreateControl(LPCTSTR pstrClassName)
	{

		return nullptr;
	}

	LRESULT UIWebBrowser::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		UINT iKey = (UINT)wParam;
		if (iKey == VK_RETURN)
		{

		}
		bHandled = true;
		return 0;
	}

	LRESULT UIWebBrowser::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		switch (uMsg)
		{
		case WM_HOTKEY:
		{
			auto idHotKey = (int)wParam; // identifier of hot key
			auto fuModifiers = (UINT)LOWORD(lParam); // key-modifier flags
			auto uVirtKey = (UINT)HIWORD(lParam); // virtual-key code

		}break;
		}///switch

		bHandled = FALSE;
		return 0UL;
	}

	LRESULT UIWebBrowser::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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
	LRESULT UIWebBrowser::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		dwExStyle ^= WS_EX_TOOLWINDOW;
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);

		m_pm.Init(GetHWND());
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(sk::string(GetWindowClassName()).append(_TEXT(".xml")).c_str(), (LPCTSTR)0, NULL, &m_pm);
		ASSERT(pRoot && "Failed to parse 'ui.xml'");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);
		InitWindow();
		this->Open();
		bHandled = TRUE;
		return 0;
	}

	void UIWebBrowser::InitWindow()
	{
		//<WebBrowser name="DEF_2730FCBC767AAD64BCB02BB1446EF61A" clsid="{8856F961-340A-11D0-A96B-00C04FD705A2}" delaycreate="false"/>
		m_pWebBrowserUI = GetCtrl<CWebBrowserUI>(_T("DEF_2730FCBC767AAD64BCB02BB1446EF61A"));
		if (m_pWebBrowserUI) {
			m_pWebBrowserUI->SetHomePage(_T("http://cn.bing.com"));
			m_pWebBrowserUI->NavigateHomePage();
		}
#if 0
		CefRefPtr<cef::Client> g_handler(new cef::Client());
		CefBrowserSettings browser_settings;
		CefWindowInfo window_info;
		RECT rt;
		GetWindowRect(m_hWnd, &rt);
		window_info.SetAsChild(m_hWnd, rt);

		CefBrowserHost::CreateBrowser(window_info, g_handler, _T("http://cn.bing.com"), browser_settings,
			nullptr, nullptr);
#endif
	}

	void UIWebBrowser::OpenSite(const sk::string& url) const
	{
		if (url.empty())
			return;
		if (m_pWebBrowserUI) {
			m_pWebBrowserUI->SetHomePage(url.c_str());
			m_pWebBrowserUI->NavigateHomePage();
		}
	}

	void UIWebBrowser::Notify(TNotifyUI& msg)
	{
		if (msg.sType == DUI_MSGTYPE_CLICK)
		{
		}
	}

}///namespace client