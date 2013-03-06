#ifndef _LIBBASE_STRING_HPP_
#define _LIBBASE_STRING_HPP_

#include <libbase/std.hpp>
#include <libbase/char.hpp>
#include <libbase/ref_cnt.hpp>

namespace Base {
	template<typename CharType, typename Traits = char_traits<CharType> >
	class basic_string {
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
			m_data(allocate_impl(MIN_ALLOC_BLOCK, sizeof(CharType)))
		{
		}

		basic_string(const this_type & right) :
			m_data(right.m_data)
		{
			m_data->increase_ref();
		}

		basic_string(size_type count, CharType in) :
			m_data(allocate_impl(count + 1, sizeof(CharType)))
		{
			m_data->init(in, count);
		}

		basic_string(const CharType * in, size_type count = npos) :
			m_data(allocate_impl(((in && count == npos) ? (count = Traits::length(in)) : count) + 1, sizeof(CharType)))
		{
			m_data->init(in, count);
		}

		bool empty() const {return m_data->m_size == 0;}

		size_type capacity() const {return m_data->m_capa;}

		size_t size() const {return m_data->m_size;}

		const CharType * c_str() const {return m_data->m_str;}

		void swap(this_type & in) {using std::swap; swap(m_data, in.m_data);}

		iterator begin() {return (iterator)c_str();}

		const_iterator begin() const {return c_str();}

		const_iterator cbegin() const {return c_str();}

		iterator end() {return (iterator)(c_str() + size());}

		const_iterator end() const {return c_str() + size();}

		const_iterator cend() const {return c_str() + size();}

		void clear()
		{
			if (!empty()) {
				split();
				m_data->m_str[0] = 0;
				m_data->m_size = 0;
			}
		}

