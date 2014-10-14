#ifndef BASIS_STRING_BASIC_STRING2_HPP_
#define BASIS_STRING_BASIC_STRING2_HPP_

#include <basis/ext/pattern.hpp>
#include <basis/simstd/algorithm>
#include <basis/simstd/string>
#include <basis/simstd/iterator>
#include <basis/simstd/memory>
#include <basis/sys/crt.hpp>

namespace simstd {

	namespace Test {
		ssize_t a_string(aPrintFunc printFunc);
		ssize_t w_string(wPrintFunc printFunc);
	}

}

namespace simstd {

	template<typename CharType,
		typename Traits = simstd::char_traits<CharType>,
		typename Allocator = simstd::allocator<CharType> >
	class basic_string {
		typedef basic_string                                this_type;

	public:
		typedef Traits                                      traits_type;
		typedef size_t                                      size_type;
		typedef ptrdiff_t                                   difference_type;

		typedef typename traits_type::char_type             value_type;
		typedef value_type &                                reference;
		typedef const value_type &                          const_reference;
		typedef value_type *                                pointer;
		typedef const value_type *                          const_pointer;

		typedef simstd::pvt::normal_iterator<pointer>       iterator;
		typedef simstd::pvt::normal_iterator<const_pointer> const_iterator;
		typedef simstd::reverse_iterator<iterator>          reverse_iterator;
		typedef simstd::reverse_iterator<const_iterator>    const_reverse_iterator;

		static const size_type npos = ~static_cast<size_type>(0);

		~basic_string();

		basic_string();
		basic_string(size_type count, value_type ch);
		basic_string(const this_type& other, size_type pos, size_type count = npos);
		basic_string(const_pointer str, size_type count = npos);
		template<typename InputIt>
		basic_string(InputIt first, InputIt last);
		basic_string(const this_type& other);
		basic_string(this_type&& other);
//		basic_string(std::initializer_list<value_type> ilist);

		this_type & operator =(const this_type & right);
		this_type & operator =(this_type && right);
		this_type & operator =(const_pointer right);
		this_type & operator =(value_type ch);

		bool empty() const;

		size_type capacity() const;
		size_type size() const;
		size_type length() const;

		const_pointer c_str() const;

		void swap(this_type & in);

		const_iterator cbegin() const;
		const_iterator begin() const;
		iterator begin();

		const_iterator cend() const;
		const_iterator end() const;
		iterator end();

		void clear();

		void reserve(size_type capa);

		int compare(const_pointer str) const;
		int compare(const this_type & str) const;
		int compare(size_type pos, size_type count, const this_type & str) const;
		int compare(size_type pos, size_type count, const_pointer str) const;
		int compare(size_type pos1, size_type count1, const_pointer str, size_type count2) const;

		this_type & append(size_type count, value_type ch);
		this_type & append(const this_type & str);
		this_type & append(const this_type & str, size_type pos, size_type count);
		this_type & append(const_pointer str, size_type count);
		this_type & append(const_pointer str);

		this_type & assign(size_type count, value_type ch);
		this_type & assign(const this_type & str);
		this_type & assign(const this_type & str, size_type pos, size_type count = npos);
		this_type & assign(const_pointer str, size_type count);
		this_type & assign(const_pointer str);

		iterator insert(const_iterator pos, value_type ch);
		iterator insert(iterator pos, size_type count, value_type ch);
		this_type & insert(size_type index, size_type count, value_type ch);
		this_type & insert(size_type index, const_pointer str);
		this_type & insert(size_type index, const_pointer str, size_type count);
		this_type & insert(size_type index, const this_type & str);
		this_type & insert(size_type index, const this_type & str, size_type index_str, size_type count);

		this_type & operator +=(const this_type & str);
		this_type & operator +=(const_pointer str);
		this_type & operator +=(value_type ch);

		const_reference operator [](size_type in) const;
		reference operator [](size_type in);

