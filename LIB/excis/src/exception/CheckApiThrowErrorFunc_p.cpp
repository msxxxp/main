#include "exception_pvt.hpp"

namespace exception {

	bool HiddenFunctions::CheckApiThrowErrorFunc(bool r, DWORD err)
	{
		if (!r) {
			throw WinError(err);
		}
		return r;
	}

}
