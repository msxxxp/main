#ifndef _LIBEXT_DLL_HPP_
#define _LIBEXT_DLL_HPP_

#include <system/configure.hpp>
#include <system/linkage.hpp>

#include <simstd/iosfwd>

namespace Ext {

	///============================================================================== DinamicLibrary
	class DynamicLibrary: private linkage::DynamicLibrary {
		typedef linkage::DynamicLibrary base_type;
		typedef DynamicLibrary this_type;

	public:
		~DynamicLibrary() noexcept;

		DynamicLibrary(PCWSTR path, DWORD flags = 0);

		DynamicLibrary(HMODULE hndl, DWORD flags = 0);

		DynamicLibrary(const this_type & rhs);

		this_type & operator = (const this_type & rhs);

		using linkage::DynamicLibrary::get_hmodule;
		using linkage::DynamicLibrary::get_flags;

		ustring get_path() const;

		FARPROC get_function(PCSTR name) const;

		FARPROC get_function_nt(PCSTR name) const noexcept
		{
			return base_type::get_function(name);
		}

		void swap(this_type & rhs)
		{
			base_type::swap(rhs);
		}
	};

}

#endif
