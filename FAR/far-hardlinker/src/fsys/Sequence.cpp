#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/totext.hpp>

#include <fsys.hpp>

namespace fsys {

	Sequence::Sequence(const ustring& path, const ustring& mask, const Options& options) :
		m_path(path),
		m_mask(mask),
		m_options(options)
	{
		LogTraceObj();
		LogDebug(L"path:                '%s'\n", m_path.c_str());
		LogDebug(L"options.mask:        '%s'\n", m_mask.c_str());
//		LogDebug(L"options.fileMinSize: %I64u\n", options.fileMinSize);
//		LogDebug(L"options.fileMaxSize: %I64u\n", options.fileMaxSize);
//		LogDebug(L"options.flags:       0x%I64X\n", static_cast<uint64_t>(options.flags));
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
			if (!options.apply_filters(st))
				break;
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
