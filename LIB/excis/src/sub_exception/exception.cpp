#include <excis/exception.hpp>

//#include <basis/sys/console.hpp>
//#include <basis/sys/totext.hpp>

namespace exception {

#ifdef NDEBUG
#else
	PCWSTR const THROW_PLACE_FORMAT = L"%S: %d [%S]";

	ustring ThrowPlaceString(PCSTR file, int line, PCSTR func)
	{
		wchar_t buf[MAX_PATH];
		safe_snprintf(buf, lengthof(buf), THROW_PLACE_FORMAT, file, line, func);
		return ustring(buf);
	}
#endif

	AbstractError::~AbstractError()
	{
	}

#ifdef NDEBUG
	AbstractError::AbstractError():
		m_prev_exc(nullptr)
	{
	}

	AbstractError::AbstractError(const AbstractError & prev):
		m_prev_exc(prev.clone())
	{
	}
#else
	AbstractError::AbstractError(PCSTR file, size_t line, PCSTR func) :
		m_prev_exc(nullptr),
		m_where(THROW_PLACE_STR)
	{
	}

	AbstractError::AbstractError(const AbstractError & prev, PCSTR file, size_t line, PCSTR func) :
		m_prev_exc(prev.clone()),
		m_where(THROW_PLACE_STR)
	{
	}
#endif

	PCWSTR AbstractError::where() const
	{
#ifdef NDEBUG
//		return L"Programm compiled with NDEBUG define";
		return EMPTY_STR;
#else
		return m_where.c_str();
#endif
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
