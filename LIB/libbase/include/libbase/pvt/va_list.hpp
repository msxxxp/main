#ifndef LIBBASE_VA_LIST_HPP_
#define LIBBASE_VA_LIST_HPP_

#include <stdarg.h>

namespace Base {

	struct Va_list {
		~Va_list()
		{
			va_end(m_args);
		}

//		va_list & get_args()
//		{
//			return m_args;
//		}

		operator va_list & ()
		{
			return m_args;
		}

	private:
		va_list m_args;
	};
}

#endif
