#ifndef _LIBEXT_FILEVERSION_HPP_
#define _LIBEXT_FILEVERSION_HPP_

#include <basis/configure.hpp>

#include <basis/std/string>

namespace Ext {

	///================================================================================= FileVersion
	struct FileVersion {
		FileVersion(PCWSTR path);

		ustring get_version() const {
			return m_ver;
		}

	private:
		ustring m_ver;
	};

}

#endif
