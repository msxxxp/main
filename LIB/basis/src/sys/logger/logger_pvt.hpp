#ifndef BASIS_SYS_LOGGER_LOGGER_PVT_HPP_
#define BASIS_SYS_LOGGER_LOGGER_PVT_HPP_

#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/sync.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/simstd/memory>
#include <basis/simstd/mutex>

namespace logger {

	const wchar_t* to_str(Level lvl);

	const wchar_t* to_str(Prefix::flags fl);

	typedef sync::CriticalSection sync_type;

	typedef simstd::lock_guard<sync_type> lock_type;

	///==================================================================================== Target_i
	struct Target_i {
		virtual ~Target_i() = default;

		virtual void out(const Module * module, Level lvl, const wchar_t* str, size_t size) const = 0;

		virtual void out(const Module * module, WORD color, Level lvl, const wchar_t* str, size_t size) const = 0;

		virtual void out(const wchar_t* str, size_t size) const = 0;

		virtual lock_type lock_scope() const = 0;
	};

	typedef simstd::shared_ptr<Target_i> Target_t;

	Target_t get_TargetToNull();

	Target_t get_TargetToConsole();

	Target_t get_TargetToFile(const wchar_t* path, bool overwrite = false);

	Target_t get_TargetToSys(const wchar_t* name, const wchar_t* path = nullptr);

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second);

	///================================================================================== ModuleImpl
	struct ModuleImpl: public Module, public pattern::Destroyable, private pattern::Uncopyable {
		~ModuleImpl();

		ModuleImpl(const wchar_t* name, const Target_t & tgt, Level lvl);

		void destroy() const override;

		const wchar_t* get_name() const;

		bool is_color_mode() const override;

		bool is_utf8_mode() const override;

		void out(Level lvl, const wchar_t* format, ...) const override;

		void out_with_color(WORD color, Level lvl, const wchar_t* format, ...) const override;

		void out_with_place(const char* file, int line, const char* func, Level lvl, const wchar_t* format, ...) const override;

		Level get_level() const;

		size_t get_prefix() const;

		void set_level(Level lvl);

		void set_prefix(size_t prefix);

		void set_color_mode(bool mode);

		void set_target(const Target_t & target);

		void set_enabled(bool enabled);

		lock_type lock_scope() const;

	private:
		wchar_t* add_prefix(Level lvl, wchar_t* buff, size_t size) const;

		wchar_t* add_place(wchar_t* buff, size_t size, const char* file, int line, const char* func) const;

		void out_args(Level lvl, wchar_t* buff, wchar_t* pend, size_t size, const wchar_t* frmat, va_list & args) const;

		void out_args(WORD color, Level lvl, const wchar_t* frmat, va_list args) const;

		wchar_t       m_name[16];
		Target_t      m_target;
		Level         m_lvl;
		Prefix::flags m_prefix;
		uint32_t      m_color:1;
		uint32_t      m_enabled:1;
		uint32_t      m_utf8_out:1;
	};

	ModuleImpl * get_module_impl(const wchar_t* name);

	ModuleImpl * create_Module_impl(const wchar_t* name, const Target_t & tgt, Level lvl);

	namespace defaults {
		Level get_level();

		size_t get_prefix();

		Target_t get_target();

		void set_level(Level lvl);

		void set_prefix(size_t prefix);

		void set_target(Target_t target);
	}  // namespace defaults

//	inline void set_target(ModuleImpl * module, const Target_t & target)
//	{
//		module->set_target(target);
//	}
//
//	inline void set_level(ModuleImpl * module, Level lvl)
//	{
//		module->set_level(lvl);
//	}
//
//	inline void set_prefix(ModuleImpl * module, size_t prefix)
//	{
//		module->set_prefix(prefix);
//	}
//
//	inline Prefix::flags get_prefix(ModuleImpl * module)
//	{
//		return module->get_prefix();
//	}
//
//	inline Level get_level(ModuleImpl * module)
//	{
//		return module->get_level();
//	}
//
//	inline void set_color_mode(ModuleImpl * module, bool mode)
//	{
//		module->set_color_mode(mode);
//	}
//
//	inline void set_enabled(ModuleImpl * module, bool enabled)
//	{
//		module->set_enabled(enabled);
//	}
//
//	inline sync::ScopeGuard lock_scope(ModuleImpl * module)
//	{
//		return module->lock_scope();
//	}

}

#endif
