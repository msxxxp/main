#ifndef BASIS_OS_MPR_
#define BASIS_OS_MPR_

#include <basis/sys/linkage.hpp>

namespace os {

	struct mpr_dll: private linkage::DynamicLibrary {
		typedef DWORD (WINAPI *FWNetAddConnection2W)(LPNETRESOURCEW, LPCWSTR, LPCWSTR, DWORD);
		typedef DWORD (WINAPI *FWNetCancelConnection2W)(LPCWSTR, DWORD, WINBOOL);

		DEFINE_FUNC(WNetAddConnection2W);
		DEFINE_FUNC(WNetCancelConnection2W);

		static mpr_dll& inst();

	private:
		mpr_dll();
	};

}

#endif
