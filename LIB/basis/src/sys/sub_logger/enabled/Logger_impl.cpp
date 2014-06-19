#include "../logger_pvt.hpp"

#include <basis/sys/cstr.hpp>

#include <basis/std/mutex>
#include <basis/std/vector>

namespace logger {

	///=============================================================================================
	struct pModule_less {
		bool operator ()(const Module_impl * lhs, const Module_impl * rhs) const
		{
			return cstr::compare_ci(lhs->get_name(), rhs->get_name()) < 0;
		}
	};

	struct pModule_PCWSTR_less {
		bool operator ()(Module_impl * lhs, const wchar_t * rhs) const
		{
			return cstr::compare_ci(lhs->get_name(), rhs) < 0;
		}

		bool operator ()(const wchar_t * left, Module_impl * right) const
		{
			return cstr::compare_ci(left, right->get_name()) < 0;
		}

		bool operator ()(const Module_impl * lhs, const Module_impl * rhs) const
		{
			return cstr::compare_ci(lhs->get_name(), rhs->get_name()) < 0;
		}
	};

	///================================================================================= Logger_impl
	struct Logger_impl: private pattern::Uncopyable {
		static Logger_impl & get_instance();

		~Logger_impl();

		Module_impl * get_module(const wchar_t * name);

		void free_module(Module_impl * module);

	private:
		Logger_impl();

		Module_impl * register_module(const wchar_t * name, const Target_t & target, Level lvl);

		typedef simstd::vector<Module_impl*> ModulesArray;
		typedef sync::CriticalSection SyncUnit;


		ModulesArray m_modules;
		SyncUnit     m_sync;

	public:
		static Level                 defaultLevel;
		static size_t                defaultPrefix;
		static Target_t              defaultTarget;
		static Module_impl *         defaultModule;
		static const wchar_t * const defaultModuleName;
	};

	Level                 Logger_impl::defaultLevel      = Level::Atten;
	size_t                Logger_impl::defaultPrefix     = Prefix::Medium;
	Target_t              Logger_impl::defaultTarget     = get_TargetToConsole();
	Module_impl *         Logger_impl::defaultModule     = nullptr;
	const wchar_t * const Logger_impl::defaultModuleName = L"default";

	Logger_impl & Logger_impl::get_instance()
	{
		static Logger_impl ret;
		return ret;
	}

	Logger_impl::~Logger_impl()
	{
		auto lockScope(simstd::auto_lock(m_sync));
		defaultModule->out(Level::Force, L"Logger is being destroyed\n");
		while (!m_modules.empty()) {
			m_modules.back()->destroy();
			m_modules.pop_back();
		}
	}

	Module_impl * Logger_impl::get_module(const wchar_t * name)
	{
		auto lockScope(simstd::auto_lock(m_sync));
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second)
			return *range.first;
		return register_module(name, defaults::get_target(), defaults::get_level());
	}

	void Logger_impl::free_module(Module_impl * module)
	{
		auto lockScope(simstd::auto_lock(m_sync));
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), module, pModule_less());
		simstd::for_each(range.first, range.second, [](Module_impl * found_module) {
			found_module->destroy();
		});
		m_modules.erase(range.first, range.second);
	}

	Logger_impl::Logger_impl()
	{
		auto lockScope(simstd::auto_lock(m_sync));
		defaultModule = register_module(defaultModuleName, defaultTarget, defaultLevel);
		defaultModule->out(Level::Force, L"Logger has been created\n");
	}

	Module_impl * Logger_impl::register_module(const wchar_t * name, const Target_t & target, Level lvl)
	{
		auto lockScope(simstd::auto_lock(m_sync));
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second) {
			return *range.first;
		}
		auto module = create_Module_impl(name, target, lvl);
		m_modules.insert(range.second, module);
		return module;
	}

	namespace defaults {
		Level get_level()
		{
			return Logger_impl::defaultLevel;
		}

		void set_level(Level lvl)
		{
			Logger_impl::defaultLevel = lvl;
		}

		size_t get_prefix()
		{
			return Logger_impl::defaultPrefix;
		}

		void set_prefix(size_t prefix)
		{
			Logger_impl::defaultPrefix = prefix;
		}

		Target_t get_target()
		{
			return Logger_impl::defaultTarget;
		}

		void set_target(Target_t target)
		{
			Logger_impl::defaultTarget = target;
		}

		Module_i * get_module()
		{
			return Logger_impl::get_instance().defaultModule; // defaultModule initializes only on Logger construct
		}
	} // namespace defaults

	Module_i * get_module(const wchar_t * name)
	{
		return Logger_impl::get_instance().get_module(name);
	}

	Module_impl * get_module_impl(const wchar_t * name)
	{
		return Logger_impl::get_instance().get_module(name);
	}

}
