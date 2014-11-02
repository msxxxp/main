#include "../logger_pvt.hpp"

#include <basis/sys/cstr.hpp>

//#define TraceFunc() console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
#define TraceFunc()

namespace logger {

	const size_t default_buffer_size = 4 * 1024;

	///================================================================================= Module_impl
	Module_impl::Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl) :
		m_target(tgt),
		m_lvl(lvl),
		m_prefix(defaults::get_prefix()),
		m_color(1),
		m_enabled(1),
		m_utf8_out(0)
	{
		TraceFunc();
		cstr::copy(m_name, name, lengthof(m_name));
//		out(Level::Logger, L"Logger module has been created\n");
		TraceFunc();
	}

	void Module_impl::destroy() const
	{
		delete this;
	}

	const wchar_t * Module_impl::get_name() const
	{
		return m_name;
	}

	Level Module_impl::get_level() const
	{
		return m_lvl;
	}

	size_t Module_impl::get_prefix() const
	{
		return m_prefix;
	}

	bool Module_impl::is_color_mode() const
	{
		return m_color;
	}

	bool Module_impl::is_utf8_mode() const
	{
		return m_utf8_out;
	}

	void Module_impl::set_level(Level lvl)
	{
		auto scopeLock(m_target->lock_scope());
		m_lvl = lvl;
	}

	void Module_impl::set_prefix(size_t prefix)
	{
		auto scopeLock(m_target->lock_scope());
		m_prefix = prefix;
	}

	void Module_impl::set_color_mode(bool mode)
	{
		auto scopeLock(m_target->lock_scope());
		m_color = mode;
	}

	void Module_impl::set_target(const Target_t & target)
	{
		auto scopeLock(m_target->lock_scope());
		m_target = target;
	}

	void Module_impl::set_enabled(bool enabled)
	{
		auto scopeLock(m_target->lock_scope());
		m_enabled = enabled;
	}

	void Module_impl::out_debug(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const
	{
		TraceFunc();
		if (m_enabled && m_lvl <= lvl) {
			wchar_t buff[default_buffer_size];
			auto pend = add_prefix(lvl, buff, lengthof(buff));
			pend = add_place(pend, lengthof(buff) - (pend - buff), file, line, func);
			Va_list args;
			va_start(args, format);
			out_args(lvl, buff, pend, lengthof(buff) - (pend - buff), format, args);
		}
		TraceFunc();
	}

	void Module_impl::out(Level lvl, const wchar_t * format, ...) const
	{
		TraceFunc();
		if (m_enabled && lvl >= m_lvl) {
			wchar_t buff[default_buffer_size];
			auto pend = add_prefix(lvl, buff, lengthof(buff));
			Va_list args;
			va_start(args, format);
			out_args(lvl, buff, pend, lengthof(buff) - (pend - buff), format, args);
		}
		TraceFunc();
	}

	void Module_impl::out_console(WORD color, Level lvl, const wchar_t * format, ...) const
	{
		TraceFunc();
		if (m_enabled && lvl >= m_lvl) {
			Va_list args;
			va_start(args, format);
			out_args(color, lvl, format, args);
		}
		TraceFunc();
	}

	lock_type Module_impl::lock_scope() const
	{
		return m_target->lock_scope();
	}

	wchar_t * Module_impl::add_prefix(Level lvl, wchar_t * buff, size_t size) const
	{
		TraceFunc();
		size_t written = 0;
		if (m_prefix & (Prefix::Date | Prefix::Time)) {
			SYSTEMTIME time;
			::GetLocalTime(&time);
			if (m_prefix & Prefix::Date) {
				written += safe_snprintf(buff + written, size - written, L"%04u-%02u-%02u ", time.wYear, time.wMonth, time.wDay);
			}
			if (m_prefix & Prefix::Time) {
				written += safe_snprintf(buff + written, size - written, L"%02u:%02u:%02u.%03u ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			}
		}
		if (m_prefix & Prefix::Level) {
			written += safe_snprintf(buff + written, size - written, L"%s ", to_str(lvl));
		}
		if (m_prefix & Prefix::Module) {
			written += safe_snprintf(buff + written, size - written, L"%11s ", m_name);
		}
		if (m_prefix & Prefix::Thread) {
			written += safe_snprintf(buff + written, size - written, L"%5u ", ::GetCurrentThreadId());
		}
		TraceFunc();
		return buff + written;
	}

	wchar_t * Module_impl::add_place(wchar_t * buff, size_t size, const char * file, int line, const char * func) const
	{
		TraceFunc();
		size_t written = 0;
		if (m_prefix & Prefix::Place) {
			written += safe_snprintf(buff + written, size - written, L"%14.14S:%5d ", file, line);
		}
		if (m_prefix & Prefix::Function) {
			written += safe_snprintf(buff + written, size - written, L"%S() ", func);
		}
		TraceFunc();
		return buff + written;
	}

	void Module_impl::out_args(Level lvl, wchar_t * buff, wchar_t * pend, size_t size, const wchar_t * frmat, va_list & args) const
	{
		TraceFunc();
		size_t written = safe_vsnprintf(pend, size, frmat, args);
		auto scopeLock(m_target->lock_scope());
		m_target->out(this, lvl, buff, pend - buff + written);
		TraceFunc();
	}

	void Module_impl::out_args(WORD color, Level lvl, const wchar_t * frmat, va_list args) const
	{
		TraceFunc();
		wchar_t buff[4096];
		size_t written = safe_vsnprintf(buff, lengthof(buff), frmat, args);
		auto scopeLock(m_target->lock_scope());
		m_target->out(this, color, lvl, buff, written);
		TraceFunc();
	}

	///=============================================================================================
	Module_impl * create_Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl)
	{
		return new Module_impl(name, tgt, lvl);
	}

}
