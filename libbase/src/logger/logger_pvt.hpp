#ifndef _LIBBASE_LOGGER_PVT_HPP_
#define _LIBBASE_LOGGER_PVT_HPP_

#include <libbase/logger.hpp>

namespace Logger {

	///==================================================================================== Logger_i
	struct Logger_i {
		Module_i * get_module(PCWSTR name);

		Module_i * register_module(PCWSTR name, const Target_t & target = get_default_target(), Level lvl = get_default_level());

		void free_module(Module_i * module);

		virtual ~Logger_i();

	private:
		virtual Module_i * get_module_(PCWSTR name) = 0;

		virtual Module_i * register_module_(PCWSTR name, const Target_t & target, Level lvl) = 0;

		virtual void free_module_(Module_i * module) = 0;
	};

	Logger_i & get_instance();
}

#endif
