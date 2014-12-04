#include <excis/exception.hpp>
#include <basis/sys/memory.hpp>

//#include <basis/sys/console.hpp>
//#include <basis/sys/totext.hpp>

namespace exception {

	namespace
	{
		const wchar_t* const THROW_PLACE_FORMAT = L"%S:%d {%S}";
		const wchar_t* const DEFAULT_STRING = L"";

		const wchar_t* throw_place_string(const char* file, size_t line, const char* func)
		{
			const size_t LENGTH = MAX_PATH;
			auto buff = memory::malloc<wchar_t*>(LENGTH);
			safe_snprintf(buff, LENGTH, THROW_PLACE_FORMAT, file, line, func);
			return buff;
		}

		void cleanup(const wchar_t* str)
		{
			if (str != DEFAULT_STRING)
				memory::free(str);
		}
	}

	Abstract::~Abstract()
	{
		cleanup(m_where);
	}

	Abstract::Abstract():
		file(""),
		func(""),
		line(static_cast<size_t>(-1)),
		m_what(nullptr),
		m_where(DEFAULT_STRING),
		m_prev_exc()
	{
	}

	Abstract::Abstract(const this_type& prev):
		file(""),
		func(""),
		line(static_cast<size_t>(-1)),
		m_what(nullptr),
		m_where(DEFAULT_STRING),
		m_prev_exc(prev.clone())
	{
	}

	const wchar_t* Abstract::what() const
	{
		return m_what;
	}

	const wchar_t* Abstract::where() const
	{
		if (m_where == DEFAULT_STRING && line != static_cast<size_t>(-1))
			m_where = throw_place_string(file, line, func);
		return m_where;
	}

	void Abstract::format_error(cstr::mstring& out) const
	{
		wchar_t buf[MAX_PATH_LEN] = {0};

		safe_snprintf(buf, lengthof(buf), L"Exception: %s", type());
		out.push_back(buf);
		safe_snprintf(buf, lengthof(buf), L"Error: %s", what());
		out.push_back(buf);
		safe_snprintf(buf, lengthof(buf), L"Where: %s", where());
		out.push_back(buf);
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

	void Abstract::change_what(const wchar_t* newWhat) const
	{
		const wchar_t* buff = (newWhat) ? cstr::dup(newWhat) : DEFAULT_STRING;

		using simstd::swap;
		swap(m_what, buff);

		cleanup(buff);
	}

	void Abstract::change_where(const wchar_t* newWhere) const
	{
		const wchar_t* buff = (newWhere) ? cstr::dup(newWhere) : DEFAULT_STRING;

		using simstd::swap;
		swap(m_where, buff);

		cleanup(buff);
	}

}
