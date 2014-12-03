#include "exception_pvt.hpp"

namespace exception {

	PVOID HiddenFunctions::CheckPointerFuncVoid(PVOID ptr)
	{
		if (!ptr) {
			throw WinError(E_POINTER);
		}
		return ptr;
	}

}
