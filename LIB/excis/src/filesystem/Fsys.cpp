#include <basis/sys/logger.hpp>
#include <excis/filesystem.hpp>

namespace fsys {
	logger::Module_i * get_logger_module()
	{
		auto static module = logger::get_module(L"fsys");
		return module;
	}
}
