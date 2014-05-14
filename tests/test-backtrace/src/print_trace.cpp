#include <system/traceback.hpp>
#include <system/logger.hpp>
#include <simstd/string>

namespace local {

	LogRegisterLocal(L"print_trace");

	void init_print_trace()
	{
		LogSetOptions(L"logger:///module?name=print_trace;prefix=48");
	}

	void print_trace()
	{
		traceback::Enum tb;

		//	auto module = get_logger_module();
		//	auto scopeLock(module->lock_scope());
		LogForce(L"Backtrace: [%Iu]\n", tb.size());
		for (size_t i = 0; i < tb.size(); ++i)
			LogForce(L"[%02Iu] %s\n", tb.size() - (i + 1), tb[i].to_str().c_str());
	}

}

void init_print_trace()
{
	local::init_print_trace();
}

void print_trace()
{
	local::print_trace();
}
