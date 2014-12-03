#include "exception_pvt.hpp"

namespace exception {

	HRESULT HiddenFunctions::CheckComFunc(HRESULT res)
	{
		if (FAILED(res))
			throw WinError(res);
		return res;
	}

}
