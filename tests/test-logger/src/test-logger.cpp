#include <liblog/logger.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Full/* | Prefix::Place*/);
		set_default_target(get_TargetToConsole());
	}
}

int main() {
	setup_logger();

	LogTrace();

	return 0;
}
