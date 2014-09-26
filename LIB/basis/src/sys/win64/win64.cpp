#include <basis/sys/win64.hpp>
#include <basis/os/kernel32.hpp>

namespace win64 {

	bool is_WOW64()
	{
		if (os::kernel32_dll::inst().IsWow64Process) {
			BOOL Result = false;
			if (os::kernel32_dll::inst().IsWow64Process(::GetCurrentProcess(), &Result) != 0) {
				return Result != 0;
			}
		}
		return false;
	}

	bool disable_WOW64(PVOID & oldValue)
	{
		if (os::kernel32_dll::inst().Wow64DisableWow64FsRedirection) {
			return os::kernel32_dll::inst().Wow64DisableWow64FsRedirection(&oldValue) != 0;
		}
		return false;
	}

	bool enable_WOW64(PVOID & oldValue)
	{
		if (os::kernel32_dll::inst().Wow64RevertWow64FsRedirection) {
			return os::kernel32_dll::inst().Wow64RevertWow64FsRedirection(&oldValue) != 0;
		}
		return false;
	}

}
