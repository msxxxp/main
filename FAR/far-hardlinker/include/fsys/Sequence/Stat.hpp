#ifndef _FSYS_SEQUENCE_STAT_HPP_
#define _FSYS_SEQUENCE_STAT_HPP_

namespace fsys {

	struct Sequence::FindStat {
		const wchar_t* name() const;

		uint64_t size() const;

		size_t attr() const;

		FILETIME ctime_ft() const;
		FILETIME atime_ft() const;
		FILETIME mtime_ft() const;

		int64_t ctime() const;
		int64_t atime() const;
		int64_t mtime() const;

		bool is_file() const;
		bool is_dir() const;
		bool is_link() const;

	private:
		WIN32_FIND_DATAW m_stat;
		friend struct ci_iterator;
	};

	inline const wchar_t* Sequence::FindStat::name() const
	{
		return m_stat.cFileName;
	}

	inline uint64_t Sequence::FindStat::size() const
	{
		return make_uint64(m_stat.nFileSizeHigh, m_stat.nFileSizeLow);
	}

	inline size_t Sequence::FindStat::attr() const
	{
		return m_stat.dwFileAttributes;
	}

	inline FILETIME Sequence::FindStat::ctime_ft() const
	{
		return m_stat.ftCreationTime;
	}

	inline FILETIME Sequence::FindStat::atime_ft() const
	{
		return m_stat.ftLastAccessTime;
	}

	inline FILETIME Sequence::FindStat::mtime_ft() const
	{
		return m_stat.ftLastWriteTime;
	}

	inline int64_t Sequence::FindStat::ctime() const
	{
		return static_cast<int64_t>(make_uint64(m_stat.ftCreationTime.dwHighDateTime, m_stat.ftCreationTime.dwLowDateTime));
	}

	inline int64_t Sequence::FindStat::atime() const
	{
		return static_cast<int64_t>(make_uint64(m_stat.ftLastAccessTime.dwHighDateTime, m_stat.ftLastAccessTime.dwLowDateTime));
	}

	inline int64_t Sequence::FindStat::mtime() const
	{
		return static_cast<int64_t>(make_uint64(m_stat.ftLastWriteTime.dwHighDateTime, m_stat.ftLastWriteTime.dwLowDateTime));
	}

	inline bool Sequence::FindStat::is_file() const
	{
		return !fsys::is_dir(m_stat.dwFileAttributes);
	}

	inline bool Sequence::FindStat::is_dir() const
	{
		return fsys::is_dir(m_stat.dwFileAttributes);
	}

	inline bool Sequence::FindStat::is_link() const
	{
		return fsys::is_link(m_stat.dwFileAttributes);
	}

}

#endif
