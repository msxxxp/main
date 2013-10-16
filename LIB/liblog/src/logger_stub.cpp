#include <liblog/logger.hpp>

namespace Logger {
	///==================================================================================== Target_i
	Target_i::~Target_i()
	{
	}

	///==================================================================================== Module_i
	Module_i::~Module_i()
	{
	}

	///================================================================================= Module_impl
	struct Module_impl: public Module_i, private Base::Uncopyable {
		PCWSTR get_name() const override {return Base::EMPTY_STR;}

		Level get_level() const override {return Level::Atten;}

		size_t get_prefix() const override {return 0;}

		bool is_color_mode() const override {return false;}

		void set_level(Level /*lvl*/) override {}

		void set_prefix(size_t /*prefix*/) override {}

		void set_color_mode(bool /*mode*/) override {}

		void set_target(const Target_t & /*target*/) override {}

		void set_enabled(bool /*enabled*/) override {}

		void out(PCSTR /*file*/, int /*line*/, PCSTR /*func*/, Level /*lvl*/, PCWSTR /*format*/, ...) const override {}

		void out(Level /*lvl*/, PCWSTR /*format*/, ...) const override {}

		void batch_lock() const override {}

		void batch_unlock() const override {}

		static Module_impl & inst()
		{
			static Module_impl instance;
			return instance;
		}
	};

	Level get_default_level()
	{
		return Level::Atten;
	}

	void set_default_level(Level /*lvl*/)
	{
	}

	size_t get_default_prefix()
	{
		return Prefix::Medium;
	}

	void set_default_prefix(size_t /*prefix*/)
	{
	}

	Target_t get_default_target()
	{
		return get_TargetToNull();
	}

	void set_default_target(Target_t /*target*/)
	{
	}

	Module_i * get_default_module()
	{
		return &Module_impl::inst();
	}

	Module_i * get_module(PCWSTR /*name*/, const Target_t & /*target*/, Level /*lvl*/)
	{
		return &Module_impl::inst();
	}

	Target_t get_TargetToConsole()
	{
		return get_TargetToNull();
	}

	Target_t get_TargetToFile(PCWSTR /*path*/, bool /*overwrite*/)
	{
		return get_TargetToNull();
	}

	Target_t get_TargetToSys(PCWSTR /*name*/, PCWSTR /*path*/)
	{
		return get_TargetToNull();
	}

	Target_t get_TargetToMult(const Target_t & /*first*/, const Target_t & /*second*/)
	{
		return get_TargetToNull();
	}
}
