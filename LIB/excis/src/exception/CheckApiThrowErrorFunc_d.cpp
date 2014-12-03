#include "exception_pvt.hpp"

namespace exception {

	bool HiddenFunctions::CheckApiThrowErrorFunc(bool r, DWORD err, PCSTR file, size_t line, PCSTR func)
	{
		if (!r) {
			throw WinError(err, file, line, func);
		}
		return r;
	}

}
