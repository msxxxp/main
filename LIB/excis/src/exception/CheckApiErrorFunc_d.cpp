#include "exception_pvt.hpp"

namespace exception {

	DWORD HiddenFunctions::CheckApiErrorFunc(DWORD err, PCSTR file, size_t line, PCSTR func)
	{
		if (err != ERROR_SUCCESS) {
			throw WinError(err, file, line, func);
		}
		return err;
	}

}
