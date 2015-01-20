#include <basis/os/wininet.hpp>

namespace os {

	wininet_dll & wininet_dll::inst()
	{
		static wininet_dll ret;
		return ret;
	}

	wininet_dll::~wininet_dll()
	{
		TraceFunc();
	}

	wininet_dll::wininet_dll() :
		DynamicLibrary(L"wininet.dll")
	{
		TraceFunc();
		GET_DLL_FUNC(InternetCrackUrlW);
	}

}
