#ifndef EXCIS_EXCEPTION_EXCEPTION_PVT_HPP_
#define EXCIS_EXCEPTION_EXCEPTION_PVT_HPP_

#include <excis/exception.hpp>

namespace exception {

	class WinError: public Abstract {
	public:
		WinError* clone() const override;

		const wchar_t* type() const override;

		const wchar_t* what() const override;

		DWORD code() const override;

	protected:
		WinError();
		WinError(DWORD code);
		WinError(PCSTR file, size_t line, PCSTR func);
		WinError(DWORD code, PCSTR file, size_t line, PCSTR func);

	private:
		DWORD m_code;

		friend struct HiddenFunctions;
	};

	class SehError: public Abstract {
	public:
		SehError(PEXCEPTION_POINTERS ep);

		SehError* clone() const override;

		const wchar_t* type() const override;

		const wchar_t* what() const override;

		DWORD code() const override;

	private:
		DWORD m_code;
		void* m_address;

		friend struct HiddenFunctions;
	};

	class WmiError: public WinError {
	public:
		WmiError* clone() const override;

		const wchar_t* type() const override;

		const wchar_t* what() const override;

	private:
		WmiError(HRESULT code);
		WmiError(HRESULT code, PCSTR file, size_t line, PCSTR func);

		friend struct HiddenFunctions;
	};

	class HMailError: public WinError {
	public:
		HMailError* clone() const override;

		const wchar_t* type() const override;

		const wchar_t* what() const override;

	private:
		HMailError(HRESULT code);
		HMailError(HRESULT code, PCSTR file, size_t line, PCSTR func);

		friend struct HiddenFunctions;
	};

	class WSockError: public WinError {
	public:
		WSockError* clone() const override;

		const wchar_t* type() const override;

	private:
		WSockError();
		WSockError(DWORD code);
		WSockError(PCSTR file, size_t line, PCSTR func);
		WSockError(DWORD code, PCSTR file, size_t line, PCSTR func);

		friend struct HiddenFunctions;
	};

}

#endif
