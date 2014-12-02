#include <excis/service.hpp>

#include <basis/sys/memory.hpp>

namespace service {

	memory::auto_buf<LPQUERY_SERVICE_CONFIGW> QueryConfig(SC_HANDLE hndl);

	memory::auto_buf<PBYTE> QueryConfig2(SC_HANDLE hndl, DWORD level);

}
