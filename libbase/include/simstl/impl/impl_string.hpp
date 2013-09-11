#ifndef SARALIB_STL_STRING_HPP_
#define SARALIB_STL_STRING_HPP_

#include "impl_types.hpp"
#include "impl_string_char_traits.hpp"
#include "impl_iterator_base.hpp"
#include "impl_iterator_pvt_normal_iterator.hpp"
#include "impl_iterator_pvt_const_value_iterator.hpp"
#include "impl_iterator_reverse_iterator.hpp"
#include "impl_memory.hpp"

namespace sarastd {
	template<typename Char, typename Traits>
	class basic_string {
		static const sarastd::size_t MIN_ALLOC_BLOCK = 16;
		typedef basic_string this_type;

	public:
		typedef Traits traits_type;
		typedef sarastd::size_t size_type;
		typedef sarastd::ptrdiff_t difference_type;

		typedef typename traits_type::char_type value_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * pointer;
		typedef const value_type * const_pointer;

		typedef sarastd::pvt::_normal_iterator<pointer> iterator;
		typedef sarastd::pvt::_normal_iterator<const_pointer> const_iterator;
		typedef sarastd::reverse_iterator<iterator> reverse_iterator;
		typedef sarastd::reverse_iterator<const_iterator> const_reverse_iterator;

		static const size_type npos = ~(size_type)0;

		~basic_string();

		basic_string();
		basic_string(const this_type & right);
		basic_string(size_type count, value_type in);
		basic_string(const_pointer in, size_type count = npos);

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
		int compare(size_type pos1, size_type count1, const this_type & str) const;
		int compare(size_type pos1, size_type count1, const_pointer str) const;
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

		this_type & operator =(const this_type & right);
		this_type & operator =(const_pointer right);
		this_type & operator =(value_type ch);

		this_type & operator +=(const this_type & str);
		this_type & operator +=(const_pointer str);
		this_type & operator +=(value_type ch);

		this_type operator +(const this_type & str) const;
		this_type operator +(const_pointer str) const;
		this_type operator +(value_type ch) const;

		bool operator ==(const this_type & str) const;
		bool operator ==(const_pointer str) const;

		bool operator !=(const this_type & in) const;
		bool operator !=(const_pointer in) const;

		bool operator <(const this_type & str) const;
		bool operator <(const_pointer str) const;

		bool operator >(const this_type & str) const;
		bool operator >(const_pointer str) const;

		bool operator <=(const this_type & str) const;
		bool operator <=(const_pointer str) const;

		bool operator >=(const this_type & str) const;
		bool operator >=(const_pointer str) const;

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

		size_type get_necessary_capacity(size_type addToSize) const;

		void split(size_type necessaryCapacity);

