#include <libbase/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/shared_ptr.hpp>
#include <libbase/str.hpp>

#include <vector>

#include <stdio.h>

namespace Base {
	namespace Logger {

		PCWSTR const LogLevelNames[(int)Level::Fatal + 1] = {
			L"TRACE",
			L"DEBUG",
			L"INFO ",
			L"REPRT",
			L"ATTEN",
			L"WARN ",
			L"ERROR",
			L"FATAL",
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
		struct Module_impl: public Module_i, private Uncopyable {
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

			void out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const override;

			void out(Level lvl, PCWSTR format, ...) const override;

		private:
			ustring create_prefix(Level lvl) const;

			ustring & add_place(ustring & prefix, PCSTR file, int line, PCSTR func) const;

			void out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const;

			ustring m_name;
			shared_ptr<Target_i> m_target;
			Level m_lvl;
			size_t m_prefix;
			uint32_t m_color:1;

			friend class Logger_impl;
		};

		Module_impl::Module_impl(PCWSTR name, const Target_t & tgt, Level lvl) :
			m_name(name),
			m_target(tgt),
			m_lvl(lvl),
			m_prefix(get_default_prefix()),
			m_color(1)
		{
		}

		Module_impl::~Module_impl()
		{
		}

		PCWSTR Module_impl::get_name() const
		{
			return m_name.data();
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
			m_lvl = lvl;
		}

		void Module_impl::set_prefix(size_t prefix)
		{
			m_prefix = prefix;
		}

		void Module_impl::set_color_mode(bool mode)
		{
			m_color = mode;
		}

		void Module_impl::set_target(const Target_t & target)
		{
			m_target = target;
		}

		void Module_impl::out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const
		{
			if (lvl >= m_lvl) {
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
			if (lvl >= m_lvl) {
				va_list args;
				va_start(args, format);
				out_args(lvl, create_prefix(lvl), format, args);
				va_end(args);
			}
		}

		ustring Module_impl::create_prefix(Level lvl) const
		{
			ustring prefix;
			if (m_prefix & Prefix::Level) {
				prefix += format_str(L"%s ", LogLevelNames[(int)lvl]);
			}
//			if (m_prefix & Prefix::Time) {
//				prefix += format_str(L"%s ", LogLevelNames[(int)lvl]);
//			}
			if (m_prefix & Prefix::Module) {
				prefix += format_str(L"{%s} ", m_name.data());
			}
			if (m_prefix & Prefix::Thread) {
				prefix += format_str(L"%5u ", ::GetCurrentThreadId());
			}
			return prefix;
		}

		ustring & Module_impl::add_place(ustring & prefix, PCSTR file, int line, PCSTR func) const
		{
			if (m_prefix & Prefix::Place) {
				prefix += format_str(L"%14.14S:%-4d ", file, line);
			}
			if (m_prefix & Prefix::Function) {
				prefix += format_str(L"[%S] ", func);
			}
			return prefix;
		}

		void Module_impl::out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const
		{
			ustring tmp(prefix);
			tmp += format_str(format, args);
			m_target->out(this, lvl, tmp.c_str(), tmp.size());
		}

//		struct pModule_pModule_less: public std::binary_function<const Module_i *, const Module_i *, bool> {
//		};
//
		struct pModule_PCWSTR_less: public std::binary_function<const Module_i *, PCWSTR, bool> {
			bool operator ()(const Module_i * lhs, const Module_i * rhs) const
			{
				return Str::compare(lhs->get_name(), rhs->get_name()) < 0;
			}

			bool operator ()(const Module_i * lhs, PCWSTR rhs) const
			{
				return Str::compare(lhs->get_name(), rhs) < 0;
			}

			bool operator ()(PCWSTR left, const Module_i * right) const
			{
				return Str::compare(left, right->get_name()) < 0;
			}
		};

//		struct pModule_pModule_equal: public std::binary_function<const Module_i *, const Module_i *, bool> {
//			bool operator ()(const Module_i * lhs, const Module_i * rhs)
//			{
//				return Str::compare(lhs->get_name(), rhs->get_name()) == 0;
//			}
//		};
//
//		struct pModule_PCWSTR_equal: public std::binary_function<const Module_i *, PCWSTR, bool> {
//			bool operator ()(const Module_i * lhs, PCWSTR rhs)
//			{
//				return Str::compare(lhs->get_name(), rhs) == 0;
//			}
//		};

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
			Logger_impl();

			std::vector<Module_i*> m_modules;
			Base::auto_destroy<Lock::SyncUnit_i*> m_sync;

			static Level defLevel;
			static size_t defPrefix;
			static Target_t defTarget;
			static Module_i * defModule;
			static PCWSTR const defModuleName;

			friend Logger_i & get_instance();
		};

		Level Logger_impl::defLevel = Level::Warn;
		size_t Logger_impl::defPrefix = Prefix::Medium;
		Target_t Logger_impl::defTarget = get_TargetToNull();
		Module_i * Logger_impl::defModule = nullptr;
		PCWSTR const Logger_impl::defModuleName = L"default";

		Logger_impl::Logger_impl() :
			m_sync(Lock::get_ReadWrite())
		{
			defModule = register_module_(defModuleName, defTarget, defLevel);
		}

		Logger_impl::~Logger_impl()
		{
			auto lk(m_sync->lock_scope());
			while (!m_modules.empty()) {
				free_module_(m_modules.back());
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
			m_modules.emplace(std::upper_bound(m_modules.begin(), m_modules.end(), module, pModule_PCWSTR_less()), module);
			return module;
		}

		void Logger_impl::free_module_(Module_i * module)
		{
			auto lk(m_sync->lock_scope());
			auto range = std::equal_range(m_modules.begin(), m_modules.end(), module, pModule_PCWSTR_less());
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

//		Module_i * get_module(PCSTR name, const Target_t & target, Level lvl)
//		{
//			wchar_t buf[MAX_PATH_LEN];
//			Str::convert(buf, lengthof(buf), name, CP_ACP);
//			return get_module(buf, target, lvl);
//		}

		Module_i * get_module(PCWSTR name, const Target_t & target, Level lvl)
		{
			return get_instance().register_module(name, target, lvl);
		}

		Logger_i & get_instance()
		{
			static Logger_impl ret;
			return ret;
		}

		void set_module_level(Level lvl, Module_i * module)
		{
			module->set_level(lvl);
		}

		void set_module_prefix(size_t prefix, Module_i * module)
		{
			module->set_prefix(prefix);
		}

		void set_module_target(const Target_t & target, Module_i * module)
		{
			module->set_target(target);
		}

		void set_module_color_mode(bool mode, Module_i * module)
		{
			module->set_color_mode(mode);
		}

		///================================================================================ Target_i
		Target_i::~Target_i()
		{
		}

	}
}
