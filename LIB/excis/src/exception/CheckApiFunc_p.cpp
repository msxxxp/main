#include "exception_pvt.hpp"

namespace exception {

	bool HiddenFunctions::CheckApiFunc(bool r)
	{
		if (!r) {
			DWORD err = ::GetLastError();
			throw WinError(err);
		}
		return r;
	}

}
