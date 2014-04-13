#ifndef LIBSTL_SYSTEM_THREAD_BASE_HPP_
#define LIBSTL_SYSTEM_THREAD_BASE_HPP_

#include <system/thread.hpp>

namespace thread {

	enum class Priority: ssize_t {
		IDLE          = THREAD_PRIORITY_IDLE,
		LOWEST        = THREAD_PRIORITY_LOWEST,
		BELOW_NORMAL  = THREAD_PRIORITY_BELOW_NORMAL,
		NORMAL        = THREAD_PRIORITY_NORMAL,
		ABOVE_NORMAL  = THREAD_PRIORITY_ABOVE_NORMAL,
		HIGHEST       = THREAD_PRIORITY_HIGHEST,
		TIME_CRITICAL = THREAD_PRIORITY_TIME_CRITICAL,
	};

	enum class IoPriority: ssize_t {
		VERY_LOW,
		LOW,
		NORMAL,
		HIGH,
		CRITICAL,
	};

	const wchar_t * to_str(Priority prio);

	const wchar_t * to_str(IoPriority prio);
}

#endif
