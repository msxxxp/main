#ifndef BASIS_SYS_LOGGER_LOGGER_PVT_HPP_
#define BASIS_SYS_LOGGER_LOGGER_PVT_HPP_

#include <basis/sys/logger.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/sync.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/simstd/memory>
#include <basis/simstd/string>

namespace logger {

	typedef Module_i TypeTag;
	typedef memory::heap::DefaultStatTag<TypeTag> heap_type;
	typedef typename simstd::AllocatorHeap<wchar_t, heap_type> Allocator;
	typedef typename simstd::basic_string<wchar_t, simstd::char_traits<wchar_t>, Allocator> ustring;


	namespace Prefix {
		typedef size_t flags;
		const static flags Function = 0x0001;
		const static flags Level    = 0x0002;
		const static flags Module   = 0x0004;
		const static flags Place    = 0x0008;
		const static flags Thread   = 0x0010;
		const static flags Time     = 0x0020;
		const static flags Date     = 0x0040;

		const static flags Lite     = Time | Level;
		const static flags Medium   = Time | Level | Function;
		const static flags Full     = Time | Level | Function | Date | Module | Thread | Place;
	}

	const wchar_t * to_str(Prefix::flags fl);

	///==================================================================================== Target_i
	struct Target_i {
		virtual ~Target_i();

		virtual void out(const Module_i * module, Level lvl, const wchar_t * str, size_t size) const = 0;

		virtual void out(const Module_i * module, WORD color, Level lvl, const wchar_t * str, size_t size) const = 0;

		virtual void out(const wchar_t * str, size_t size) const = 0;

		virtual sync::ScopeGuard lock_scope() const = 0;
	};

	typedef simstd::shared_ptr<Target_i> Target_t;

	Target_t get_TargetToNull();

	Target_t get_TargetToConsole();

	Target_t get_TargetToFile(const wchar_t * path, bool overwrite = false);

	Target_t get_TargetToSys(const wchar_t * name, const wchar_t * path = nullptr);

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second);

	///================================================================================= Module_impl
	struct Module_impl: public Module_i, public pattern::Destroyable, private pattern::Uncopyable {
		Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl);

		void destroy() const override;

		const wchar_t * get_name() const;

		bool is_color_mode() const override;

		void out(Level lvl, const wchar_t * format, ...) const override;

		void out_console(WORD color, Level lvl, const wchar_t * format, ...) const override;

		void out_debug(const char * file, int line, const char * func, Level lvl, const wchar_t * format, ...) const override;

		Level get_level() const;

		size_t get_prefix() const;

		void set_level(Level lvl);

		void set_prefix(size_t prefix);

		void set_color_mode(bool mode);

		void set_target(const Target_t & target);

		void set_enabled(bool enabled);

		sync::ScopeGuard lock_scope() const;

	private:
		wchar_t * create_prefix(Level lvl, wchar_t * buff, size_t size) const;

		wchar_t * add_place(wchar_t * buff, size_t size, const char * file, int line, const char * func) const;

		void out_args(Level lvl, wchar_t * buff, wchar_t * pend, size_t size, const wchar_t * frmat, va_list args) const;

		void out_args(WORD color, Level lvl, const wchar_t * frmat, va_list args) const;

		ustring       m_name;
		Target_t      m_target;
		Level         m_lvl;
		Prefix::flags m_prefix;
		uint32_t      m_color:1;
		uint32_t      m_enabled:1;
	};

	Module_impl * get_module_impl(const wchar_t * name);

	Module_impl * create_Module_impl(const wchar_t * name, const Target_t & tgt, Level lvl);

	namespace defaults {
		Level get_level();

		size_t get_prefix();

		Target_t get_target();

		void set_level(Level lvl);

		void set_prefix(size_t prefix);

		void set_target(Target_t target);
	}  // namespace defaults

//	inline void set_target(Module_impl * module, const Target_t & target)
//	{
//		module->set_target(target);
//	}
//
//	inline void set_level(Module_impl * module, Level lvl)
//	{
//		module->set_level(lvl);
//	}
//
//	inline void set_prefix(Module_impl * module, size_t prefix)
//	{
//		module->set_prefix(prefix);
//	}
//
//	inline Prefix::flags get_prefix(Module_impl * module)
//	{
//		return module->get_prefix();
//	}
//
//	inline Level get_level(Module_impl * module)
//	{
//		return module->get_level();
//	}
//
//	inline void set_color_mode(Module_impl * module, bool mode)
//	{
//		module->set_color_mode(mode);
//	}
//
//	inline void set_enabled(Module_impl * module, bool enabled)
//	{
//		module->set_enabled(enabled);
//	}
//
//	inline sync::ScopeGuard lock_scope(Module_impl * module)
//	{
//		return module->lock_scope();
//	}

}

#endif
