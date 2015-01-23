#include "logger_pvt.hpp"

#include <basis/sys/cstr.hpp>

namespace logger {

	const size_t default_buffer_size = 4 * 1024;

	///================================================================================== ModuleImpl
	ModuleImpl::~ModuleImpl()
	{
		TraceFuncFormat("%s:%d %S\n", __PRETTY_FUNCTION__, __LINE__, m_name);
	}

	ModuleImpl::ModuleImpl(const wchar_t* name, const Target_t & tgt, Level lvl) :
		m_target(tgt),
		m_lvl(lvl),
		m_prefix(defaults::get_prefix()),
		m_color(1),
		m_enabled(1),
		m_utf8_out(1)
	{
		TraceFunc();
		cstr::copy(m_name, name, lengthof(m_name));
//		out(Level::Logger, L"Logger module has been created\n");
		TraceFunc();
	}

	void ModuleImpl::destroy() const
	{
		delete this;
	}

	const wchar_t* ModuleImpl::get_name() const
	{
		return m_name;
	}

	Level ModuleImpl::get_level() const
	{
		return m_lvl;
	}

	size_t ModuleImpl::get_prefix() const
	{
		return m_prefix;
	}

	bool ModuleImpl::is_color_mode() const
	{
		return m_color;
	}

	bool ModuleImpl::is_utf8_mode() const
	{
		return m_utf8_out;
	}

	void ModuleImpl::set_level(Level lvl)
	{
		auto scopeLock(m_target->lock_scope());
		m_lvl = lvl;
	}

	void ModuleImpl::set_prefix(size_t prefix)
	{
		auto scopeLock(m_target->lock_scope());
		m_prefix = prefix;
	}

	void ModuleImpl::set_color_mode(bool mode)
	{
		auto scopeLock(m_target->lock_scope());
		m_color = mode;
	}

	void ModuleImpl::set_target(const Target_t & target)
	{
		auto scopeLock(m_target->lock_scope());
		m_target = target;
	}

	void ModuleImpl::set_enabled(bool enabled)
	{
		auto scopeLock(m_target->lock_scope());
		m_enabled = enabled;
	}

	void ModuleImpl::out(Level lvl, const wchar_t* format, ...) const
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

	void ModuleImpl::out_with_color(WORD color, Level lvl, const wchar_t* format, ...) const
	{
		TraceFunc();
		if (m_enabled && lvl >= m_lvl) {
			Va_list args;
			va_start(args, format);
			out_args(color, lvl, format, args);
		}
		TraceFunc();
	}

	void ModuleImpl::out_with_place(const char* file, int line, const char* func, Level lvl, const wchar_t* format, ...) const
	{
		TraceFunc();
		if (m_enabled && m_lvl <= lvl) {
			wchar_t buff[default_buffer_size];
			auto pend = add_prefix(lvl, buff, lengthof(buff));
			pend = add_place(pend, lengthof(buff) - (pend - buff), file, line, func);
			Va_list args;
			va_start(args, format);
			out_args(lvl, buff, pend, lengthof(buff) - (pend - buff), format, args);
			TraceFunc();
		}
	}

	lock_type ModuleImpl::lock_scope() const
	{
		return m_target->lock_scope();
	}

	wchar_t* ModuleImpl::add_prefix(Level lvl, wchar_t* buff, size_t size) const
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

	wchar_t* ModuleImpl::add_place(wchar_t* buff, size_t size, const char* file, int line, const char* func) const
	{
		TraceFunc();
		size_t written = 0;
		if (m_prefix & Prefix::Place) {
			written += safe_snprintf(buff + written, size - written, L"%14.14S:%5d ", filename_only(file), line);
		}
		if (m_prefix & Prefix::Function) {
			written += safe_snprintf(buff + written, size - written, L"%S() ", funcname_only(func));
		}
		TraceFunc();
		return buff + written;
	}

	void ModuleImpl::out_args(Level lvl, wchar_t* buff, wchar_t* pend, size_t size, const wchar_t* frmat, va_list & args) const
	{
		TraceFunc();
		size_t written = safe_vsnprintf(pend, size, frmat, args);
		TraceFuncFormat("'%S'\n", pend);
		auto scopeLock(m_target->lock_scope());
		m_target->out(this, lvl, buff, pend - buff + written);
		TraceFunc();
	}

	void ModuleImpl::out_args(WORD color, Level lvl, const wchar_t* frmat, va_list args) const
	{
		TraceFunc();
		wchar_t buff[4096];
		size_t written = safe_vsnprintf(buff, lengthof(buff), frmat, args);
		auto scopeLock(m_target->lock_scope());
		m_target->out(this, color, lvl, buff, written);
		TraceFunc();
	}

	///=============================================================================================
	ModuleImpl * create_Module_impl(const wchar_t* name, const Target_t & tgt, Level lvl)
	{
		return new ModuleImpl(name, tgt, lvl);
	}

}
