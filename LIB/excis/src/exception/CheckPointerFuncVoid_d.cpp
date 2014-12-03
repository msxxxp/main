#include "exception_pvt.hpp"

namespace exception {

	PVOID HiddenFunctions::CheckPointerFuncVoid(PVOID ptr, PCSTR file, size_t line, PCSTR func)
	{
		if (!ptr) {
			throw WinError(E_POINTER, file, line, func);
		}
		return ptr;
	}

}
