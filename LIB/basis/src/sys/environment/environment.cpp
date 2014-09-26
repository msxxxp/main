#include <basis/sys/process.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/string>

namespace process {
	namespace env {

		simstd::wstring get(const wchar_t * name)
		{
			memory::auto_array<wchar_t> buf(::GetEnvironmentVariableW(name, nullptr, 0));
			::GetEnvironmentVariableW(name, buf.data(), buf.size());
			return simstd::wstring(buf.data());
		}

		bool set(const wchar_t * name, const wchar_t * val)
		{
			return ::SetEnvironmentVariableW(name, val) != 0;
		}

		bool add(const wchar_t * name, const wchar_t * val)
		{
			memory::auto_array<wchar_t> buf(::GetEnvironmentVariableW(name, nullptr, 0) + cstr::length(val));
			::GetEnvironmentVariableW(name, buf.data(), buf.size());
			cstr::cat(buf.data(), val);
			return ::SetEnvironmentVariableW(name, buf.data()) != 0;
		}

		bool del(const wchar_t * name)
		{
			return ::SetEnvironmentVariableW(name, nullptr) != 0;
		}

	}
}
