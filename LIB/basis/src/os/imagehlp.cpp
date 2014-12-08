#include <basis/os/imagehlp.hpp>

namespace os {

	imagehlp_dll & imagehlp_dll::inst()
	{
		static imagehlp_dll ret;
		return ret;
	}

	imagehlp_dll::imagehlp_dll() :
		DynamicLibrary(L"imagehlp.dll")
	{
		GET_DLL_FUNC(ImageDirectoryEntryToData);
	}

}
