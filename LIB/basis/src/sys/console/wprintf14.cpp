#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace {
	const size_t DEFAULT_PRINTF_BUFFER = 16 * 1024;
}

namespace console {

	size_t puts8(const wchar_t* str, Handle hnd)
	{
		static auto codepage = get_output_codepage();
		size_t ret = 0;
		auto size = cstr::convert(str, codepage/*CP_UTF8*/);
		if (size) {
			if (size < DEFAULT_PRINTF_BUFFER) {
				char buff[DEFAULT_PRINTF_BUFFER];
				cstr::convert(buff, size, str, codepage/*CP_UTF8*/);
				ret = puts(buff, size - 1, hnd);
			} else {
				auto buff = static_cast<char*>(HostAlloc(memory::heap::DefaultStat, size));
				cstr::convert(buff, size, str, codepage/*CP_UTF8*/);
				ret = puts(buff, size - 1, hnd);
				HostFree(memory::heap::DefaultStat, buff);
			}
		}
		return ret;
	}

}
