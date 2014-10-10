#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>

#include <fsys.hpp>

namespace fsys {

	Sequence::Sequence(const ustring & path, const SearchOptions & options, SearchStatistics & statistics) :
		m_path(path),
		m_options(options),
		m_statistics(statistics)
	{
	}

	Sequence::const_iterator Sequence::begin() const
	{
		return const_iterator(*this);
	}

	Sequence::const_iterator Sequence::end() const
	{
		return const_iterator();
	}

	bool Sequence::empty() const
	{
		return begin() == end();
	}

	const Sequence::SearchOptions & Sequence::options() const
	{
		return m_options;
	}

	const Sequence::SearchStatistics & Sequence::statistics() const
	{
		return m_statistics;
	}

	///=================================================================================================================
	Sequence::SearchOptions::SearchOptions():
		fileMinSize(0),
		fileMaxSize(UINT64_MAX),
		mask(L"*"),
		flags()
	{
	}

	///=================================================================================================================
	Sequence::ci_iterator & Sequence::ci_iterator::operator ++()
	{
		const SearchOptions & opt = m_impl->m_sequence->m_options;
		while (true) {
			FindStat & st = m_impl->m_fstat;
			if (m_impl->m_find_handle == INVALID_HANDLE_VALUE) {
				ustring pattern = path::make(m_impl->m_sequence->path(), opt.mask);
				m_impl->m_find_handle = ::FindFirstFileW(pattern.c_str(), &st.m_stat);
				if (m_impl->m_find_handle == INVALID_HANDLE_VALUE) {
					m_impl.reset(new impl);
					break;
				}
			} else {
				if (!::FindNextFileW(m_impl->m_find_handle, &st.m_stat)) {
					m_impl.reset(new impl);
					break;
				}
			}

			if (st.is_dir()) {
				if (is_filtered_folder(st))
					break;
			} else {
				if (is_filtered_file(st))
					break;
			}

		}
		return *this;
	}

	Sequence::ci_iterator Sequence::ci_iterator::operator ++(int)
	{
		this_type ret(*this);
		operator ++();
		return ret;
	}

	const Sequence::value_type & Sequence::ci_iterator::operator *() const
	{
		return m_impl->m_fstat;
	}

	const Sequence::value_type * Sequence::ci_iterator::operator ->() const
	{
		return &m_impl->m_fstat;
	}

	bool Sequence::ci_iterator::operator ==(const this_type & rhs) const
	{
		return m_impl->m_find_handle == rhs.m_impl->m_find_handle;
	}

	bool Sequence::ci_iterator::operator !=(const this_type & rhs) const
	{
		return m_impl->m_find_handle != rhs.m_impl->m_find_handle;
	}

	Sequence::ci_iterator::ci_iterator() :
		m_impl(new impl)
	{
	}

	Sequence::ci_iterator::ci_iterator(const Sequence & seq) :
		m_impl(new impl(seq))
	{
		operator++();
	}

	bool Sequence::ci_iterator::is_filtered_folder(const FindStat & stat)
	{
		bool ret = false;

		auto & options = m_impl->m_sequence->m_options;
//		const SearchOptions & options = m_impl->m_sequence->m_options;
		auto & statistics = m_impl->m_sequence->m_statistics;
//		SearchStatistics & statistics = m_impl->m_sequence->m_statistics;

		if (stat.is_link()) {
			++statistics.foldersLinksFound;
		} else {
			++statistics.foldersFound;
		}

		if ((options.flags & folderSkipLink) && stat.is_link()) {
			++statistics.foldersIgnoredLink;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [link]: ");
		} else if ((options.flags & folderSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			++statistics.foldersIgnoredReadOnly;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [read only]: ");
		} else if ((options.flags & folderSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			++statistics.foldersIgnoredHidden;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [hidden]: ");
		} else if ((options.flags & folderSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			++statistics.foldersIgnoredSystem;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"Dir  ignored [system]: ");
		} else {
			ret = (options.flags & incDots) ? true : (cstr::compare(stat.name(), L".") != 0 && cstr::compare(stat.name(), L"..") != 0);
		}

		LogConsoleDebug(-1, L"found folder: '%s' -> %d\n", stat.name(), ret);
		return ret;
	}

	bool Sequence::ci_iterator::is_filtered_file(const FindStat & stat)
	{
		bool ret = false;

		const SearchOptions & options = m_impl->m_sequence->m_options;
		SearchStatistics & statistics = m_impl->m_sequence->m_statistics;

		if (stat.is_link()) {
			++statistics.filesLinksFound;
			statistics.filesLinksFoundSize += stat.size();
		} else {
			++statistics.filesFound;
			statistics.filesFoundSize += stat.size();
		}

		if ((options.flags & fileSkipLink) && stat.is_link()) {
			++statistics.filesIgnoredLink;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [link]: ");
		} else if ((options.flags & fileSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			++statistics.filesIgnoredHidden;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [read only]: ");
		} else if ((options.flags & fileSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			++statistics.filesIgnoredHidden;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [hidden]: ");
		} else if ((options.flags & fileSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			++statistics.filesIgnoredSystem;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [system]: ");
		} else if ((options.flags & fileSkipZeroSize) && stat.size() == 0LL) {
			++statistics.filesIgnoredZeroSize;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [zero]: ");
		} else if (stat.size() < options.fileMinSize) {
			++statistics.filesIgnoredMinSize;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [small]: ");
		} else if (stat.size() > options.fileMaxSize) {
			++statistics.filesIgnoredMaxSize;
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"File ignored [big]: ");
		} else {
			ret = true;
		}

		LogConsoleDebug(-1, L"found file: '%s' -> %d\n", stat.name(), ret);
		return ret;
	}

	///=================================================================================================================
	Sequence::ci_iterator::impl::~impl() noexcept
	{
		if (m_find_handle && m_find_handle != INVALID_HANDLE_VALUE ) {
			::FindClose(m_find_handle);
		}
	}

}
