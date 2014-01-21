#ifndef LIBLOG_LOGGER_IMPL_HPP_
#define LIBLOG_LOGGER_IMPL_HPP_

#include <liblog/logger.hpp>

namespace logger {

	///==================================================================================== Logger_i
	struct Logger_i {
		virtual ~Logger_i();

		virtual Module_i * get_module(const wchar_t * name) = 0;

		virtual Module_i * register_module(const wchar_t * name, const Target_t & target, Level lvl) = 0;

		virtual void free_module(Module_i * module) = 0;
	};

	Logger_i & get_instance();

}

#endif
