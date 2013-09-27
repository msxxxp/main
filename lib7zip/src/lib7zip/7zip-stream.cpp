#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>
#include <libbase/logger.hpp>

namespace SevenZip {
	///============================================================================== FileReadStream
	ULONG WINAPI FileReadStream::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI FileReadStream::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI FileReadStream::QueryInterface(REFIID riid, void** object)
	{
//		LogTrace();
		if (IsEqualIID(riid, IID_IInStream) && object) {
//			LogNoise(L"IID_IInStream\n");
			*object = static_cast<IInStream*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ISequentialInStream) && object) {
//			LogNoise(L"IID_ISequentialInStream\n");
			*object = static_cast<ISequentialInStream*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	FileReadStream::~FileReadStream()
	{
		LogTrace();
	}

	FileReadStream::FileReadStream(const ustring & path) :
		Fsys::File::Facade(path, FILE_GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN)
	{
		LogNoise(L"'%s', %Iu\n", path.c_str(), size());
	}

	HRESULT WINAPI FileReadStream::Read(void * data, UInt32 size, UInt32 * processedSize)
	{
		DWORD read = 0;
		bool ret = Fsys::File::Facade::read_nt(data, size, read);
		if (processedSize)
			*processedSize = read;
		LogNoise(L"%p, %u, %u -> %d\n", data, size, read, ret);
		return Com::ConvertBoolToHRESULT(ret);
	}

	HRESULT WINAPI FileReadStream::Seek(Int64 offset, UInt32 seekOrigin, UInt64 * newPosition)
	{
		HRESULT ret = S_OK;
		try {
			uint64_t new_position;
			Fsys::File::Facade::set_position(offset, seekOrigin);
			new_position = Fsys::File::Facade::get_position();
			if (newPosition)
				*newPosition = new_position;
		} catch (Ext::AbstractError & e) {
			ret = e.code();
		} catch (...) {
			ret = E_FAIL;
		}
		LogNoise(L"%I64d, %u -> %d\n", offset, seekOrigin, ret);
		return ret;
	}

	///============================================================================= FileWriteStream
	ULONG WINAPI FileWriteStream::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI FileWriteStream::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI FileWriteStream::QueryInterface(REFIID riid, void ** object)
	{
//		LogTrace();
		if (IsEqualIID(riid, IID_IOutStream) && object) {
//			LogNoise(L"IID_IOutStream\n");
			*object = static_cast<IOutStream*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ISequentialOutStream) && object) {
//			LogNoise(L"IID_ISequentialOutStream\n");
			*object = static_cast<ISequentialOutStream*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	FileWriteStream::~FileWriteStream()
	{
		LogTrace();
	}

	FileWriteStream::FileWriteStream(const ustring & path, DWORD creat) :
		Fsys::File::Facade(path, GENERIC_READ | GENERIC_WRITE, 0, nullptr, creat, 0)
	{
		LogNoise(L"'%s', %u\n", path.c_str(), creat);
	}

	HRESULT WINAPI FileWriteStream::Write(PCVOID data, UInt32 size, UInt32 * processedSize)
	{
		DWORD written;
		bool ret = write_nt(data, size, written);
		if (processedSize)
			*processedSize = written;
		LogNoise(L"%p, %u, %u -> %d\n", data, size, written, ret);
		return Com::ConvertBoolToHRESULT(ret);
	}

	HRESULT WINAPI FileWriteStream::Seek(Int64 offset, UInt32 seekOrigin, UInt64 * newPosition)
	{
		LogNoise(L"%I64d, %u\n", offset, seekOrigin);
		HRESULT result = Com::ConvertBoolToHRESULT(set_position_nt(offset, seekOrigin));
		if (newPosition != NULL)
			*newPosition = get_position();
		return result;
	}

	HRESULT WINAPI FileWriteStream::SetSize(UInt64 newSize)
	{
		LogNoise(L"%I64u\n", newSize);
		uint64_t currentPos = get_position();
		set_position(newSize);
		HRESULT result = Com::ConvertBoolToHRESULT(set_eof());
		set_position(currentPos);
		return result;
	}
}
