#include <liblog/logger.hpp>
#include <libbase/cstr.hpp>
#include <libbase/pvt/va_list.hpp>
#include <simstl/string>
#include "Module_impl.hpp"

namespace Logger {
	typedef sarastd::wstring ustring;

	namespace {
		const size_t default_buffer_size = 4 * 1024;

		ustring format(PCWSTR fmt, va_list args)
		{
			wchar_t buf[default_buffer_size];
			Base::safe_vsnprintf(buf, Base::lengthof(buf), fmt, args);
			return ustring(buf);
		}

		ustring format(PCWSTR fmt, ...)
		{
			Base::Va_list args;
			va_start(args, fmt);
			return format(fmt, args);
		}
	}

	const wchar_t * const LogLevelNames[(int)Level::Force + 1] = {
		L"TRACE",
		L"DEBUG",
		L"INFO ",
		L"REPRT",
		L"ATTEN",
		L"WARN ",
		L"ERROR",
		L"FATAL",
		L"ALERT",
		L"EMERG",
		L"     ",
	};

	const size_t default_buffer_size = 4 * 1024;

	///================================================================================= Module_impl
	struct Module_impl: public Module_i, private Base::Uncopyable {
		~Module_impl();

		Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl);

		void destroy() const override;

		const wchar_t * get_name() const override;

		Level get_level() const override;

		size_t get_prefix() const override;

		bool is_color_mode() const override;

		void set_level(Level lvl) override;

		void set_prefix(size_t prefix) override;

		void set_color_mode(bool mode) override;

		void set_target(const Target_t & target) override;

		void set_enabled(bool enabled) override;

		void out(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const override;

		void out(Level lvl, const wchar_t * format, ...) const override;

		Lock::ScopeGuard lock_scope() const override;

	private:
		ustring create_prefix(Level lvl) const;

		ustring & add_place(ustring & prefix, const char * file, int line, const char * func) const;

		void out_args(Level lvl, const ustring & prefix, const wchar_t * frmat, va_list args) const;

		ustring m_name;
		std::shared_ptr<Target_i> m_target;
		Level m_lvl;
		size_t m_prefix;
		uint32_t m_color:1;
		uint32_t m_enabled:1;
	};

	Module_impl::~Module_impl()
	{
	}

	Module_impl::Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl) :
		m_name(name),
		m_target(tgt),
		m_lvl(lvl),
		m_prefix(Default::get_prefix()),
		m_color(1),
		m_enabled(1)
	{
//		out(Level::Logger, L"Logger module has been created\n");
	}

	void Module_impl::destroy() const
	{
		delete this;
	}

	const wchar_t * Module_impl::get_name() const
	{
		return m_name.c_str();
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

	void Module_impl::out(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const
	{
		if (m_enabled && m_lvl <= lvl) {
			ustring prefix = create_prefix(lvl);
			add_place(prefix, file, line, func);
			va_list args;
			va_start(args, format);
			out_args(lvl, prefix, format, args);
			va_end(args);
		}
	}

	void Module_impl::out(Level lvl, const wchar_t * format, ...) const
	{
		if (m_enabled && lvl >= m_lvl) {
			va_list args;
			va_start(args, format);
			out_args(lvl, create_prefix(lvl), format, args);
			va_end(args);
		}
	}

	Lock::ScopeGuard Module_impl::lock_scope() const
	{
		return m_target->lock_scope();
	}

	ustring Module_impl::create_prefix(Level lvl) const
	{
		ustring prefix;
		if (m_prefix & (Prefix::Date | Prefix::Time)) {
			SYSTEMTIME time;
			::GetLocalTime(&time);
			if (m_prefix & Prefix::Date) {
				prefix += format(L"%04u-%02u-%02u ", time.wYear, time.wMonth, time.wDay);
			}
			if (m_prefix & Prefix::Time) {
				prefix += format(L"%02u:%02u:%02u.%03u ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			}
		}
		if (m_prefix & Prefix::Level) {
			prefix += format(L"%s ", LogLevelNames[(int)lvl]);
		}
		if (m_prefix & Prefix::Module) {
			prefix += format(L"{%8s} ", m_name.c_str());
		}
		if (m_prefix & Prefix::Thread) {
			prefix += format(L"<%5u> ", ::GetCurrentThreadId());
		}
		return prefix;
	}

	ustring & Module_impl::add_place(ustring & prefix, const char * file, int line, const char * func) const
	{
		if (m_prefix & Prefix::Place) {
			prefix += format(L"%14.14S:%4d ", file, line);
		}
		if (m_prefix & Prefix::Function) {
			prefix += format(L"[%S] ", func);
		}
		return prefix;
	}

	void Module_impl::out_args(Level lvl, const ustring & prefix, const wchar_t * frmat, va_list args) const
	{
		ustring tmp(prefix);
		tmp += format(frmat, args);
		auto scopeLock(lock_scope());
		m_target->out(this, lvl, tmp.c_str(), tmp.size());
	}

	///=============================================================================================
	Module_i * create_Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl)
	{
		return new Module_impl(name, tgt, lvl);
	}

}
