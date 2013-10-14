#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>
#include <libext/pipe.hpp>
#include <liblog/logger.hpp>

namespace SevenZip {
	namespace {
		HRESULT SeekWrapper(Fsys::File::Facade & file, Int64 offset, UInt32 seekOrigin, UInt64 * newPosition)
		try {
			HRESULT result = Com::ConvertBoolToHRESULT(file.set_position_nt(offset, seekOrigin));
			if (newPosition != NULL)
				*newPosition = file.get_position();
			LogNoise(L"%I64d, %u, %I64u\n", offset, seekOrigin, newPosition ? *newPosition : 0ULL);
			return result;
		} catch (Ext::AbstractError & e) {
			LogError(L"%I64d, %u, %I64u -> '%s'\n", offset, seekOrigin, e.what().c_str());
			return Com::ConvertErrorToHRESULT(e.code());
		}
	}

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
		return SeekWrapper(*this, offset, seekOrigin, newPosition);
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

	HRESULT WINAPI FileWriteStream::Write(const void * data, UInt32 size, UInt32 * processedSize)
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
		return SeekWrapper(*this, offset, seekOrigin, newPosition);
	}

	HRESULT WINAPI FileWriteStream::SetSize(UInt64 newSize)
	try {
		LogNoise(L"%I64u\n", newSize);
		uint64_t currentPos = get_position();
		set_position(newSize);
		set_eof();
		set_position(currentPos);
		return S_OK;
	} catch (Ext::AbstractError & e) {
		LogError(L"%I64u -> '%s'\n", newSize, e.what().c_str());
		return Com::ConvertErrorToHRESULT(e.code());
	}

	///====================================================================== VirtualReadWriteStream
	struct VirtualReadWriteStream::impl {
		impl(size_t size = 0):
			m_pipe(size)
		{
		}

		HRESULT read(void * data, UInt32 size, UInt32 * processedSize)
		{
			DWORD rd = 0;
			bool ret = m_pipe.read_nt(data, size, rd) || ::GetLastError() == ERROR_BROKEN_PIPE;

			if (processedSize)
				*processedSize = rd;

			return Com::ConvertBoolToHRESULT(ret);
		}

		HRESULT write(const void * data, UInt32 size, UInt32 * processedSize)
		{
			DWORD written = 0;
			bool ret = m_pipe.write_nt(data, size, written);

			if (processedSize)
				*processedSize = written;

			return Com::ConvertBoolToHRESULT(ret);
		}

		void close_write()
		{
			m_pipe.close_write();
		}

	private:
		Ext::AnonPipe m_pipe;
	};

	ULONG WINAPI VirtualReadWriteStream::AddRef()
	{
		return UnknownImp::AddRef();
	}

	ULONG WINAPI VirtualReadWriteStream::Release()
	{
		return UnknownImp::Release();
	}

	HRESULT WINAPI VirtualReadWriteStream::QueryInterface(REFIID riid, void ** object)
	{
		if (IsEqualIID(riid, IID_ISequentialOutStream) && object) {
			*object = static_cast<ISequentialOutStream*>(this);
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_ISequentialInStream) && object) {
			*object = static_cast<ISequentialInStream*>(this);
			AddRef();
			return S_OK;
		}
		return UnknownImp::QueryInterface(riid, object);
	}

	VirtualReadWriteStream::~VirtualReadWriteStream()
	{
		LogTrace();
	}

	VirtualReadWriteStream::VirtualReadWriteStream():
		m_impl(new impl)
	{
		LogTrace();
	}

	HRESULT WINAPI VirtualReadWriteStream::Write(const void * data, UInt32 size, UInt32 * processedSize)
	{
		LogTrace();
		return m_impl->write(data, size, processedSize);
	}

	HRESULT WINAPI VirtualReadWriteStream::Read(void * data, UInt32 size, UInt32 * processedSize)
	{
		LogTrace();
		return m_impl->read(data, size, processedSize);
	}

	void VirtualReadWriteStream::close_write()
	{
		LogTrace();
		m_impl->close_write();
	}

}
