//#include <libbase/wstr.hpp>
//
//#include <libbase/memory.hpp>
//#include <libbase/cstr.hpp>
//#include <libbase/string.hpp>
//#include <system/logger.hpp>
//
//namespace Base {
//
//	///=============================================================================================
//	mstring::~mstring()
//	{
//		Memory::free(m_data);
//	}
//
//	mstring::mstring(const_pointer mstr) :
//		m_data(nullptr),
//		m_capa(0),
//		m_size(0)
//	{
//		const_pointer ptr = mstr;
//		while (ptr && *ptr) {
//			ptr += (cstr::length(ptr) + 1);
//			++m_size;
//		}
//		m_capa = ptr - mstr;
//		add_data(mstr);
//	}
//
//	mstring::mstring(const this_type & other):
//		m_data(nullptr),
//		m_capa(other.m_capa),
//		m_size(other.m_size)
//	{
//		add_data(other.m_data);
//	}
//
//	mstring & mstring::operator =(const this_type & other)
//	{
//		if (m_data != other.m_data)
//			mstring(other).swap(*this);
//		return *this;
//	}
//
//	mstring::mstring(this_type && other) :
//		mstring(nullptr)
//	{
//		swap(other);
//	}
//
//	mstring & mstring::operator =(this_type && other)
//	{
//		if (m_data != other.m_data)
//			swap(other);
//		return *this;
//	}
//
//	void mstring::push_back(const_pointer str)
//	{
//		if (!cstr::is_empty(str)) {
//			size_t size = cstr::length(str) + 1;
//			size_t new_index = m_capa;
//			m_capa += size;
//			Memory::realloc(m_data, sizeof(value_type) * (m_capa + 1));
//			cstr::char_traits<value_type>::copy(&m_data[new_index], str, size);
//			m_data[m_capa] = static_cast<value_type>(0);
//			++m_size;
//		}
//	}
//
//	mstring::size_type mstring::size() const
//	{
//		return m_size;
//	}
//
//	mstring::size_type mstring::capacity() const
//	{
//		return m_capa;
//	}
//
//	mstring::const_pointer mstring::c_str() const
//	{
//		return m_data;
//	}
//
//	mstring::const_pointer mstring::operator [](size_type index) const
//	{
//		auto ptr = c_str();
//		size_t cnt = 0;
//		while (*ptr && (cnt++ < index)) {
//			ptr += (cstr::length(ptr) + 1);
//		}
//		return ptr;
//	}
//
//	void mstring::swap(this_type & other)
//	{
//		using std::swap;
//		swap(m_data, other.m_data);
//		swap(m_size, other.m_size);
//		swap(m_capa, other.m_capa);
//	}
//
//	void mstring::add_data(const_pointer mstr)
//	{
//		if (m_capa) {
//			m_data = Memory::calloc<pointer>(m_capa + 1);
//			cstr::char_traits<value_type>::copy(m_data, mstr, m_capa);
//			m_data[m_capa] = static_cast<value_type>(0);
//		}
//	}
//
//}
