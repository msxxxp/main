#ifndef _LIBEXT_EXCEPTION_EXCEPTION_NDEBUG_HPP_
#define _LIBEXT_EXCEPTION_EXCEPTION_NDEBUG_HPP_

#define THROW_PLACE_STR ustring()

namespace exception {

#define CheckApi(arg) (exception::HiddenFunctions::CheckApiFunc((arg)))

#define CheckApiThrowError(arg1, arg2) (exception::HiddenFunctions::CheckApiThrowErrorFunc((arg1), (arg2)))

#define CheckApiError(arg) (exception::HiddenFunctions::CheckApiErrorFunc((arg)))

#define CheckHMailError(arg) (exception::HiddenFunctions::CheckHMailErrorFunc((arg)))

#define CheckWSock(arg) (exception::HiddenFunctions::CheckWSockFunc((arg)))

#define CheckCom(arg) (exception::HiddenFunctions::CheckComFunc((arg)))

#define CheckWmi(arg) (exception::HiddenFunctions::CheckWmiFunc((arg)))

#define CheckHandle(arg) (exception::HiddenFunctions::CheckHandleFunc((arg)))

#define CheckHandleErr(arg) (exception::HiddenFunctions::CheckHandleErrFunc((arg)))

#define CheckPointer(arg) (exception::HiddenFunctions::CheckPointerFunc((arg)))

#define CheckPointerErr(arg) (exception::HiddenFunctions::CheckPointerErrFunc((arg)))

#define Rethrow(arg1, arg2) (exception::HiddenFunctions::RethrowExceptionFunc((arg1), (arg2)))

	struct HiddenFunctions {
		static bool CheckApiFunc(bool r);

		static bool CheckApiThrowErrorFunc(bool r, DWORD err);

		static DWORD CheckApiErrorFunc(DWORD err);

		static HRESULT CheckHMailErrorFunc(HRESULT err);

		static int CheckWSockFunc(int err);

		static HRESULT CheckComFunc(HRESULT res);

		static HRESULT CheckWmiFunc(HRESULT res);

		static HANDLE CheckHandleFuncHan(HANDLE hnd);

		static HANDLE CheckHandleErrFuncHan(HANDLE hnd);

		static PVOID CheckPointerFuncVoid(PVOID ptr);

		static PVOID CheckPointerErrFuncVoid(PVOID ptr);

		template<typename Type>
		static Type CheckHandleFunc(Type hnd)
		{
			return (Type)CheckHandleFuncHan((HANDLE)hnd);
		}

		template<typename Type>
		static Type CheckHandleErrFunc(Type hnd)
		{
			return (Type)CheckHandleErrFuncHan((HANDLE)hnd);
		}

		template<typename Type>
		static Type CheckPointerFunc(Type ptr)
		{
			return (Type)CheckPointerFuncVoid((PVOID)ptr);
		}

		template<typename Type>
		static Type CheckPointerErrFunc(Type ptr)
		{
			return (Type)CheckPointerErrFuncVoid((PVOID)ptr);
		}

		static void RethrowExceptionFunc(const AbstractError & prev, const ustring & what);
	};

}

#endif
