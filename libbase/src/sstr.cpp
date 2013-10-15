#include <libbase/sstr.hpp>
#include <libbase/cstr.hpp>

#include <algorithm>

#include <stdarg.h>

namespace Base {
	struct Va_list {
		~Va_list()
		{
			va_end(m_args);
		}

		va_list & get_args()
		{
			return m_args;
		}

	private:
		va_list m_args;
	};

	sstr::~sstr()
	{
		free(m_data);
	}

	sstr::sstr():
		m_data(make_copy(L""))
	{
	}

	sstr::sstr(const_pointer ptr):
		m_data(make_copy(ptr))
	{
	}

	sstr::sstr(const this_type & other):
		m_data(make_copy(other.m_data))
	{
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	sstr::sstr(this_type && other):
		m_data(make_copy(L""))
	{
		swap(other);
	}

	sstr& sstr::operator =(const this_type & other)
	{
		if (m_data != other.m_data)
			this_type(other).swap(*this);
		return *this;
	}

	sstr & sstr::operator =(this_type && other)
	{
		if (m_data != other.m_data)
			this_type(std::move(other)).swap(*this);
		return *this;
	}

	sstr::const_pointer sstr::c_str() const
	{
		return m_data;
	}

	sstr::reference sstr::operator [](size_type index)
	{
		return m_data[index];
	}

	sstr::const_reference sstr::operator [](size_type index) const
	{
		return m_data[index];
	}

	void sstr::swap(this_type & other)
	{
		using std::swap;
		swap(m_data, other.m_data);
	}

	bool sstr::empty() const
	{
		return !static_cast<bool>(m_data[0]);
	}

	sstr::size_type sstr::size() const
	{
		return Cstr::length(m_data);
	}

	sstr::sstr(pointer ptr, bool /*construct*/):
		m_data(ptr)
	{
	}

	sstr::pointer sstr::make_copy(const_pointer ptr, size_type length)
	{
		pointer ret = Memory::calloc<pointer>(length + 1);
		Cstr::copy(ret, ptr, length);
		return ret;
	}

	sstr::pointer sstr::make_copy(const_pointer ptr)
	{
		return make_copy(ptr, Cstr::length(ptr));
	}

	sstr sstr::format(const_pointer fmt, va_list args)
	{
		const size_type default_buffer_size = 256;

		size_type currSize = default_buffer_size;
		pointer ptr = make_copy(L"", currSize);

		while (static_cast<unsigned>(::_vsnwprintf(ptr, currSize, fmt, args)) > currSize) {
			currSize *= 2;
			free(ptr);
			ptr = make_copy(L"", currSize);
		}
		return this_type(ptr, true);
	}

	sstr sstr::format(const_pointer fmt, ...)
	{
		Va_list args;
		va_start(args.get_args(), fmt);
		return format(fmt, args.get_args());
	}

	void sstr::free(pointer ptr)
	{
		Memory::free(ptr);
	}
}
