#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckWmiFunc(HRESULT res, PCSTR file, size_t line, PCSTR func)
	{
		if (res != S_OK)
			throw WmiError(res, file, line, func);
		return res;
	}

}
