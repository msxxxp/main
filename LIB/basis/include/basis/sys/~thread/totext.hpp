#ifndef BASIS_SYS_THREAD_TOTEXT_HPP_
#define BASIS_SYS_THREAD_TOTEXT_HPP_

#include <basis/sys/thread.hpp>

namespace totext {

	const wchar_t* c_str(thread::Priority prio);

	const wchar_t* c_str(thread::IoPriority prio);

}

#endif
