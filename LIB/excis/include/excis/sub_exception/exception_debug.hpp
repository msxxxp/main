#ifndef _LIBEXT_EXCEPTION_EXCEPTION_DEBUG_HPP_
#define _LIBEXT_EXCEPTION_EXCEPTION_DEBUG_HPP_

#define THROW_PLACE THIS_FILE, __LINE__, __PRETTY_FUNCTION__

#define THROW_PLACE_STR exception::ThrowPlaceString(file, line, func)

namespace exception {

#define CheckApi(arg) (exception::HiddenFunctions::CheckApiFunc((arg), THROW_PLACE))

#define CheckApiThrowError(arg1, arg2) (exception::HiddenFunctions::CheckApiThrowErrorFunc((arg1), (arg2), THROW_PLACE))

#define CheckApiError(arg) (exception::HiddenFunctions::CheckApiErrorFunc((arg), THROW_PLACE))

#define CheckHMailError(arg) (exception::HiddenFunctions::CheckHMailErrorFunc((arg), THROW_PLACE))

#define CheckWSock(arg) (exception::HiddenFunctions::CheckWSockFunc((arg), THROW_PLACE))

#define CheckCom(arg) (exception::HiddenFunctions::CheckComFunc((arg), THROW_PLACE))

#define CheckWmi(arg) (exception::HiddenFunctions::CheckWmiFunc((arg), THROW_PLACE))

#define CheckHandle(arg) (exception::HiddenFunctions::CheckHandleFunc((arg), THROW_PLACE))

#define CheckHandleErr(arg) (exception::HiddenFunctions::CheckHandleErrFunc((arg), THROW_PLACE))

#define CheckPointer(arg) (exception::HiddenFunctions::CheckPointerFunc((arg), THROW_PLACE))

#define CheckPointerErr(arg) (exception::HiddenFunctions::CheckPointerErrFunc((arg), THROW_PLACE))

#define Rethrow(arg1, arg2) (exception::HiddenFunctions::RethrowExceptionFunc((arg1), (arg2), THROW_PLACE))

	struct HiddenFunctions {
		static bool CheckApiFunc(bool r, PCSTR file, size_t line, PCSTR func);

		static bool CheckApiThrowErrorFunc(bool r, DWORD err, PCSTR file, size_t line, PCSTR func);

		static DWORD CheckApiErrorFunc(DWORD err, PCSTR file, size_t line, PCSTR func);

		static HRESULT CheckHMailErrorFunc(HRESULT err, PCSTR file, size_t line, PCSTR func);

		static int CheckWSockFunc(int err, PCSTR file, size_t line, PCSTR func);

		static HRESULT CheckComFunc(HRESULT res, PCSTR file, size_t line, PCSTR func);

		static HRESULT CheckWmiFunc(HRESULT res, PCSTR file, size_t line, PCSTR func);

		static HANDLE CheckHandleFuncHan(HANDLE hnd, PCSTR file, size_t line, PCSTR func);

		static HANDLE CheckHandleErrFuncHan(HANDLE hnd, PCSTR file, size_t line, PCSTR func);

		static PVOID CheckPointerFuncVoid(PVOID ptr, PCSTR file, size_t line, PCSTR func);

		static PVOID CheckPointerErrFuncVoid(PVOID ptr, PCSTR file, size_t line, PCSTR func);

		template<typename Type>
		static Type CheckHandleFunc(Type hnd, PCSTR file, size_t line, PCSTR func)
		{
			return (Type)CheckHandleFuncHan((HANDLE)hnd, file, line, func);
		}

		template<typename Type>
		static Type CheckHandleErrFunc(Type hnd, PCSTR file, size_t line, PCSTR func)
		{
			return (Type)CheckHandleErrFuncHan((HANDLE)hnd, file, line, func);
		}

		template<typename Type>
		static Type CheckPointerFunc(Type ptr, PCSTR file, size_t line, PCSTR func)
		{
			return (Type)CheckPointerFuncVoid((PVOID)ptr, file, line, func);
		}

		template<typename Type>
		static Type CheckPointerErrFunc(Type ptr, PCSTR file, size_t line, PCSTR func)
		{
			return (Type)CheckPointerErrFuncVoid((PVOID)ptr, file, line, func);
		}

		static void RethrowExceptionFunc(const AbstractError & prev, const ustring & what, PCSTR file, size_t line, PCSTR func);
	};
	extern PCWSTR const THROW_PLACE_FORMAT;

	ustring ThrowPlaceString(PCSTR file, int line, PCSTR func);

}

#endif
