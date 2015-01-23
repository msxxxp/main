#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace traceback {

	Frame_i::~Frame_i()
	{
	}

	LazyFrame::~LazyFrame()
	{
		delete m_data;
	}

	LazyFrame::LazyFrame(void * address) :
		m_address(address),
		m_data(nullptr)
	{
	}

	LazyFrame::LazyFrame(LazyFrame && other) :
		m_address(nullptr),
		m_data(nullptr)
	{
		swap(other);
	}

	LazyFrame & LazyFrame::operator =(LazyFrame && right)
	{
		LazyFrame(simstd::move(right)).swap(*this);
		return *this;
	}

	void LazyFrame::swap(LazyFrame & other)
	{
		using simstd::swap;
		swap(m_address, other.m_address);
		swap(m_data, other.m_data);
	}

	void* LazyFrame::address() const
	{
		return m_address;
	}

	const ustring& LazyFrame::module() const
	{
		init_data();
		return m_data->module();
	}

	const ustring& LazyFrame::file() const
	{
		init_data();
		return m_data->file();
	}

	const ustring& LazyFrame::function() const
	{
		init_data();
		return m_data->function();
	}

	size_t LazyFrame::line() const
	{
		init_data();
		return m_data->line();
	}

	size_t LazyFrame::offset() const
	{
		init_data();
		return m_data->offset();
	}

	ustring LazyFrame::to_str() const
	{
		wchar_t buf[MAX_PATH];
		if (file().empty())
			if (function().empty())
				safe_snprintf(buf, lengthof(buf), L"[%s] (%p)", module().c_str(), address());
			else
				safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix", module().c_str(), address(), function().c_str(), offset());
		else
			safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix {%s:%Iu}", module().c_str(), address(), function().c_str(), offset(), file().c_str(), line());
		return simstd::move(ustring(buf));
	}

	void LazyFrame::init_data() const
	{
		TraceFunc();
		if (!m_data)
			m_data = read_frame_data(m_address);
		TraceFunc();
	}

}
