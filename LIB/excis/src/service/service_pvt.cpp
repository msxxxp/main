#include "service_pvt.hpp"
#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

namespace service {

	memory::auto_buf<LPQUERY_SERVICE_CONFIGW> QueryConfig(SC_HANDLE hndl)
	{
		LogNoise2(L"handle: %p\n", hndl);
		DWORD bytesNeeded = 0;
		CheckApi(!::QueryServiceConfigW(hndl, nullptr, 0, &bytesNeeded) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		memory::auto_buf<LPQUERY_SERVICE_CONFIGW> buf(bytesNeeded);
		CheckApi(::QueryServiceConfigW(hndl, buf, buf.size(), &bytesNeeded));
		return buf;
	}

	memory::auto_buf<PBYTE> QueryConfig2(SC_HANDLE hndl, DWORD level)
	{
		LogNoise2(L"handle: %p\n", hndl);
		DWORD bytesNeeded = 0;
		CheckApi(!::QueryServiceConfig2W(hndl, level, nullptr, 0, &bytesNeeded) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		memory::auto_buf<PBYTE> buf(bytesNeeded);
		CheckApi(::QueryServiceConfig2W(hndl, level, buf, buf.size(), &bytesNeeded));
		return buf;
	}
}
