#include "exception_pvt.hpp"

namespace exception {

	HANDLE HiddenFunctions::CheckHandleErrFuncHan(HANDLE hnd)
	{
		if (!hnd || hnd == INVALID_HANDLE_VALUE) {
			DWORD err = ::GetLastError();
			throw WinError(err);
		}
		return hnd;
	}

}
