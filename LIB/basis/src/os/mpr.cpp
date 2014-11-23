#include <basis/os/mpr.hpp>

namespace os {

	mpr_dll& mpr_dll::inst()
	{
		static mpr_dll ret;
		return ret;
	}

	mpr_dll::mpr_dll() :
		DynamicLibrary(L"mpr.dll")
	{
		GET_DLL_FUNC(WNetAddConnection2W);
		GET_DLL_FUNC(WNetCancelConnection2W);
	}

}
