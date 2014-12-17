#ifndef BASIS_SYS_LOGGER_MACROS_HPP_
#define BASIS_SYS_LOGGER_MACROS_HPP_

#if (defined(DEBUG) || defined(ENABLE_LOGGER)) && !defined(DISABLE_LOGGER)
#   define LogSetOptions(url)                                logger::set_options(url)
#   define LogTraceObj()                                     get_logger_module()->out_with_place(THIS_PLACE, logger::Level::TrObj, L"[%p, %I64u]\n", this, sizeof(*this))
#   define LogTraceObjBegin()                                get_logger_module()->out_with_place(THIS_PLACE, logger::Level::TrObj, L"[%p, %I64u] begin\n", this, sizeof(*this))
#   define LogTraceObjEnd()                                  get_logger_module()->out_with_place(THIS_PLACE, logger::Level::TrObj, L"[%p, %I64u] end\n", this, sizeof(*this))
#   define LogTrace2()                                       get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace2, L"\n")
#   define LogTrace2If(condition)             if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace2, L"\n")
#   define LogTrace()                                        get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace1, L"\n")
#   define LogTraceIf(condition)              if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace1, L"\n")
//#   ifdef _MSC_VER
//#       define LogNoise(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Trace, format, __VA_ARGS__)
//#       define LogNoiseIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Trace, format, __VA_ARGS__)
//#       define LogDebug(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Debug, format, __VA_ARGS__)
//#       define LogDebugIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Debug, format, __VA_ARGS__)
//#       define LogInfo(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Info, format, __VA_ARGS__)
//#       define LogReport(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Report, format, __VA_ARGS__)
//#       define LogAtten(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Atten, format, __VA_ARGS__)
//#       define LogWarn(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Warn, format, __VA_ARGS__)
//#       define LogWarnIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Warn, format, __VA_ARGS__)
//#       define LogError(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Error, format, __VA_ARGS__)
//#       define LogErrorIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Error, format, __VA_ARGS__)
//#       define LogFatal(format, ...)	            get_logger_module()->out(THIS_PLACE, logger::Level::Fatal, format, __VA_ARGS__)
//#   else
#   define LogNoise2(format, ...)                             get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace2, format, ##__VA_ARGS__)
#   define LogNoise2If(condition, format, ...) if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace2, format, ##__VA_ARGS__)
#   define LogNoise(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace1, format, ##__VA_ARGS__)
#   define LogNoiseIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Trace1, format, ##__VA_ARGS__)
#   define LogDebug2(format, ...)                             get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Debug2, format, ##__VA_ARGS__)
#   define LogDebug2If(condition, format, ...) if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Debug2, format, ##__VA_ARGS__)
#   define LogDebug(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Debug1, format, ##__VA_ARGS__)
#   define LogDebugIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Debug1, format, ##__VA_ARGS__)
#   define LogInfo(format, ...)                               get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Info, format, ##__VA_ARGS__)
#   define LogInfoIf(condition, format, ...)   if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Info, format, ##__VA_ARGS__)
#   define LogReport(format, ...)                             get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Report, format, ##__VA_ARGS__)
#   define LogReportIf(condition, format, ...) if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Report, format, ##__VA_ARGS__)
#   define LogAtten(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Atten, format, ##__VA_ARGS__)
#   define LogAttenIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Atten, format, ##__VA_ARGS__)
#   define LogWarn(format, ...)                               get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Warn, format, ##__VA_ARGS__)
#   define LogWarnIf(condition, format, ...)   if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Warn, format, ##__VA_ARGS__)
#   define LogError(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Error, format, ##__VA_ARGS__)
#   define LogErrorIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Error, format, ##__VA_ARGS__)
#   define LogFatal(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Fatal, format, ##__VA_ARGS__)
#   define LogFatalIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Fatal, format, ##__VA_ARGS__)
#   define LogAlert(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Alert, format, ##__VA_ARGS__)
#   define LogAlertIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Alert, format, ##__VA_ARGS__)
#   define LogEmerg(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Emerg, format, ##__VA_ARGS__)
#   define LogEmergIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Emerg, format, ##__VA_ARGS__)
#   define LogForce(format, ...)                              get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Force, format, ##__VA_ARGS__)
#   define LogForceIf(condition, format, ...)  if (condition) get_logger_module()->out_with_place(THIS_PLACE, logger::Level::Force, format, ##__VA_ARGS__)
//#   endif

#	define LogDeclare() \
logger::Module* get_logger_module();

#	define LogRegister(name) \
logger::Module* get_logger_module() \
{ \
	auto static module = logger::get_module(name); \
	return module; \
}

#	define LogRegisterClass(className, name) \
logger::Module* className::get_logger_module() \
{ \
	auto static module = logger::get_module(name); \
	return module; \
}

#	define LogRegisterLocal(name) \
namespace { \
	logger::Module* get_logger_module() \
	{ \
		auto static module = logger::get_module(name); \
		return module; \
	} \
}

