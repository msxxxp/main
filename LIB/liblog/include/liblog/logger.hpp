#ifndef LIBLOG_LOGGER_HPP_
#define LIBLOG_LOGGER_HPP_

#include <libbase/std.hpp>
#include <libbase/lock.hpp>

#ifdef ENABLE_LOGGER
#include <memory>
#else
#include <libbase/console.hpp>
#endif

#ifdef ENABLE_LOGGER
#       define LogTrace()                         get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, L"\n")
#       define LogTraceIf(condition)              if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, L"\n")
//#   ifdef _MSC_VER
//#       define LogNoise(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, __VA_ARGS__)
//#       define LogNoiseIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, __VA_ARGS__)
//#       define LogDebug(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, __VA_ARGS__)
//#       define LogDebugIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, __VA_ARGS__)
//#       define LogInfo(format, ...)	              get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, __VA_ARGS__)
//#       define LogReport(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, __VA_ARGS__)
//#       define LogAtten(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, __VA_ARGS__)
//#       define LogWarn(format, ...)	              get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, __VA_ARGS__)
//#       define LogWarnIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, __VA_ARGS__)
//#       define LogError(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, __VA_ARGS__)
//#       define LogErrorIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, __VA_ARGS__)
//#       define LogFatal(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, __VA_ARGS__)
//#   else
#       define LogNoise(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, ##__VA_ARGS__)
#       define LogNoiseIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, ##__VA_ARGS__)
#       define LogDebug(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, ##__VA_ARGS__)
#       define LogDebugIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, ##__VA_ARGS__)
#       define LogInfo(format, ...)               get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, ##__VA_ARGS__)
#       define LogInfoIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, ##__VA_ARGS__)
#       define LogReport(format, ...)             get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, ##__VA_ARGS__)
#       define LogReportIf(condition,format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, ##__VA_ARGS__)
#       define LogAtten(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, ##__VA_ARGS__)
#       define LogAttenIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, ##__VA_ARGS__)
#       define LogWarn(format, ...)               get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, ##__VA_ARGS__)
#       define LogWarnIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, ##__VA_ARGS__)
#       define LogError(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, ##__VA_ARGS__)
#       define LogErrorIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, ##__VA_ARGS__)
#       define LogFatal(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, ##__VA_ARGS__)
#       define LogFatalIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, ##__VA_ARGS__)
#       define LogAlert(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Alert, format, ##__VA_ARGS__)
#       define LogAlertIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Alert, format, ##__VA_ARGS__)
#       define LogEmerg(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Emerg, format, ##__VA_ARGS__)
#       define LogEmergIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Emerg, format, ##__VA_ARGS__)
#       define LogForce(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Force, format, ##__VA_ARGS__)
#       define LogForceIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Force, format, ##__VA_ARGS__)
//#   endif
#else
#   define LogTrace()
#   define LogTraceIf(condition)                       (void)(condition)
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
#   define LogForce(format, ...)                       Base::Console::printf(format, ##__VA_ARGS__)
#   define LogForceIf(condition, format, ...)          if (condition) Base::Console::printf(format, ##__VA_ARGS__)
#endif

namespace Logger {
	struct Module_i;
	struct Target_i;
}

namespace Logger {

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

	namespace Prefix {
		typedef size_t flags;
		const static flags Function = 0x0001;
		const static flags Level    = 0x0002;
		const static flags Module   = 0x0004;
		const static flags Place    = 0x0008;
		const static flags Thread   = 0x0010;
		const static flags Time     = 0x0020;
		const static flags Date     = 0x0040;

		const static flags Lite     = Time | Level;
		const static flags Medium   = Time | Level | Function;
		const static flags Full     = Time | Level | Function | Date | Module | Thread | Place;
	}

#ifdef ENABLE_LOGGER
	///==================================================================================== Target_i
	struct Target_i {
		virtual ~Target_i();

		virtual void out(const Module_i * module, Level lvl, const wchar_t * str, size_t size) const = 0;

		virtual void out(const wchar_t * str, size_t size) const = 0;

		virtual Lock::ScopeGuard lock_scope() const = 0;
	};

	typedef std::shared_ptr<Target_i> Target_t;

	Target_t get_TargetToNull();

	Target_t get_TargetToConsole();

	Target_t get_TargetToFile(const wchar_t * path, bool overwrite = false);

