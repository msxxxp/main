#include <excis/exception.hpp>

//#include <basis/sys/console.hpp>
//#include <basis/sys/totext.hpp>

namespace exception {

	const wchar_t* const THROW_PLACE_FORMAT = L"%S: %d [%S]";
	const wchar_t* const DEFAULT_WHERE = L"";

	const wchar_t* throw_place_string(const char* file, size_t line, const char* func)
	{
		const size_t LENGTH = MAX_PATH;
		auto buff = memory::malloc<wchar_t*>(LENGTH);
		safe_snprintf(buff, LENGTH, THROW_PLACE_FORMAT, file, line, func);
		return buff;
	}

	Abstract::~Abstract()
	{
		if (m_where != DEFAULT_WHERE)
			memory::free(m_where);
	}

	Abstract::Abstract():
		file(""),
		func(""),
		line(static_cast<size_t>(-1)),
		m_where(DEFAULT_WHERE),
		m_prev_exc()
	{
	}

	Abstract::Abstract(const this_type& prev):
		file(""),
		func(""),
		line(static_cast<size_t>(-1)),
		m_where(DEFAULT_WHERE),
		m_prev_exc(prev.clone())
	{
	}

	Abstract::Abstract(PCSTR file, size_t line, PCSTR func) :
		file(file),
		func(func),
		line(line),
		m_where(DEFAULT_WHERE),
		m_prev_exc()
	{
	}

	Abstract::Abstract(const this_type& prev, PCSTR file, size_t line, PCSTR func) :
		file(file),
		func(func),
		line(line),
		m_where(DEFAULT_WHERE),
		m_prev_exc(prev.clone())
	{
	}

	ustring Abstract::where() const
	{
		if (m_where == DEFAULT_WHERE && line != static_cast<size_t>(-1))
			m_where = throw_place_string(file, line, func);
		return ustring(m_where);
	}

	Abstract::this_type* Abstract::get_prev() const
	{
		return m_prev_exc.get();
	}

	cstr::mstring Abstract::format_error() const
	{
		cstr::mstring msg;
		format_error(msg);
		return simstd::move(msg);
	}

}
