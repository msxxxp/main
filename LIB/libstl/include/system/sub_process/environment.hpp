#ifndef LIBSTL_SYSTEM_PROCESS_ENVIRONMENT_HPP_
#define LIBSTL_SYSTEM_PROCESS_ENVIRONMENT_HPP_

#include <system/configure.hpp>

#include <simstd/iosfwd>

namespace process {
	namespace env {

		simstd::wstring get(const wchar_t * name);

		bool set(const wchar_t * name, const wchar_t * val);

		bool add(const wchar_t * name, const wchar_t * val);

		bool del(const wchar_t * name);

	}
}

#endif
