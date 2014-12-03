#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckHMailErrorFunc(HRESULT err, PCSTR file, size_t line, PCSTR func)
	{
		if (err != ERROR_SUCCESS) {
			throw HMailError(err, file, line, func);
		}
		return err;
	}

}
