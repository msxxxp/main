#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libext/dll.hpp>
#include <libext/file.hpp>
#include <libext/exception.hpp>

namespace Ext {
	namespace FS {

		Sequence::Sequence(const ustring & path, flags_type flags) :
			m_path(path), m_mask(L"*"), m_flags(flags)
		{
		}

		Sequence::Sequence(const ustring & path, const ustring & mask, flags_type flags) :
			m_path(path), m_mask(mask), m_flags(flags)
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

		Sequence::const_input_iterator & Sequence::const_input_iterator::operator ++()
		{
			flags_type flags = m_impl->m_seq->flags();

			while (true) {
				WIN32_FIND_DATAW& st = m_impl->m_stat;
				if (m_impl->m_handle == INVALID_HANDLE_VALUE ) {
					ustring path = Base::MakePath(m_impl->m_seq->path(), m_impl->m_seq->mask());
					m_impl->m_handle = ::FindFirstFileW(path.c_str(), &m_impl->m_stat);
					if (m_impl->m_handle == INVALID_HANDLE_VALUE )
						throw "Cant";
				} else {
					if (!::FindNextFileW(m_impl->m_handle, &st)) {
						::FindClose(m_impl->m_handle);
						m_impl->m_handle = NULL; // end()
						break;
					}
				}

				const ustring& name = this->name();
				if (!(flags & incDots) && (name == L"." || name == L"..")) {
					continue;
				}

				if ((flags & skipHidden) && (st.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
					continue;
				}

				if ((flags & skipDirs) && (st.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					continue;
				}

				if ((flags & skipLinks) && (st.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) {
					continue;
				}

				if ((flags & skipFiles) && !((st.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (st.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT))) {
					continue;
				}

				break;
			}
			return *this;
		}

		Sequence::const_input_iterator Sequence::const_input_iterator::operator ++(int)
		{
			this_type ret(*this);
			operator ++();
			return ret;
		}

		const Sequence::value_type Sequence::const_input_iterator::operator *() const
		{
			return Sequence::value_type(m_impl->m_seq->path().c_str());
		}

		PCWSTR Sequence::const_input_iterator::name() const
		{
			return m_impl->m_stat.cFileName;
		}

		ustring Sequence::const_input_iterator::path() const
		{
			return Base::MakePath(m_impl->m_seq->path(), m_impl->m_stat.cFileName);
		}

		uint64_t Sequence::const_input_iterator::size() const
		{
			return Base::make_uint64(m_impl->m_stat.nFileSizeHigh, m_impl->m_stat.nFileSizeLow);
		}

		size_t Sequence::const_input_iterator::attr() const
		{
			return m_impl->m_stat.dwFileAttributes;
		}

		bool Sequence::const_input_iterator::is_file() const
		{
			return !(m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && !(m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		bool Sequence::const_input_iterator::is_dir() const
		{
			return !(m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && (m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		bool Sequence::const_input_iterator::is_link() const
		{
			return m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT;
		}

		bool Sequence::const_input_iterator::is_link_file() const
		{
			return (m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && !(m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		bool Sequence::const_input_iterator::is_link_dir() const
		{
			return (m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && (m_impl->m_stat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
		}

		bool Sequence::const_input_iterator::operator ==(const this_type & rhs) const
		{
			return m_impl->m_handle == rhs.m_impl->m_handle;
		}

		bool Sequence::const_input_iterator::operator !=(const this_type & rhs) const
		{
			return m_impl->m_handle != rhs.m_impl->m_handle;
		}

		Sequence::const_input_iterator::const_input_iterator() :
			m_impl(new impl())
		{
		}

		Sequence::const_input_iterator::const_input_iterator(const Sequence & seq) :
			m_impl(new impl(seq))
		{
			operator++();
		}

		Sequence::const_input_iterator::impl::~impl() throw()
		{
			if (m_handle && m_handle != INVALID_HANDLE_VALUE ) {
				::FindClose(m_handle);
			}
		}

		Sequence::const_input_iterator::impl::impl() :
			m_seq(nullptr), m_handle(nullptr)
		{
		}

		Sequence::const_input_iterator::impl::impl(const Sequence & seq) :
			m_seq(&seq), m_handle(INVALID_HANDLE_VALUE )
		{
		}

	}
}
