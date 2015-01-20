#ifndef BASIS_OS_WININET_
#define BASIS_OS_WININET_

#include <basis/sys/linkage.hpp>

namespace os {

	struct wininet_dll: private linkage::DynamicLibrary {
		typedef BOOL (WINAPI *FInternetCrackUrlW)(const wchar_t *, DWORD, DWORD, void *);

		DEFINE_FUNC(InternetCrackUrlW);

		static wininet_dll & inst();

		~wininet_dll();

	private:
		wininet_dll();
	};

}

#endif
