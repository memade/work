#include "stdafx.h"
#include "Core.h"

namespace client {

	Core::Core() 
	{
		if (Init()) { sk::Helper::ErrorFatal(__FUNCTION__, nullptr, "%s", "Module initialize failed !"); }
	}

	Core::~Core()
	{
		UnInit();
	}

	int Core::Init()
	{
		return 0;
	}

	int Core::UnInit()
	{
		return 0;
	}
	int Core::Open() {
		if (m_IsOpen.load()) return 0;
		if (!extern_global_hinstance)
			return -1;
		m_IsOpenT.store(true);
		m_Threads.emplace_back([this]() {ProcessT(); });
		m_Threads.emplace_back([this]() {MsgBoxT(); });
		m_Threads.emplace_back(
			[this]()
			{

				auto other = ::OleInitialize(NULL);
				::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

				//CPaintManagerUI::SetInstance(hInstance);
				//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_TEXT("skin"));
				/*CPaintManagerUI::SetResourceZip(_TEXT("skskin"));*/
				//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
				//CPaintManagerUI::SetResourceZip(_TEXT("skskin"),false,_TEXT("www.skstu.com"));
				CPaintManagerUI::SetInstance(extern_global_hinstance);
				CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _TEXT("NewsTicker.skin"));
				m_pClient = new UIClient();
				m_pClient->Create(NULL, _TEXT("新生联创"), UI_WNDSTYLE_FRAME, 0L);
				m_pClient->CenterWindow();
				CPaintManagerUI::MessageLoop();

				::CoUninitialize();
				::OleUninitialize();
			});

		m_IsOpen.store(true);
		return 0;
	}
	int Core::Close() {
		if (!m_IsOpen.load()) return 0;

		m_IsOpenT.store(false);
		for (auto it = m_Threads.begin(); it != m_Threads.end(); ++it) {
			it->join();
		}
		m_Threads.clear();

		m_IsOpen.store(false);
		return 0;
	}

	void Core::ProcessT() {
		do {

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			if (!m_IsOpenT.load()) break;
		} while (1);
	}

	void Core::MsgBoxT() {
		do {
			auto msg = m_MsgBoxPakQ.pop();
			if (msg)
			{
				MessageBox(msg->hWnd, msg->Text.c_str(), msg->CaptionText.c_str(), msg->uType);
			}
			if (!m_IsOpenT.load()) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		} while (1);
	}
}///namespace client