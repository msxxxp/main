#ifndef _FSYS_SEQUENCE_HPP_
#define _FSYS_SEQUENCE_HPP_

#include <fsys.hpp>

#include <basis/sys/fsys.hpp>
#include <basis/simstd/string>

#include <atomic>

namespace fsys {

	///=================================================================================================================
	class Sequence: private pattern::Uncopyable {
		typedef Sequence this_type;
		class ci_iterator;

	public:
		struct FindStat;
		struct SearchOptions;
		struct SearchStatistics;

		enum SearchFlags {
			folderIncludeDots  = 0x0000001,

			folderSkipAll      = 0x0000010,
			folderSkipArchive  = 0x0000020,
			folderSkipReadOnly = 0x0000040,
			folderSkipHidden   = 0x0000080,
			folderSkipSystem   = 0x0000100,
			folderSkipLink     = 0x0000200,

			fileSkipAll        = 0x0001000,
			fileSkipArchive    = 0x0002000,
			fileSkipReadOnly   = 0x0004000,
			fileSkipHidden     = 0x0008000,
			fileSkipSystem     = 0x0010000,
			fileSkipLink       = 0x0020000,
			fileSkipStreamed   = 0x0040000,
			fileSkipCompressed = 0x0080000,
			fileSkipEncrypted  = 0x0100000,
			fileSkipSparse     = 0x0200000,
			fileSkipTemporary  = 0x0400000,
			fileSkipOffline    = 0x0800000,
			fileSkipZeroSize   = 0x1000000,
		};

		typedef FindStat    value_type;
		typedef size_t      size_type;
		typedef uint64_t    flags_type;
		typedef ci_iterator iterator;
		typedef ci_iterator const_iterator;

		Sequence(const ustring & path, const ustring & mask, const SearchOptions & options, SearchStatistics & statistics);

		const_iterator begin() const;

		const_iterator end() const;

		bool empty() const;

		const ustring& path() const;

		const ustring& mask() const;

		const SearchOptions & options() const;

		const SearchStatistics & statistics() const;

	private:
		ustring               m_path;
		ustring               m_mask;
		const SearchOptions & m_options;
		SearchStatistics    & m_statistics;
	};

	inline const ustring& Sequence::path() const
	{
		return m_path;
	}

	inline const ustring& Sequence::mask() const
	{
		return m_mask;
	}

	///=================================================================================================================
	struct Sequence::FindStat {
		const wchar_t * name() const;

		uint64_t size() const;

		size_t attr() const;

		FILETIME ctime_ft() const;
		FILETIME atime_ft() const;
		FILETIME mtime_ft() const;

		uint64_t cmtime() const;
		uint64_t atime() const;
		uint64_t mtime() const;

		bool is_file() const;
		bool is_dir() const;
		bool is_link() const;

	private:
		WIN32_FIND_DATAW m_stat;
		friend struct ci_iterator;
	};

	inline const wchar_t * Sequence::FindStat::name() const
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

	inline uint64_t Sequence::FindStat::mtime() const
	{
		return make_uint64(m_stat.ftLastWriteTime.dwHighDateTime, m_stat.ftLastWriteTime.dwLowDateTime);
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

	///=================================================================================================================
	struct Sequence::SearchOptions {
		uint64_t   fileMinSize;
		uint64_t   fileMaxSize;
		flags_type flags;

		SearchOptions();

		void set_flag(SearchFlags flag, bool value);
		bool get_flag(SearchFlags flag) const;
	};

	///=================================================================================================================
	struct Sequence::SearchStatistics {
		std::atomic<uint64_t> filesFound;
		std::atomic<uint64_t> filesFoundSize;
		std::atomic<uint64_t> filesLinksFound;
		std::atomic<uint64_t> filesLinksFoundSize;
		std::atomic<uint64_t> filesIgnoredMinSize;
		std::atomic<uint64_t> filesIgnoredMaxSize;
		std::atomic<uint64_t> filesIgnoredArchive;
		std::atomic<uint64_t> filesIgnoredReadOnly;
		std::atomic<uint64_t> filesIgnoredHidden;
		std::atomic<uint64_t> filesIgnoredSystem;
		std::atomic<uint64_t> filesIgnoredLink;
		std::atomic<uint64_t> filesIgnoredStreamed;
		std::atomic<uint64_t> filesIgnoredCompressed;
		std::atomic<uint64_t> filesIgnoredEncrypted;
		std::atomic<uint64_t> filesIgnoredSparse;
		std::atomic<uint64_t> filesIgnoredTemporary;
		std::atomic<uint64_t> filesIgnoredOffline;
		std::atomic<uint64_t> filesIgnoredZeroSize;
		std::atomic<uint64_t> foldersFound;
		std::atomic<uint64_t> foldersLinksFound;
		std::atomic<uint64_t> foldersIgnored;
		std::atomic<uint64_t> foldersIgnoredArchive;
		std::atomic<uint64_t> foldersIgnoredReadOnly;
		std::atomic<uint64_t> foldersIgnoredHidden;
		std::atomic<uint64_t> foldersIgnoredSystem;
		std::atomic<uint64_t> foldersIgnoredLink;
	};

	///=================================================================================================================
	class Sequence::ci_iterator {
		typedef ci_iterator this_type;

	public:
		this_type & operator ++();

		this_type operator ++(int);

		const value_type & operator *() const;

		const value_type * operator ->() const;

		bool operator ==(const this_type & rhs) const;

		bool operator !=(const this_type & rhs) const;

	private:
		ci_iterator();

		ci_iterator(const Sequence & seq);

		bool is_filtered_folder(const FindStat & stat);

		bool is_filtered_file(const FindStat & stat);

		struct impl;
		simstd::shared_ptr<impl> m_impl;

		friend class Sequence;
	};

	///=================================================================================================================
	struct Sequence::ci_iterator::impl {
		~impl() noexcept;
		impl() noexcept;
		impl(const Sequence & seq) noexcept;

		const Sequence * m_sequence;
		HANDLE           m_find_handle;
		FindStat         m_fstat;
	};

}

#endif
