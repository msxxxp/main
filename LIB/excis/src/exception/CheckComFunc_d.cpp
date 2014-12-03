#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckComFunc(HRESULT res, PCSTR file, size_t line, PCSTR func)
	{
		if (FAILED(res)) {
			throw WinError(res, file, line, func);
		}
		return res;
	}

}
