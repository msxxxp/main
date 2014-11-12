#include "../logger_pvt.hpp"

#include <basis/sys/cstr.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/mutex>
#include <basis/simstd/vector>

//#define TraceFunc() console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
#define TraceFunc()

namespace logger {

	struct pModule_less {
		bool operator ()(const Module_impl* left, const wchar_t* right) const
		{
			return cstr::compare_ci(left->get_name(), right) < 0;
		}

		bool operator ()(const wchar_t* left, const Module_impl* right) const
		{
			return cstr::compare_ci(left, right->get_name()) < 0;
		}

		bool operator ()(const Module_impl* left, const Module_impl* right) const
		{
			return cstr::compare_ci(left->get_name(), right->get_name()) < 0;
		}
	};

	///================================================================================= Logger_impl
	struct Logger_impl: private pattern::Uncopyable {
		static Logger_impl& get_instance();

		~Logger_impl();

		Module_impl * get_module(const wchar_t * name);

		void free_module(Module_impl * module);

	private:
		Logger_impl();

		Module_impl * register_module(const wchar_t * name, const Target_t & target, Level lvl);

		typedef simstd::vector<Module_impl*> ModulesArray;

		sync_type    m_sync;
		ModulesArray m_modules;

	public:
		static Level                 defaultLevel;
		static size_t                defaultPrefix;
		static Target_t              defaultTarget;
		static Module_impl *         defaultModule;
		static const wchar_t * const defaultModuleName;
	};

	Level                 Logger_impl::defaultLevel      = Level::Atten;
	size_t                Logger_impl::defaultPrefix     = Prefix::Medium;
	Target_t              Logger_impl::defaultTarget;
	Module_impl *         Logger_impl::defaultModule     = nullptr;
	const wchar_t * const Logger_impl::defaultModuleName = L"default";

	Logger_impl & Logger_impl::get_instance()
	{
		static Logger_impl ret;
		return ret;
	}

	Logger_impl::~Logger_impl()
	{
//		TraceFunc();
//		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		defaultModule->out(Level::Force, L"Logger is being destroyed\n");
		while (!m_modules.empty()) {
			m_modules.back()->destroy();
			m_modules.pop_back();
		}
		TraceFunc();
	}

	Module_impl * Logger_impl::get_module(const wchar_t * name)
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_less());
		if (range.first != range.second)
			return *range.first;
		return register_module(name, defaults::get_target(), defaults::get_level());
	}

	void Logger_impl::free_module(Module_impl * module)
	{
		TraceFunc();
		auto lockScope(simstd::auto_lock(m_sync));
		TraceFunc();
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), module, pModule_less());
		simstd::for_each(range.first, range.second, [](Module_impl * found_module) {
			found_module->destroy();
		});
		m_modules.erase(range.first, range.second);
	}

	Logger_impl::Logger_impl()
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

	Module_impl * Logger_impl::register_module(const wchar_t * name, const Target_t & target, Level lvl)
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
