#ifndef BASIS_SYS_LOGGER_MODULE_HPP_
#define BASIS_SYS_LOGGER_MODULE_HPP_

#include <basis/sys/logger.hpp>

namespace logger {

	class Module;

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

	/// LogSetOptions(L"logger:///default?level=tr;prefix=f;target=co");
	/// LogSetOptions(L"logger:///module?name=traceback;prefix=fu;level=in");
	/// level = to | TrObj,  | 0
	/// level = t2 | Trace2, | 1
	/// level = t  | Trace1, | 2
	/// level = d2 | Debug2, | 3
	/// level = d  | Debug1, | 4
	/// level = i  | Info,   | 5
	/// level = r  | Report, | 6
	/// level = at | Atten,  | 7
	/// level = w  | Warn,   | 8
	/// level = er | Error,  | 9
	/// level = f  | Fatal,  | 10
	/// level = al | Alert,  | 11
	/// level = em | Emerg,  | 12
	/// prefix = no | 0
	/// prefix = l  | lite
	/// prefix = m  | medium
	/// prefix = f  | full
	/// prefix = Prefix::flags as number
	/// target = n  | null
	/// target = c  | console
	/// target = f(path)  | file(path)
	/// target = fo(path) | fileowerwrite(path)
	/// target = s  | system

	void set_options(const wchar_t* url);

	Module* get_default_module();

	Module* get_console_module();

	Module* get_module(const wchar_t* name);

	class Module {
	public:
		virtual ~Module() = default;

		virtual bool is_color_mode() const = 0;

		virtual bool is_utf8_mode() const = 0;

		virtual void out(Level lvl, const wchar_t* format, ...) const = 0;

		virtual void out_with_color(WORD color, Level lvl, const wchar_t* format, ...) const = 0;

		virtual void out_with_place(const char* file, int line, const char* func, Level lvl, const wchar_t* format, ...) const = 0;
	};

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
