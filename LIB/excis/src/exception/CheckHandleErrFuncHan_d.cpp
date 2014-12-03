#include "exception_pvt.hpp"

namespace exception {

	HANDLE HiddenFunctions::CheckHandleErrFuncHan(HANDLE hnd, PCSTR file, size_t line, PCSTR func)
	{
		if (hnd == nullptr || hnd == INVALID_HANDLE_VALUE ) {
			DWORD err = ::GetLastError();
			throw WinError(err, file, line, func);
		}
		return hnd;
	}

}
