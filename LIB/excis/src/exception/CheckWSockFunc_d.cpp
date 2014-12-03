#include "exception_pvt.hpp"

namespace exception {

	int HiddenFunctions::CheckWSockFunc(int err, PCSTR file, size_t line, PCSTR func)
	{
		if (err) {
			throw WSockError(::WSAGetLastError(), file, line, func);
		}
		return err;
	}

}
