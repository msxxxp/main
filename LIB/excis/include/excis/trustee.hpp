#ifndef _LIBEXT_TRUSTEE_HPP_
#define _LIBEXT_TRUSTEE_HPP_

#include <basis/configure.hpp>
#include <excis/sid.hpp>

#include <aclapi.h>

#include <basis/simstd/iosfwd>

namespace Ext {

	///=================================================================================== trustee_t
	struct trustee_t: public TRUSTEEW {
		trustee_t(const wchar_t* name);

		trustee_t(PSID sid);
	};

	///======================================================================================= ExpAccess
	struct ExpAccess: public EXPLICIT_ACCESSW {
		ExpAccess(const wchar_t* name, ACCESS_MASK acc, ACCESS_MODE mode, DWORD inh = SUB_CONTAINERS_AND_OBJECTS_INHERIT);

		ExpAccess(PSID sid, ACCESS_MASK acc, ACCESS_MODE mode, DWORD inh = SUB_CONTAINERS_AND_OBJECTS_INHERIT);

		ustring get_name() const;

		Sid get_sid() const;
	};

	///============================================================================== ExpAccessArray
	struct ExpAccessArray {
		~ExpAccessArray();

		explicit ExpAccessArray(size_t count_ea);

		ExpAccessArray(PACL acl);

		ExpAccess & operator [] (size_t index) const;

		size_t size() const;

	private:
		ExpAccess * m_eacc;
		ULONG m_size;
	};

}

#endif
