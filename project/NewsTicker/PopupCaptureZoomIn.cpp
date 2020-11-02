#include "stdafx.h"
#include "PopupCaptureZoomIn.h"

namespace client {

	static WNDPROC __spOldScriptEditProc;


	PopupCaptureZoomIn::PopupCaptureZoomIn(HINSTANCE hInst, bool _IsShow/*=false*/) :
		m_hInstance(hInst) {
		m_IsShow = _IsShow;
		m_hWnd = NULL;
		m_hInstance = NULL;
		m_hWndTrace = NULL;
	}
	PopupCaptureZoomIn::~PopupCaptureZoomIn() {
		Close();
	}
	void PopupCaptureZoomIn::Close() {
		//Gdiplus::GdiplusShutdown(m_gdiplusToken);
		::SendMessage(m_hWnd, WM_CLOSE, 1, 0);
		for (auto it = m_vt_thread.begin(); it != m_vt_thread.end(); ++it) {
			it->join();
		}
		m_vt_thread.clear();
	}
	bool PopupCaptureZoomIn::Create() {
		m_vt_thread.emplace_back([&]() {
			WNDCLASS wndcls;
			wndcls.cbClsExtra = 0;
			wndcls.cbWndExtra = 0;
			wndcls.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
			wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndcls.hIcon = NULL;
			wndcls.hInstance = m_hInstance;
			wndcls.lpfnWndProc = WndProc;
			wndcls.lpszClassName = _T("DEF_C7A4957CA91781B153DB873A0A7A86BC");
			wndcls.lpszMenuName = NULL;
			wndcls.style = CS_HREDRAW | CS_VREDRAW;

			RegisterClass(&wndcls);
			m_hWnd = CreateWindow(_T("DEF_C7A4957CA91781B153DB873A0A7A86BC"), _T("新生联创®"), WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, 0, 0, 640, 480, NULL, NULL, m_hInstance, this);
			if (m_hWnd) {
				m_IsShow ? ::ShowWindow(m_hWnd, SW_SHOW) : ::ShowWindow(m_hWnd, SW_HIDE);
				UpdateWindow(m_hWnd);

				WindowCenterAndChangeSize(m_hWnd,640,480);

				MSG msg;
				while (GetMessage(&msg, NULL, 0, 0)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				auto sk = 0;
			}
			});

		// 初始化GDI+  
		//Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		//Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

		return true;
	}
	bool PopupCaptureZoomIn::Show(bool _IsShow /*= true*/) {
		if (m_hWnd) {
			if (!_IsShow) {
				::ShowWindow(m_hWnd, SW_HIDE);
			}
			else {
				::ShowWindow(m_hWnd, SW_SHOW);
				::BringWindowToTop(m_hWnd);
				::SetForegroundWindow(m_hWnd);
				::SetActiveWindow(m_hWnd);
				::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			return true;
		}
		return false;
	}
	static PopupCaptureZoomIn* pThis = nullptr;
	LRESULT CALLBACK PopupCaptureZoomIn::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		pThis = (PopupCaptureZoomIn*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		static HBRUSH hBrushBk; 
		switch (uMsg) {
		case WM_CREATE: {
			//LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			//::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToUlong(pcs->lpCreateParams));
			//hBrushBk = ::CreateSolidBrush(0x1e1e1e);
			LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			SetWindowLongPtr(hWnd, GWLP_USERDATA, LONG_PTR(pcs->lpCreateParams));
		}break;
		case WM_SIZE: {		
		}break;
		case WM_NOTIFY: {
		}break;
		case WM_CTLCOLORSTATIC:{
		}break;
		case WM_CHAR:
			//char szChar[50];
			//sprintf(szChar, "char code is %d", wParam);
			//MessageBoxA(hWnd, szChar, "char", 0);
			break;

		case WM_CLOSE: {
#if 0
			if (IDYES == MessageBox(hWnd, "是否真的结束?", "message", MB_YESNO)) {
				::DestroyWindow(hWnd);
			}
#else
			if (wParam == 1) {
				::DestroyWindow(hWnd);
			}
			else {
				ShowWindow(hWnd, SW_HIDE);
			}
			//Global::GetCore()->TradeTipShow(false);
#endif
		}break;
		case WM_PAINT:
		{
#if 1
			if (pThis&&pThis->m_hBitmap)
			{
				BITMAP  bminfo;
				GetObject(pThis->m_hBitmap, sizeof(bminfo), &bminfo);
				PAINTSTRUCT ps;
				HDC hDC = BeginPaint(hWnd, &ps);
				HDC memdc = CreateCompatibleDC(hDC);
				SelectObject(memdc, pThis->m_hBitmap);
				BitBlt(hDC, 0, 0, bminfo.bmWidth, bminfo.bmHeight, memdc, 0, 0, SRCCOPY);
				DeleteDC(memdc);
				EndPaint(hWnd, &ps);
			}
#else
			PAINTSTRUCT ps;
			HDC dc = BeginPaint(hWnd, &ps);
			dc = BeginPaint(hWnd, &ps);
			// TODO:  在此添加任意绘图代码...
			//加载图像
			Gdiplus::Image image(LR"(D:\fx168.com\Project2020\memade\manager\bin\x64\Debug\capture\B0100000000.jpg)");
			if (image.GetLastStatus() != Gdiplus::Status::Ok) 
			{
				break;
			}
			//取得宽度和高度
			int width = image.GetWidth();
			int height = image.GetHeight();
			//绘图
			Gdiplus::Graphics graphics(dc);
			graphics.DrawImage(&image, 0, 0, width, height);
			EndPaint(hWnd, &ps);
#endif
		}break;
		case WM_DESTROY: {
			::PostQuitMessage(0);
		}break;
		default: {
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}///default
		}///switch (uMsg)
		return 0;
	}

	LRESULT CALLBACK PopupCaptureZoomIn::EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		auto self = reinterpret_cast<PopupCaptureZoomIn*>(static_cast<LONG_PTR>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)));
		switch (uMsg) {
			//case WM_CREATE: {
			//	LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
			//	auto pApp = (PopupCaptureZoomIn*)pcs->lpCreateParams;
			//	::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToUlong(pApp));
			//}break;
		case WM_IME_CHAR: {
			if (wParam == 'a') {
				auto sk = 0;
			}
		}break;
		case WM_CHAR: {

			char szInput[5] = { 0 };
			sprintf_s(szInput, "%c", (char)wParam);

			sk::stringa text;
			char* text_buffer = nullptr;
			auto text_length = Edit_GetTextLength(hWnd);
			if (text_length) {
				text_buffer = new char[text_length];
				text_length = ::GetWindowTextA(hWnd, text_buffer, text_length);
				text.append((char*)text_buffer, text_length);
			}
			text.append(szInput);
			::SetWindowTextA(hWnd, text.c_str());
			SK_DELETE_PTR_BUFFER(text_buffer);
#if 0
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
			switch (wParam) {
			case VK_RETURN: {
				auto sk = 0;
			}break;
			}
			auto sk = 0;
#endif
		}break;
		default: {
#if 1
			return ::CallWindowProc(__spOldScriptEditProc, hWnd, uMsg, wParam, lParam);
#else
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
#endif
		}///default:
		}///switch (uMsg)

