#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>

#include <basis/std/string>

namespace path {

	ustring temp_file(const wchar_t * path)
	{
		wchar_t buf[MAX_PATH];
		wchar_t pid[32];
		buf[0] = 0;
		cstr::convert_num(pid, ::GetCurrentProcessId());
		::GetTempFileNameW(path, pid, 0, buf);
		return buf;
	}

}
