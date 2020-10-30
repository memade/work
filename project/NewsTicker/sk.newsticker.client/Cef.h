#if !defined(AFX_SK_H__5D3AA4D6_7C80_47DB_B965_623497D7B2D9__HEAD__)
#define AFX_SK_H__5D3AA4D6_7C80_47DB_B965_623497D7B2D9__HEAD__

#if CEF_ENABLE
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_parser.h"
#include "include/cef_browser.h"
#include "include/base/cef_bind.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_closure_task.h"


namespace cef
{
    class WindowDelegate : public CefWindowDelegate 
    {
    public:
        explicit WindowDelegate(CefRefPtr<CefBrowserView> browser_view) : browser_view_(browser_view) {}
        void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE 
        {
            window->AddChildView(browser_view_);
            window->Show();
            browser_view_->RequestFocus();
        }
        void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE 
        {
            browser_view_ = nullptr;
        }
        bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE 
        {
            CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
            if (browser)
                return browser->GetHost()->TryCloseBrowser();
            return true;
        }
        CefSize GetPreferredSize(CefRefPtr<CefView> view) OVERRIDE
        {
            return CefSize(800, 600);
        }
    private:
        CefRefPtr<CefBrowserView> browser_view_;
        IMPLEMENT_REFCOUNTING(WindowDelegate);
        DISALLOW_COPY_AND_ASSIGN(WindowDelegate);
    };

    class BrowserViewDelegate : public CefBrowserViewDelegate {
    public:
        BrowserViewDelegate() {}
        bool OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view,
            CefRefPtr<CefBrowserView> popup_browser_view,
            bool is_devtools) OVERRIDE 
        {
            CefWindow::CreateTopLevelWindow(new WindowDelegate(popup_browser_view));
            return true;
        }
    private:
        IMPLEMENT_REFCOUNTING(BrowserViewDelegate);
        DISALLOW_COPY_AND_ASSIGN(BrowserViewDelegate);
    };

    class Client : public CefClient,
        public CefDisplayHandler,
        public CefLifeSpanHandler,
        public CefLoadHandler 
    {
    public:
        explicit Client(bool use_views = false);
        ~Client();
        static Client* GetInstance();
        virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {return this;}
        virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {return this;}
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }
        virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title) OVERRIDE;
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
        virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
        virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            ErrorCode errorCode,
            const CefString& errorText,
            const CefString& failedUrl) OVERRIDE;
        bool IsClosing() const { return is_closing_; }
        void CloseAllBrowsers(const bool& force_close);
    private:
        void PlatformTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title);
        const bool use_views_;
        typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
        BrowserList browser_list_;
        bool is_closing_;
        IMPLEMENT_REFCOUNTING(Client);
    };

	class App : public CefApp, public CefBrowserProcessHandler,public CefRenderProcessHandler
	{
	public:
		App();
		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }
		virtual void OnContextInitialized() OVERRIDE;
	private:
		IMPLEMENT_REFCOUNTING(App);
	};
}///namespace cef

#endif

//!@ /*新生联创®（上海）*/
//!@ /*Tue Sep 29 08:40:02 UTC+0800 2020*/
//!@ /*___www.skstu.com___*/
#endif /*AFX_SK_H__5D3AA4D6_7C80_47DB_B965_623497D7B2D9__HEAD__*/