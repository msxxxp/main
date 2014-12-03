#ifndef EXCIS_EXCEPTION_EXCEPTION_DEBUG_HPP_
#define EXCIS_EXCEPTION_EXCEPTION_DEBUG_HPP_

#define THROW_PLACE __FILE__, __LINE__, __PRETTY_FUNCTION__

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

#endif
