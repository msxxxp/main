#include "logger_pvt.hpp"

#include <basis/sys/cstr.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/mutex>
#include <basis/simstd/vector>

namespace logger {

	struct pModule_less {
		bool operator ()(const ModuleImpl* left, const wchar_t* right) const
		{
			return cstr::compare_ci(left->get_name(), right) < 0;
		}

		bool operator ()(const wchar_t* left, const ModuleImpl* right) const
		{
			return cstr::compare_ci(left, right->get_name()) < 0;
		}

		bool operator ()(const ModuleImpl* left, const ModuleImpl* right) const
		{
			return cstr::compare_ci(left->get_name(), right->get_name()) < 0;
		}
	};

	///================================================================================= Logger_impl
	struct Logger: private pattern::Uncopyable {
		static Logger& get_instance();

		~Logger();

		ModuleImpl* get_module(const wchar_t* name);

		void free_module(ModuleImpl* module);

	private:
		Logger();

		ModuleImpl* register_module(const wchar_t* name, const Target_t& target, Level lvl);

		typedef simstd::vector<ModuleImpl*> Modules;

		sync_type m_sync;
		Modules m_modules;

	public:
		static Level                defaultLevel;
		static size_t               defaultPrefix;
		static Target_t             defaultTarget;
		static ModuleImpl*          defaultModule;
		static const wchar_t* const defaultModuleName;
	};

	Level                Logger::defaultLevel      = Level::Atten;
	size_t               Logger::defaultPrefix     = Prefix::Medium;
	Target_t             Logger::defaultTarget;
	ModuleImpl*          Logger::defaultModule     = nullptr;
	const wchar_t* const Logger::defaultModuleName = L"default";

	Logger & Logger::get_instance()
	{
		static Logger ret;
		return ret;
	}

	Logger::~Logger()
	{
		TraceFunc();
//		auto lockScope(simstd::auto_lock(m_sync));
		defaultModule->out(Level::Force, L"Logger is being destroyed. Modules: %Iu\n", m_modules.size());
		while (!m_modules.empty()) {
			m_modules.back()->destroy();
			m_modules.pop_back();
		}
		TraceFunc();
	}

	ModuleImpl* Logger::get_module(const wchar_t* name)
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_less());
		if (range.first != range.second)
			return *range.first;
		return register_module(name, defaults::get_target(), defaults::get_level());
	}

	void Logger::free_module(ModuleImpl* module)
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), module, pModule_less());
		simstd::for_each(range.first, range.second, [](ModuleImpl* found_module) {
			found_module->destroy();
		});
		m_modules.erase(range.first, range.second);
	}

	Logger::Logger()
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		if (!defaultTarget) {
			TraceFunc();
			defaultTarget = get_TargetToConsole();
		}

		TraceFunc();
		defaultModule = register_module(defaultModuleName, defaultTarget, defaultLevel);
		TraceFunc();
		defaultModule->out(Level::Force, L"Logger has been created\n");
		TraceFunc();
	}

	ModuleImpl* Logger::register_module(const wchar_t* name, const Target_t& target, Level lvl)
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_less());
		if (range.first != range.second) {
			return *range.first;
		}
		TraceFunc();
		auto module = create_Module_impl(name, target, lvl);
		m_modules.insert(range.second, module);
		TraceFunc();
		return module;
	}

	namespace defaults {
		Level get_level()
		{
			return Logger::defaultLevel;
		}

		void set_level(Level lvl)
		{
			Logger::defaultLevel = lvl;
		}

		size_t get_prefix()
		{
			return Logger::defaultPrefix;
		}

		void set_prefix(size_t prefix)
		{
			Logger::defaultPrefix = prefix;
		}

		Target_t get_target()
		{
			return Logger::defaultTarget;
		}

		void set_target(Target_t target)
		{
			Logger::defaultTarget = target;
		}

	}

	Module* get_default_module()
	{
		auto static module = Logger::get_instance().defaultModule; // defaultModule being initialized only on Logger construct
		return module;
	}

	Module* get_console_module()
	{
		auto static module = get_module(L"std_console");
		return module;
	}

	Module* get_module(const wchar_t* name)
	{
		return Logger::get_instance().get_module(name);
	}

	ModuleImpl* get_module_impl(const wchar_t* name)
	{
		return Logger::get_instance().get_module(name);
	}

}
