#ifndef LIBBASE_CSTR_NAMEDVALUES_HPP_
#define LIBBASE_CSTR_NAMEDVALUES_HPP_

#include <system/cstr.hpp>

namespace cstr {

	template<typename Type>
	struct NamedValues {
		PCWSTR name;
		Type value;

		static PCWSTR GetName(const NamedValues<Type> dim[], size_t size, const Type & in)
		{
			for (size_t i = 0; i < size; ++i) {
				if (dim[i].value == in) {
					return dim[i].name;
				}
			}
			return L"unknown";
		}

		static Type GetValue(const NamedValues<Type> dim[], size_t size, PCWSTR name)
		{
			for (size_t i = 0; i < size; ++i) {
				if (cstr::compare(dim[i].name, name)) {
					return dim[i].value;
				}
			}
			return 0;
		}
	};

}

#endif
