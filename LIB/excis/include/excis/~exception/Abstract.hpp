#ifndef EXCIS_EXCEPTION_ABSTRACT_HPP_
#define EXCIS_EXCEPTION_ABSTRACT_HPP_

namespace exception {

	class Abstract {
		typedef Abstract this_type;
	public:
		virtual ~Abstract();

		virtual this_type* clone() const = 0;

		virtual const wchar_t* type() const = 0;

		virtual DWORD code() const = 0;

		virtual const wchar_t* what() const = 0;

		virtual const wchar_t* where() const;

		virtual void format_error(cstr::mstring& out) const;

		this_type* get_prev() const;

		cstr::mstring format_error() const;

	protected:
		Abstract();
		Abstract(const this_type& prev);

		void change_what(const wchar_t* newWhere) const;
		void change_where(const wchar_t* newWhere) const;

	protected:
		const char* file;
		const char* func;
		size_t line;

	private:
		mutable const wchar_t* m_what;
		mutable const wchar_t* m_where;
		simstd::shared_ptr<this_type> m_prev_exc;
//		simstd::Backtrace             m_backtrace;
	};

}

#endif
