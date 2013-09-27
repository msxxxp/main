#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>
#include <libbase/logger.hpp>

namespace SevenZip {
	///================================================================================ OpenCallback
	OpenCallback::~OpenCallback()
	{
		LogTrace();
	}

	OpenCallback::OpenCallback()
	{
		LogTrace();
	}

	ULONG WINAPI OpenCallback::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI OpenCallback::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI OpenCallback::QueryInterface(REFIID riid, void ** object)
	{
//		LogTrace();
		if (IsEqualIID(riid, IID_IArchiveOpenCallback) && object) {
//			LogNoise(L"IID_IArchiveOpenCallback\n");
			*object = static_cast<IArchiveOpenCallback*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ICryptoGetTextPassword) && object) {
//			LogNoise(L"IID_ICryptoGetTextPassword\n");
			*object = static_cast<ICryptoGetTextPassword*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	HRESULT WINAPI OpenCallback::SetTotal(const UInt64 *files, const UInt64 *bytes)
	{
		UNUSED(files);
		UNUSED(bytes);

		LogNoise(L"%I64u, %I64u\n", files ? *files : 0ULL, bytes ? *bytes : 0ULL);

		return S_OK;
	}

	HRESULT WINAPI OpenCallback::SetCompleted(const UInt64 *files, const UInt64 *bytes)
	{
		UNUSED(files);
		UNUSED(bytes);

		LogNoise(L"%I64u, %I64u\n", files ? *files : 0ULL, bytes ? *bytes : 0ULL);

		return S_OK;
	}

	HRESULT WINAPI OpenCallback::CryptoGetTextPassword(BSTR */*password*/)
	{
		LogTrace();
		if (Password.empty()) {
			// You can ask real password here from user
			// Password = GetPassword(OutStream);
			// PasswordIsDefined = true;
			return E_ABORT;
		}
		return S_OK;
	}
}
