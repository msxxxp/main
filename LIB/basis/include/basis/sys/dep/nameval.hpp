#ifndef LIBBASE_NAMEVAL_HPP_
#define LIBBASE_NAMEVAL_HPP_

#include <libbase/std.hpp>
#include <initializer_list>

namespace Base {
	template<typename Type>
	struct NamedValue {
		const wchar_t * name;
		Type value;
	};

	template<typename Type, int Size>
	struct NameVal {
		NameVal(std::initializer_list<NamedValue<Type> > list)
		{
			size_t i = 0;
			for (auto it = list.begin(); it != list.end() && i < Size; ++it, ++i)
				m_values[i] = *it;
		}

		size_t size() const
		{
			return static_cast<size_t>(Size);
		}

	private:
		NamedValue<Type> m_values[Size];
	};

	template<typename Type, int Size>
	inline const wchar_t * GetName(const NameVal<Type, Size> dim[], const Type & in) {
		for (size_t i = 0; i < Size; ++i) {
			if (dim[i].value == in) {
				return dim[i].name;
			}
		}
		return L"unknown";
	}

	template<typename Type, int Size>
	inline Type GetValue(const NameVal<Type, Size> dim[], const wchar_t * name) {
		for (size_t i = 0; i < Size; ++i) {
			if (::wcscmp(dim[i].name, name)) {
				return dim[i].value;
			}
		}
		return 0;
	}
}

#endif
