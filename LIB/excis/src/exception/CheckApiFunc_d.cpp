#include "exception_pvt.hpp"

namespace exception {

	bool HiddenFunctions::CheckApiFunc(bool r, PCSTR file, size_t line, PCSTR func)
	{
		if (!r) {
			DWORD err = ::GetLastError();
			throw WinError(err, file, line, func);
		}
		return r;
	}

}
