#ifndef _LIBEXT_FILEVERSION_HPP_
#define _LIBEXT_FILEVERSION_HPP_

#include <basis/configure.hpp>

#include <basis/simstd/string>

namespace Ext {

	///================================================================================= FileVersion
	struct FileVersion {
		FileVersion(const wchar_t* path);

		ustring get_version() const {
			return m_ver;
		}

	private:
		ustring m_ver;
	};

}

#endif