#else
#   include <basis/sys/console.hpp>
#   define LogSetOptions(url)
#   define LogTraceObj()
#   define LogTraceObjBegin()
#   define LogTraceObjEnd()
#   define LogTrace()
#   define LogTraceIf(condition)                          (void)(condition)
#   define LogNoise2(format, ...)
#   define LogNoise2If(condition, format, ...)            (void)(condition)
#   define LogNoise(format, ...)
#   define LogNoiseIf(condition, format, ...)             (void)(condition)
#   define LogDebug2(format, ...)
#   define LogDebug2If(condition, format, ...)            (void)(condition)
#   define LogDebug(format, ...)
#   define LogDebugIf(condition, format, ...)             (void)(condition)
#   define LogInfo(format, ...)
#   define LogInfoIf(condition, format, ...)              (void)(condition)
#   define LogReport(format, ...)
#   define LogReportIf(condition, format, ...)            (void)(condition)
#   define LogAtten(format, ...)
#   define LogAttenIf(condition, format, ...)             (void)(condition)
#   define LogWarn(format, ...)
#   define LogWarnIf(condition, format, ...)              (void)(condition)
#   define LogError(format, ...)
#   define LogErrorIf(condition, format, ...)             (void)(condition)
#   define LogFatal(format, ...)
#   define LogFatalIf(condition, format, ...)             (void)(condition)
#   define LogAlert(format, ...)
#   define LogAlertIf(condition, format, ...)             (void)(condition)
#   define LogEmerg(format, ...)
#   define LogEmergIf(condition, format, ...)             (void)(condition)
#   define LogForce(format, ...)                          console::printf(format, ##__VA_ARGS__)
#   define LogForceIf(condition, format, ...)             if (condition) console::printf(format, ##__VA_ARGS__)
#	define LogDeclare()
#	define LogRegister(name)
#	define LogRegisterLocal(name)
#endif

#define LogConsole(color, level, format, ...)                            get_console_logger_module()->out_with_color((WORD)color, level, format, ##__VA_ARGS__)
#define LogConsoleNoise2(color, format, ...)                             get_console_logger_module()->out_with_color((WORD)color, logger::Level::Trace2, format, ##__VA_ARGS__)
#define LogConsoleNoise2If(condition, color, format, ...) if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Trace2, format, ##__VA_ARGS__)
#define LogConsoleNoise(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Trace1, format, ##__VA_ARGS__)
#define LogConsoleNoiseIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Trace1, format, ##__VA_ARGS__)
#define LogConsoleDebug2(color, format, ...)                             get_console_logger_module()->out_with_color((WORD)color, logger::Level::Debug2, format, ##__VA_ARGS__)
#define LogConsoleDebug2If(condition, color, format, ...) if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Debug2, format, ##__VA_ARGS__)
#define LogConsoleDebug(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Debug1, format, ##__VA_ARGS__)
#define LogConsoleDebugIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Debug1, format, ##__VA_ARGS__)
#define LogConsoleInfo(color, format, ...)                               get_console_logger_module()->out_with_color((WORD)color, logger::Level::Info, format, ##__VA_ARGS__)
#define LogConsoleInfoIf(condition, color, format, ...)   if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Info, format, ##__VA_ARGS__)
#define LogConsoleReport(color, format, ...)                             get_console_logger_module()->out_with_color((WORD)color, logger::Level::Report, format, ##__VA_ARGS__)
#define LogConsoleReportIf(condition, color, format, ...) if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Report, format, ##__VA_ARGS__)
#define LogConsoleAtten(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Atten, format, ##__VA_ARGS__)
#define LogConsoleAttenIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Atten, format, ##__VA_ARGS__)
#define LogConsoleWarn(color, format, ...)                               get_console_logger_module()->out_with_color((WORD)color, logger::Level::Warn, format, ##__VA_ARGS__)
#define LogConsoleWarnIf(condition, color, format, ...)   if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Warn, format, ##__VA_ARGS__)
#define LogConsoleError(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Error, format, ##__VA_ARGS__)
#define LogConsoleErrorIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Error, format, ##__VA_ARGS__)
#define LogConsoleFatal(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Fatal, format, ##__VA_ARGS__)
#define LogConsoleFatalIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Fatal, format, ##__VA_ARGS__)
#define LogConsoleAlert(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Alert, format, ##__VA_ARGS__)
#define LogConsoleAlertIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Alert, format, ##__VA_ARGS__)
#define LogConsoleEmerg(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Emerg, format, ##__VA_ARGS__)
#define LogConsoleEmergIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Emerg, format, ##__VA_ARGS__)
#define LogConsoleForce(color, format, ...)                              get_console_logger_module()->out_with_color((WORD)color, logger::Level::Force, format, ##__VA_ARGS__)
#define LogConsoleForceIf(condition, color, format, ...)  if (condition) get_console_logger_module()->out_with_color((WORD)color, logger::Level::Force, format, ##__VA_ARGS__)

#endif
