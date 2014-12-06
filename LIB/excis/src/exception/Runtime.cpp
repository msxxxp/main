#include "exception_pvt.hpp"

#include <basis/sys/logger.hpp>

namespace exception {

	struct RuntimeError: public Abstract {
		RuntimeError * clone() const;

		const wchar_t* type() const;

		const wchar_t* what() const;

		DWORD code() const;

		void format_error(cstr::mstring& out) const;

	protected:
		RuntimeError(const ustring& wh, size_t code = 0);
		RuntimeError(const Abstract& prev, const ustring& wh, size_t code = 0);
		RuntimeError(const ustring& wh, PCSTR file, size_t line, PCSTR func, size_t code = 0);
		RuntimeError(const Abstract & prev, const ustring& wh, PCSTR file, size_t line, PCSTR func, size_t code = 0);

	private:
		size_t m_code;

		friend struct HiddenFunctions;
	};

	///================================================================================ RuntimeError
	RuntimeError::RuntimeError(const ustring& wh, size_t code) :
		m_code(code),
		m_what(wh)
	{
		LogNoise(L"%s\n", what());
	}

	RuntimeError::RuntimeError(const Abstract& prev, const ustring& wh, size_t code) :
		Abstract(prev),
		m_code(code),
		m_what(wh)
	{
		LogNoise(L"%s\n", what());
	}

	RuntimeError::RuntimeError(const ustring& wh, PCSTR file, size_t line, PCSTR func, size_t code) :
		Abstract(file, line, func),
		m_code(code),
		m_what(wh)
	{
		LogNoise(L"%s\n", what());
	}

	RuntimeError::RuntimeError(const Abstract& prev, const ustring& wh, PCSTR file, size_t line, PCSTR func, size_t code) :
		Abstract(prev, file, line, func),
		m_code(code),
		m_what(wh)
	{
		LogNoise(L"%s\n", what());
	}

	RuntimeError * RuntimeError::clone() const
	{
		return new RuntimeError(*this);
	}

	const wchar_t* RuntimeError::type() const
	{
		return L"RuntimeError";
	}

	const wchar_t* RuntimeError::what() const
	{
		return m_what;
	}

	DWORD RuntimeError::code() const
	{
		return m_code;
	}

	void RuntimeError::format_error(cstr::mstring& out) const
	{
		out.push_back(what());
		if (get_prev()) {
			get_prev()->format_error(out);
		}
	}

	void HiddenFunctions::RethrowExceptionFunc(const Abstract& prev, const ustring& what)
	{
		throw RuntimeError(prev, what, prev.code());
	}

	void HiddenFunctions::RethrowExceptionFunc(const Abstract& prev, const ustring& what, PCSTR file, size_t line, PCSTR func)
	{
		throw RuntimeError(prev, what, file, line, func, prev.code());
	}

}
