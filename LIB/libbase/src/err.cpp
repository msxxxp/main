#include <libbase/err.hpp>

#include <algorithm>

namespace Base {

	sstr ErrAsStr(DWORD err, PCWSTR lib) {
		HMODULE mod = nullptr;
		if (err && lib) {
			mod = ::LoadLibraryExW(lib, nullptr, DONT_RESOLVE_DLL_REFERENCES); //LOAD_LIBRARY_AS_DATAFILE
		}
		PWSTR buf = nullptr;
		::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | ((mod) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM), mod, err,
		                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //GetSystemDefaultLangID(),
		                 (PWSTR)&buf, 0, nullptr);

		if (mod)
			::FreeLibrary(mod);

		if (!buf) {
			if (lib) {
				return ErrAsStr(err);
			} else {
				return sstr::format(L"[0x%x (%u)] Unknown error", err, err);
			}
		}

		sstr ret = sstr::format(L"[0x%x (%u)] %s", err, err, buf);
		ret[ret.size() - 2] = L'\0'; // delete return char
		::LocalFree(buf);

		return std::move(ret);
	}

	sstr NTStatusAsStr(ULONG status)
	{
		return ErrAsStr(status, L"NTDLL.DLL");
	}

	sstr ErrAsStrWmi(HRESULT err)
	{
		return ErrAsStr(err, L"WMIUTILS.DLL");
	}

}
