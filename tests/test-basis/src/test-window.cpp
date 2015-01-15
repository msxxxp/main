#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/simstd/memory>
//#include <basis/sys/window.hpp>

//#include <stdio.h>
//#include <functional>
//#include <memory>
//#include <vector>
//
//struct CMyWnd: public Base::Window {
//	CMyWnd()
//	{
//		LogTrace();
//		add_handler(WM_CREATE, &CMyWnd::OnCreate);
//		add_handler(WM_DESTROY, &CMyWnd::OnDestroy);
//		add_handler(WM_CLOSE, &CMyWnd::OnClose);
//		add_handler(WM_ACTIVATE, &CMyWnd::OnActivate);
//		add_handler(WM_SETFOCUS, &CMyWnd::OnSetFocus);
//		add_handler(WM_KILLFOCUS, &CMyWnd::OnKillFocus);
//	}
//
//	LRESULT OnCreate(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnDestroy(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		::PostQuitMessage(0);
//		return 0;
//	}
//
//	LRESULT OnClose(LPARAM lparam, WPARAM wparam)
//	{
//		// chose close option
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		::DestroyWindow(get_handle());
//		return 0;
//	}
//
//	LRESULT OnActivate(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnSetFocus(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//
//	LRESULT OnKillFocus(LPARAM lparam, WPARAM wparam)
//	{
//		LogNoise(L"lparam: %Id wparam: %Id\n", lparam, wparam);
//		return 0;
//	}
//};

void test_window()
{
//	LogTrace();
//	CMyWnd * wnd = new CMyWnd;
//	wnd->create(0, L"HelloWorld!", 0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 300, 300, 500, 400, 0);
//
//	Base::WindowApplication().Run();
}
