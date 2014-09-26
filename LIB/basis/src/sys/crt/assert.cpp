#include <basis/sys/crt.hpp>
#include <basis/sys/console.hpp>

namespace crt {

	void assrt(const char* message, const char* file, int line)
	{
		console::printf(console::Handle::ERR, "Assertion failed at %s line %d: '%s'\n", file, line, message);
		::TerminateProcess(::GetCurrentProcess(), static_cast<UINT>(-1));
	}

	void assrt(const wchar_t* message, const char* file, int line)
	{
		console::printf(console::Handle::ERR, L"Assertion failed at %S line %d: '%s'\n", file, line, message);
		::TerminateProcess(::GetCurrentProcess(), static_cast<UINT>(-1));
	}

}
