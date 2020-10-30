#include "stdafx.h"
#include "Cef.h"

#if CEF_ENABLE
namespace cef
{

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Client* global_client = nullptr;

    std::string GetDataURI(const std::string& data, const std::string& mime_type)
    {
        return "data:" + mime_type + ";base64," +
            CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
            .ToString();
    }

    Client::Client(bool use_views)
        : use_views_(use_views), is_closing_(false) 
    {
        DCHECK(!global_client);
        global_client = this;
    }

    Client::~Client() {
        global_client = nullptr;
    }

    // static
    Client* Client::GetInstance() 
    {
        return global_client;
    }

    void Client::OnTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title) 
    {
        CEF_REQUIRE_UI_THREAD();
        if (use_views_) 
        {
            CefRefPtr<CefBrowserView> browser_view = CefBrowserView::GetForBrowser(browser);
            if (browser_view) 
            {
                CefRefPtr<CefWindow> window = browser_view->GetWindow();
                if (window)
                    window->SetTitle(title);
            }
        }
        else
        {
            PlatformTitleChange(browser, title);
        }
    }

    void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();
        browser_list_.push_back(browser);
    }

    bool Client::DoClose(CefRefPtr<CefBrowser> browser) 
    {
        CEF_REQUIRE_UI_THREAD();
        if (browser_list_.size() == 1)
        {
            is_closing_ = true;
        }
        return false;
    }

    void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser) 
    {
        CEF_REQUIRE_UI_THREAD();
        BrowserList::iterator bit = browser_list_.begin();
        for (; bit != browser_list_.end(); ++bit)
        {
            if ((*bit)->IsSame(browser)) 
            {
                browser_list_.erase(bit);
                break;
            }
        }
        if (browser_list_.empty()) 
        {
            CefQuitMessageLoop();
        }
    }

    void Client::OnLoadError(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        ErrorCode errorCode,
        const CefString& errorText,
        const CefString& failedUrl)
    {
        CEF_REQUIRE_UI_THREAD();
        if (errorCode == ERR_ABORTED)
            return;
        std::stringstream ss;
        ss << "<html><body bgcolor=\"white\">"
            "<h2>Failed to load URL "
            << std::string(failedUrl) << " with error " << std::string(errorText)
            << " (" << errorCode << ").</h2></body></html>";
        frame->LoadURL(GetDataURI(ss.str(), "text/html"));
    }

    void Client::CloseAllBrowsers(const bool& force_close)
    {
        if (!CefCurrentlyOn(TID_UI)) 
        {
            CefPostTask(TID_UI, base::Bind(&Client::CloseAllBrowsers, this,force_close));
            return;
        }
        if (browser_list_.empty())
            return;
        BrowserList::const_iterator it = browser_list_.begin();
        for (; it != browser_list_.end(); ++it)
            (*it)->GetHost()->CloseBrowser(force_close);
    }

    void Client::PlatformTitleChange(CefRefPtr<CefBrowser> browser,const CefString& title)
    {
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        if (hwnd)
            SetWindowText(hwnd, std::wstring(title).c_str());
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






     ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    App::App() 
    {
    
    }

    void App::OnContextInitialized()
    {
        CEF_REQUIRE_UI_THREAD();
        CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
        const bool enable_chrome_runtime = command_line->HasSwitch("enable-chrome-runtime");
#if defined(OS_WIN) || defined(OS_LINUX)
        const bool use_views = command_line->HasSwitch("use-views");
#else
        const bool use_views = false;
#endif

        // SimpleHandler implements browser-level callbacks.
        CefRefPtr<Client> handler(new Client(use_views));

        // Specify CEF browser settings here.
        CefBrowserSettings browser_settings;

        std::string url;

        // Check if a "--url=" value was provided via the command-line. If so, use
        // that instead of the default URL.
        url = command_line->GetSwitchValue("url");
        if (url.empty())
            url = "http://cn.bing.com";

        if (use_views && !enable_chrome_runtime) {
            // Create the BrowserView.
            CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
                handler, url, browser_settings, nullptr, nullptr,
                new BrowserViewDelegate());

            // Create the Window. It will show itself after creation.
            CefWindow::CreateTopLevelWindow(new WindowDelegate(browser_view));
        }
        else {
            // Information used when creating the native window.
            CefWindowInfo window_info;

#if defined(OS_WIN)
            // On Windows we need to specify certain flags that will be passed to
            // CreateWindowEx().
            window_info.SetAsPopup(NULL, "cefsimple");
#endif

            // Create the first browser window.
            CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                nullptr, nullptr);
        }
    }
}///namespace cef

#endif