#if !defined(AFX_SK_H__E210489E_F28A_4196_9809_9DDDDB66CE21__HEAD__)
#define AFX_SK_H__E210489E_F28A_4196_9809_9DDDDB66CE21__HEAD__

namespace client {

	class PopupCaptureZoomIn {
	public:
		PopupCaptureZoomIn(HINSTANCE hInst, bool _IsShow = true);
		~PopupCaptureZoomIn();
	public:
		bool Create();
		void Close();
		bool Show(bool _IsShow = true);
	public:
		HWND GetSafeWnd() const { return m_hWnd; }
		void ShowImage(const sk::stringa& path);
	private:
		HBITMAP m_hBitmap = nullptr;
		bool m_IsShow;
		HWND m_hWnd;
		HWND m_hWndTrace;
		HINSTANCE m_hInstance;
		std::vector<std::thread> m_vt_thread;
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//! 调整子窗口显示布局
		void AdjustChildWnd();

		//! 窗口居中
		static void WindowCenterAndChangeSize(HWND hTarget, int cx, int cy) {
			auto scrWidth = GetSystemMetrics(SM_CXSCREEN);
			auto scrHeight = GetSystemMetrics(SM_CYSCREEN);
			RECT rect;
			//::GetWindowRect(hTarget, &rect);
			rect.right = cx;
			rect.bottom = cy;
			rect.left = (scrWidth - rect.right) / 2;
			rect.top = (scrHeight - rect.bottom) / 2;
			::MoveWindow(hTarget, rect.left, rect.top, rect.right, rect.bottom, TRUE);
		}
	};






}///namespace client








//!@ /*新生联创®（上海）*/
//!@ /*Sun Aug 16 17:33:32 UTC+0800 2020*/
//!@ /*___www.skstu.com___*/
#endif /*AFX_SK_H__E210489E_F28A_4196_9809_9DDDDB66CE21__HEAD__*/

