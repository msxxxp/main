#ifndef LIBBASE_WINDOW_HPP_
#define LIBBASE_WINDOW_HPP_

#include <basis/configure.hpp>

#include <algorithm>
#include <vector>

namespace Base {

	class WindowApplication {
	public:
		virtual ~WindowApplication();

		virtual int Run();
	};

	class Window {
	public:
		typedef LRESULT (Window::*FuncPointer)(LPARAM, WPARAM);

		struct Handler_t {
			UINT message;
			Window * context;
			FuncPointer func;

			Handler_t(UINT message_, Window * context_, FuncPointer func_): message(message_), context(context_), func(func_) {}
		};

		bool create(HWND parent, const wchar_t* title, DWORD exstyle, DWORD style, int x, int y, int w, int h, HMENU id);

		static LRESULT CALLBACK MainHandler(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

		HWND get_handle() const;

	protected:
		template<typename Context>
		void add_handler(UINT message, LRESULT (Context::*funcpointer)(LPARAM, WPARAM))
		{
			using namespace std;
			m_handlers.emplace(std::upper_bound(begin(m_handlers), end(m_handlers), message), message, this, reinterpret_cast<FuncPointer>(funcpointer));
//			sort(begin(m_handlers), end(m_handlers));
		}

		struct Handlers: private std::vector<Handler_t> {
			typedef std::vector<Handler_t> base_type;
			using base_type::begin;
			using base_type::end;
			using base_type::empty;
			using base_type::emplace;

			base_type::iterator find(UINT message);
		} m_handlers;

		private:
			HWND m_handle;
	};

	inline bool operator <(const Window::Handler_t & handler, UINT message)
	{
		return handler.message < message;
	}

	inline bool operator <(UINT message, const Window::Handler_t & handler)
	{
		return message < handler.message;
	}
}

#endif
