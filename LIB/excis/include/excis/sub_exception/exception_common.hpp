#ifndef _LIBEXT_EXCEPTION_EXCEPTION_COMMON_HPP_
#define _LIBEXT_EXCEPTION_EXCEPTION_COMMON_HPP_

namespace exception {

	class AbstractError {
	public:
		virtual ~AbstractError();

		virtual AbstractError * clone() const = 0;

		virtual ustring type() const = 0;

		virtual ustring what() const = 0;

		virtual DWORD code() const = 0;

		virtual void format_error(cstr::mstring & out) const = 0;

		PCWSTR where() const;

		AbstractError * get_prev() const;

		cstr::mstring format_error() const;

	protected:
#ifdef NDEBUG
		AbstractError();
		AbstractError(const AbstractError & prev);
#else
		AbstractError(PCSTR file, size_t line, PCSTR func);
		AbstractError(const AbstractError & prev, PCSTR file, size_t line, PCSTR func);
#endif

	private:
		simstd::shared_ptr<AbstractError> m_prev_exc;
//		simstd::Backtrace                 m_backtrace;
#ifndef NDEBUG
		ustring	                          m_where;
#endif
	};

}

#endif
