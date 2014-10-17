#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/totext.hpp>

#include <fsys.hpp>

namespace fsys {

	Sequence::Sequence(const ustring & path, const ustring & mask, const SearchOptions & options, SearchStatistics & statistics) :
		m_path(path),
		m_mask(mask),
		m_options(options),
		m_statistics(statistics)
	{
		LogTraceObj();
		LogDebug(L"path:                '%s'\n", m_path.c_str());
		LogDebug(L"options.mask:        '%s'\n", m_mask.c_str());
		LogDebug(L"options.fileMinSize: %I64u\n", options.fileMinSize);
		LogDebug(L"options.fileMaxSize: %I64u\n", options.fileMaxSize);
		LogDebug(L"options.flags:       0x%I64X\n", static_cast<uint64_t>(options.flags));
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
		flags()
	{
	}

	void Sequence::SearchOptions::set_flag(SearchFlags flag, bool value)
	{
		if (value)
			flags |= flag;
		else
			flags &= ~flag;
	}

	bool Sequence::SearchOptions::get_flag(SearchFlags flag) const
	{
		return flags & flag;
	}

	///=================================================================================================================
	Sequence::ci_iterator & Sequence::ci_iterator::operator ++()
	{
		while (true) {
			FindStat & st = m_impl->m_fstat;
			if (m_impl->m_find_handle == INVALID_HANDLE_VALUE) {
				ustring pattern = path::make(m_impl->m_sequence->path(), m_impl->m_sequence->mask());
				m_impl->m_find_handle = ::FindFirstFileW(pattern.c_str(), &st.m_stat);
				LogErrorIf(m_impl->m_find_handle == INVALID_HANDLE_VALUE, L"'%s' -> %s\n", pattern.c_str(), totext::api_error().c_str());
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

		if (options.flags & folderSkipAll) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [not recursive]:");
			++statistics.foldersIgnored;
		} else if ((options.flags & folderSkipLink) && stat.is_link()) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [link]:");
			++statistics.foldersIgnoredLink;
		} else if ((options.flags & folderSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [archive]:");
			++statistics.foldersIgnoredArchive;
		} else if ((options.flags & folderSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [read only]:");
			++statistics.foldersIgnoredReadOnly;
		} else if ((options.flags & folderSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [hidden]:");
			++statistics.foldersIgnoredHidden;
		} else if ((options.flags & folderSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [system]:");
			++statistics.foldersIgnoredSystem;
		} else if (!(options.flags & folderIncludeDots) && ((cstr::compare(stat.name(), L".") == 0 || cstr::compare(stat.name(), L"..") == 0))) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [invalid]:");
		} else {
			ret = true;
		}

		if (ret)
			LogConsoleDebug(-1, L"  folder accepted: '%s'\n", stat.name());
		else
			LogConsoleDebug(-1, L" '%s'\n", stat.name());
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
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [link]:");
			++statistics.filesIgnoredLink;
		} else if ((options.flags & fileSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
			++statistics.filesIgnoredArchive;
		} else if ((options.flags & fileSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
			++statistics.filesIgnoredReadOnly;
		} else if ((options.flags & fileSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [hidden]:");
			++statistics.filesIgnoredHidden;
		} else if ((options.flags & fileSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [system]:");
			++statistics.filesIgnoredSystem;
//		} else if ((options.flags & fileSkipStreamed) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"file ignored [streamed]:");
//			++statistics.filesIgnoredStreamed;
		} else if ((options.flags & fileSkipCompressed) && (stat.attr() & FILE_ATTRIBUTE_COMPRESSED)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [compressed]:");
			++statistics.filesIgnoredCompressed;
		} else if ((options.flags & fileSkipEncrypted) && (stat.attr() & FILE_ATTRIBUTE_ENCRYPTED)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [encrypted]:");
			++statistics.filesIgnoredEncrypted;
		} else if ((options.flags & fileSkipSparse) && (stat.attr() & FILE_ATTRIBUTE_SPARSE_FILE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [sparse]:");
			++statistics.filesIgnoredSparse;
		} else if ((options.flags & fileSkipTemporary) && (stat.attr() & FILE_ATTRIBUTE_TEMPORARY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [temporary]:");
			++statistics.filesIgnoredTemporary;
		} else if ((options.flags & fileSkipOffline) && (stat.attr() & FILE_ATTRIBUTE_OFFLINE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [offline]:");
			++statistics.filesIgnoredOffline;
		} else if ((options.flags & fileSkipZeroSize) && stat.size() == 0LL) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [zero]: ");
			++statistics.filesIgnoredZeroSize;
		} else if (stat.size() < options.fileMinSize) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to small]:");
			++statistics.filesIgnoredMinSize;
		} else if (stat.size() > options.fileMaxSize) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to big]:");
			++statistics.filesIgnoredMaxSize;
		} else {
			ret = true;
		}

		if (ret)
			LogConsoleDebug(-1, L"  file accepted: '%s'\n", stat.name());
		else
			LogConsoleDebug(-1, L" '%s'\n", stat.name());
		return ret;
	}

	///=================================================================================================================
	Sequence::ci_iterator::impl::~impl() noexcept
	{
		LogTraceObj();
		if (m_find_handle && m_find_handle != INVALID_HANDLE_VALUE ) {
			::FindClose(m_find_handle);
		}
	}

	Sequence::ci_iterator::impl::impl() noexcept :
		m_sequence(nullptr),
		m_find_handle(nullptr)
	{
		LogTraceObj();
	}

	Sequence::ci_iterator::impl::impl(const Sequence & seq) noexcept :
		m_sequence(&seq),
		m_find_handle(INVALID_HANDLE_VALUE)
	{
		LogTraceObj();
	}


}
