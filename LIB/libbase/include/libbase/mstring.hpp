#ifndef _LIBBASE_MSTRING_HPP_
#define _LIBBASE_MSTRING_HPP_

#include <libbase/std.hpp>

namespace Base {

	struct mstring {
		~mstring();

		mstring(PCWSTR in = EMPTY_STR);

		mstring(const mstring & other);

		mstring & operator = (const mstring & other);

		mstring(mstring && rhs);

		mstring & operator = (mstring && rhs);

		void push_back(PCWSTR str);

		size_t size() const;

		size_t capacity() const;

		PCWSTR c_str() const;

		PCWSTR operator [] (size_t index) const;

		void swap(mstring & other);

	private:
		struct impl;

		impl * m_str;
	};

}

#endif
