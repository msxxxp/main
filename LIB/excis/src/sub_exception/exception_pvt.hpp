#ifndef EXCIS_EXCEPTION_EXCEPTION_PVT_HPP_
#define EXCIS_EXCEPTION_EXCEPTION_PVT_HPP_

#include <excis/exception.hpp>

namespace exception {

	///==================================================================================== WinError
	struct WinError: public AbstractError {
		virtual WinError * clone() const override;

		virtual ustring type() const override;

		virtual ustring	 what() const override;

		virtual DWORD code() const override;

		virtual void format_error(cstr::mstring& out) const override;

	protected:
#ifdef NDEBUG
		WinError();
		WinError(DWORD code);
#else
		WinError(PCSTR file, size_t line, PCSTR func);
		WinError(DWORD code, PCSTR file, size_t line, PCSTR func);
#endif

	private:
		DWORD m_code;

		friend struct HiddenFunctions;
	};


	///========================================================================================= Seh
	struct SehError: public AbstractError {
		SehError(PEXCEPTION_POINTERS ep);

		virtual SehError * clone() const override;

		virtual ustring type() const override;

		virtual ustring	 what() const override;

		virtual DWORD code() const override;

		virtual void format_error(cstr::mstring& out) const override;

	private:
		DWORD  m_code;
		void * m_address;

		friend struct HiddenFunctions;
	};

	///==================================================================================== WmiError
	struct WmiError: public WinError {
		virtual WmiError * clone() const override;

		virtual ustring type() const override;

		virtual ustring	 what() const override;

	private:
#ifdef NDEBUG
		WmiError(HRESULT code);
#else
		WmiError(HRESULT code, PCSTR file, size_t line, PCSTR func);
#endif

		friend struct HiddenFunctions;
	};


	///================================================================================== HMailError
	struct HMailError: public WinError {
		virtual HMailError * clone() const override;

		virtual ustring type() const override;

		virtual ustring	 what() const override;

	private:
#ifdef NDEBUG
		HMailError(HRESULT code);
#else
		HMailError(HRESULT code, PCSTR file, size_t line, PCSTR func);
#endif

		friend struct HiddenFunctions;
	};


	///================================================================================== WSockError
	struct WSockError: public WinError {
		virtual WSockError * clone() const override;

		virtual ustring type() const override;

	private:
#ifdef NDEBUG
		WSockError();
		WSockError(DWORD code);
#else
		WSockError(PCSTR file, size_t line, PCSTR func);
		WSockError(DWORD code, PCSTR file, size_t line, PCSTR func);
#endif

		friend struct HiddenFunctions;
	};

}

#endif