		return 0;
	}
	void PopupCaptureZoomIn::ShowImage(const sk::stringa& path)
	{
		m_hBitmap = (HBITMAP)LoadImageA(extern_global_hinstance, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (m_hBitmap)
		{
			BITMAP  bminfo;
			GetObject(m_hBitmap, sizeof(bminfo), &bminfo);
			SetWindowPos(m_hWnd, HWND_TOP, 0, 0, bminfo.bmWidth, bminfo.bmHeight, SWP_NOACTIVATE);
			Show();
		}

	}
	void PopupCaptureZoomIn::AdjustChildWnd() {
		if (!m_hWnd) {
			return;
		}
		RECT rtClient;
		::GetClientRect(m_hWnd, &rtClient);
		 if (!::IsWindowVisible(m_hWndTrace) && ::IsWindowVisible(m_hWndTrace) && ::IsWindowVisible(m_hWndTrace)) {
			 ::MoveWindow(m_hWndTrace, rtClient.left, rtClient.top, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, TRUE);
#if 0
			::MoveWindow(m_hWndEdit, rtScriptEditCtrl.left, rtScriptEditCtrl.top, rtScriptEditCtrl.right - rtScriptEditCtrl.left, rtScriptEditCtrl.bottom - rtScriptEditCtrl.top, TRUE);

			RECT rtTraceEditCtrl;
			rtTraceEditCtrl.left = rtScriptEditCtrl.left;
			rtTraceEditCtrl.top = rtScriptEditCtrl.bottom;
			rtTraceEditCtrl.right = rtScriptEditCtrl.right;
			rtTraceEditCtrl.bottom = rtClient.bottom;
			::MoveWindow(m_hWndTrace, rtTraceEditCtrl.left, rtTraceEditCtrl.top, rtTraceEditCtrl.right - rtTraceEditCtrl.left, rtTraceEditCtrl.bottom - rtTraceEditCtrl.top, TRUE);
#endif
		 
		 }
	}

}///namespace client