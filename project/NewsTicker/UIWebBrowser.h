#if !defined(AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE32__HEAD__)
#define AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE32__HEAD__

namespace client {

	class UIWebBrowser final : public sk::object::IObject, public sk::ui::UIFrame, public sk::network::INetworkSpi
	{
	public:
		UIWebBrowser();
		virtual ~UIWebBrowser();
	private:
		int Init() override final;
		int UnInit() override final;
	public:
		int Open() override final;
		int Close() override final;
	protected:
		void InitWindow() override final;
		void Notify(TNotifyUI& msg) override final;
		CControlUI* CreateControl(LPCTSTR pstrClassName) override final;
		LPCTSTR GetWindowClassName() const override final { return _TEXT("UIWebBrowser"); }
		LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;		
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;
		LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;
	public:
		void OpenSite(const sk::string&) const;
	private:
		CWebBrowserUI* m_pWebBrowserUI = nullptr;
#if CEF_ENABLE
		CefRefPtr<cef::Client> m_CefClient = nullptr;
#endif
	};

#if CEF_ENABLE
	class CefWebBrowserUI : public CControlUI
	{
	public:
		CefWebBrowserUI() : m_Browser(nullptr)
		{

		}
		~CefWebBrowserUI()
		{

		}
		virtual LPCTSTR GetClass() const
		{
			return _T("CefWebBrowserUI");
		}
		virtual LPVOID GetInterface(LPCTSTR pstrName)
		{
			if (_tcsicmp(pstrName, _T("CefWebBrowser")) == 0)
			{
				return static_cast<CefWebBrowserUI*>(this);
			}
			return CControlUI::GetInterface(pstrName);
		}
		//这个一定要设置，不然控件无法显示
		virtual void SetPos(RECT rc, bool bNeedInvalidate = true)
		{
			CControlUI::SetPos(rc, bNeedInvalidate);
#if 0
			if (m_Browser.get())
			{

				CefRefPtr<CefBrowser> browser = m_Browser->GetBrowser();
				DuiLib::CDuiRect rc = GetPos();
				if (browser && !rc.IsNull())
				{
					::SetWindowPos(browser->GetHost()->GetWindowHandle(),
						NULL, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), SWP_NOZORDER | SWP_NOACTIVATE);
				}

			}
#endif
		}


		CefRefPtr<CefBrowser> GetBrowser()
		{
			//return m_Browser->GetBrowser();
			return nullptr;
		}

		CefRefPtr<CefFrame> GetMainFrame()
		{
			//return m_Browser->GetMainFrame();
			return nullptr;
		}

		void CloseAllBrowser()
		{
			if (m_Browser.get())
			{
				m_Browser->CloseAllBrowsers(true);
			}
		}
		bool CreateBrowser(const CefString& szHomePage, cef::Client* pWnd)
		{
			//CefWindowInfo info;
			//if (m_Browser == NULL)
			//{
			//	m_Browser = new CHaoBrowserHandler(pWnd);
			//}
			//m_Browser->SetHomePage(szHomePage);
			//if (m_Browser != NULL)
			//{
			//	info.SetAsChild(GetManager()->GetPaintWindow(), GetPos());
			//	CefBrowserSettings browserSettings;
			//	return CefBrowserHost::CreateBrowser(info, m_Browser.get(), szHomePage, browserSettings, NULL);
			//}

			return false;
		}
		void LoadUrl(const CefString& url)
		{

		}

	protected:
		CefRefPtr<cef::Client> m_Browser;
	};

#endif

}///namespace client




/// /*新生联创（上海）**/
/// /*2019年12月28日 22:7:14|570**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE32__HEAD__