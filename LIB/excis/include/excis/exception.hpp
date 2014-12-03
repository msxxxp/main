#ifndef EXCIS_EXCEPTION_HPP_
#define EXCIS_EXCEPTION_HPP_

#include <basis/configure.hpp>

#include <basis/sys/cstr.hpp>

#include <basis/simstd/iosfwd>
#include <basis/simstd/memory>
//#include <basis/simstd/string>

#include <excis/~exception/Abstract.hpp>
#include <excis/~exception/SEH.hpp>

#ifdef NDEBUG
#include <excis/~exception/exception_ndebug.hpp>
#else
#include <excis/~exception/exception_debug.hpp>
#endif

namespace exception {

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

		static void RethrowExceptionFunc(const Abstract & prev, const ustring& what);

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

		static void RethrowExceptionFunc(const Abstract& prev, const ustring& what, PCSTR file, size_t line, PCSTR func);
	};

}

#endif
