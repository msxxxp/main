#ifndef _LIBBASE_LITE_STRING_HPP_
#define _LIBBASE_LITE_STRING_HPP_

#include <libbase/std.hpp>
#include <libbase/char.hpp>
#include <libbase/ref_cnt.hpp>

namespace Base {
	template<typename CharType, typename Traits>
	class basic_string
	{
		static const size_t MIN_ALLOC_BLOCK = 16;
		typedef basic_string this_type;

	public:
		typedef Traits traits_type;
		typedef typename Traits::char_type value_type;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type & reference;
		typedef const value_type & const_reference;
		typedef value_type * iterator;
		typedef const value_type * const_iterator;

		static const size_type npos = ~(size_type)0;

		~basic_string()
		{
			m_data->decrease_ref();
		}

		basic_string() :
			m_data(allocate_impl(MIN_ALLOC_BLOCK))
		{
		}

		basic_string(const this_type & right) :
			m_data(right.m_data)
		{
			m_data->increase_ref();
		}

		basic_string(size_type count, CharType in) :
			m_data(allocate_impl(count + 1))
		{
			m_data->init(in, count);
		}

		basic_string(const CharType * in, size_type count = npos) :
			m_data(allocate_impl(((in && count == npos) ? (count = Traits::length(in)) : count) + 1))
		{
			m_data->init(in, count);
		}

		bool empty() const {return m_data->size() == 0;}

		size_type capacity() const {return m_data->capacity();}

		size_t size() const {return m_data->size();}

		size_t length() const {return m_data->size();}

		const CharType * c_str() const {return cbegin();}

		void swap(this_type & in) {using std::swap; swap(m_data, in.m_data);}

		const_iterator cbegin() const {return const_iterator(m_data->c_str());}

		const_iterator begin() const {return const_iterator(m_data->c_str());}

		iterator begin() {return iterator(m_data->c_str());}

		const_iterator cend() const {return const_iterator(m_data->c_str()) + size();}

		const_iterator end() const {return const_iterator(m_data->c_str()) + size();}

		iterator end() {return iterator(m_data->c_str()) + size();}

		void clear()
		{
			if (!empty()) {
				split();
				m_data->set_size(0);
			}
		}

		void reserve(size_t capa)
		{
			if (capacity() < capa) {
				if (m_data->is_shared()) {
					this_type(cbegin(), size(), capa).swap(*this);
				} else {
					Memory::realloc(m_data, sizeof(*m_data) + capa * sizeof(CharType));
					m_data->set_capacity(capa);
				}
			}
		}

		int compare(const CharType * str) const noexcept {return compare(c_str(), length(), str, traits_type::length(str));}

		int compare(const this_type & str) const noexcept {return compare(c_str(), length(), str.c_str(), str.length());}

		int compare(size_type pos1, size_type count1, const this_type & str) const noexcept {return compare(&c_str()[pos1], count1, str.c_str(), str.length());}

		int compare(size_type pos1, size_type count1, const CharType * str) const noexcept {return compare(&c_str()[pos1], count1, str, traits_type::length(str));}

		int compare(size_type pos1, size_type count1, const CharType * str, size_type count2) const noexcept {return compare(&c_str()[pos1], count1, str, count2);}

		this_type & append(size_type count, CharType ch) {return append(this_type(count, ch));}

		this_type & append(const this_type & str)
		{
			if (m_data == str.m_data)
				this_type(c_str(), size(), size() + str.size()).swap(*this);
			append(str.c_str(), str.size());
			return *this;
		}

		this_type & append(const this_type & str, size_type pos, size_type count) {return append(&str[pos], count);}

		this_type & append(const CharType * str, size_type count)
		{
			if (str && count) {
				reserve(size() + count);
				m_data->append(str, count);
			}
			return *this;
		}

		this_type & append(const CharType * str) {return append(str, Traits::length(str));}

		this_type & assign(size_type count, CharType ch) {this_type(count, ch).swap(*this); return *this;}

		this_type & assign(const this_type & str) {this_type(str).swap(*this); return *this;}

		this_type & assign(const this_type & str, size_type pos, size_type count = npos) {this_type(&str[pos], count).swap(*this); return *this;}

		this_type & assign(this_type && str) {swap(str); return *this;}

		this_type & assign(const CharType * str, size_type count) {this_type(str, count).swap(*this); return *this;}

