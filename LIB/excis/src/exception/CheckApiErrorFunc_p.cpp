#include "exception_pvt.hpp"

namespace exception {

	DWORD HiddenFunctions::CheckApiErrorFunc(DWORD err)
	{
		if (err != ERROR_SUCCESS) {
			throw WinError(err);
		}
		return err;
	}

}
