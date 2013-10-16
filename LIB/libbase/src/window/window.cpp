#include <libbase/window.hpp>
#include <libbase/err.hpp>
#include <libbase/string.hpp>
#include <liblog/logger.hpp>

#include <winuser.h>

namespace Base {

	WindowApplication::~WindowApplication()
	{
		LogTrace();
	}

	int WindowApplication::Run()
	{
		LogTrace();
		MSG msg;
		BOOL ret;
		while ((ret = ::GetMessageW(&msg, 0, 0, 0))) {
			if (ret == 0) {
				// quit
				break;
			} else if (ret == -1) {
				// error
			} else {
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
		return 0;
	}

	bool Window::create(HWND parent, PCWSTR title, DWORD exstyle, DWORD style, int x, int y, int w, int h, HMENU id)
	{
		LogTrace();
		WNDCLASSEX wndc;
		wndc.lpszClassName = L"BaseWindow";
		wndc.cbSize = sizeof(wndc);
		wndc.lpfnWndProc = WNDPROC(MainHandler);
		wndc.cbClsExtra = 0;
		wndc.cbWndExtra = 0;
		wndc.hbrBackground = HBRUSH(COLOR_WINDOW);
		wndc.hInstance = GetModuleHandle(nullptr);
		wndc.hCursor = LoadCursor(0, IDC_ARROW);
		wndc.style = CS_HREDRAW | CS_VREDRAW;
		wndc.hIcon = 0;
		wndc.hIconSm = 0;
		wndc.lpszMenuName = 0;
		::RegisterClassExW(&wndc);

		//Создаем само окно
		m_handle = ::CreateWindowExW(exstyle, L"BaseWindow", title, style | WS_CLIPCHILDREN, //Стиль WS_CLIPCHILDREN нужен для того, чтобы дочерние контролы не мигали при перерисовке
		                       x, y, w, h, parent,
		                       id,
		                       ::GetModuleHandleW(nullptr),
		                       this);
		LogDebugIf(!m_handle, L"%s\n", ErrAsStr().c_str());

		if (!m_handle)
			return false;

//		::ShowWindow(m_handle, SW_SHOW);
//		::UpdateWindow(m_handle);

		return true;
	}

	LRESULT CALLBACK Window::MainHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		Window * window = nullptr;
		if (message == WM_NCCREATE) {
			LogTrace();
			CREATESTRUCTW * createStruct = reinterpret_cast<CREATESTRUCTW*>(lparam);
			window = static_cast<Window*>(createStruct->lpCreateParams);
			::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		} else {
			window = reinterpret_cast<Window*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
			if (window) {
				auto handler = window->m_handlers.find(message);

				if (handler != window->m_handlers.end()) {
					return (handler->context->*handler->func)(lparam, wparam);
				}
			}
		}
		return ::DefWindowProcW(hwnd, message, wparam, lparam);
	}

	HWND Window::get_handle() const
	{
		return m_handle;
	}

	Window::Handlers::base_type::iterator Window::Handlers::find(UINT message)
	{
		using namespace std;
		auto range = equal_range(begin(), end(), message);
		return range.first != range.second ? range.first : end();
	}
}
