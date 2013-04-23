#include <libbase/std.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/memory.hpp>
#include <libext/dll.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

namespace Fsys {

	namespace File {

		Facade::~Facade()
		{
			::CloseHandle(m_hndl);
		}

		Facade::Facade(const ustring & path, bool write) :
			m_hndl(Open(m_path, write)),
			m_path(path)
		{
			Fsys::Stat::refresh(m_hndl);
		}

		Facade::Facade(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags) :
			m_hndl(Open(m_path, access, share, sa, creat, flags)),
			m_path(path)
		{
			Fsys::Stat::refresh(m_hndl);
		}

		uint64_t Facade::size() const
		{
			uint64_t ret = 0;
			CheckApi(size_nt(ret));
			return ret;
		}

		bool Facade::size_nt(uint64_t & size) const
		{
			LARGE_INTEGER fs;
			if (::GetFileSizeEx(m_hndl, &fs)) {
				size = fs.QuadPart;
				return true;
			}
			return false;
		}

		DWORD Facade::read(PVOID data, size_t size)
		{
			DWORD read;
			CheckApi(read_nt(data, size, read));
			return read;
		}

		bool Facade::read_nt(PVOID buf, size_t size, DWORD & read)
		{
			return ::ReadFile(m_hndl, buf, size, &read, nullptr);
		}

		DWORD Facade::write(PCVOID buf, size_t size)
		{
			DWORD written;
			CheckApi(write_nt(buf, size, written));
			return written;
		}

		bool Facade::write_nt(PCVOID buf, size_t size, DWORD & written)
		{
			return ::WriteFile(m_hndl, buf, size, &written, nullptr);
		}

		bool Facade::set_attr(DWORD at)
		{
			bool ret = ::SetFileAttributesW(m_path.c_str(), at);
			if (ret)
				dwFileAttributes = at;
			return ret;
		}

		uint64_t Facade::get_position() const
		{
			LARGE_INTEGER tmp, np;
			tmp.QuadPart = 0;
			CheckApi(::SetFilePointerEx(m_hndl, tmp, &np, FILE_CURRENT));
			return np.QuadPart;
		}

		void Facade::set_position(int64_t dist, DWORD method)
		{
			CheckApi(set_position_nt(dist, method));
		}

		bool Facade::set_position_nt(int64_t dist, DWORD method)
		{
			LARGE_INTEGER tmp;
			tmp.QuadPart = dist;
			return ::SetFilePointerEx(m_hndl, tmp, nullptr, method);
		}

		bool Facade::set_eof()
		{
			return ::SetEndOfFile(m_hndl);
		}

		bool Facade::set_time(const FILETIME & ctime, const FILETIME & atime, const FILETIME & mtime)
		{
			bool ret = ::SetFileTime(m_hndl, &ctime, &atime, &mtime);
			if (ret) {
				ftCreationTime = ctime;
				ftLastAccessTime = atime;
				ftLastWriteTime = mtime;
			}
			return ret;
		}

		bool Facade::set_mtime(const FILETIME & mtime)
		{
			bool ret = ::SetFileTime(m_hndl, nullptr, nullptr, &mtime);
			if (ret) {
				ftLastWriteTime = mtime;
			}
			return ret;
		}

		HANDLE Facade::Open(const ustring & path, bool write)
		{
			ACCESS_MASK access = (write) ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
			DWORD share = (write) ? 0 : FILE_SHARE_DELETE | FILE_SHARE_READ;
			DWORD creat = (write) ? OPEN_ALWAYS : OPEN_EXISTING;
			DWORD flags = (write) ? FILE_ATTRIBUTE_NORMAL : FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS;
			return Open(path, access, share, nullptr, creat, flags);
		}

		HANDLE Facade::Open(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags)
		{
			return CheckHandleErr(::CreateFileW(path.c_str(), access, share, sa, creat, flags, nullptr));
		}

	}
}
