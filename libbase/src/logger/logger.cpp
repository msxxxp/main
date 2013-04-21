#include <libbase/logger.hpp>
#include "logger_pvt.hpp"

#include <libbase/lock.hpp>
#include <libbase/string.hpp>

#include <vector>

#include <stdio.h>

namespace Logger {

	PCWSTR const LogLevelNames[(int)Level::Logger + 1] = {
		L"TRACE",
		L"DEBUG",
		L"INFO ",
		L"REPRT",
		L"ATTEN",
		L"WARN ",
		L"ERROR",
		L"FATAL",
		L"LOGGR",
	};

	struct FmtString {
		PCWSTR const place;
		PCWSTR const additional;
	};

	///================================================================================ Module_i
	Module_i::~Module_i()
	{
	}

	///============================================================================= Module_impl
	struct Module_impl: public Module_i, private Base::Uncopyable {
		Module_impl(PCWSTR name, const Target_t & tgt, Level lvl);

		~Module_impl();

		PCWSTR get_name() const override;

		Level get_level() const override;

		size_t get_prefix() const override;

		bool is_color_mode() const override;

		void set_level(Level lvl) override;

		void set_prefix(size_t prefix) override;

		void set_color_mode(bool mode) override;

		void set_target(const Target_t & target) override;

		void set_enabled(bool enabled) override;

		void out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const override;

		void out(Level lvl, PCWSTR format, ...) const override;

		void lock() const override;

		void unlock() const override;

	private:
		ustring create_prefix(Level lvl) const;

		ustring & add_place(ustring & prefix, PCSTR file, int line, PCSTR func) const;

		void out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const;

		ustring m_name;
		Base::shared_ptr<Target_i> m_target;
		Level m_lvl;
		size_t m_prefix;
		uint32_t m_color:1;
		uint32_t m_enabled:1;
	};

	Module_impl::Module_impl(PCWSTR name, const Target_t & tgt, Level lvl) :
		m_name(name),
		m_target(tgt),
		m_lvl(lvl),
		m_prefix(get_default_prefix()),
		m_color(1),
		m_enabled(1)
	{
//		out(Level::Logger, L"Logger module has been created\n");
	}

	Module_impl::~Module_impl()
	{
	}

