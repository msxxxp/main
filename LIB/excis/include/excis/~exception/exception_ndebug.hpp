#ifndef EXCIS_EXCEPTION_EXCEPTION_NDEBUG_HPP_
#define EXCIS_EXCEPTION_EXCEPTION_NDEBUG_HPP_

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

#endif
