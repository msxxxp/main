#ifndef EXCIS_EXCEPTION_ABSTRACT_HPP_
#define EXCIS_EXCEPTION_ABSTRACT_HPP_

namespace exception {

	class Abstract {
		typedef Abstract this_type;
	public:
		virtual ~Abstract();

		virtual this_type* clone() const = 0;

		virtual ustring type() const = 0;

		virtual ustring what() const = 0;

		virtual ustring where() const;

		virtual DWORD code() const = 0;

		virtual void format_error(cstr::mstring& out) const = 0;

		this_type* get_prev() const;

		cstr::mstring format_error() const;

	protected:
		Abstract();
		Abstract(const this_type& prev);
		Abstract(PCSTR file, size_t line, PCSTR func);
		Abstract(const this_type& prev, PCSTR file, size_t line, PCSTR func);

	protected:
		const char* file;
		const char* func;
		size_t line;

	private:
		const wchar_t* m_where;
		simstd::shared_ptr<this_type> m_prev_exc;
//		simstd::Backtrace             m_backtrace;
	};

}

#endif
