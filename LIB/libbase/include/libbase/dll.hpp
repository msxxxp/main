#ifndef LIBBASE_DLL_HPP_
#define LIBBASE_DLL_HPP_

#include <libbase/std.hpp>
#include <patterns/Uncopyable.hpp>

namespace Base {
	struct DynamicLibrary: private Pattern::Uncopyable {
		virtual ~DynamicLibrary() noexcept;

		virtual bool is_valid() const noexcept;

	public:
		DynamicLibrary(PCWSTR path, DWORD flags = 0) noexcept;

		DynamicLibrary(HMODULE hndl, DWORD flags) noexcept;

		DynamicLibrary(DynamicLibrary && right) noexcept ;

		DynamicLibrary & operator = (DynamicLibrary && right) noexcept;

		void swap(DynamicLibrary & right) noexcept;

		operator HMODULE () const noexcept;

		HMODULE get_hmodule() const noexcept;

		DWORD get_flags() const noexcept;

		bool get_path(PWSTR path, size_t size) const noexcept;

		FARPROC get_function(PCSTR name) const noexcept;

	private:
		HMODULE m_hnd;
		DWORD m_flags;
	};
}

#endif
