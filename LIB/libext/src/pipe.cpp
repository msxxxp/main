#include <basis/sys/logger.hpp>
#include <libext/pipe.hpp>
#include <libext/exception.hpp>

namespace Ext {
	AnonPipe::~AnonPipe()
	{
		close_write();
		close_read();
	}

	AnonPipe::AnonPipe(size_t size)
	{
		LogNoise(L"%Iu\n", size);
		CheckApi(::CreatePipe(&m_rpipe, &m_wpipe, nullptr, size));
	}

	bool AnonPipe::read_nt(void * buffer, size_t size, DWORD & rd) noexcept
	{
		bool ret = ::ReadFile(m_rpipe, buffer, size, &rd, nullptr);
		LogNoise(L"%p, %Iu, %u -> %d\n", buffer, size, rd, ret);
		return ret;
	}

	void AnonPipe::read(void * buffer, size_t bufSize, DWORD & rd)
	{
		CheckApi(read_nt(buffer, bufSize, rd));
	}

	bool AnonPipe::write_nt(const void * buffer, size_t size, DWORD & written) noexcept
	{
		bool ret = ::WriteFile(m_wpipe, buffer, size, &written, nullptr);
		LogNoise(L"%p, %Iu, %u -> %d\n", buffer, size, written, ret);
		return ret;
	}

	void AnonPipe::write(const void * buffer, size_t bufSize, DWORD & written)
	{
		CheckApi(write_nt(buffer, bufSize, written));
	}

	void AnonPipe::close_write() noexcept
	{
		::CloseHandle(m_wpipe);
		m_wpipe = INVALID_HANDLE_VALUE;
	}

	void AnonPipe::close_read() noexcept
	{
		::CloseHandle(m_rpipe);
		m_rpipe = INVALID_HANDLE_VALUE;
	}

	Pipe::~Pipe()
	{
		::CloseHandle(m_pipe);
	}

	Pipe::Pipe(PCWSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInstances, LPSECURITY_ATTRIBUTES securityAttributes)
	{
		m_pipe = CheckHandle(::CreateNamedPipeW(name, openMode, pipeMode, maxInstances, 0, 0, 0, securityAttributes));
	}

	Pipe::Pipe(PCWSTR name, size_t /*timeout*/)
	{
		m_pipe = CheckHandle(::CreateFileW(name,
		                                   GENERIC_READ | GENERIC_WRITE,
		                                   0,              // no sharing
		                                   NULL,           // default security attributes
		                                   OPEN_EXISTING,  // opens existing pipe
		                                   SECURITY_SQOS_PRESENT | SECURITY_IMPERSONATION,              // default attributes
		                                   NULL));          // no template file

		//		if (m_pipe != INVALID_HANDLE_VALUE)
		//			break;
		//
		//		// Exit if an error other than ERROR_PIPE_BUSY occurs.
		//
		//		if (GetLastError() != ERROR_PIPE_BUSY)
		//		{
		//			_tprintf( TEXT("Could not open pipe. GLE=%d\n"), GetLastError() );
		//			return -1;
		//		}
		//
		//		// All pipe instances are busy, so wait for 20 seconds.
		//
		//		if ( ! WaitNamedPipe(lpszPipename, 20000))
		//		{
		//			printf("Could not open pipe: 20 second wait timed out.");
		//			return -1;
		//		}
	}

	void Pipe::connect()
	{
		CheckApi(::ConnectNamedPipe(m_pipe, nullptr));
	}

	void Pipe::disconnect()
	{
		::DisconnectNamedPipe(m_pipe);
	}

	Pipe::operator HANDLE() const
	{
		return m_pipe;
	}

	void Pipe::read(void * buffer, size_t bufSize, DWORD & read)
	{
		//		DWORD read_ = 0;
		CheckApi(::ReadFile(m_pipe, buffer, bufSize, &read, nullptr));
		//		read = read_;
	}

	void Pipe::write(const void * buffer, size_t bufSize, DWORD & written)
	{
		CheckApi(::WriteFile(m_pipe, buffer, bufSize, &written, nullptr));
	}
}
