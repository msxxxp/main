﻿#include <libbase/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/shared_ptr.hpp>
#include <libbase/str.hpp>

#include <vector>

#include <stdio.h>

namespace Base {
	namespace Logger {

		PCWSTR const LogLevelNames[(int)Level::Fatal + 1] = {
			L"TRACE ",
			L"DEBUG ",
			L"INFO  ",
			L"REPRT ",
			L"ATTEN ",
			L"WARN  ",
			L"ERROR ",
			L"FATAL ",
		};

		struct FmtString {
			PCWSTR const place;
			PCWSTR const additional;
		};

		const FmtString fmtStrings[(int)Wideness::Short + 1] = {
			{L"%S: %-4d [%S] ", L"%s{%s}:%-5u "},
			{L"%S: %-4d [%S] ", L"%s{%s} "},
			{L"%S: %-4d [%S] ", L"%s"},
			{L"%S: %-4d [%S] ", L""},
		};

		///================================================================================ Module_i
		Module_i::~Module_i()
		{
		}

		///============================================================================= Module_impl
		struct Module_impl: public Module_i, private Uncopyable {
			Module_impl(PCWSTR name, const Target_t & tgt, Level lvl, ssize_t index);

			~Module_impl();

			PCWSTR get_name() const override;

			Level get_level() const override;

			Wideness get_wideness() const override;

			bool is_color_mode() const override;

			void set_level(Level lvl) override;

			void set_wideness(Wideness mode) override;

			void set_color_mode(bool mode) override;

			void set_target(const Target_t & target) override;

			void out(PCSTR file, int line, PCSTR func, Level lvl, PCWSTR format, ...) const override;

			void out(Level lvl, PCWSTR format, ...) const override;

			ssize_t get_index() const override;

		private:
			void out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const;

			auto_array<wchar_t> m_name;
			shared_ptr<Target_i> m_target;
			ssize_t m_index;
			Level m_lvl;
			Wideness m_wide;
			uint32_t m_color :1;

			friend class Logger_impl;
		};

		Module_impl::Module_impl(PCWSTR name, const Target_t & tgt, Level lvl, ssize_t index) :
			m_name(Str::length(name) + 1, name),
			m_target(tgt),
			m_index(index),
			m_lvl(lvl),
			m_wide(get_default_wideness()),
			m_color(0)
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

		Wideness Module_impl::get_wideness() const
		{
			return m_wide;
		}

		bool Module_impl::is_color_mode() const
		{
			return m_color;
		}

		void Module_impl::set_level(Level lvl)
		{
			m_lvl = lvl;
		}

		void Module_impl::set_wideness(Wideness wide)
		{
			m_wide = wide;
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
				va_list args;
				va_start(args, format);
				ustring tmp = format_str(fmtStrings[(int)m_wide].additional, LogLevelNames[(int)lvl], m_name.data(), ::GetCurrentThreadId());
				tmp += format_str(fmtStrings[(int)m_wide].place, file, line, func);
				;
				out_args(lvl, tmp, format, args);
				va_end(args);
			}
		}

		void Module_impl::out(Level lvl, PCWSTR format, ...) const
		{
			if (lvl >= m_lvl) {
				va_list args;
				va_start(args, format);
				ustring tmp = format_str(fmtStrings[(int)m_wide].additional, LogLevelNames[(int)lvl], m_name.data(), ::GetCurrentThreadId());
				out_args(lvl, tmp, format, args);
				va_end(args);
			}
		}

		void Module_impl::out_args(Level lvl, const ustring & prefix, PCWSTR format, va_list args) const
		{
			ustring tmp(prefix);
			tmp += format_str(format, args);
			m_target->out(this, lvl, tmp.c_str(), tmp.size());
		}

		ssize_t Module_impl::get_index() const
		{
			return m_index;
		}

		struct pModule_pModule_less: public std::binary_function<const Module_i *, const Module_i *, bool> {
			bool operator ()(const Module_i * lhs, const Module_i * rhs)
			{
				return Str::compare(lhs->get_name(), rhs->get_name()) < 0;
			}
		};

		struct pModule_PCWSTR_less: public std::binary_function<const Module_i *, PCWSTR, bool> {
			bool operator ()(const Module_i * lhs, PCWSTR rhs)
			{
				return Str::compare(lhs->get_name(), rhs) < 0;
			}
		};

		struct pModule_pModule_equal: public std::binary_function<const Module_i *, const Module_i *, bool> {
			bool operator ()(const Module_i * lhs, const Module_i * rhs)
			{
				return Str::compare(lhs->get_name(), rhs->get_name()) == 0;
			}
		};

		struct pModule_PCWSTR_equal: public std::binary_function<const Module_i *, PCWSTR, bool> {
			bool operator ()(const Module_i * lhs, PCWSTR rhs)
			{
				return Str::compare(lhs->get_name(), rhs) == 0;
			}
		};

		///================================================================================ Logger_i
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

//			Module_i * get_module_(PCWSTR name) const override;

			Module_i * register_module_(PCWSTR name, const Target_t & target, Level lvl) override;

			void free_module_(Module_i * module) override;

			static Level get_default_level()
			{
				return defLevel;
			}

			static Wideness get_default_wideness()
			{
				return defWideness;
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

			static void set_default_wideness(Wideness wideness)
			{
				defWideness = wideness;
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
			static Wideness defWideness;
			static Target_t defTarget;
			static Module_i * defModule;
			static PCWSTR const defModuleName;

			friend Logger_i & get_instance();
		};

		Level Logger_impl::defLevel = Level::Warn;
		Wideness Logger_impl::defWideness = Wideness::Medium;
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

//		Module_i & Logger_impl::get_module_(PCWSTR name) const {
//			auto lk(m_sync->get_lock_read());
//			return *(m_modules[module.index].iface);
//		}

		Module_i * Logger_impl::register_module_(PCWSTR name, const Target_t & target, Level lvl)
		{
			auto lk(m_sync->lock_scope());
			m_modules.push_back(new Module_impl(name, target, lvl, m_modules.size()));
			return m_modules.back();
		}

		void Logger_impl::free_module_(Module_i * module)
		{
			auto lk(m_sync->lock_scope());
			if (module) {
				ssize_t index = module->get_index();
				delete m_modules[index];
				m_modules[index] = nullptr;
			}
		}

		Level get_default_level()
		{
			return Logger_impl::get_default_level();
		}

		void set_default_level(Level lvl)
		{
			Logger_impl::set_default_level(lvl);
		}

		Wideness get_default_wideness()
		{
			return Logger_impl::get_default_wideness();
		}

		void set_default_wideness(Wideness wdns)
		{
			Logger_impl::set_default_wideness(wdns);
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

		Logger_i & get_instance()
		{
			static Logger_impl ret;
			return ret;
		}

		void set_target(Target_t & target, Module_i * module)
		{
			module->set_target(target);
		}

		void set_level(Level lvl, Module_i * module)
		{
			module->set_level(lvl);
		}

		void set_wideness(Wideness mode, Module_i * module)
		{
			module->set_wideness(mode);
		}

		void set_color_mode(bool mode, Module_i * module)
		{
			module->set_color_mode(mode);
		}

		///================================================================================ Target_i
		Target_i::~Target_i()
		{
		}

	}
}
