#include <liblog/logger.hpp>
#include <system/configure.hpp>
#include <system/console.hpp>
#include <system/cstr.hpp>
//#include <libbase/memory.hpp>
#include <extra/pattern.hpp>
#include <simstl/vector>

#include "Logger_impl.hpp"
#include "Module_impl.hpp"

namespace logger {

	///=============================================================================================
	struct pModule_less {
		bool operator ()(const Module_i * lhs, const Module_i * rhs) const
		{
			return Cstr::compare(lhs->get_name(), rhs->get_name()) < 0;
		}
	};

	struct pModule_PCWSTR_less {
		bool operator ()(Module_i * lhs, const wchar_t * rhs) const
		{
			return Cstr::compare(lhs->get_name(), rhs) < 0;
		}

		bool operator ()(const wchar_t * left, Module_i * right) const
		{
			return Cstr::compare(left, right->get_name()) < 0;
		}

		bool operator ()(const Module_i * lhs, const Module_i * rhs) const
		{
			return Cstr::compare(lhs->get_name(), rhs->get_name()) < 0;
		}
	};

	///================================================================================= Logger_impl
	struct Logger_impl: public Logger_i, private pattern::Uncopyable {
		~Logger_impl();

		Module_i * get_module(const wchar_t * name) override;

		Module_i * register_module(const wchar_t * name, const Target_t & target, Level lvl) override;

		void free_module(Module_i * module) override;

	private:
		Logger_impl();

		typedef simstd::vector<Module_i*> ModulesArray;
		typedef memory::auto_destroy<sync::SyncUnit_i*> SyncUnit;

		ModulesArray m_modules;
		SyncUnit     m_sync;

	public:
		static Level                 defaultLevel;
		static size_t                defaultPrefix;
		static Target_t              defaultTarget;
		static Module_i *            defaultModule;
		static const wchar_t * const defaultModuleName;

		friend Logger_i & get_instance();
	};

	Logger_impl::~Logger_impl()
	{
		auto lockScope(m_sync->lock_scope());
		defaultModule->out(Level::Force, L"Logger is being destroyed\n");
		while (!m_modules.empty()) {
			m_modules.back()->destroy();
			m_modules.pop_back();
		}
	}

	Logger_impl::Logger_impl() :
		m_sync(sync::get_CritSection())
	{
		auto lockScope(m_sync->lock_scope());
		defaultModule = register_module(defaultModuleName, defaultTarget, defaultLevel);
		defaultModule->out(Level::Force, L"Logger has been created\n");
	}

	Module_i * Logger_impl::get_module(const wchar_t * name)
	{
		auto lockScope(m_sync->lock_scope());
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second)
			return *range.first;
		return register_module(name, Default::get_target(), Default::get_level());
	}

	Module_i * Logger_impl::register_module(const wchar_t * name, const Target_t & target, Level lvl)
	{
		auto lockScope(m_sync->lock_scope());
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), name, pModule_PCWSTR_less());
		if (range.first != range.second) {
			return *range.first;
		}
		auto module = create_Module_impl(name, target, lvl);
		m_modules.insert(range.second, module);
		return module;
	}

	void Logger_impl::free_module(Module_i * module)
	{
		auto lockScope(m_sync->lock_scope());
		auto range = simstd::equal_range(m_modules.begin(), m_modules.end(), module, pModule_less());
		simstd::for_each(range.first, range.second, [](Module_i * found_module) {
			found_module->destroy();
		});
		m_modules.erase(range.first, range.second);
	}

	Level Logger_impl::defaultLevel = Level::Atten;
	size_t Logger_impl::defaultPrefix = Prefix::Medium;
	Target_t Logger_impl::defaultTarget = get_TargetToConsole();
	Module_i * Logger_impl::defaultModule = nullptr;
	const wchar_t * const Logger_impl::defaultModuleName = L"default";

	namespace Default {
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
			get_instance(); // defaultModule initializes only on Logger construct
			return Logger_impl::defaultModule;
		}
	} // namespace Default

	Module_i * get_module(const wchar_t * name, const Target_t & target, Level lvl)
	{
		return get_instance().register_module(name, target, lvl);
	}

	///=============================================================================================
	Logger_i & get_instance()
	{
		static Logger_impl ret;
		return ret;
	}

}
