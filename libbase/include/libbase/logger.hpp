#ifndef _LIBBASE_LOGGER_HPP_
#define _LIBBASE_LOGGER_HPP_

#include <libbase/std.hpp>

#ifdef NO_LOGGER
#else
#include <libbase/shared_ptr.hpp>
#endif

#ifdef NO_LOGGER
#   define LogTrace()
#   define LogTraceIf(condition) (condition)
#   define LogNoise(format, args ...)
#   define LogNoiseIf(condition, format, ...) (condition)
#   define LogDebug(format, args ...)
#   define LogDebugIf(condition, format, ...) (condition)
#   define LogInfo(format, args ...)
#   define LogInfoIf(condition, format, ...) (condition)
#   define LogReport(format, args ...)
#   define LogReportIf(condition, format, ...) (condition)
#   define LogAtten(format, args ...)
#   define LogAttenIf(condition, format, ...) (condition)
#   define LogWarn(format, args ...)
#   define LogWarnIf(condition, format, ...) (condition)
#   define LogError(format, args ...)
#   define LogErrorIf(condition, format, ...) (condition)
#   define LogFatal(format, args ...)
#   define LogFatalIf(condition, format, ...) (condition)
#   define LogPrint(format, ...)
#   define LogPrintIf(condition, format, ...) (condition)
#else
#       define LogTrace()                         get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, L"\n")
#       define LogTraceIf(condition)              if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, L"\n")
#   ifdef _MSC_VER
#       define LogNoise(format, ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, __VA_ARGS__)
#       define LogNoiseIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, __VA_ARGS__)
#       define LogDebug(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, __VA_ARGS__)
#       define LogDebugIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, __VA_ARGS__)
#       define LogInfo(format, ...)	              get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, __VA_ARGS__)
#       define LogReport(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, __VA_ARGS__)
#       define LogAtten(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, __VA_ARGS__)
#       define LogWarn(format, ...)	              get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, __VA_ARGS__)
#       define LogWarnIf(condition, format, ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, __VA_ARGS__)
#       define LogError(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, __VA_ARGS__)
#       define LogErrorIf(condition, format, ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, __VA_ARGS__)
#       define LogFatal(format, ...)	          get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, __VA_ARGS__)
#   else
#       define LogNoise(format, ...)                   get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, ##__VA_ARGS__)
#       define LogNoiseIf(condition, format, ...)      if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Trace, format, ##__VA_ARGS__)
#       define LogDebug(format, ...)                   get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, ##__VA_ARGS__)
#       define LogDebugIf(condition, format, ...)      if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Debug, format, ##__VA_ARGS__)
#       define LogInfo(format, args ...)               get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, ##args)
#       define LogInfoIf(condition, format, args ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Info, format, ##args)
#       define LogReport(format, args ...)             get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, ##args)
#       define LogReportIf(condition,format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Report, format, ##args)
#       define LogAtten(format, args ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, ##args)
#       define LogAttenIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Atten, format, ##args)
#       define LogWarn(format, args ...)               get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, ##args)
#       define LogWarnIf(condition, format, args ...)  if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Warn, format, ##args)
#       define LogError(format, args ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, ##args)
#       define LogErrorIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Error, format, ##args)
#       define LogFatal(format, args ...)              get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, ##args)
#       define LogFatalIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Fatal, format, ##args)
#       define LogForce(format, ...)                   get_logger_module()->out(THIS_PLACE, Logger::Level::Force, format, ##__VA_ARGS__)
#       define LogForceIf(condition, format, ...)      if (condition) get_logger_module()->out(THIS_PLACE, Logger::Level::Force, format, ##__VA_ARGS__)
#   endif
#endif

namespace Logger {
	struct Module_i;
	struct Target_i;
}

Logger::Module_i * get_logger_module();

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
		const static flags Full     = Date | Time | Level | Function | Module | Thread | Place;
	};

