#ifndef LIBEXT_PIPE_HPP_
#define LIBEXT_PIPE_HPP_

#include <basis/configure.hpp>

namespace Ext {

	struct AnonPipe {
		~AnonPipe();

		AnonPipe(size_t size = 0);

		bool read_nt(void * buffer, size_t size, DWORD & rd) noexcept;

		void read(void * buffer, size_t bufSize, DWORD & rd);

		bool write_nt(const void * buffer, size_t size, DWORD & written) noexcept;

		void write(const void * buffer, size_t bufSize, DWORD & written);

		void close_write() noexcept;

		void close_read() noexcept;

	private:
		HANDLE m_rpipe;
		HANDLE m_wpipe;
	};

	struct Pipe {
		~Pipe();

		Pipe(PCWSTR name, DWORD openMode, DWORD pipeMode, DWORD maxInstances, LPSECURITY_ATTRIBUTES securityAttributes = nullptr);

		Pipe(PCWSTR name, size_t /*timeout*/);

		void connect();

		void disconnect();

		operator HANDLE() const;

		void read(void * buffer, size_t bufSize, DWORD & read);

		void write(const void * buffer, size_t bufSize, DWORD & written);

	private:
		HANDLE m_pipe;
	};

}

#endif
