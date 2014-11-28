#ifndef BASIS_SYS_LINKAGE_DLL_HPP_
#define BASIS_SYS_LINKAGE_DLL_HPP_

namespace linkage {

	struct DynamicLibrary: private pattern::Uncopyable {
		virtual ~DynamicLibrary() noexcept;

		virtual bool is_valid() const noexcept;

	public:
		DynamicLibrary(const wchar_t* path, DWORD flags = 0) noexcept;

		DynamicLibrary(HMODULE hndl, DWORD flags) noexcept;

		DynamicLibrary(DynamicLibrary&& other) noexcept;

		DynamicLibrary& operator =(DynamicLibrary&& other) noexcept;

		void swap(DynamicLibrary& other) noexcept;

		operator HMODULE() const noexcept;

		HMODULE get_hmodule() const noexcept;

		DWORD get_flags() const noexcept;

		bool get_path(wchar_t* path, size_t size) const noexcept;

		FARPROC get_function(const char* name) const noexcept;

	private:
		HMODULE m_hnd;
		DWORD m_flags;
	};

}

#endif
