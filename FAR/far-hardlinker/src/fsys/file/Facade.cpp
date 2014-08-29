#include <fsys.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>
#include <basis/std/string>

#include "../Stat.hpp"

namespace {

	struct Facade_impl: public fsys::file::Facade_i, private pattern::Uncopyable {
		~Facade_impl();

		Facade_impl(const ustring & path, const fsys::file::OpenOptions & options);

		bool is_valid() const;

		ustring path() const override;

		fsys::Stat stat() const override;

		bool size(uint64_t & size) const override;

		bool read(void * buf, size_t size, size_t & read) override;

		bool write(const void* buf, size_t size, size_t & written) override;

		bool position(uint64_t & pos) const override;

		bool set_position(int64_t dist, fsys::file::Seek base) override;

		bool set_eof() override;

		bool set_time(const uint64_t & ctime, const uint64_t & atime, const uint64_t & mtime) override;

		bool set_mtime(const uint64_t & mtime) override;

		bool set_ctime(const uint64_t & ctime) override;

		bool set_atime(const uint64_t & atime) override;

//		template<typename Type>
//		bool io_control_out_nt(DWORD code, Type & data) throw() {
//			DWORD size_ret;
//			return ::DeviceIoControl(m_hndl, code, nullptr, 0, &data, sizeof(Type), &size_ret, nullptr) != 0;
//		}

		static HANDLE Open(const ustring & path, const fsys::file::OpenOptions & options);

		static HANDLE Open(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags);

	private:
		ustring m_path;
		HANDLE  m_hndl;
	};

	Facade_impl::~Facade_impl()
	{
		if (m_hndl != INVALID_HANDLE_VALUE)
			::CloseHandle(m_hndl);
	}

	Facade_impl::Facade_impl(const ustring & path, const fsys::file::OpenOptions & options):
		m_path(path),
		m_hndl(Open(path, options))
	{
	}

	bool Facade_impl::is_valid() const
	{
		return m_hndl && m_hndl != INVALID_HANDLE_VALUE;
	}

	ustring Facade_impl::path() const
	{
		return m_path;
	}

	fsys::Stat Facade_impl::stat() const
	{
		return fsys::stat(m_hndl);
	}

	bool Facade_impl::size(uint64_t & size) const
	{
		LARGE_INTEGER fs;
//			if (::GetFileSizeEx(m_hndl, &fs)) {
//				size = fs.QuadPart;
//				return true;
//			}
//			return false;
//
		return ::GetFileSizeEx(m_hndl, &fs) ? size = fs.QuadPart, true : size = 0, false;
	}

	bool Facade_impl::read(void * buf, size_t size, size_t & read)
	{
		DWORD l_read = 0;
		return ::ReadFile(m_hndl, buf, size, &l_read, nullptr) ? read = l_read, true : read = 0, false;
	}

	bool Facade_impl::write(const void* buf, size_t size, size_t & written)
	{
		DWORD l_written = 0;
		return ::WriteFile(m_hndl, buf, size, &l_written, nullptr) ? written = l_written, true : written = 0, false;
	}

	bool Facade_impl::position(uint64_t & pos) const
	{
		LARGE_INTEGER tmp, np;
		tmp.QuadPart = 0;
		if (::SetFilePointerEx(m_hndl, tmp, &np, FILE_CURRENT)) {
			pos = static_cast<uint64_t>(tmp.QuadPart);
			return false;
		}
		return false;
	}

	bool Facade_impl::set_position(int64_t dist, fsys::file::Seek base)
	{
		LARGE_INTEGER tmp;
		tmp.QuadPart = dist;
		return ::SetFilePointerEx(m_hndl, tmp, nullptr, static_cast<DWORD>(base));
	}

	bool Facade_impl::set_eof()
	{
		return ::SetEndOfFile(m_hndl);
	}

	bool Facade_impl::set_time(const uint64_t & ctime, const uint64_t & atime, const uint64_t & mtime)
	{
		FILETIME l_ctime;
		FILETIME l_atime;
		FILETIME l_mtime;

		l_ctime.dwHighDateTime = high_part_64(ctime);
		l_ctime.dwLowDateTime = low_part_64(ctime);
		l_atime.dwHighDateTime = high_part_64(atime);
		l_atime.dwLowDateTime = low_part_64(atime);
		l_mtime.dwHighDateTime = high_part_64(mtime);
		l_mtime.dwLowDateTime = low_part_64(mtime);
		return ::SetFileTime(m_hndl, &l_ctime, &l_atime, &l_mtime);
	}

	bool Facade_impl::set_ctime(const uint64_t & ctime)
	{
		FILETIME l_time;
		l_time.dwHighDateTime = high_part_64(ctime);
		l_time.dwLowDateTime = low_part_64(ctime);
		return ::SetFileTime(m_hndl, &l_time, nullptr, nullptr);
	}

	bool Facade_impl::set_atime(const uint64_t & atime)
	{
		FILETIME l_time;
		l_time.dwHighDateTime = high_part_64(atime);
		l_time.dwLowDateTime = low_part_64(atime);
		return ::SetFileTime(m_hndl, nullptr, &l_time, nullptr);
	}

	bool Facade_impl::set_mtime(const uint64_t & mtime)
	{
		FILETIME l_time;
		l_time.dwHighDateTime = high_part_64(mtime);
		l_time.dwLowDateTime = low_part_64(mtime);
		return ::SetFileTime(m_hndl, nullptr, nullptr, &l_time);
	}

	HANDLE Facade_impl::Open(const ustring & path, const fsys::file::OpenOptions & options)
	{

		ACCESS_MASK access = (options.write) ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
		DWORD share = (options.write) ? 0 : FILE_SHARE_DELETE | FILE_SHARE_READ;
		DWORD creat = (options.write) ? OPEN_ALWAYS : OPEN_EXISTING;
		DWORD flags = (options.write) ? FILE_ATTRIBUTE_NORMAL : FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS;
		return Open(path, access, share, nullptr, creat, flags);
	}

	HANDLE Facade_impl::Open(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags)
	{
//		LogNoise(L"'%s', 0x%08X, 0x%08X, %p\n", path.c_str(), access, share, sa);
		return ::CreateFileW(path.c_str(), access, share, sa, creat, flags, nullptr);
	}

}

namespace fsys {

	namespace file {

		OpenOptions::OpenOptions():
			write(false)
		{
		}

		///=============================================================================================================
		Facade open(const ustring & path, const OpenOptions & options)
		{
			simstd::unique_ptr<Facade_impl> tmp(new Facade_impl(path, options));
			return tmp->is_valid() ? Facade(simstd::move(tmp)) : Facade();
		}

	}
}