		this_type & assign(const CharType * str) {this_type(str).swap(*this); return *this;}

		this_type & insert(size_type index, const this_type & str) {return replace(index, 0, str.c_str(), str.length());}

		this_type & operator =(const this_type & right) {return assign(right);}

		this_type & operator =(const CharType * right) {return assign(right);}

		this_type & operator =(CharType ch) {return assign(1, ch);}

		this_type & operator =(this_type && right) noexcept  {swap(right); return *this;}

		this_type & operator +=(const this_type & str) {return append(str);}

		this_type & operator +=(const CharType * str) {return append(str);}

		this_type & operator +=(CharType ch) {return append(1, ch);}

		this_type operator +(const this_type & str) const {return this_type(c_str(), size(), size() + str.size()) += str;}

		this_type operator +(const CharType * str) const {return this_type(c_str(), size(), size() + Traits::length(str)) += str;}

		this_type operator +(CharType ch) const {return this_type(c_str(), size(), size() + 1) += ch;}

		bool operator ==(const this_type & str) const {return Traits::compare(c_str(), str.c_str(), std::min(size(), str.size())) == 0;}

		bool operator ==(const CharType * str) const {return Traits::compare(c_str(), str, std::min(size(), Traits::length(str))) == 0;}

		bool operator !=(const this_type & in) const {return !operator ==(in);}

		bool operator !=(const CharType * in) const {return !operator ==(in);}

		bool operator <(const this_type & str) const {return Traits::compare(c_str(), str.c_str(), std::min(size(), str.size())) < 0;}

		bool operator <(const CharType * str) const {return Traits::compare(c_str(), str, std::min(size(), Traits::length(str))) < 0;}

		bool operator >(const this_type & str) const {return Traits::compare(c_str(), str.c_str(), std::min(size(), str.size())) > 0;}

		bool operator >(const CharType * str) const {return Traits::compare(c_str(), str, std::min(size(), Traits::length(str))) > 0;}

		bool operator <=(const this_type & str) const {return Traits::compare(c_str(), str.c_str(), std::min(size(), str.size())) <= 0;}

		bool operator <=(const CharType * str) const {return Traits::compare(c_str(), str, std::min(size(), Traits::length(str))) <= 0;}

		bool operator >=(const this_type & str) const {return Traits::compare(c_str(), str.c_str(), std::min(size(), str.size())) >= 0;}

		bool operator >=(const CharType * str) const {return Traits::compare(c_str(), str, std::min(size(), Traits::length(str))) >= 0;}

		const CharType & operator [](size_type in) const {return *(cbegin() + in);}

		CharType & operator [](size_type in) {split(); return *(begin() + in);}

		const CharType & at(size_type in) const {return *(cbegin() + in);}

		CharType & at(size_type in) {split(); return *(begin() + in);}

		this_type & replace(size_type pos, size_type len, const this_type & str) {return replace(pos, len, str.c_str(), str.size());}

		this_type & replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen) {return replace(pos, len, &str[subpos], sublen);}

		this_type & replace(size_type pos, size_type len, const CharType * str) {return replace(pos, len, str, Traits::length(str));}

		this_type & replace(size_type pos, size_type len, const CharType * str, size_type count)
		{
			pos = std::min(pos, size());
			len = std::min(len, size() - pos);
			this_type tmp(cbegin(), pos, size() - len + count);
			tmp.append(str, count);
			pos = std::min(pos + len, size());
			tmp.append(cbegin() + pos, size() - pos);
			swap(tmp);
			return *this;
		}

		this_type & replace(size_type pos, size_type len, size_type count, CharType ch) {return replace(pos, len, this_type(count, ch));}

		this_type substr(size_t pos = 0, size_t n = npos) const {return this_type(cbegin() + pos, n);}

		this_type & erase(size_type pos = 0, size_type count = npos)
		{
			if (pos < size()) {
				size_type last = (count == npos) ? size() : std::min(size(), pos + count);
				this_type tmp(cbegin(), pos, capacity());
				tmp.append(cbegin() + last, size() - last);
				tmp.swap(*this);
			}
			return *this;
		}

		size_type find(const this_type & str, size_type pos = 0) const {return find(str.c_str(), pos);}

		size_type find(const CharType * str, size_type pos = 0) const
		{
			auto find = Traits::find(cbegin() + pos, str);
			return (find) ? find - cbegin() : npos;
		}

