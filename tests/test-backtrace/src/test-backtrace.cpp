#include <system/traceback.hpp>
#include <simstd/string>

#include <liblog/logger.hpp>

namespace {
	void setup_logger()
	{
		using namespace logger;
		Default::set_level(Level::Trace);
		Default::set_prefix(Prefix::Full);//Prefix::Medium | Prefix::Place);
		Default::set_target(get_TargetToConsole());
	}
}

void Print()
{
	traceback::Enum tb;

	auto module = get_logger_module();
	auto scopeLock(module->lock_scope());
	auto savedPrefix = logger::get_prefix(module);
	logger::set_prefix(module, logger::Prefix::Full);//logger::Prefix::Time | logger::Prefix::Thread);
	LogForce(L"Backtrace: [%Iu]\n", tb.size());
	for (size_t i = 0; i < tb.size(); ++i)
		LogForce(L"[%02Iu] %s\n", tb.size() - (i + 1), tb[i].to_str().c_str());
	logger::set_prefix(module, savedPrefix);
}

void test_backtrace()
{
	LogTrace();
	Print();
}

int main()
{
	setup_logger();

	test_backtrace();

	return 0;
}
