#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/totext.hpp>

#include <fsys.hpp>

namespace fsys {

	Sequence::Sequence(const ustring & path, const ustring & mask, const Options & options) :
		m_path(path),
		m_mask(mask),
		m_options(options)
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

	const Sequence::Options& Sequence::options() const
	{
		return m_options;
	}

	///=================================================================================================================
	Sequence::Options::Options():
		fileMinSize(0),
		fileMaxSize(UINT64_MAX),
		flags()
	{
	}

	void Sequence::Options::set_flag(SearchFlags flag, bool value)
	{
		if (value)
			flags |= flag;
		else
			flags &= ~flag;
	}

	bool Sequence::Options::get_flag(SearchFlags flag) const
	{
		return flags & flag;
	}

	bool Sequence::Options::is_filtered_folder(const FindStat & stat) const
	{
		bool ret = false;

		statistics.folder_found(stat);

		if (flags & folderSkipAll) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [not recursive]:");
			statistics.folder_ignored();
		} else if ((flags & folderSkipLink) && stat.is_link()) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [link]:");
			statistics.folder_ignored_link();
		} else if ((flags & folderSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [archive]:");
			statistics.folder_ignored_archive();
		} else if ((flags & folderSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [read only]:");
			statistics.folder_ignored_readonly();
		} else if ((flags & folderSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [hidden]:");
			statistics.folder_ignored_hidden();
		} else if ((flags & folderSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  folder ignored [system]:");
			statistics.folder_ignored_system();
		} else if (!(flags & folderIncludeDots) && ((cstr::compare(stat.name(), L".") == 0 || cstr::compare(stat.name(), L"..") == 0))) {
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

	bool Sequence::Options::is_filtered_file(const FindStat & stat) const
	{
		bool ret = false;

		statistics.file_found(stat);

		if ((flags & fileSkipLink) && stat.is_link()) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [link]:");
			statistics.file_ignored_link();
		} else if ((flags & fileSkipArchive) && (stat.attr() & FILE_ATTRIBUTE_ARCHIVE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
			statistics.file_ignored_archive();
		} else if ((flags & fileSkipReadOnly) && (stat.attr() & FILE_ATTRIBUTE_READONLY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [read only]:");
			statistics.file_ignored_readonly();
		} else if ((flags & fileSkipHidden) && (stat.attr() & FILE_ATTRIBUTE_HIDDEN)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [hidden]:");
			statistics.file_ignored_hidden();
		} else if ((flags & fileSkipSystem) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [system]:");
			statistics.file_ignored_system();
//		} else if ((flags & fileSkipStreamed) && (stat.attr() & FILE_ATTRIBUTE_SYSTEM)) {
//			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"file ignored [streamed]:");
//			statistics.file_ignored_streamed();
		} else if ((flags & fileSkipCompressed) && (stat.attr() & FILE_ATTRIBUTE_COMPRESSED)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [compressed]:");
			statistics.file_ignored_compressed();
		} else if ((flags & fileSkipEncrypted) && (stat.attr() & FILE_ATTRIBUTE_ENCRYPTED)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [encrypted]:");
			statistics.file_ignored_encrypted();
		} else if ((flags & fileSkipSparse) && (stat.attr() & FILE_ATTRIBUTE_SPARSE_FILE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [sparse]:");
			statistics.file_ignored_sparse();
		} else if ((flags & fileSkipTemporary) && (stat.attr() & FILE_ATTRIBUTE_TEMPORARY)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [temporary]:");
			statistics.file_ignored_temporary();
		} else if ((flags & fileSkipOffline) && (stat.attr() & FILE_ATTRIBUTE_OFFLINE)) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [offline]:");
			statistics.file_ignored_offline();
		} else if ((flags & fileSkipZeroSize) && stat.size() == 0LL) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [zero]: ");
			statistics.file_ignored_zero();
		} else if (stat.size() < fileMinSize) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to small]:");
			statistics.file_ignored_to_small();
		} else if (stat.size() > fileMaxSize) {
			LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  file ignored [to big]:");
			statistics.file_ignored_to_big();
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

			const Options& options = m_impl->m_sequence->options();
			if (st.is_dir()) {
				if (options.is_filtered_folder(st))
					break;
			} else {
				if (options.is_filtered_file(st))
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
