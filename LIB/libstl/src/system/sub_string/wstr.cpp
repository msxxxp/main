//#include <libbase/std.hpp>
//#include <libbase/wstr.hpp>
//#include <libbase/cstr.hpp>
//
//#include <algorithm>
//
//namespace Base {
//	wstr::~wstr()
//	{
//		free(m_data);
//	}
//
//	wstr::wstr() :
//		m_data(make_copy(L""))
//	{
//	}
//
//	wstr::wstr(const_pointer ptr) :
//		m_data(make_copy(ptr))
//	{
//	}
//
//	wstr::wstr(const this_type & other) :
//		m_data(make_copy(other.m_data))
//	{
//	}
//
//	wstr::wstr(this_type && other) :
////		m_data(make_copy(L""))
//		this_type()
//	{
//		swap(other);
//	}
//
//	wstr& wstr::operator =(const this_type & other)
//	{
//		if (m_data != other.m_data)
//			this_type(other).swap(*this);
//		return *this;
//	}
//
//	wstr & wstr::operator =(this_type && other)
//	{
//		if (m_data != other.m_data)
//			this_type(std::move(other)).swap(*this);
//		return *this;
//	}
//
//	wstr::this_type & wstr::operator += (const this_type & right)
//	{
//		this_type(m_data, size(), right.m_data, right.size()).swap(*this);
//		return *this;
//	}
//
//	wstr::this_type wstr::operator + (const this_type & right) const
//	{
//		return this_type(m_data, size(), right.m_data, right.size());
//	}
//
//	wstr::const_pointer wstr::c_str() const
//	{
//		return m_data;
//	}
//
//	wstr::reference wstr::operator [](size_type index)
//	{
//		return m_data[index];
//	}
//
//	wstr::const_reference wstr::operator [](size_type index) const
//	{
//		return m_data[index];
//	}
//
//	void wstr::swap(this_type & other)
//	{
//		using std::swap;
//		swap(m_data, other.m_data);
//	}
//
//	bool wstr::empty() const
//	{
//		return !static_cast<bool>(m_data[0]);
//	}
//
//	wstr::size_type wstr::size() const
//	{
//		return cstr::length(m_data);
//	}
//
//	wstr::wstr(pointer ptr, bool /*construct*/) :
//		m_data(ptr)
//	{
//	}
//
//	wstr::wstr(const_pointer str1, size_type length1, const_pointer str2, size_type length2):
//		m_data(make_copy(str1, length1, str2, length2))
//	{
//	}
//
//	wstr::pointer wstr::make_copy(const_pointer str, size_type length)
//	{
//		pointer ret = Memory::calloc<pointer>(length + 1);
//		cstr::copy(ret, str, length);
//		ret[length] = 0;
//		return ret;
//	}
//
//	wstr::pointer wstr::make_copy(const_pointer str)
//	{
//		return make_copy(str, cstr::length(str));
//	}
//
//	wstr::pointer wstr::make_copy(const_pointer str1, size_type length1, const_pointer str2, size_type length2)
//	{
//		pointer ret = Memory::calloc<pointer>(length1 + length2 + 1);
//		cstr::copy(ret, str1, length1);
//		cstr::copy(ret + length1, str2, length2);
//		ret[length1 + length2] = 0;
//		return ret;
//	}
//
//	wstr::pointer wstr::make_copy(const_pointer str1, const_pointer str2)
//	{
//		return make_copy(str1, cstr::length(str1), str2, cstr::length(str2));
//	}
//
//	void wstr::free(pointer str)
//	{
//		Memory::free(str);
//	}
//
//	wstr wstr::format(const_pointer fmt, va_list args)
//	{
//		const size_type default_buffer_size = 1024;
//
//		size_type currSize = default_buffer_size;
//		pointer ptr = make_copy(L"", currSize);
//
//		while (!Base::safe_vsnprintf(ptr, currSize, fmt, args)) {
//			free(ptr);
//			currSize *= 2;
//			ptr = make_copy(L"", currSize);
//		}
//		return this_type(ptr, true);
//	}
//
//	wstr wstr::format(const_pointer fmt, ...)
//	{
//		Base::Va_list args;
//		va_start(args, fmt);
//		return format(fmt, args);
//	}
//}
