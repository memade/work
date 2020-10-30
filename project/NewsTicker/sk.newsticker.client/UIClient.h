#if !defined(AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__HEAD__)
#define AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__HEAD__

#include "PopupCaptureZoomIn.h"

namespace client {

	class UIClient final : public sk::object::IObject, public sk::ui::UIFrame, public sk::network::INetworkSpi
	{
		PopupCaptureZoomIn* m_pWndZoomIn = nullptr;
		UIWebBrowser* m_pWebBrowserUI = nullptr;
	public:
		UIClient();
		virtual ~UIClient();
	private:
		int Init() override final;
		int UnInit() override final;

		sk::Timer m_Timer;
		static int OnTimer(const sk::int64&, const sk::intptr&, const sk::int64&);
	public:
		int Open() override final;
		int Close() override final;
		static void MsgBox(HWND, const sk::string&, const sk::string&, const UINT&);
		PopupCaptureZoomIn* WndZoomIn() const { return m_pWndZoomIn; }
		UIWebBrowser* UIWebBrowserGet() const { return m_pWebBrowserUI; }
	private:
		CHorizontalLayoutUI* m_pWebUIParent = nullptr;
		void AdjustWebUIPos();
		void ProcessUrlT();
		void ProcessMDT();
		void ProcessNewsT();
	protected:
		void InitWindow() override final;
		void Notify(TNotifyUI& msg) override final;
		CControlUI* CreateControl(LPCTSTR pstrClassName) override final;
		LPCTSTR GetWindowClassName() const override final { return _TEXT("UIMain"); }
		LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;		
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;
		LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;
		LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override final;

		void OnMinimized(CControlUI*);
		void OnUIClose(CControlUI* ctrl = nullptr);
		void OnUIWebMoveToLeft();
		void OnUIWebMoveToRight();
		void OnUILogon(CControlUI*);
		void OnUILogout(CControlUI* crtl = nullptr);
		void OnUIRequestSystemClientObjs(CControlUI*);
		void OnUIRefreshTargetSystemStatus(CControlUI*);
		void OnUIRefreshTargetSystemCapture(CControlUI*);
		void OnUISelectTargetServerAt(CControlUI*);
		void OnUISystemCaptureZoomIn(CControlUI*);
		void OnUISystemCaptureUpdate();
		void OnUISystemStatusUpdate();
		void OnUISystemConUpdate();
		void OnUISystemClientObjsUpdate(shared::SystemStatusInfo* pSystemStatusInfo = nullptr);
		void OnUISystemConProcessUpdate(const std::string&);
		void OnUICreateSystemConProcess(CControlUI*);
		void OnUIUpdateSystemConProcess(CControlUI*);
		void OnUIDeleteSystemConProcess(CControlUI*);
		void OnUISaveSystemConChanged(CControlUI*);
		void OnUISystemPoweroffProcess(CControlUI*);
		void OnUISystemRebootProcess(CControlUI*);
		void OnUISystemKillProcess(CControlUI*);






		void OnUIFx168NewStickerMD(sk::container::map<sk::packet, sk::packet>&);
		void OnUIFx168NewStickerNews(const std::vector<sk::string>&);
		void OnUIFx168NewStickerUrl(const sk::string&);
	private:
		void OnNetLoginResult(const sk::network::Header&, const sk::packet&);
		void OnNetSystemClientObjs(const sk::network::Header&, const sk::packet&);
		void OnNetSystemCon(const sk::network::Header&, const sk::packet&);
		void OnNetSystemStatusInfo(const sk::network::Header&, const sk::packet&);
		void OnNetSystemCapture(const sk::network::Header&, const sk::packet&);
		void OnNetSystemProcessStatus(const sk::network::Header&, const sk::packet&);
		void OnNetSystemLogoffAck(const sk::network::Header&, const sk::packet&);
		void OnNetSystemRebootAck(const sk::network::Header&, const sk::packet&);
		void OnNetSystemPoweroffAck(const sk::network::Header&, const sk::packet&);
		void OnNetSystemProcessKillAck(const sk::network::Header&, const sk::packet&);

		void OnNetServerMessage(const sk::network::Header&, const sk::packet&);
	public:
		int NetLoginRequest(const shared::ClientField&);
		int NetRequestSystemStatusInfo(const shared::SystemID& id);
		int NetRequestSystemClientObjs();
		int NetRequestSystemCapture(const shared::SystemID&);
		int NetRequestSystemCon(const int&/*0:req 1:update 2:delete*/, const shared::SystemID&,const sk::packet& pak);
		int NetRequestSystemPoweroff(const shared::SystemID&);
		int NetRequestSystemReboot(const shared::SystemID&);
		int NetRequestSystemLogoff(const shared::SystemID&);
		int NetRequestSystemKillProcess(const shared::SystemID&, const sk::packet&);



		int NetRequestNewStickerMD() const;
		int NetRequestNewStickerNews() const;
		int NetRequestNewStickerHttpUrl() const;
	protected:
		void OnTcpClientConnect() override final;
		void OnTcpClientDisconnect() override final;
		void OnTcpClientRead(const sk::network::Header&, const sk::packet&) override final;
		void OnTcpClientWelcome(const sk::network::Header&, const sk::packet&) override final;
	private:
		int m_HotKeyShow = 0;
		int m_HotKeyClose = 0;
		int m_HotKeyLeftTo = 0;
		int m_HotKeyRightTo = 0;
		int m_HotKeyUIWebShow = 0;
	};





}///namespace client




/// /*新生联创（上海）**/
/// /*2019年12月28日 22:7:14|570**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///AFX_SKSTU_CLIENT_H__7D94D_18B8F940_228864BAE6B9E29_AE31__TAIL__