#include <basis/sys/traceback.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

namespace local {

	LogRegisterLocal(L"print_trace");

	void init_print_trace()
	{
		LogSetOptions(L"logger:///module?name=print_trace;prefix=fu;level=in");
		LogSetOptions(L"logger:///module?name=traceback;prefix=fu;level=tr");
		traceback::init();
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

	void print_trace(PCONTEXT context, void* address)
	{
		traceback::Enum tb(context, address);

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

void print_trace(PCONTEXT context, void* address)
{
	local::print_trace(context, address);
}
