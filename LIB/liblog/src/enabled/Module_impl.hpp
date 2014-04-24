#ifndef LIBLOG_MODULE_IMPL_HPP_
#define LIBLOG_MODULE_IMPL_HPP_

#include <liblog/logger.hpp>

namespace logger {

	Module_i * create_Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl);

}

#endif
