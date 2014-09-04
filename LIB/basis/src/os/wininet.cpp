#include <basis/os/wininet.hpp>

namespace os {

	wininet_dll & wininet_dll::inst()
	{
		static wininet_dll ret;
		return ret;
	}

	wininet_dll::wininet_dll() :
		DynamicLibrary(L"wininet.dll")
	{
			GET_DLL_FUNC(InternetCrackUrlW);
	}

}
