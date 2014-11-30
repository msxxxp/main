#ifndef BASIS_SYS_SSTR_WSTR_HPP_
#define BASIS_SYS_SSTR_WSTR_HPP_

#include <basis/sys/sstr.hpp>

namespace Base {

	class wstr {
		typedef wstr this_type;

	public:
		typedef wchar_t value_type;
		typedef size_t size_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * pointer;
		typedef const value_type * const_pointer;

		~wstr();

		wstr();

		wstr(const_pointer ptr);

		wstr(const this_type & other);

		wstr(this_type && other);

		wstr & operator =(const this_type & other);

		wstr & operator =(this_type && other);

		reference operator [](size_type index);

		const_reference operator [](size_type index) const;

		this_type & operator += (const this_type & right);

		this_type operator + (const this_type & right) const;

		const_pointer c_str() const;

		bool empty() const;

		size_type size() const;

		void swap(this_type & other);

		static wstr format(const_pointer fmt, va_list args);

		static wstr format(const_pointer fmt, ...);

		static ssize_t unit_test();

	private:
		wstr(pointer ptr, bool capture);

		wstr(const_pointer str1, size_type length1, const_pointer str2, size_type length2);

		static pointer make_copy(const_pointer str, size_type length);

		static pointer make_copy(const_pointer str);

		static pointer make_copy(const_pointer str1, size_type length1, const_pointer str2, size_type length2);

		static pointer make_copy(const_pointer str1, const_pointer str2);

		static void free(pointer str);

		pointer m_data;
	};

	class mstring {
		typedef mstring this_type;

	public:
		typedef wchar_t value_type;
		typedef size_t size_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * pointer;
		typedef const value_type * const_pointer;

		~mstring();

		mstring(const_pointer in = EMPTY_STR);

		mstring(const this_type & other);

		mstring& operator =(const this_type & other);

		mstring(this_type && rhs);

		mstring& operator =(this_type && rhs);

		void push_back(const_pointer str);

		size_type size() const;

		size_type capacity() const;

		const_pointer c_str() const;

		const_pointer operator [](size_type index) const;

		void swap(this_type & other);

		static ssize_t unit_test();

	private:
		void add_data(const_pointer mstr);

		pointer   m_data;
		size_type m_capa;
		size_type m_size;
	};
}

#endif
