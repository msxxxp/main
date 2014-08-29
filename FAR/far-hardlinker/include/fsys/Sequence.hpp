#ifndef _FSYS_SEQUENCE_HPP_
#define _FSYS_SEQUENCE_HPP_

#include <fsys.hpp>

#include <basis/sys/fsys.hpp>

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
			incDots            = 0x00001,

			folderSkipAll      = 0x00010,
			folderSkipReadOnly = 0x00020,
			folderSkipHidden   = 0x00040,
			folderSkipSystem   = 0x00080,
			folderSkipLink     = 0x00100,

			fileSkipAll        = 0x01000,
			fileSkipReadOnly   = 0x02000,
			fileSkipHidden     = 0x04000,
			fileSkipSystem     = 0x08000,
			fileSkipLink       = 0x10000,
			fileSkipZeroSize = 0x20000,
		};

		typedef FindStat value_type;
		typedef size_t size_type;
		typedef size_t flags_type;
		typedef ci_iterator iterator;
		typedef ci_iterator const_iterator;

		Sequence(const simstd::wstring & path, const SearchOptions & options, SearchStatistics & statistics);

		const_iterator begin() const;

		const_iterator end() const;

		bool empty() const;

		simstd::wstring path() const;

		const SearchOptions & options() const;

		const SearchStatistics & statistics() const;

	private:
		simstd::wstring       m_path;
		const SearchOptions & m_options;
		SearchStatistics    & m_statistics;
	};

	inline simstd::wstring Sequence::path() const
	{
		return m_path;
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
		uint64_t        fileMinSize;
		uint64_t        fileMaxSize;
		simstd::wstring mask;
		flags_type      flags;

		SearchOptions();
	};

	///=================================================================================================================
	struct Sequence::SearchStatistics {
		std::atomic<uint64_t> filesFound;
		std::atomic<uint64_t> filesFoundSize;
		std::atomic<uint64_t> filesLinksFound;
		std::atomic<uint64_t> filesLinksFoundSize;
		std::atomic<uint64_t> filesIgnoredReadOnly;
		std::atomic<uint64_t> filesIgnoredHidden;
		std::atomic<uint64_t> filesIgnoredSystem;
		std::atomic<uint64_t> filesIgnoredLink;
		std::atomic<uint64_t> filesIgnoredZeroSize;
		std::atomic<uint64_t> filesIgnoredMinSize;
		std::atomic<uint64_t> filesIgnoredMaxSize;
		std::atomic<uint64_t> foldersFound;
		std::atomic<uint64_t> foldersLinksFound;
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

		bool is_filtered_folder(const FindStat & stat, const SearchOptions & options, SearchStatistics & statistics);

		bool is_filtered_file(const FindStat & stat, const SearchOptions & options, SearchStatistics & statistics);

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

	inline Sequence::ci_iterator::impl::impl() noexcept :
		m_sequence(nullptr),
		m_find_handle(nullptr)
	{
	}

	inline Sequence::ci_iterator::impl::impl(const Sequence & seq) noexcept :
		m_sequence(&seq),
		m_find_handle(INVALID_HANDLE_VALUE)
	{
	}
}

#endif