#ifdef NO_LOGGER
	typedef Target_i * Target_t;

	inline Level get_default_level()
	{
		return Level::Atten;
	}

	inline void set_default_level(Level /*lvl*/)
	{
	}

	inline size_t get_default_prefix()
	{
		return Prefix::Medium;
	}

	inline void set_default_prefix(size_t /*prefix*/)
	{
	}

	inline Target_t get_default_target()
	{
		return Target_t();
	}

	inline void set_default_target(Target_t /*target*/)
	{
	}

	inline Module_i * get_default_module()
	{
		return nullptr;
	}

	inline Module_i * get_module(PCWSTR /*name*/, const Target_t & /*target*/ = get_default_target(), Level /*lvl*/ = get_default_level())
	{
		return nullptr;
	}

	inline void set_module_target(const Target_t & /*target*/, Module_i * /*module*/)
	{
	}

	inline void set_module_level(Level /*lvl*/, Module_i * /*module*/)
	{
	}

	inline void set_module_prefix(size_t /*prefix*/, Module_i * /*module*/)
	{
	}

	inline Prefix::flags get_module_prefix(Module_i * /*module*/)
	{
		return 0;
	}

	inline void set_module_color_mode(bool /*mode*/, Module_i * /*module*/)
	{
	}

	inline void set_module_enabled(bool /*enabled*/, Module_i * /*module*/)
	{
	}

	inline void lock_module(Module_i * /*module*/)
	{
	}

	inline void unlock_module(Module_i * /*module*/)
	{
	}

	inline Target_t get_TargetToNull()
	{
		return Target_t();
	}

	inline Target_t get_TargetToConsole()
	{
		return Target_t();
	}

	inline Target_t get_TargetToFile(PCWSTR /*path*/, bool /*overwrite*/ = false)
	{
		return Target_t();
	}

	inline Target_t get_TargetToSys(PCWSTR /*name*/, PCWSTR /*path*/= nullptr)
	{
		return Target_t();
	}

	inline Target_t get_TargetToMult(const Target_t & /*first*/, const Target_t & /*second*/)
	{
		return Target_t();
	}
#else
	typedef Base::shared_ptr<Target_i> Target_t;

	///==================================================================================== Module_i
	struct Module_i {
		virtual ~Module_i();

		virtual PCWSTR get_name() const = 0;

		virtual Level get_level() const = 0;

		virtual size_t get_prefix() const = 0;

		virtual bool is_color_mode() const = 0;

		virtual void set_level(Level lvl) = 0;

		virtual void set_prefix(size_t prefix) = 0;

		virtual void set_color_mode(bool mode) = 0;

		virtual void set_target(const Target_t & target) = 0;

		virtual void set_enabled(bool enabled) = 0;

		virtual void out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const = 0;

		virtual void out(Level lvl, PCWSTR format, ...) const = 0;

		virtual void batch_lock() const = 0;

		virtual void batch_unlock() const = 0;
	};

	Level get_default_level();

	void set_default_level(Level lvl);

	size_t get_default_prefix();

	void set_default_prefix(size_t prefix);

	Target_t get_default_target();

	void set_default_target(Target_t target);

	Module_i * get_default_module();

	Module_i * get_module(PCWSTR name, const Target_t & target = get_default_target(), Level lvl = get_default_level());

	inline void set_module_target(const Target_t & target, Module_i * module)
	{
		module->set_target(target);
	}

	inline void set_module_level(Level lvl, Module_i * module)
	{
		module->set_level(lvl);
	}

	inline void set_module_prefix(size_t prefix, Module_i * module)
	{
		module->set_prefix(prefix);
	}

	inline Prefix::flags get_module_prefix(Module_i * module)
	{
		return module->get_prefix();
	}

	inline void set_module_color_mode(bool mode, Module_i * module)
	{
		module->set_color_mode(mode);
	}

	inline void set_module_enabled(bool enabled, Module_i * module)
	{
		module->set_enabled(enabled);
	}

	inline void lock_module(Module_i * module)
	{
		module->batch_lock();
	}

	inline void unlock_module(Module_i * module)
	{
		module->batch_unlock();
	}

	///==================================================================================== Target_i
	struct Target_i {
		virtual ~Target_i();

		virtual void out(const Module_i * module, Level lvl, PCWSTR str, size_t size) const = 0;

		virtual void out(PCWSTR str, size_t size) const = 0;

		virtual void lock() const = 0;

		virtual void unlock() const = 0;
	};

	Target_t get_TargetToNull();

	Target_t get_TargetToConsole();

	Target_t get_TargetToFile(PCWSTR path, bool overwrite = false);

	Target_t get_TargetToSys(PCWSTR name, PCWSTR path = nullptr);

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second);
#endif

}

inline Logger::Module_i * get_logger_module()
{
	return Logger::get_default_module();
}

#endif
