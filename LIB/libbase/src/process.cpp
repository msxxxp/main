#include <libbase/std.hpp>
#include <libbase/string.hpp>

namespace Base {
	namespace Process {

		ustring get_path()
		{
			wchar_t buf[MAX_PATH_LEN];
			::GetModuleFileNameW(nullptr, buf, lengthof(buf));
			return ustring(buf);
		}

	}
}
