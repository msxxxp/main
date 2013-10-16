#include <libext/filesystem.hpp>
#include <liblog/logger.hpp>

namespace Fsys {
	Logger::Module_i * get_logger_module()
	{
		auto static module = Logger::get_module(L"Fsys");
		return module;
	}
}
