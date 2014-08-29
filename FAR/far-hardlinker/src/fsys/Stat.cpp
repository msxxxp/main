#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/memory.hpp>

#include <fsys.hpp>
#include "Stat.hpp"

namespace {

	struct Stat_impl: public fsys::Stat_i {
//		static HANDLE open(const char * path);
		static HANDLE open(const wchar_t * path);

		Stat_impl(HANDLE hndl);

		bool is_valid() const;

		size_t attr() const override;
		size_t volume_sn() const override;
		size_t num_links() const override;


		uint64_t size() const override;
		uint64_t inode() const override;
		uint64_t ctime() const override;
		uint64_t atime() const override;
		uint64_t mtime() const override;

		bool is_dir() const override;
		bool is_file() const override;
		bool is_lnk() const override;

	private:
		BY_HANDLE_FILE_INFORMATION m_info;
	};

//	HANDLE Stat_impl::open(const char * path)
//	{
//	}

	HANDLE Stat_impl::open(const wchar_t * path)
	{
//		Ext::Privilege priv(SE_BACKUP_NAME);

		return ::CreateFileW(path, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ, nullptr, OPEN_EXISTING,
//		                         FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
		                     	 0,
		                     	 nullptr);
	}

	Stat_impl::Stat_impl(HANDLE hndl)
	{
		memory::zero(m_info);
		::GetFileInformationByHandle(hndl, &m_info);
	}

	bool Stat_impl::is_valid() const
	{
		return !(m_info.nFileIndexHigh == 0 && m_info.nFileIndexLow == 0 && m_info.dwVolumeSerialNumber == 0);
	}

	size_t Stat_impl::attr() const
	{
		return m_info.dwFileAttributes;
	}

	size_t Stat_impl::volume_sn() const
	{
		return m_info.dwVolumeSerialNumber;
	}

	size_t Stat_impl::num_links() const
	{
		return m_info.nNumberOfLinks;
	}

	uint64_t Stat_impl::size() const
	{
		return make_uint64(m_info.nFileSizeHigh, m_info.nFileSizeLow);
	}

	uint64_t Stat_impl::inode() const
	{
		return make_uint64(m_info.nFileIndexHigh, m_info.nFileIndexLow);// & 0x0000FFFFFFFFFFFFULL;
	}

	uint64_t Stat_impl::ctime() const
	{
		return make_uint64(m_info.ftCreationTime.dwHighDateTime, m_info.ftCreationTime.dwLowDateTime);
	}

	uint64_t Stat_impl::atime() const
	{
		return make_uint64(m_info.ftLastAccessTime.dwHighDateTime, m_info.ftLastAccessTime.dwLowDateTime);
	}

	uint64_t Stat_impl::mtime() const
	{
		return make_uint64(m_info.ftLastWriteTime.dwHighDateTime, m_info.ftLastWriteTime.dwLowDateTime);
	}

	bool Stat_impl::is_dir() const
	{
		return fsys::is_dir(m_info.dwFileAttributes);
	}

	bool Stat_impl::is_file() const
	{
		return fsys::is_file(m_info.dwFileAttributes);
	}

	bool Stat_impl::is_lnk() const
	{
		return fsys::is_link(m_info.dwFileAttributes);
	}

}

namespace fsys {

	Stat stat(HANDLE hndl)
	{
		if (hndl != INVALID_HANDLE_VALUE) {
			simstd::unique_ptr<Stat_impl> tmp(new Stat_impl(hndl));
			::CloseHandle(hndl);

			if (tmp->is_valid())
				return Stat(simstd::move(tmp));
		}
		return Stat();
	}

//	Stat stat(const char * path)
//	{
//		return get_stat(Stat_impl::open(path));
//	}

	Stat stat(const wchar_t * path)
	{
		return stat(Stat_impl::open(path));
	}

}
