#include <basis/sys/configure.hpp>

#include <stdarg.h>

Va_list::~Va_list()
{
	va_end(m_args);
}
