#include "exception_pvt.hpp"

namespace exception {

	int HiddenFunctions::CheckWSockFunc(int err)
	{
		if (err) {
			throw WSockError(::WSAGetLastError());
		}
		return err;
	}

}