		size_type find(CharType ch, size_type pos = 0) const
		{
			auto find = Traits::find(cbegin() + pos, size(), ch);
			return (find) ? find - cbegin() : npos;
		}

		size_t rfind(const this_type & str, size_type pos = npos) const {return rfind(str.c_str(), pos, str.length());}

		size_t rfind(const CharType * str, size_type pos, size_type count) const
		{
			pos = std::min(pos, size());
			if (count != 0 && count <= pos) {
				pos -= (count - 1);
				while (pos > 0)
					if (Traits::find(cbegin() + --pos, str))
						return pos;
			}
			return npos;
		}

		size_t rfind(const CharType * str, size_type pos = npos) const {return rfind(str, pos, Traits::length(str));}

		size_type rfind(CharType ch, size_type pos = npos) const
		{
			pos = std::min(pos, size());
			while (pos > 0)
				if (Traits::eq(at(--pos), ch))
					return pos;
			return npos;
		}

		size_type find_first_of(const this_type & str, size_t pos = 0) const
		{
			for (; pos < size(); ++pos)
				if (Traits::find(str.c_str(), str.size(), at(pos)))
					return pos;
			return npos;
		}

		size_type find_last_of(const this_type & str, size_type pos = npos) const
		{
			pos = std::min(pos, size());
			while (pos > 0)
				if (Traits::find(str.c_str(), str.size(), at(--pos)))
					return pos;
			return npos;
		}

		size_type find_first_not_of(const this_type & str, size_type pos = 0) const
		{
			for (; pos < size(); ++pos)
				if (Traits::find(str.c_str(), str.size(), at(pos)) == nullptr)
					return pos;
			return npos;
		}

		size_type find_last_not_of(const this_type & str, size_type pos = npos) const
		{
			pos = std::min(pos, size());
			while (pos > 0)
				if (Traits::find(str.c_str(), str.size(), at(--pos)) == nullptr)
					return pos;
			return npos;
		}

	private:
		int compare(const CharType * str1, size_type count1, const CharType * str2, size_type count2) const noexcept
		{
		    int result = traits_type::compare(str1, str2, std::min(count1, count2));
		    if (result != 0)
		        return result;
		    if (count1 < count2)
		        return -1;
		    if (count1 > count2)
		        return 1;
		    return 0;
		}

		struct string_impl: public Base::ref_counter {
			void free() const override {
				this->~string_impl();
				Memory::free(this);
			}

			string_impl(): m_capa(1), m_size(0) {m_str[0] = 0;}

			string_impl(size_type capa): m_capa(capa), m_size(0) {m_str[0] = 0;}

			void init(CharType ch, size_type count)
			{
				if (count) {
					Traits::assign(m_str, count, ch);
					set_size(count);
				}
			}

			void init(const CharType * str, size_type count)
			{
				if (str && count) {
					Traits::copy(m_str, str, count);
					set_size(count);
				}
			}

			void append(const CharType * str, size_type count)
			{
				if (str && count) {
					Traits::copy(m_str + m_size, str, count);
					set_size(m_size + count);
				}
			}

			void set_size(size_type new_size) {m_size = new_size; m_str[m_size] = 0;}

			void set_capacity(size_type new_capacity) {m_capa = new_capacity;}

			size_type capacity() const {return m_capa;}

			size_type size() const {return m_size;}

			void * c_str() const {return (void*)m_str;}

//			void print() {
//				LogNoise(L"this: %p, sizeof: %Id\n", this, sizeof(*this));
//				LogNoise(L"capa: %Id, size: %Id, ref: %Id, str: '%s'\n", m_capa, m_size, count(), m_str);
//			}
		private:
			~string_impl() {}

			size_type m_capa;
			size_type m_size;
			value_type m_str[1];
		} * m_data;

		string_impl * allocate_impl(size_t capa)
		{
			string_impl * impl = Memory::malloc<string_impl*>(sizeof(string_impl) + capa * sizeof(CharType));
			new (impl) string_impl(capa);
			return impl;
		}

		void split()
		{
			if (m_data->is_shared())
				this_type(cbegin(), size(), capacity()).swap(*this);
		}

		basic_string(const CharType * str, size_type count, size_type capacity) :
			m_data(allocate_impl(capacity))
		{
			m_data->init(str, count);
		}
	};
}

#endif
