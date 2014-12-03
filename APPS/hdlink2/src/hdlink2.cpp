#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

#ifdef DEBUG
#	include <basis/sys/traceback.hpp>
#endif

#include <console_messages.hpp>
#include <command_line.hpp>
#include <global.hpp>
#include <process.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
		LogSetOptions(L"logger:///module?name=user_console;prefix=0;level=tr");

#ifdef DEBUG
		traceback::init();
#endif
	}
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	setup_logger();

//	exception::set_vectored_exception_filter();
//	exception::set_unhandled_exception_filter();

//	try {
		{
			global::vars().cryptProvider.reset(new crypt::Provider);

			CommandLineParser(::GetCommandLineW()).execute();

			if (!global::vars().folders.empty())
				process_all_paths();
		}

		ShowStatistics().execute();

		return NO_ERROR;
//	} catch (exception::AbstractError & e) {
//		LogConsoleFatal(-1, L"%s\n", e.what().c_str());
//		LogConsoleFatal(-1, L"%s\n", e.where().c_str());
//	} catch (...) {
//		LogConsoleFatal(-1, L"std::exception\n");
//	}

	return 0;
}

///=========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int	mainCRTStartup() {
		crt::init_atexit();
		int ret = 0;

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		ret = wmain(argc, argv);

		::LocalFree(argv);

		crt::invoke_atexit();

		::ExitProcess(ret);
		return ret;
	}

	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}
}
#endif
