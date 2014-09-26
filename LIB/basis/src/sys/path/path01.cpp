#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/string>

namespace path {

	ustring get_work_directory()
	{
		memory::auto_array<wchar_t> Result(::GetCurrentDirectoryW(0, nullptr));
		::GetCurrentDirectoryW(Result.size(), Result.data());
		return ustring(Result.data());
	}

}