	PCWSTR Module_impl::get_name() const
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
		lock();
		m_lvl = lvl;
		unlock();
	}

	void Module_impl::set_prefix(size_t prefix)
	{
		lock();
		m_prefix = prefix;
		unlock();
	}

	void Module_impl::set_color_mode(bool mode)
	{
		lock();
		m_color = mode;
		unlock();
	}

	void Module_impl::set_target(const Target_t & target)
	{
		lock();
		m_target = target;
		unlock();
	}

	void Module_impl::set_enabled(bool enabled)
	{
		lock();
		m_enabled = enabled;
		unlock();
	}

	void Module_impl::out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const
	{
		if (m_enabled && lvl >= m_lvl) {
			ustring prefix = create_prefix(lvl);
			add_place(prefix, file, line, func);
			va_list args;
			va_start(args, format);
			out_args(lvl, prefix, format, args);
			va_end(args);
		}
	}

	void Module_impl::out(Level lvl, PCWSTR format, ...) const
	{
		if (m_enabled && lvl >= m_lvl) {
			va_list args;
			va_start(args, format);
			out_args(lvl, create_prefix(lvl), format, args);
			va_end(args);
		}
	}

	void Module_impl::lock() const
	{
		m_target->lock();
	}

	void Module_impl::unlock() const
	{
		m_target->unlock();
	}

	ustring Module_impl::create_prefix(Level lvl) const
	{
		ustring prefix;
		if (m_prefix & (Prefix::Date | Prefix::Time)) {
			SYSTEMTIME time;
			::GetLocalTime(&time);
			if (m_prefix & Prefix::Date) {
				prefix += Base::String::format(L"%04u-%02u-%02u ", time.wYear, time.wMonth, time.wDay);
			}
			if (m_prefix & Prefix::Time) {
				prefix += Base::String::format(L"%02u:%02u:%02u.%03u ", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
			}
		}
		if (m_prefix & Prefix::Level) {
			prefix += Base::String::format(L"%s ", LogLevelNames[(int)lvl]);
		}
		if (m_prefix & Prefix::Module) {
			prefix += Base::String::format(L"{%s} ", m_name.c_str());
		}
		if (m_prefix & Prefix::Thread) {
			prefix += Base::String::format(L"<%05u> ", ::GetCurrentThreadId());
		}
		return prefix;
	}

	ustring & Module_impl::add_place(ustring & prefix, PCSTR file, int line, PCSTR func) const
	{
		if (m_prefix & Prefix::Place) {
			prefix += Base::String::format(L"%14.14S:%4d ", file, line);
		}
		if (m_prefix & Prefix::Function) {
			prefix += Base::String::format(L"[%S] ", func);
		}
		return prefix;
	}

	void Module_impl::out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const
	{
		ustring tmp(prefix);
		tmp += Base::String::format(format, args);
		lock();
		m_target->out(this, lvl, tmp.c_str(), tmp.size());
		unlock();
	}

	struct pModule_less: public std::binary_function<const Module_i *, const Module_i *, bool> {
		bool operator ()(const Module_i * lhs, const Module_i * rhs) const
		{
			return Base::Str::compare(lhs->get_name(), rhs->get_name()) < 0;
		}
	};

	struct pModule_PCWSTR_less {
		bool operator ()(const Module_i * lhs, PCWSTR rhs) const
		{
			return Base::Str::compare(lhs->get_name(), rhs) < 0;
		}

		bool operator ()(PCWSTR left, const Module_i * right) const
		{
			return Base::Str::compare(left, right->get_name()) < 0;
		}
	};

	///================================================================================ Logger_i
	Module_i * Logger_i::get_module(PCWSTR name)
	{
		return get_module_(name);
	}

	Module_i * Logger_i::register_module(PCWSTR name, const Target_t & target, Level lvl)
	{
		return register_module_(name, target, lvl);
	}

	void Logger_i::free_module(Module_i * module)
	{
		free_module_(module);
	}

	Logger_i::~Logger_i()
	{
	}

	///============================================================================= Logger_impl
	struct Logger_impl: public Logger_i, private Base::Uncopyable {
		~Logger_impl();

		Module_i * get_module_(PCWSTR name) override;

		Module_i * register_module_(PCWSTR name, const Target_t & target, Level lvl) override;

		void free_module_(Module_i * module) override;

		static Level get_default_level()
		{
			return defLevel;
		}

		static size_t get_default_prefix()
		{
			return defPrefix;
		}

		static Target_t get_default_target()
		{
			return defTarget;
		}

		static Module_i * get_default_module()
		{
			return defModule;
		}

		static PCWSTR get_default_module_name()
		{
			return defModuleName;
		}

		static void set_default_level(Level level)
		{
			defLevel = level;
		}

		static void set_default_prefix(size_t prefix)
		{
			defPrefix = prefix;
		}

		static void set_default_target(Target_t target)
		{
			defTarget = target;
		}

	private:
		typedef std::vector<Module_i*> ModulesArray;

		Logger_impl();

		ModulesArray m_modules;
		Base::auto_destroy<Base::Lock::SyncUnit_i*> m_sync;

		static Level defLevel;
		static size_t defPrefix;
		static Target_t defTarget;
		static Module_i * defModule;
		static PCWSTR const defModuleName;

		friend Logger_i & get_instance();
	};

	Level Logger_impl::defLevel = Level::Atten;
	size_t Logger_impl::defPrefix = Prefix::Medium;
	Target_t Logger_impl::defTarget = get_TargetToNull();
	Module_i * Logger_impl::defModule = nullptr;
	PCWSTR const Logger_impl::defModuleName = L"default";

	Logger_impl::Logger_impl() :
		m_sync(Base::Lock::get_ReadWrite())
	{
		auto lk(m_sync->lock_scope());
		defModule = register_module_(defModuleName, defTarget, defLevel);
		auto prefix = defModule->get_prefix();
		defModule->set_prefix(Prefix::Thread | Prefix::Time | Prefix::Date);
		defModule->out(Level::Logger, L"Logger has been created\n");
		defModule->set_prefix(prefix);
	}

	Logger_impl::~Logger_impl()
	{
		auto lk(m_sync->lock_scope());
		defModule->set_prefix(Prefix::Thread | Prefix::Time | Prefix::Date);
		defModule->out(Level::Logger, L"Logger is being destroyed\n");
		while (!m_modules.empty()) {
			delete m_modules.back();
			m_modules.pop_back();
		}
	}

	Module_i * Logger_impl::get_module_(PCWSTR name)
	{
		auto lk(m_sync->lock_scope());
		auto range = std::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second)
			return *range.first;
		return register_module(name);
	}

	Module_i * Logger_impl::register_module_(PCWSTR name, const Target_t & target, Level lvl)
	{
		auto lk(m_sync->lock_scope());
		auto range = std::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second) {
			return *range.first;
		}
		Module_i * module = new Module_impl(name, target, lvl);
		m_modules.emplace(range.second, module);
		return module;
	}

	void Logger_impl::free_module_(Module_i * module)
	{
		auto lk(m_sync->lock_scope());
		auto range = std::equal_range(m_modules.begin(), m_modules.end(), module, pModule_less());
		for_each(range.first, range.second, [](Module_i * found_module) {
			delete found_module;
		});
		m_modules.erase(range.first, range.second);
	}

	Level get_default_level()
	{
		return Logger_impl::get_default_level();
	}

	void set_default_level(Level lvl)
	{
		Logger_impl::set_default_level(lvl);
	}

	size_t get_default_prefix()
	{
		return Logger_impl::get_default_prefix();
	}

	void set_default_prefix(size_t prefix)
	{
		Logger_impl::set_default_prefix(prefix);
	}

	Target_t get_default_target()
	{
		return Logger_impl::get_default_target();
	}

	void set_default_target(Target_t target)
	{
		Logger_impl::set_default_target(target);
	}

	Module_i * get_default_module()
	{
		get_instance();
		return Logger_impl::get_default_module();
	}

//	Module_i * get_module(PCSTR name, const Target_t & target, Level lvl)
//	{
//		wchar_t buf[MAX_PATH_LEN];
//		Str::convert(buf, lengthof(buf), name, CP_ACP);
//		return get_module(buf, target, lvl);
//	}

	Module_i * get_module(PCWSTR name, const Target_t & target, Level lvl)
	{
		return get_instance().register_module(name, target, lvl);
	}

	Logger_i & get_instance()
	{
		static Logger_impl ret;
		return ret;
	}


	///================================================================================ Target_i
	Target_i::~Target_i()
	{
	}

}
