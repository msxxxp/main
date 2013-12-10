#include <libbase/std.hpp>

#include <stdarg.h>

namespace Base {

	Va_list::~Va_list()
	{
		va_end(m_args);
	}

}
