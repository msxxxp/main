#include <liblog/logger.hpp>
#include <patterns/Uncopyable.hpp>

namespace Logger {
	///================================================================================= Module_impl
	struct Module_impl: public Module_i, private Pattern::Uncopyable {
		void destroy() const override {}

		const wchar_t * get_name() const override {return Base::EMPTY_STR;}

		Level get_level() const override {return Level::Atten;}

		size_t get_prefix() const override {return 0;}

		bool is_color_mode() const override {return false;}

		void set_level(Level lvl) override {UNUSED(lvl);}

		void set_prefix(size_t prefix) override {UNUSED(prefix);}

		void set_color_mode(bool mode) override {UNUSED(mode);}

		void set_target(const Target_t & target) override {UNUSED(target);}

		void set_enabled(bool enabled) override {UNUSED(enabled);}

		void out(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const override
		{
			UNUSED(file);
			UNUSED(line);
			UNUSED(func);
			UNUSED(lvl);
			UNUSED(format);
		}

		void out(Level lvl, const wchar_t * format, ...) const override
		{
			UNUSED(lvl);
			UNUSED(format);
		}

		Lock::ScopeGuard lock_scope() const override {return Lock::ScopeGuard();}

		static Module_impl & inst()
		{
			static Module_impl instance;
			return instance;
		}
	};

	namespace Default {
		Level get_level()
		{
			return Level::Atten;
		}

		void set_level(Level lvl)
		{
			UNUSED(lvl);
		}

		size_t get_prefix()
		{
			return Prefix::Medium;
		}

		void set_prefix(size_t prefix)
		{
			UNUSED(prefix);
		}

		Target_t get_target()
		{
			return get_TargetToNull();
		}

		void set_target(Target_t target)
		{
			UNUSED(target);
		}

		Module_i * get_module()
		{
			return &Module_impl::inst();
		}
	} // namespace Default

	Module_i * get_module(const wchar_t * name, const Target_t & target, Level lvl)
	{
		UNUSED(name);
		UNUSED(target);
		UNUSED(lvl);
		return &Module_impl::inst();
	}

	Target_t get_TargetToConsole()
	{
		return get_TargetToNull();
	}

	Target_t get_TargetToFile(const wchar_t * path, bool overwrite)
	{
		UNUSED(path);
		UNUSED(overwrite);
		return get_TargetToNull();
	}

	Target_t get_TargetToSys(const wchar_t * name, const wchar_t * path)
	{
		UNUSED(name);
		UNUSED(path);
		return get_TargetToNull();
	}

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second)
	{
		UNUSED(first);
		UNUSED(second);
		return get_TargetToNull();
	}
}
