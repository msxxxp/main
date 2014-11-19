#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

namespace console {

	size_t puts8(const wchar_t* str, Handle hnd)
	{
		static UINT codepage = GetConsoleOutputCP();
		size_t ret = 0;
		auto size = cstr::convert(str, codepage/*CP_UTF8*/);
		if (size) {
			auto buff = memory::malloc<char*>(size);
			cstr::convert(buff, size, str, codepage/*CP_UTF8*/);
			ret = puts(buff, size - 1, hnd);
			memory::free(buff);
		}
		return ret;
	}

}
