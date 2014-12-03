#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckHMailErrorFunc(HRESULT err)
	{
		if (err != ERROR_SUCCESS) {
			throw HMailError(err);
		}
		return err;
	}

}
