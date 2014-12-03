#include "exception_pvt.hpp"

namespace exception {

	HANDLE HiddenFunctions::CheckHandleFuncHan(HANDLE hnd, PCSTR file, size_t line, PCSTR func)
	{
		if (hnd == nullptr || hnd == INVALID_HANDLE_VALUE ) {
			throw WinError(ERROR_INVALID_HANDLE, file, line, func);
		}
		return hnd;
	}

}
