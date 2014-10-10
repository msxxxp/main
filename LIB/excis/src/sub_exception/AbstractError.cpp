#include <excis/exception.hpp>

//#include <basis/sys/console.hpp>
//#include <basis/sys/totext.hpp>

namespace exception {

	PCWSTR const THROW_PLACE_FORMAT = L"%S: %d [%S]";

	ustring ThrowPlaceString(PCSTR file, int line, PCSTR func)
	{
		wchar_t buf[MAX_PATH];
		safe_snprintf(buf, lengthof(buf), THROW_PLACE_FORMAT, file, line, func);
		return ustring(buf);
	}

	AbstractError::~AbstractError()
	{
	}

	AbstractError::AbstractError():
		m_prev_exc(nullptr)
	{
	}

	AbstractError::AbstractError(const AbstractError & prev):
		m_prev_exc(prev.clone())
	{
	}

	AbstractError::AbstractError(PCSTR file, size_t line, PCSTR func) :
		m_where(THROW_PLACE_STR),
		m_prev_exc(nullptr)
	{
		UNUSED(file);
		UNUSED(line);
		UNUSED(func);
	}

	AbstractError::AbstractError(const AbstractError & prev, PCSTR file, size_t line, PCSTR func) :
		m_where(THROW_PLACE_STR),
		m_prev_exc(prev.clone())
	{
		UNUSED(file);
		UNUSED(line);
		UNUSED(func);
	}

	ustring AbstractError::where() const
	{
		return m_where;
	}

	AbstractError * AbstractError::get_prev() const
	{
		return m_prev_exc.get();
	}

	cstr::mstring AbstractError::format_error() const
	{
		cstr::mstring msg;
		format_error(msg);
		return simstd::move(msg);
	}

}