		void reserve(size_t capa)
		{
			if (capacity() < capa) {
				if (m_data->is_shared()) {
					this_type(c_str(), size(), capacity()).swap(*this);
				} else {
					Memory::realloc(m_data, sizeof(*m_data) + capa * sizeof(CharType));
					m_data->m_capa = capa;
				}
			}
		}

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
			if (count) {
				reserve(size() + count);
				Traits::copy(&m_data->m_str[size()], str, count);
				m_data->m_size += count;
				m_data->m_str[size()] = 0;
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

		bool operator ==(const this_type & in) const {return Traits::compare(c_str(), in.c_str()) == 0;}

		bool operator ==(const CharType * in) const {return Traits::compare(c_str(), in) == 0;}

		bool operator !=(const this_type & in) const {return !operator ==(in);}

		bool operator !=(const CharType * in) const {return !operator ==(in);}

		bool operator <(const this_type & in) const {return Traits::compare(c_str(), in.c_str()) < 0;}

		bool operator <(const CharType * in) const {return Traits::compare(c_str(), in) < 0;}

		bool operator >(const this_type & in) const {return Traits::compare(c_str(), in.c_str()) > 0;}

		bool operator >(const CharType * in) const {return Traits::compare(c_str(), in) > 0;}

		bool operator <=(const this_type & in) const {return Traits::compare(c_str(), in.c_str()) <= 0;}

		bool operator <=(const CharType * in) const {return Traits::compare(c_str(), in) <= 0;}

		bool operator >=(const this_type & in) const {return Traits::compare(c_str(), in.c_str()) >= 0;}

		bool operator >=(const CharType * in) const {return Traits::compare(c_str(), in) >= 0;}

		const CharType & operator [](size_type in) const {return m_data->m_str[in];}

		CharType & operator [](size_type in) {split(); return m_data->m_str[in];}

		const CharType & at(size_type in) const {return m_data->m_str[in];}

		CharType & at(size_type in) {split(); return m_data->m_str[in];}

		this_type & replace(size_type pos, size_type len, const this_type & str) {return replace(pos, len, str.c_str(), str.size());}

		this_type & replace(size_type pos, size_type len, const this_type & str, size_type subpos, size_type sublen) {return replace(pos, len, &str[subpos], sublen);}

		this_type & replace(size_type pos, size_type len, const CharType * str) {return replace(pos, len, str, Traits::length(str));}

		this_type & replace(size_type pos, size_type len, const CharType * str, size_type count)
		{
			pos = std::min(pos, size());
			this_type tmp(nullptr, 0, size() + count);
			tmp.append(m_data->m_str, pos);
			tmp.append(str, count);
			pos = std::min(pos + len, size());
			tmp.append(&m_data->m_str[pos], size() - pos);
			swap(tmp);
			return *this;
		}

		this_type & replace(size_type pos, size_type len, size_type count, CharType ch) {return replace(pos, len, this_type(count, ch));}

		this_type substr(size_t pos = 0, size_t n = npos) const {return this_type(&m_data->m_str[pos], n);}

		this_type & erase(size_type pos = 0, size_type count = npos)
		{
			if (pos < size()) {
				size_type size2 = count == npos ? size() : std::min(size(), pos + count);
				this_type(c_str(), pos, c_str() + size2, size() - size2).swap(*this);
			}
			return *this;
		}

		size_type find(const this_type & str, size_type pos = 0) const {return find(str.c_str(), pos);}

		size_type find(const CharType * str, size_type pos = 0) const
		{
			auto find = Traits::find(c_str() + pos, str);
			return (find) ? find - c_str() : npos;
		}

		size_type find(CharType ch, size_type pos = 0) const
		{
			auto find = Traits::find(c_str() + pos, size(), ch);
			return (find) ? find - c_str() : npos;
		}

		size_t rfind(const this_type & str, size_type pos = npos) const
		{
//			pos = std::min(pos, size());
//			while (pos > 0)
//				if (Traits::eq(at(--pos), ch))
//					return pos;
			return npos;
		}

		size_t rfind(const CharType * str, size_type pos = npos) const
		{
			return npos;
		}

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
		struct string_impl: public Base::ref_counter {
			size_type m_capa;
			size_type m_size;
			value_type m_str[1];

			void free() override {
				this->~string_impl();
				Memory::free(this);
			}

			string_impl(): m_capa(1), m_size(0) {m_str[0] = 0;}
			string_impl(size_type capa): m_capa(capa), m_size(0) {m_str[0] = 0;}

			void init(CharType ch, size_type count)
			{
				if (count) {
					Traits::assign(m_str, count, ch);
					m_size = count;
					m_str[count] = 0;
				}
			}

			void init(const CharType * str, size_type count)
			{
				if (str) {
					Traits::copy(m_str, str, count);
					m_str[count] = 0;
					m_size = count;
				}
			}

//			void print() {
//				LogNoise(L"this: %p, sizeof: %Id\n", this, sizeof(*this));
//				LogNoise(L"capa: %Id, size: %Id, ref: %Id, str: '%s'\n", m_capa, m_size, count(), m_str);
//			}

		private:
			~string_impl() {}

		} * m_data;

		string_impl * allocate_impl(size_t capa, size_type sizeOfChar)
		{
			string_impl * impl = Memory::malloc<string_impl*>(sizeof(string_impl) + capa * sizeOfChar);
			new (impl) string_impl(capa);
			return impl;
		}

		void split()
		{
			if (m_data->is_shared())
				this_type(c_str(), size(), capacity()).swap(*this);
		}

//		basic_string(const Type * str1, size_t size1, const Type * str2, size_t size2) :
//			m_data(alloc_cstr(size1 + size2 + 1))
//		{
//			if (size1) {
//				Memory::copy(m_data->m_str, str1, size1 * sizeof(Type));
//				m_data->m_size = size1;
//			}
//			if (size2) {
//				Memory::copy(m_data->m_str + size1, str2, size2 * sizeof(Type));
//				m_data->m_size += size2;
//			}
//		}

		basic_string(const CharType * str, size_type count, size_type capacity) :
			m_data(allocate_impl(capacity, sizeof(CharType)))
		{
			m_data->init(str, count);
		}
	};
}

typedef Base::basic_string<char> t_astring;
typedef Base::basic_string<wchar_t> t_ustring;

#endif
