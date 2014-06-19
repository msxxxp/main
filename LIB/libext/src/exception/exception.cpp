#include <libext/exception.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/totext.hpp>

namespace Ext {

	PCWSTR const THROW_PLACE_FORMAT = L"%S: %d [%S]";

#ifdef NDEBUG
#else
	ustring ThrowPlaceString(PCSTR file, int line, PCSTR func)
	{
		wchar_t buf[MAX_PATH];
		::_snwprintf(buf, lengthof(buf), THROW_PLACE_FORMAT, file, line, func);
		return ustring(buf);
	}
#endif

	///=============================================================================== AbstractError
	AbstractError::~AbstractError()
	{
	}

#ifndef NDEBUG
	AbstractError::AbstractError(PCSTR file, size_t line, PCSTR func) :
		m_where(THROW_PLACE_STR),
		m_prev_exc(nullptr)
	{
	}

	AbstractError::AbstractError(const AbstractError & prev, PCSTR file, size_t line, PCSTR func) :
		m_where(THROW_PLACE_STR),
		m_prev_exc(prev.clone())
	{
	}
#else
	AbstractError::AbstractError():
		m_prev_exc(nullptr)
	{
	}

	AbstractError::AbstractError(const AbstractError & prev):
		m_prev_exc(prev.clone())
	{
	}
#endif

	PCWSTR AbstractError::where() const
	{
#ifndef NDEBUG
		return m_where.c_str();
#else
		return L"Programm compiled with NDEBUG define";
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
