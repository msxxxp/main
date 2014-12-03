#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckWmiFunc(HRESULT res)
	{
		if (res != S_OK)
			throw WmiError(res);
		return res;
	}

}
