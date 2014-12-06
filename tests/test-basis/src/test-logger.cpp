#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>

void test_logger()
{
	LogTrace();
	LogDebug(L"\n");
	LogInfo(L"\n");
	LogReport(L"\n");
	LogAtten(L"\n");
	LogWarn(L"\n");
	LogError(L"\n");
	LogFatal(L"\n");
	LogAlert(L"\n");
	LogEmerg(L"\n");
	LogForce(L"\n");
}
