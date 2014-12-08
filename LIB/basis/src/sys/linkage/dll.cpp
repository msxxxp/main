#include <basis/sys/linkage.hpp>
#include <basis/simstd/algorithm>

namespace linkage {

	DynamicLibrary::~DynamicLibrary() noexcept
	{
		::FreeLibrary(m_hnd);
	}

	bool DynamicLibrary::is_valid() const noexcept
	{
		return m_hnd;
	}

	DynamicLibrary::DynamicLibrary(const wchar_t* path, DWORD flags) noexcept :
		m_hnd(::LoadLibraryExW(path, nullptr, flags)),
		m_flags(flags)
	{
	}

	DynamicLibrary::DynamicLibrary(HMODULE hndl, DWORD flags) noexcept :
		m_hnd(hndl),
		m_flags(flags)
	{
	}

	DynamicLibrary::DynamicLibrary(DynamicLibrary && right) noexcept :
		m_hnd(nullptr),
		m_flags(0)
	{
		swap(right);
	}

	DynamicLibrary & DynamicLibrary::operator = (DynamicLibrary && right) noexcept
	{
		if (this != &right)
			DynamicLibrary(simstd::move(right)).swap(*this);
		return *this;
	}

	void DynamicLibrary::swap(DynamicLibrary & right) noexcept
	{
		using simstd::swap;
		swap(m_hnd, right.m_hnd);
		swap(m_flags, right.m_flags);
	}

	DynamicLibrary::operator HMODULE () const noexcept
	{
		return m_hnd;
	}

	HMODULE DynamicLibrary::get_hmodule() const noexcept
	{
		return m_hnd;
	}

	DWORD DynamicLibrary::get_flags() const noexcept
	{
		return m_flags;
	}

	bool DynamicLibrary::get_path(wchar_t* path, size_t size) const noexcept
	{
		return ::GetModuleFileNameW(m_hnd, path, size);
	}

	FARPROC DynamicLibrary::get_function(const char* name) const noexcept
	{
		return ::GetProcAddress(m_hnd, name);
	}

}