	Target_t get_TargetToSys(const wchar_t * name, const wchar_t * path = nullptr);

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second);

	///==================================================================================== Module_i
	struct Module_i: public Base::Destroyable {
		virtual ~Module_i();

		virtual const wchar_t * get_name() const = 0;

		virtual Level get_level() const = 0;

		virtual size_t get_prefix() const = 0;

		virtual bool is_color_mode() const = 0;

		virtual void set_level(Level lvl) = 0;

		virtual void set_prefix(size_t prefix) = 0;

		virtual void set_color_mode(bool mode) = 0;

		virtual void set_target(const Target_t & target) = 0;

		virtual void set_enabled(bool enabled) = 0;

		virtual void out(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const = 0;

		virtual void out(Level lvl, const wchar_t * format, ...) const = 0;

		virtual Lock::ScopeGuard lock_scope() const = 0;
	};

	namespace Default {
		Level get_level();

		void set_level(Level lvl);

		size_t get_prefix();

		void set_prefix(size_t prefix);

		Target_t get_target();

		void set_target(Target_t target);

		Module_i * get_module();
	}  // namespace Default

	Module_i * get_module(const wchar_t * name, const Target_t & target = Default::get_target(), Level lvl = Default::get_level());

	inline void set_target(Module_i * module, const Target_t & target)
	{
		module->set_target(target);
	}

	inline void set_level(Module_i * module, Level lvl)
	{
		module->set_level(lvl);
	}

	inline void set_prefix(Module_i * module, size_t prefix)
	{
		module->set_prefix(prefix);
	}

	inline Prefix::flags get_prefix(Module_i * module)
	{
		return module->get_prefix();
	}

	inline Level get_level(Module_i * module)
	{
		return module->get_level();
	}

	inline void set_color_mode(Module_i * module, bool mode)
	{
		module->set_color_mode(mode);
	}

	inline void set_enabled(Module_i * module, bool enabled)
	{
		module->set_enabled(enabled);
	}

	inline Lock::ScopeGuard lock_scope(Module_i * module)
	{
		return module->lock_scope();
	}

#else
	typedef Target_i * Target_t;

	inline Target_t get_TargetToNull()
	{
		return Target_t();
	}

	inline Target_t get_TargetToConsole()
	{
		return Target_t();
	}

	inline Target_t get_TargetToFile(const wchar_t * path, bool overwrite = false)
	{
		UNUSED(path);
		UNUSED(overwrite);
		return Target_t();
	}

	inline Target_t get_TargetToSys(const wchar_t * name, const wchar_t * path = nullptr)
	{
		UNUSED(name);
		UNUSED(path);
		return Target_t();
	}

	inline Target_t get_TargetToMult(const Target_t & first, const Target_t & second)
	{
		UNUSED(first);
		UNUSED(second);
		return Target_t();
	}

	namespace Default {
		inline Level get_level()
		{
			return Level::Atten;
		}

		inline void set_level(Level lvl)
		{
			UNUSED(lvl);
		}

		inline size_t get_prefix()
		{
			return Prefix::Medium;
		}

		inline void set_prefix(size_t prefix)
		{
			UNUSED(prefix);
		}

		inline Target_t get_target()
		{
			return Target_t();
		}

		inline void set_target(Target_t target)
		{
			UNUSED(target);
		}

		inline Module_i * get_module()
		{
			return nullptr;
		}
	}  // namespace Default

	inline Module_i * get_module(const wchar_t * name, const Target_t & target = Default::get_target(), Level lvl = Default::get_level())
	{
		UNUSED(name);
		UNUSED(target);
		UNUSED(lvl);
		return nullptr;
	}

	inline void set_target(Module_i * module, const Target_t & target)
	{
		UNUSED(module);
		UNUSED(target);
	}

	inline void set_level(Module_i * module, Level lvl)
	{
		UNUSED(module);
		UNUSED(lvl);
	}

	inline void set_prefix(Module_i * module, size_t prefix)
	{
		UNUSED(module);
		UNUSED(prefix);
	}

	inline Prefix::flags get_prefix(Module_i * module)
	{
		UNUSED(module);
		return 0;
	}

	inline void set_color_mode(Module_i * module, bool mode)
	{
		UNUSED(module);
		UNUSED(mode);
	}

	inline void set_enabled(Module_i * module, bool enabled)
	{
		UNUSED(module);
		UNUSED(enabled);
	}

	inline Lock::ScopeGuard lock_module(Module_i * module)
	{
		UNUSED(module);
		return Lock::ScopeGuard();
	}

#endif
}

inline Logger::Module_i * get_logger_module()
{
	return Logger::Default::get_module();
}

#endif
