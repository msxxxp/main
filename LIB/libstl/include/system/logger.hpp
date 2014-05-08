#ifndef LIBSTL_SYSTEM_LOGGER_HPP_
#define LIBSTL_SYSTEM_LOGGER_HPP_

#include <system/configure.hpp>
#include <system/console.hpp>

#if (defined(DEBUG) || defined(ENABLE_LOGGER)) && !defined(DISABLE_LOGGER)
#   define LogSetOptions(url)                 logger::set_options(url)
#   define LogTrace()                         get_logger_module()->out(THIS_PLACE, logger::Level::Trace, L"\n")
#   define LogTraceIf(condition)              if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Trace, L"\n")
#   define LogTraceObj()                      get_logger_module()->out(THIS_PLACE, logger::Level::Trace, L"this: %p\n", this)
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
#   define LogNoise(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Trace, format, ##__VA_ARGS__)
#   define LogNoiseIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Trace, format, ##__VA_ARGS__)
#   define LogDebug(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Debug, format, ##__VA_ARGS__)
#   define LogDebugIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Debug, format, ##__VA_ARGS__)
#   define LogInfo(format, ...)               get_logger_module()->out(THIS_PLACE, logger::Level::Info, format, ##__VA_ARGS__)
#   define LogInfoIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Info, format, ##__VA_ARGS__)
#   define LogReport(format, ...)             get_logger_module()->out(THIS_PLACE, logger::Level::Report, format, ##__VA_ARGS__)
#   define LogReportIf(condition,format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Report, format, ##__VA_ARGS__)
#   define LogAtten(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Atten, format, ##__VA_ARGS__)
#   define LogAttenIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Atten, format, ##__VA_ARGS__)
#   define LogWarn(format, ...)               get_logger_module()->out(THIS_PLACE, logger::Level::Warn, format, ##__VA_ARGS__)
#   define LogWarnIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Warn, format, ##__VA_ARGS__)
#   define LogError(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Error, format, ##__VA_ARGS__)
#   define LogErrorIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Error, format, ##__VA_ARGS__)
#   define LogFatal(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Fatal, format, ##__VA_ARGS__)
#   define LogFatalIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Fatal, format, ##__VA_ARGS__)
#   define LogAlert(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Alert, format, ##__VA_ARGS__)
#   define LogAlertIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Alert, format, ##__VA_ARGS__)
#   define LogEmerg(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Emerg, format, ##__VA_ARGS__)
#   define LogEmergIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Emerg, format, ##__VA_ARGS__)
#   define LogForce(format, ...)              get_logger_module()->out(THIS_PLACE, logger::Level::Force, format, ##__VA_ARGS__)
#   define LogForceIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, logger::Level::Force, format, ##__VA_ARGS__)
//#   endif
#	define LogDeclare() \
logger::Module_i * get_logger_module();
#	define LogRegister(name) \
logger::Module_i * get_logger_module() \
{ \
	auto static module = logger::get_module(name); \
	return module; \
}
#	define LogRegisterLocal(name) \
namespace { \
	logger::Module_i * get_logger_module() \
	{ \
		auto static module = logger::get_module(name); \
		return module; \
	} \
}
#else
#   define LogSetOptions(url)
#   define LogTrace()
#   define LogTraceIf(condition)                       (void)(condition)
#   define LogTraceObj()
#   define LogNoise(format, ...)
#   define LogNoiseIf(condition, format, ...)          (void)(condition)
#   define LogDebug(format, ...)
#   define LogDebugIf(condition, format, ...)          (void)(condition)
#   define LogInfo(format, ...)
#   define LogInfoIf(condition, format, ...)           (void)(condition)
#   define LogReport(format, ...)
#   define LogReportIf(condition, format, ...)         (void)(condition)
#   define LogAtten(format, ...)
#   define LogAttenIf(condition, format, ...)          (void)(condition)
#   define LogWarn(format, ...)
#   define LogWarnIf(condition, format, ...)           (void)(condition)
#   define LogError(format, ...)
#   define LogErrorIf(condition, format, ...)          (void)(condition)
#   define LogFatal(format, ...)
#   define LogFatalIf(condition, format, ...)          (void)(condition)
#   define LogAlert(format, ...)
#   define LogAlertIf(condition, format, ...)          (void)(condition)
#   define LogEmerg(format, ...)
#   define LogEmergIf(condition, format, ...)          (void)(condition)
#   define LogForce(format, ...)                       console::printf(format, ##__VA_ARGS__)
#   define LogForceIf(condition, format, ...)          if (condition) console::printf(format, ##__VA_ARGS__)
#	define LogDeclare()
#	define LogRegister(name)
#	define LogRegisterLocal(name)
#endif

namespace logger {

	enum class Level : ssize_t {
		Trace,
		Debug,
		Info,
		Report,
		Atten,
		Warn,
		Error,
		Fatal,
		Alert,
		Emerg,
		Force,
	};

	const wchar_t * to_str(Level lvl);

	void set_options(const wchar_t * url);

	///==================================================================================== Module_i
	struct Module_i {
		virtual ~Module_i();

		virtual bool is_color_mode() const = 0;

		virtual void out(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const = 0;

		virtual void out(Level lvl, const wchar_t * format, ...) const = 0;
	};

	Module_i * get_module(const wchar_t * name);

	namespace defaults {

		Module_i * get_module();

	}  // namespace defaults

}  // namespace logger

inline logger::Module_i * get_logger_module()
{
	return logger::defaults::get_module();
}

#endif
