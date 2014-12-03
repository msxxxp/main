#include "exception_pvt.hpp"

namespace exception {

	PVOID HiddenFunctions::CheckPointerErrFuncVoid(PVOID ptr)
	{
		if (!ptr) {
			DWORD err = ::GetLastError();
			throw WinError(err);
		}
		return ptr;
	}

}