		const_reference at(size_type in) const;
		reference at(size_type in);

		this_type & replace(size_type pos, size_type len, const this_type & str);
		this_type & replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen);
		this_type & replace(size_type pos, size_type len, const_pointer str);
		this_type & replace(size_type pos, size_type len, const_pointer str, size_type count);
		this_type & replace(size_type pos, size_type len, size_type count, value_type ch);

		this_type substr(size_type pos = 0, size_type n = npos) const;

		this_type & erase(size_type pos = 0, size_type count = npos);
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);

		size_type find(const this_type & str, size_type pos = 0) const;
		size_type find(const_pointer str, size_type pos = 0) const;
		size_type find(value_type ch, size_type pos = 0) const;

		size_type rfind(const this_type & str, size_type pos = npos) const;
		size_type rfind(const_pointer str, size_type pos, size_type count) const;
		size_type rfind(const_pointer str, size_type pos = npos) const;
		size_type rfind(value_type ch, size_type pos = npos) const;

		size_type find_first_of(const this_type & str, size_type pos = 0) const;
		size_type find_last_of(const this_type & str, size_type pos = npos) const;
		size_type find_first_not_of(const this_type & str, size_type pos = 0) const;
		size_type find_last_not_of(const this_type & str, size_type pos = npos) const;

	private:
		struct string_impl;
		string_impl * m_data;

		basic_string(const_pointer str, size_type count, size_type capacity);

		pointer _str() const;

		size_type get_new_capacity(size_type capacity) const;

		size_type get_necessary_capacity(ptrdiff_t addToSize) const;

		void split(size_type necessaryCapacity);

		static int compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2);

		size_type find(const_pointer str, size_type pos, size_type length) const;

		bool is_same_str(const_pointer str) const;

		bool is_shared() const;

		static const size_t MIN_ALLOC_BLOCK = 16;

		template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(C left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, C right);

		template<typename C, typename T, typename A> friend bool operator ==(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator ==(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator ==(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator !=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator !=(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator !=(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator <(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator <=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <=(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator <=(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator >(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >(const basic_string<C, T, A> & left, const C * right);
		template<typename C, typename T, typename A> friend bool operator >=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >=(const C * left, const basic_string<C, T, A> & right);
		template<typename C, typename T, typename A> friend bool operator >=(const basic_string<C, T, A> & left, const C * right);
	};

	///=============================================================================================
	template<typename C, typename T, typename A>
	struct basic_string<C, T, A>::string_impl: public pattern::RefCounter {
		static string_impl * allocate(size_type capa)
		{
			string_impl * impl = (string_impl*)simstd::pvt::_allocate<char>(sizeof(string_impl) + capa * sizeof(value_type));
			simstd::pvt::_construct(impl, capa);
			return impl;
		}

		~string_impl();

		string_impl(size_type capa = 1);

		void append(const_pointer str, size_type count);

		void append(value_type ch, size_type count);

		void append(const_pointer first, const_pointer last);

		void set_size(size_type new_size);

		void change_size(size_type new_size);

		size_type get_capacity() const
		{
			return end_of_storage - begin;
		}

		size_type get_free_capacity() const
		{
			return end_of_storage - end;
		}

		size_type get_size() const
		{
			return end - begin;
		}

		pointer get_str_data() const
		{
			return const_cast<pointer>(begin);
		}

	private:
		void deallocate() const override;

		value_type* end_of_storage;
		value_type* end;
		value_type  begin[1];
	};

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::string_impl::~string_impl()
	{
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::string_impl::string_impl(size_type capa) :
		end_of_storage(begin + capa),
		end(begin)
	{
		begin[0] = static_cast<value_type>(0);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::string_impl::append(const_pointer str, size_type count)
	{
		CRT_ASSERT(count <= get_free_capacity());
		if (str && count) {
			traits_type::copy(end, str, count);
			change_size(count);
		}
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::string_impl::append(value_type ch, size_type count)
	{
		CRT_ASSERT(count <= get_free_capacity());
		if (count) {
			traits_type::assign(end, count, ch);
			change_size(count);
		}
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::string_impl::set_size(size_type new_size)
	{
		end = begin + new_size;
		CRT_ASSERT(end < end_of_storage);
		*end = static_cast<value_type>(0);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::string_impl::change_size(size_type delta)
	{
		end += delta;
		CRT_ASSERT(end <= end_of_storage);
		*end = static_cast<value_type>(0);
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::string_impl::deallocate() const
	{
		simstd::pvt::_destroy(this);
		simstd::pvt::_deallocate((char*)this);
	}

	///=============================================================================================
	template<typename C, typename T, typename A>
	basic_string<C, T, A>::~basic_string()
	{
		m_data->decrease_ref();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string() :
		m_data(string_impl::allocate(get_new_capacity(0)))
	{
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const this_type & right) :
		m_data(right.m_data)
	{
		m_data->increase_ref();
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(size_type count, value_type in) :
		m_data(string_impl::allocate(get_new_capacity(count)))
	{
		m_data->append(in, count);
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const_pointer in, size_type count) :
		m_data(string_impl::allocate(get_new_capacity((in && count == npos) ? count = traits_type::length(in) : count)))
	{
		m_data->append(in, count);
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(this_type && right) :
		m_data(string_impl::allocate(get_new_capacity(0)))
	{
		swap(right);
	}

//	template<typename C, typename T, typename A>
//	basic_string<C, T, A>::basic_string(std::initializer_list<value_type> ilist) :
//		m_data(string_impl::allocate(get_new_capacity(ilist.size())))
//	{
//		m_data->append(ilist.begin(), ilist.end());
//	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator =(const this_type & right)
	{
		return assign(right);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator =(this_type && right)
	{
		clear();
		swap(right);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator =(const_pointer right)
	{
		return assign(right);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator =(value_type ch)
	{
		return assign(1, ch);
	}

	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::empty() const
	{
		return m_data->get_size() == 0;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::capacity() const
	{
		return m_data->get_capacity();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::size() const
	{
		return m_data->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::length() const
	{
		return m_data->get_size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_pointer basic_string<C, T, A>::c_str() const
	{
		return (const_pointer)m_data->get_str_data();
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::swap(this_type & in)
	{
		using simstd::swap;
		swap(m_data, in.m_data);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::cbegin() const
	{
		return const_iterator(c_str());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::begin() const
	{
		return cbegin();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::begin()
	{
		return iterator(_str());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::cend() const
	{
		return cbegin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_iterator basic_string<C, T, A>::end() const
	{
		return cbegin() + size();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::end()
	{
		return begin() + size();
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::clear()
	{
		if (!empty()) {
			split(capacity());
			m_data->set_size(0);
		}
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::reserve(size_type capa)
	{
		if (capacity() < capa) {
			this_type(c_str(), size(), capa).swap(*this);
		}
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(const_pointer str) const
	{
		return compare(c_str(), length(), str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(const this_type & str) const
	{
		return compare(c_str(), length(), str.c_str(), str.length());
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(size_type pos, size_type count, const this_type & str) const
	{
		return compare(c_str() + pos, count, str.c_str(), str.length());
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(size_type pos, size_type count, const_pointer str) const
	{
		return compare(c_str() + pos, count, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(size_type pos, size_type count, const_pointer str, size_type len) const
	{
		return compare(c_str() + pos, count, str, len);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::append(size_type count, value_type ch)
	{
		return append(this_type(count, ch));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::append(const this_type & str)
	{
		append(str.c_str(), str.size());
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::append(const this_type & str, size_type pos, size_type count)
	{
		return append(str.c_str() + pos, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::append(const_pointer str, size_type count)
	{
		if (str && count) {
			if (is_same_str(str)) {
				this_type tmp(c_str(), size(), get_necessary_capacity(count));
				tmp.append(str, count);
				tmp.swap(*this);
			} else {
				split(get_necessary_capacity(count));
				m_data->append(str, count);
			}
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::append(const_pointer str)
	{
		return append(str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::assign(size_type count, value_type ch)
	{
		clear();
		append(count, ch);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::assign(const this_type & str)
	{
		clear();
		append(str);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::assign(const this_type & str, size_type pos, size_type count)
	{
		clear();
		append(str.c_str() + pos, count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::assign(const_pointer str, size_type count)
	{
		clear();
		append(str, count);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::assign(const_pointer str)
	{
		clear();
		append(str);
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::insert(const_iterator pos, value_type ch)
	{
		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(cbegin(), pos);
		insert(posFirst, 0, 1, ch);
		return iterator(_str() + posFirst);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::insert(iterator pos, size_type count, value_type ch)
	{
		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(begin(), pos);
		insert(posFirst, 0, count, ch);
		return iterator(_str() + posFirst);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::insert(size_type index, size_type count, value_type ch)
	{
		return replace(index, 0, count, ch);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::insert(size_type index, const_pointer str)
	{
		return replace(index, 0, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::insert(size_type index, const_pointer str, size_type count)
	{
		return replace(index, 0, str, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::insert(size_type index, const this_type & str)
	{
		return replace(index, 0, str.c_str(), str.length());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::insert(size_type index, const this_type & str, size_type index_str, size_type count)
	{
		return replace(index, 0, str.c_str(), count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator +=(const this_type & str)
	{
		return append(str);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator +=(const_pointer str)
	{
		return append(str);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::operator +=(value_type ch)
	{
		return append(1, ch);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::operator [](size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::operator [](size_type in)
	{
		split(capacity());
		return *(_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::const_reference basic_string<C, T, A>::at(size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::reference basic_string<C, T, A>::at(size_type in)
	{
		split(capacity());
		return *(_str() + in);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::replace(size_type pos, size_type len, const this_type & str)
	{
		return replace(pos, len, str.c_str(), str.size());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen)
	{
		return replace(pos, len, &str[subpos], sublen);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::replace(size_type pos, size_type len, const_pointer str)
	{
		return replace(pos, len, str, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::replace(size_type pos, size_type len, const_pointer str, size_type count)
	{
		if (str) {
//			printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
			pos = simstd::min(pos, size());
			len = simstd::min(len, size() - pos);
//			printf("      (pos: %Iu, len: %Iu)\n", pos, len);
			if (is_same_str(str) || is_shared()) {
				this_type tmp(c_str(), pos, get_necessary_capacity(count - len));
//				printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
				tmp.append(str, count);
//				printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
				tmp.append(c_str() + pos, size() - pos);
//				printf("      (%Iu, %Iu): '%s'\n", tmp.size(), tmp.capacity(), tmp.c_str());
				tmp.swap(*this);
			} else {
				split(get_necessary_capacity(count - len));
//				printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
//				printf("replace before: '%s'\n", c_str());
				traits_type::move(_str() + pos + count, _str() + pos + len, size() - (pos + len) + 1);
//				traits_type::assign(_str() + pos, count, ' ');
//				printf("replace after : '%s'\n", c_str());
				traits_type::copy(_str() + pos, str, count);
//				printf("replace after : '%s'\n", c_str());
				m_data->set_size(size() + count - len);
//				printf("      (%Iu, %Iu): '%s'\n", size(), capacity(), c_str());
			}
		}
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::replace(size_type pos, size_type len, size_type count, value_type ch)
	{
		return replace(pos, len, this_type(count, ch));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type basic_string<C, T, A>::substr(size_type pos, size_type n) const
	{
		return this_type(c_str() + pos, n);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::this_type & basic_string<C, T, A>::erase(size_type pos, size_type count)
	{
		size_type posLast = (count == npos) ? size() : simstd::min(size(), pos + count);
		erase(simstd::next(cbegin(), pos), simstd::next(cbegin(), posLast));
		return *this;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::erase(const_iterator position)
	{
		return erase(position, simstd::next(position, 1));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::iterator basic_string<C, T, A>::erase(const_iterator first, const_iterator last)
	{
		typename simstd::iterator_traits<const_iterator>::difference_type posFirst = simstd::distance(cbegin(), first);
		typename simstd::iterator_traits<const_iterator>::difference_type posLast = simstd::distance(cbegin(), simstd::min(last, cend()));
		if (is_shared()) {
			this_type tmp(c_str(), posFirst, capacity());
			tmp.append(c_str() + posLast, size() - posLast);
			tmp.swap(*this);
		} else {
			traits_type::move(_str() + posFirst, _str() + posLast, size() - posLast + 1);
			m_data->set_size(size() - (posLast - posFirst));
		}
		return iterator(_str() + posFirst);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const this_type & str, size_type pos) const
	{
		return find(str.c_str(), pos, str.length());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const_pointer str, size_type pos) const
	{
		return find(str, pos, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(value_type ch, size_type pos) const
	{
		const_pointer find = traits_type::find(c_str() + pos, size(), ch);
		return (find) ? find - c_str() : npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const this_type & str, size_type pos) const
	{
		return rfind(str.c_str(), pos, str.length());
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const_pointer str, size_type pos, size_type count) const
	{
		pos = simstd::min(pos, size());
		if (count != 0 && count <= pos) {
			pos -= (count - 1);
			while (pos > 0)
				if (find(str, pos, count) != npos)
					return pos;
		}
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(const_pointer str, size_type pos) const
	{
		return rfind(str, pos, traits_type::length(str));
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::rfind(value_type ch, size_type pos) const
	{
		pos = simstd::min(pos, size());
		while (pos > 0)
			if (traits_type::eq(at(--pos), ch))
				return pos;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_first_of(const this_type & str, size_type pos) const
	{
		for (; pos < size(); ++pos)
			if (traits_type::find(str.c_str(), str.size(), at(pos)))
				return pos;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_last_of(const this_type & str, size_type pos) const
	{
		pos = simstd::min(pos, size());
		while (pos > 0)
			if (traits_type::find(str.c_str(), str.size(), at(--pos)))
				return pos;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_first_not_of(const this_type & str, size_type pos) const
	{
		for (; pos < size(); ++pos)
			if (traits_type::find(str.c_str(), str.size(), at(pos)) == nullptr)
				return pos;
		return npos;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find_last_not_of(const this_type & str, size_type pos) const
	{
		pos = simstd::min(pos, size());
		while (pos > 0)
			if (traits_type::find(str.c_str(), str.size(), at(--pos)) == nullptr)
				return pos;
		return npos;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A>::basic_string(const_pointer str, size_type count, size_type capacity) :
		m_data(string_impl::allocate(get_new_capacity(capacity)))
	{
		m_data->append(str, count);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::pointer basic_string<C, T, A>::_str() const
	{
		return m_data->get_str_data();
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::get_new_capacity(size_type capacity) const
	{
		return simstd::max(size_type(MIN_ALLOC_BLOCK), capacity);
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::get_necessary_capacity(ptrdiff_t addToSize) const
	{
		if (capacity() < (size() + addToSize))
			return size() + simstd::max((ptrdiff_t)size(), addToSize);
		return capacity();
	}

	template<typename C, typename T, typename A>
	void basic_string<C, T, A>::split(size_type necesseryCapacity)
	{
		if (capacity() < necesseryCapacity)
			this_type(c_str(), size(), necesseryCapacity).swap(*this);
		else if (is_shared())
			this_type(c_str(), size(), capacity()).swap(*this);
	}

	template<typename C, typename T, typename A>
	int basic_string<C, T, A>::compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2)
	{
		int result = traits_type::compare(str1, str2, simstd::min(count1, count2));
		if (result != 0)
			return result;
		if (count1 < count2)
			return -1;
		if (count1 > count2)
			return 1;
		return 0;
	}

	template<typename C, typename T, typename A>
	typename basic_string<C, T, A>::size_type basic_string<C, T, A>::find(const_pointer str, size_type pos, size_type length) const
	{
		const size_type size = this->size();
		const_pointer data = this->c_str();

		if (length == 0)
			return pos <= size ? pos : npos;

		if (length <= size)
		{
			for (; pos <= size - length; ++pos)
				if (traits_type::eq(data[pos], str[0]) && traits_type::compare(data + pos + 1, str + 1, length - 1) == 0)
					return pos;
		}
		return npos;
	}

	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::is_same_str(const_pointer str) const
	{
		return c_str() <= str && str < (c_str() + size()); // append to itself
	}

	template<typename C, typename T, typename A>
	bool basic_string<C, T, A>::is_shared() const
	{
		return m_data->count_ref() > 1;
	}

	///=============================================================================================
	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>(left.c_str(), left.size(), left.size() + right.size()) += right;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		typename basic_string<C, T, A>::size_type length = traits_type::length(left);
		return basic_string<C, T, A>(left, length, length + right.size()) += right;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(C left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>(&left, 1, 1 + right.size()) += right;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>(left.c_str(), left.size(), left.size() + traits_type::length(right)) += right;
	}

	template<typename C, typename T, typename A>
	basic_string<C, T, A> operator +(const basic_string<C, T, A> & left, C right)
	{
		return basic_string<C, T, A>(left.c_str(), left.size(), left.size() + 1) += right;
	}

	template<typename C, typename T, typename A>
	bool operator ==(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator ==(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator ==(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) == 0;
	}

	template<typename C, typename T, typename A>
	bool operator !=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return !operator ==(left, right);
	}

	template<typename C, typename T, typename A>
	bool operator !=(const C * left, const basic_string<C, T, A> & right)
	{
		return !operator ==(left, right);
	}

	template<typename C, typename T, typename A>
	bool operator !=(const basic_string<C, T, A> & left, const C * right)
	{
		return !operator ==(left, right);
	}

	template<typename C, typename T, typename A>
	bool operator <(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) < 0;
	}

	template<typename C, typename T, typename A>
	bool operator <=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) <= 0;
	}

	template<typename C, typename T, typename A>
	bool operator <=(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) <= 0;
	}

	template<typename C, typename T, typename A>
	bool operator <=(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) <= 0;
	}

	template<typename C, typename T, typename A>
	bool operator >(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) > 0;
	}

	template<typename C, typename T, typename A>
	bool operator >(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) > 0;
	}

	template<typename C, typename T, typename A>
	bool operator >(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) > 0;
	}

	template<typename C, typename T, typename A>
	bool operator >=(const basic_string<C, T, A> & left, const basic_string<C, T, A> & right)
	{
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right.c_str(), right.size()) >= 0;
	}

	template<typename C, typename T, typename A>
	bool operator >=(const C * left, const basic_string<C, T, A> & right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left, traits_type::length(left), right.c_str(), right.size()) >= 0;
	}

	template<typename C, typename T, typename A>
	bool operator >=(const basic_string<C, T, A> & left, const C * right)
	{
		typedef typename basic_string<C, T, A>::traits_type traits_type;
		return basic_string<C, T, A>::compare(left.c_str(), left.size(), right, traits_type::length(right)) >= 0;
	}

	template<typename C, typename T, typename A>
	void swap(basic_string<C, T, A> & left, basic_string<C, T, A> & right)
	{
		left.swap(right);
	}

	///=============================================================================================
	typedef basic_string<char>    string;
	typedef basic_string<wchar_t> wstring;

}

typedef simstd::string  astring;
typedef simstd::wstring ustring;

#endif
