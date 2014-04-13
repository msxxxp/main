#ifndef LIBSTL_SYSTEM_THREAD_TOTEXT_HPP_
#define LIBSTL_SYSTEM_THREAD_TOTEXT_HPP_

#include <system/thread.hpp>

namespace totext {

	const wchar_t * c_str(thread::Priority prio);

	const wchar_t * c_str(thread::IoPriority prio);

}

#endif
