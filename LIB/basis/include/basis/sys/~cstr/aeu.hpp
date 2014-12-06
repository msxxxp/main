#ifndef LIBBASE_CSTR_AEU_HPP_
#define LIBBASE_CSTR_AEU_HPP_

namespace cstr {

	class aeu: private pattern::Uncopyable {
		typedef aeu this_type;
		typedef wchar_t value_type;
		typedef wchar_t* pointer;
		typedef const wchar_t* const_pointer;
		typedef wchar_t& reference;
		typedef const wchar_t& const_reference;
		typedef size_t size_type;

	public:
		~aeu();

		explicit aeu(const_pointer str);
		aeu(const_pointer str, size_type length);

		aeu(this_type&& other);
		this_type& operator =(this_type&& other);

		void swap(this_type& other);

		size_type size() const;
		size_type length() const;

		operator const_pointer() const;
		const_pointer c_str() const;

		const_reference operator [](size_type index) const;
		reference operator [](size_type index);

	private:
		pointer ptr;
	};

	aeu format(const wchar_t* fmt, ...);
	aeu format(const wchar_t* fmt, va_list args);

}

#endif
