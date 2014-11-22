#include <libjava/jvm.hpp>
#include <libjava/exception.hpp>
#include <libjava/class.hpp>
#include <liblog/logger.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Full & ~Prefix::Date);
		set_default_target(get_TargetToConsole());
	}
}

int main() try {
	setup_logger();
	LogTrace();

	{
		Java::Env env = Java::create_vm("-Djava.class.path=C:\\test-swing.jar");

		env.get_class("org/andrewgrechkin/MainWindow").run();
	}

	return 0;
} catch (Ext::AbstractError & e) {
	LogError(L"exception cought: %s, %s\n", e.what().c_str(), e.where());
	return e.code();
} catch (std::exception & e) {
	LogError(L"std::exception [%S]:\n", typeid(e).name());
	LogError(L"What: %S\n", e.what());
	return 1;
}
