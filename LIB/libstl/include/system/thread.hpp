#ifndef LIBSTL_SYSTEM_THREAD_
#define LIBSTL_SYSTEM_THREAD_

#include <system/configure.hpp>

#include <liblog/logger.hpp>

namespace thread {
	LogDeclare();
}

#include <system/sub_thread/base.hpp>
#include <system/sub_thread/totext.hpp>
#include <system/sub_thread/Routine.hpp>
#include <system/sub_thread/Unit.hpp>
#include <system/sub_thread/Pool.hpp>

#endif