		int compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2) const;
	};

	///=============================================================================================
	template<typename Char, typename Traits>
	struct basic_string<Char, Traits>::string_impl: public sarastd::pvt::ref_counter {
		static string_impl * allocate(size_type capa);

		void append(const_pointer str, size_type count);

		void append(value_type ch, size_type count);

		void set_size(size_type new_size);

		void set_capacity(size_type new_capacity);

		size_type get_capacity() const
		{
			return m_capa;
		}

		size_type get_size() const
		{
			return m_size;
		}

		void * get_str_data() const
		{
			return (void*)m_str;
		}

	private:
		~string_impl();

		string_impl(size_type capa = 1);

		void deallocate() const;

		size_type m_capa;
		size_type m_size;
		value_type m_str[1];
	};

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::string_impl * basic_string<Char, Traits>::string_impl::allocate(size_type capa)
	{
		string_impl * impl = (string_impl*)sarastd::pvt::_allocate<char>(sizeof(string_impl) + capa * sizeof(value_type));
		new (impl, sarastd::nothrow) string_impl(capa);
		return impl;
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::string_impl::append(const_pointer str, size_type count)
	{
		if (str && count) {
			traits_type::copy(m_str + m_size, str, count);
			set_size(m_size + count);
		}
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::string_impl::append(value_type ch, size_type count)
	{
		if (count) {
			traits_type::assign(m_str + m_size, count, ch);
			set_size(m_size + count);
		}
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::string_impl::set_size(size_type new_size)
	{
		m_size = new_size;
		m_str[m_size] = 0;
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::string_impl::set_capacity(size_type new_capacity)
	{
		m_capa = new_capacity;
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::string_impl::~string_impl()
	{
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::string_impl::deallocate() const
	{
		this->~string_impl();
		sarastd::pvt::_deallocate((char*)this);
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::string_impl::string_impl(size_type capa) :
		m_capa(capa), m_size(0)
	{
		m_str[0] = 0;
	}

	///=============================================================================================
	template<typename Char, typename Traits>
	basic_string<Char, Traits>::~basic_string()
	{
		m_data->decrease_ref();
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::basic_string() :
		m_data(string_impl::allocate(get_new_capacity(0)))
	{
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::basic_string(const this_type & right) :
		m_data(right.m_data)
	{
		m_data->increase_ref();
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::basic_string(size_type count, value_type in) :
		m_data(string_impl::allocate(get_new_capacity(count)))
	{
		m_data->append(in, count);
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::basic_string(const_pointer in, size_type count) :
		m_data(string_impl::allocate(get_new_capacity((in && count == npos) ? count = traits_type::length(in) : count)))
	{
		m_data->append(in, count);
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::empty() const
	{
		return m_data->get_size() == 0;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::capacity() const
	{
		return m_data->get_capacity();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::size() const
	{
		return m_data->get_size();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::length() const
	{
		return m_data->get_size();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_pointer basic_string<Char, Traits>::c_str() const
	{
		return (const_pointer)m_data->get_str_data();
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::swap(this_type & in)
	{
		using sarastd::swap;
		swap(m_data, in.m_data);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_iterator basic_string<Char, Traits>::cbegin() const
	{
		return const_iterator(c_str());
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_iterator basic_string<Char, Traits>::begin() const
	{
		return cbegin();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::begin()
	{
		return iterator(_str());
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_iterator basic_string<Char, Traits>::cend() const
	{
		return cbegin() + size();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_iterator basic_string<Char, Traits>::end() const
	{
		return cbegin() + size();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::end()
	{
		return begin() + size();
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::clear()
	{
		if (!empty()) {
			split(capacity());
			m_data->set_size(0);
		}
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::reserve(size_type capa)
	{
		if (capacity() < capa) {
			this_type(c_str(), size(), capa).swap(*this);
		}
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(const_pointer str) const
	{
		return compare(c_str(), length(), str, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(const this_type & str) const
	{
		return compare(c_str(), length(), str.c_str(), str.length());
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(size_type pos1, size_type count1, const this_type & str) const
	{
		return compare(c_str() + pos1, count1, str.c_str(), str.length());
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(size_type pos1, size_type count1, const_pointer str) const
	{
		return compare(c_str() + pos1, count1, str, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(size_type pos1, size_type count1, const_pointer str, size_type count2) const
	{
		return compare(c_str() + pos1, count1, str, count2);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::append(size_type count, value_type ch)
	{
		return append(this_type(count, ch));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::append(const this_type & str)
	{
		append(str.c_str(), str.size());
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::append(const this_type & str, size_type pos, size_type count)
	{
		return append(str.c_str() + pos, count);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::append(const_pointer str, size_type count)
	{
		if (str && count) {
			if (c_str() <= str && str < (c_str() + size())) { // append to itself
				printf("append to itself\n");
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

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::append(const_pointer str)
	{
		return append(str, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::assign(size_type count, value_type ch)
	{
		this_type(count, ch).swap(*this);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::assign(const this_type & str)
	{
		this_type(str).swap(*this);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::assign(const this_type & str, size_type pos, size_type count)
	{
		this_type(str.c_str() + pos, count).swap(*this);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::assign(const_pointer str, size_type count)
	{
		this_type(str, count).swap(*this);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::assign(const_pointer str)
	{
		this_type(str).swap(*this);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::insert(const_iterator pos, value_type ch)
	{
		typename sarastd::iterator_traits<const_iterator>::difference_type posFirst = sarastd::distance(cbegin(), pos);
		insert(posFirst, 0, 1, ch);
		return iterator(_str() + posFirst);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::insert(iterator pos, size_type count, value_type ch)
	{
		typename sarastd::iterator_traits<const_iterator>::difference_type posFirst = sarastd::distance(begin(), pos);
		insert(posFirst, 0, count, ch);
		return iterator(_str() + posFirst);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::insert(size_type index, size_type count, value_type ch)
	{
		return replace(index, 0, count, ch);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::insert(size_type index, const_pointer str)
	{
		return replace(index, 0, str, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::insert(size_type index, const_pointer str, size_type count)
	{
		return replace(index, 0, str, count);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::insert(size_type index, const this_type & str)
	{
		return replace(index, 0, str.c_str(), str.length());
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::insert(size_type index, const this_type & str, size_type index_str, size_type count)
	{
		return replace(index, 0, str.c_str(), count);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator =(const this_type & right)
	{
		return assign(right);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator =(const_pointer right)
	{
		return assign(right);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator =(value_type ch)
	{
		return assign(1, ch);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator +=(const this_type & str)
	{
		return append(str);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator +=(const_pointer str)
	{
		return append(str);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::operator +=(value_type ch)
	{
		return append(1, ch);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type basic_string<Char, Traits>::operator +(const this_type & str) const
	{
		return this_type(c_str(), size(), size() + str.size()) += str;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type basic_string<Char, Traits>::operator +(const_pointer str) const
	{
		return this_type(c_str(), size(), size() + traits_type::length(str)) += str;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type basic_string<Char, Traits>::operator +(value_type ch) const
	{
		return this_type(c_str(), size(), size() + 1) += ch;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator ==(const this_type & str) const
	{
		return compare(c_str(), size(), str.c_str(), str.size()) == 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator ==(const_pointer str) const
	{
		return compare(c_str(), size(), str, traits_type::length(str)) == 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator !=(const this_type & str) const
	{
		return !operator ==(str);
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator !=(const_pointer str) const
	{
		return !operator ==(str);
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator <(const this_type & str) const
	{
		return compare(c_str(), size(), str.c_str(), str.size()) < 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator <(const_pointer str) const
	{
		return compare(c_str(), size(), str, traits_type::length(str)) < 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator >(const this_type & str) const
	{
		return compare(c_str(), size(), str.c_str(), str.size()) > 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator >(const_pointer str) const
	{
		return compare(c_str(), size(), str, traits_type::length(str)) > 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator <=(const this_type & str) const
	{
		return compare(c_str(), size(), str.c_str(), str.size()) <= 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator <=(const_pointer str) const
	{
		return compare(c_str(), size(), str, traits_type::length(str)) <= 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator >=(const this_type & str) const
	{
		return compare(c_str(), size(), str.c_str(), str.size()) >= 0;
	}

	template<typename Char, typename Traits>
	bool basic_string<Char, Traits>::operator >=(const_pointer str) const
	{
		return compare(c_str(), size(), str, traits_type::length(str)) >= 0;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_reference basic_string<Char, Traits>::operator [](size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::reference basic_string<Char, Traits>::operator [](size_type in)
	{
		split();
		return *(_str() + in);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::const_reference basic_string<Char, Traits>::at(size_type in) const
	{
		return *(c_str() + in);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::reference basic_string<Char, Traits>::at(size_type in)
	{
		split();
		return *(_str() + in);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::replace(size_type pos, size_type len, const this_type & str)
	{
		return replace(pos, len, str.c_str(), str.size());
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen)
	{
		return replace(pos, len, &str[subpos], sublen);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::replace(size_type pos, size_type len, const_pointer str)
	{
		return replace(pos, len, str, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::replace(size_type pos, size_type len, const_pointer str, size_type count)
	{
		pos = sarastd::min(pos, size());
		len = sarastd::min(len, size() - pos);
		this_type tmp(c_str(), pos, size() - len + count);
		tmp.append(str, count);
		pos = sarastd::min(pos + len, size());
		tmp.append(c_str() + pos, size() - pos);
		swap(tmp);
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::replace(size_type pos, size_type len, size_type count, value_type ch)
	{
		return replace(pos, len, this_type(count, ch));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type basic_string<Char, Traits>::substr(size_type pos, size_type n) const
	{
		return this_type(c_str() + pos, n);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::this_type & basic_string<Char, Traits>::erase(size_type pos, size_type count)
	{
		size_type posLast = (count == npos) ? size() : sarastd::min(size(), pos + count);
		erase(sarastd::next(cbegin(), pos), sarastd::next(cbegin(), posLast));
		return *this;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::erase(const_iterator position)
	{
		return erase(position, sarastd::next(position, 1));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::iterator basic_string<Char, Traits>::erase(const_iterator first, const_iterator last)
	{
		typename sarastd::iterator_traits<const_iterator>::difference_type posFirst = sarastd::distance(cbegin(), first);
		typename sarastd::iterator_traits<const_iterator>::difference_type posLast = sarastd::distance(cbegin(), sarastd::min(last, cend()));
		if (m_data->count() > 1) {
			this_type tmp(c_str(), posFirst, capacity());
			tmp.append(c_str() + posLast, size() - posLast);
			tmp.swap(*this);
		} else {
			traits_type::move(_str() + posFirst, _str() + posLast, size() - posLast + 1);
			m_data->set_size(size() - (posLast - posFirst));
		}
		return iterator(_str() + posFirst);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find(const this_type & str, size_type pos) const
	{
		return find(str.c_str(), pos);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find(const_pointer str, size_type pos) const
	{
		const_pointer find = traits_type::find(c_str() + pos, str);
		return (find) ? find - c_str() : npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find(value_type ch, size_type pos) const
	{
		const_pointer find = traits_type::find(c_str() + pos, size(), ch);
		return (find) ? find - c_str() : npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::rfind(const this_type & str, size_type pos) const
	{
		return rfind(str.c_str(), pos, str.length());
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::rfind(const_pointer str, size_type pos, size_type count) const
	{
		pos = sarastd::min(pos, size());
		if (count != 0 && count <= pos) {
			pos -= (count - 1);
			while (pos > 0)
				if (traits_type::find(c_str() + --pos, str))
					return pos;
		}
		return npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::rfind(const_pointer str, size_type pos) const
	{
		return rfind(str, pos, traits_type::length(str));
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::rfind(value_type ch, size_type pos) const
	{
		pos = sarastd::min(pos, size());
		while (pos > 0)
			if (traits_type::eq(at(--pos), ch))
				return pos;
		return npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find_first_of(const this_type & str, size_type pos) const
	{
		for (; pos < size(); ++pos)
			if (traits_type::find(str.c_str(), str.size(), at(pos)))
				return pos;
		return npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find_last_of(const this_type & str, size_type pos) const
	{
		pos = sarastd::min(pos, size());
		while (pos > 0)
			if (traits_type::find(str.c_str(), str.size(), at(--pos)))
				return pos;
		return npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find_first_not_of(const this_type & str, size_type pos) const
	{
		for (; pos < size(); ++pos)
			if (traits_type::find(str.c_str(), str.size(), at(pos)) == 0)
				return pos;
		return npos;
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::find_last_not_of(const this_type & str, size_type pos) const
	{
		pos = sarastd::min(pos, size());
		while (pos > 0)
			if (traits_type::find(str.c_str(), str.size(), at(--pos)) == 0)
				return pos;
		return npos;
	}

	template<typename Char, typename Traits>
	basic_string<Char, Traits>::basic_string(const_pointer str, size_type count, size_type capacity) :
		m_data(string_impl::allocate(get_new_capacity(capacity)))
	{
		m_data->append(str, count);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::pointer basic_string<Char, Traits>::_str() const
	{
		return (pointer)m_data->get_str_data();
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::get_new_capacity(size_type capacity) const
	{
		return sarastd::max(size_type(MIN_ALLOC_BLOCK), capacity);
	}

	template<typename Char, typename Traits>
	typename basic_string<Char, Traits>::size_type basic_string<Char, Traits>::get_necessary_capacity(size_type addToSize) const
	{
		if (capacity() < (size() + addToSize))
			return size() + sarastd::max(size(), addToSize);
		return capacity();
	}

	template<typename Char, typename Traits>
	void basic_string<Char, Traits>::split(size_type necesseryCapacity)
	{
		if (capacity() < necesseryCapacity)
			this_type(c_str(), size(), necesseryCapacity).swap(*this);
		else if (m_data->count() > 1)
			this_type(c_str(), size(), capacity()).swap(*this);
	}

	template<typename Char, typename Traits>
	int basic_string<Char, Traits>::compare(const_pointer str1, size_type count1, const_pointer str2, size_type count2) const
	{
		int result = traits_type::compare(str1, str2, sarastd::min(count1, count2));
		if (result != 0)
			return result;
		if (count1 < count2)
			return -1;
		if (count1 > count2)
			return 1;
		return 0;
	}

	///=============================================================================================
	typedef basic_string<char, char_traits<char> > string;

}

#endif
