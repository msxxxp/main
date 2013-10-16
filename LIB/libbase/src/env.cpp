#include <libbase/env.hpp>
#include <libbase/string.hpp>

namespace Base {
	namespace Env {

		ustring get(PCWSTR name) {
			Base::auto_array<wchar_t> buf(::GetEnvironmentVariableW(name, nullptr, 0));
			::GetEnvironmentVariableW(name, buf.data(), buf.size());
			return ustring(buf.data());
		}

		bool set(PCWSTR name, PCWSTR val) {
			return ::SetEnvironmentVariableW(name, val) != 0;
		}

		bool add(PCWSTR name, PCWSTR val) {
			Base::auto_array<wchar_t> buf(::GetEnvironmentVariableW(name, nullptr, 0) + Cstr::length(val));
			::GetEnvironmentVariableW(name, buf.data(), buf.size());
			Cstr::cat(buf.data(), val);
			return ::SetEnvironmentVariableW(name, buf.data()) != 0;
		}

		bool del(PCWSTR name) {
			return ::SetEnvironmentVariableW(name, nullptr) != 0;
		}

	}
}
