#ifndef BASIS_SYS_LOGGER_MODULE_HPP_
#define BASIS_SYS_LOGGER_MODULE_HPP_

#include <basis/sys/logger.hpp>

namespace logger {

	enum class Level : ssize_t {
		TrObj,    // Trace objects
		Trace2,   // Trace more
		Trace1,   // Trace
		Debug2,   // Debug more
		Debug1,   // Debug
		Info,     // Information
		Report,   // Report
		Atten,    // Attention
		Warn,     // Warning
		Error,    // Error
		Fatal,    // Fatal error
		Alert,    // Alert
		Emerg,    // Emergency
		Force,    // Force log without level
	};

	const wchar_t* to_str(Level lvl);

	void set_options(const wchar_t* url);

	///====================================================================================== Module
	class Module {
	public:
		virtual ~Module() = default;

		virtual bool is_color_mode() const = 0;

		virtual bool is_utf8_mode() const = 0;

		virtual void out(Level lvl, const wchar_t* format, ...) const = 0;

		virtual void out_with_color(WORD color, Level lvl, const wchar_t* format, ...) const = 0;

		virtual void out_with_place(const char* file, int line, const char* func, Level lvl, const wchar_t* format, ...) const = 0;
	};

	Module* get_default_module();

	Module* get_console_module();

	Module* get_module(const wchar_t* name);

}

inline logger::Module* get_logger_module()
{
	return logger::get_default_module();
}

inline logger::Module* get_console_logger_module()
{
	return logger::get_console_module();
}

#endif
