#include "exception_pvt.hpp"

namespace exception {

	PVOID HiddenFunctions::CheckPointerErrFuncVoid(PVOID ptr, PCSTR file, size_t line, PCSTR func)
	{
		if (!ptr) {
			DWORD err = ::GetLastError();
			throw WinError(err, file, line, func);
		}
		return ptr;
	}

}
