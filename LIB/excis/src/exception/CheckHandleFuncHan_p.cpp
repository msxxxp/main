#include "exception_pvt.hpp"

namespace exception {

	HANDLE HiddenFunctions::CheckHandleFuncHan(HANDLE hnd)
	{
		if (!hnd || hnd == INVALID_HANDLE_VALUE) {
			throw WinError(ERROR_INVALID_HANDLE);
		}
		return hnd;
	}

}
