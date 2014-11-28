#ifndef LIBBASE_CSTR_NAMEDVALUES_HPP_
#define LIBBASE_CSTR_NAMEDVALUES_HPP_

namespace cstr {

	template<typename Type>
	struct NamedValues {
		const wchar_t * name;
		Type value;

		static const wchar_t * GetName(const NamedValues<Type> dim[], size_t size, const Type & in)
		{
			for (size_t i = 0; i < size; ++i) {
				if (dim[i].value == in) {
					return dim[i].name;
				}
			}
			return L"unknown";
		}

		static Type GetValue(const NamedValues<Type> dim[], size_t size, const wchar_t * name)
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
