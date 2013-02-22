#ifndef _LIBBASE_LOGGER_HPP_
#define _LIBBASE_LOGGER_HPP_

#include <libbase/std.hpp>
#include <libbase/shared_ptr.hpp>

namespace Base {
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
		};

		namespace Prefix {
			const static size_t Function = 0x0001;
			const static size_t Level    = 0x0002;
			const static size_t Module   = 0x0004;
			const static size_t Place    = 0x0008;
			const static size_t Thread   = 0x0010;
			const static size_t Time     = 0x0020;
			const static size_t Date     = 0x0040;

			const static size_t Lite     = Time | Level;
			const static size_t Medium   = Time | Level | Function;
			const static size_t Full     = Date | Time | Level | Function | Module | Thread | Place;
		};

		struct Logger_i;
		struct Module_i;
		struct Target_i;

#ifdef NO_LOGGER
		typedef Target_i * Target_t;

		inline Level get_default_level()
		{
			return Level::Atten;
		}

		void set_default_level(Level /*lvl*/)
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

		inline void set_module_target(const Target_t & /*target*/, Module_i * /*module*/= nullptr)
		{
		}

		inline void set_module_level(Level /*lvl*/, Module_i * /*module*/= nullptr)
		{
		}

		inline void set_module_prefix(size_t /*prefix*/, Module_i * /*module*/= nullptr)
		{
		}

		inline void set_module_color_mode(bool /*mode*/, Module_i * /*module*/= nullptr)
		{
		}

		inline void set_module_enabled(bool /*enabled*/, Module_i * /*module*/ = nullptr)
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

		inline Target_t get_TargetToFile(PCWSTR /*path*/)
		{
			return Target_t();
		}

		inline Target_t get_TargetToSys(PCWSTR /*name*/, PCWSTR /*path*/= nullptr)
		{
			return Target_t();
		}
#else
		typedef Base::shared_ptr<Target_i> Target_t;

		///================================================================================ Module_i
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
		};

		///================================================================================ Target_i
		struct Target_i {
			virtual ~Target_i();

			virtual void out(const Module_i * module, Level lvl, PCWSTR str, size_t size) const = 0;

			virtual void out(PCWSTR str, size_t size) const = 0;
		};

		Level get_default_level();

		void set_default_level(Level lvl);

		size_t get_default_prefix();

		void set_default_prefix(size_t prefix);

		Target_t get_default_target();

		void set_default_target(Target_t target);

		Module_i * get_default_module();

		Module_i * get_module(PCWSTR name, const Target_t & target = get_default_target(), Level lvl = get_default_level());

		inline void set_module_target(const Target_t & target, Module_i * module = get_default_module())
		{
			module->set_target(target);
		}

		inline void set_module_level(Level lvl, Module_i * module = get_default_module())
		{
			module->set_level(lvl);
		}

		inline void set_module_prefix(size_t prefix, Module_i * module = get_default_module())
		{
			module->set_prefix(prefix);
		}

		inline void set_module_color_mode(bool mode, Module_i * module = get_default_module())
		{
			module->set_color_mode(mode);
		}

		inline void set_module_enabled(bool enabled, Module_i * module = get_default_module())
		{
			module->set_enabled(enabled);
		}

		Target_t get_TargetToNull();

		Target_t get_TargetToConsole();

		Target_t get_TargetToFile(PCWSTR path);

		Target_t get_TargetToSys(PCWSTR name, PCWSTR path = nullptr);
#endif

	}
}

inline Base::Logger::Module_i * get_logger_module()
{
	return Base::Logger::get_default_module();
}

#ifdef NO_LOGGER
#	define LogTrace()
#	define LogTraceIf(condition) (condition)
#	define LogNoise(format, args ...)
#	define LogNoiseIf(condition, format, args ...)
#	define LogDebug(format, args ...)
#	define LogDebugIf(condition, format, args ...) (condition)
#	define LogInfo(format, args ...)
#	define LogReport(format, args ...)
#	define LogAtten(format, args ...)
#	define LogWarn(format, args ...)
#	define LogWarnIf(condition, format, args ...) (condition)
#	define LogError(format, args ...)
#	define LogErrorIf(condition, format, args ...) (condition)
#	define LogFatal(format, args ...)
#else
#	ifdef NO_TRACE
#		define LogTrace()
#		define LogTraceIf(condition) (condition)
#		define LogNoise(format, args ...)
#		define LogNoiseIf(condition) (condition)
#		define LogDebug(format, args ...)
#		define LogDebugIf(condition, format, args ...) (condition)
#		define LogInfo(format, args ...)	get_logger_module()->out(Base::Logger::Level::Info, format, ##args)
#		define LogReport(format, args ...)	get_logger_module()->out(Base::Logger::Level::Report, format, ##args)
#		define LogAtten(format, args ...)	get_logger_module()->out(Base::Logger::Level::Atten, format, ##args)
#		define LogWarn(format, args ...)	get_logger_module()->out(Base::Logger::Level::Warn, format, ##args)
#   	define LogWarnIf(condition, format, args ...) if (condition) get_logger_module()->out(Base::Logger::Level::Warn, format, ##args)
#   	define LogError(format, args ...)	get_logger_module()->out(Base::Logger::Level::Error, format, ##args)
#   	define LogErrorIf(condition, format, args ...) if (condition) get_logger_module()->out(Base::Logger::Level::Error, format, ##args)
#		define LogFatal(format, args ...)	get_logger_module()->out(Base::Logger::Level::Fatal, format, ##args)
#	else
#       define LogTrace()                   get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Trace, L"\n")
#       define LogTraceIf(condition)        if (condition) get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Trace, L"\n")
#       define LogNoise(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Trace, format, ##args)
#       define LogNoiseIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Trace, format, ##args)
#       define LogDebug(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Debug, format, ##args)
#       define LogDebugIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Debug, format, ##args)
#		define LogInfo(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Info, format, ##args)
#		define LogReport(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Report, format, ##args)
#		define LogAtten(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Atten, format, ##args)
#		define LogWarn(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Warn, format, ##args)
#   	define LogWarnIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Warn, format, ##args)
#   	define LogError(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Error, format, ##args)
#   	define LogErrorIf(condition, format, args ...) if (condition) get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Error, format, ##args)
#		define LogFatal(format, args ...)	get_logger_module()->out(THIS_PLACE, Base::Logger::Level::Fatal, format, ##args)
#	endif
#endif

#endif
