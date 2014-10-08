#include <basis/configure.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/memory.hpp>
#include <excis/dll.hpp>
#include <excis/filesystem.hpp>
#include <excis/exception.hpp>

namespace fsys {

	namespace file {

		struct Map::file_map_iterator::impl {
			~impl()
			{
				close();
			}

			void close()
			{
				if (m_data) {
					::UnmapViewOfFile(m_data);
					m_data = nullptr;
				}
			}

		private:
			impl() :
				m_seq(nullptr), m_data(nullptr), m_size(0), m_offs(0)
			{
			}

			impl(const Map * seq) :
				m_seq(seq), m_data(nullptr), m_size(seq->get_frame()), m_offs(0)
			{
			}

			const Map * m_seq;
			PVOID m_data;
			size_type m_size;
			size_type m_offs;

			friend class file_map_iterator;
		};

		Map::file_map_iterator & Map::file_map_iterator::operator ++()
		{
			m_impl->close();
			if ((m_impl->m_seq->size() - m_impl->m_offs) > 0) {
				if ((m_impl->m_seq->size() - m_impl->m_offs) < m_impl->m_seq->get_frame())
					m_impl->m_size = m_impl->m_seq->size() - m_impl->m_offs;
				ACCESS_MASK amask = (m_impl->m_seq->is_writeble()) ? FILE_MAP_WRITE : FILE_MAP_READ;
				m_impl->m_data = ::MapViewOfFile(m_impl->m_seq->map(), amask, high_part_64(m_impl->m_offs), low_part_64(m_impl->m_offs), m_impl->m_size);
				CheckApi(m_impl->m_data != nullptr);
				m_impl->m_offs += m_impl->m_size;
			}
			return *this;
		}

		Map::file_map_iterator::this_type Map::file_map_iterator::operator ++(int)
		{
			this_type ret(*this);
			operator ++();
			return ret;
		}

		void * Map::file_map_iterator::operator *() const
		{
			return m_impl->m_data;
		}

		void * Map::file_map_iterator::data() const
		{
			return m_impl->m_data;
		}

		Map::size_type Map::file_map_iterator::size() const
		{
			return m_impl->m_size;
		}

		Map::size_type Map::file_map_iterator::offset() const
		{
			return m_impl->m_offs;
		}

		bool Map::file_map_iterator::operator ==(const this_type & rhs) const
		{
			return m_impl->m_data == rhs.m_impl->m_data;
		}

		bool Map::file_map_iterator::operator !=(const this_type & rhs) const
		{
			return !operator==(rhs);
		}

		Map::file_map_iterator::file_map_iterator() :
			m_impl(new impl)
		{
		}

		Map::file_map_iterator::file_map_iterator(const Map * seq) :
			m_impl(new impl(seq))
		{
			operator++();
		}

		Map::~Map()
		{
			::CloseHandle(m_map);
		}

		Map::Map(const Facade & wf, size_type size, bool write) :
			m_size(simstd::min(wf.size(), size)), m_frame(check_frame(DEFAULT_FRAME)), m_map(CheckHandle(::CreateFileMapping(wf, nullptr, (write) ? PAGE_READWRITE : PAGE_READONLY,
					high_part_64(m_size), low_part_64(m_size), nullptr))),
			m_write(write) {
			}

		Map::size_type Map::set_frame(size_type size)
		{
			return m_frame = check_frame(size);
		}

		Map::iterator Map::begin()
		{
			return file_map_iterator(this);
		}

		Map::iterator Map::end()
		{
			return file_map_iterator();
		}

		Map::const_iterator Map::begin() const
		{
			return file_map_iterator(this);
		}

		Map::const_iterator Map::end() const
		{
			return file_map_iterator();
		}

		bool Map::empty() const
		{
			return !size();
		}

		SYSTEM_INFO get_system_info()
		{
			SYSTEM_INFO info;
			::GetSystemInfo(&info);
			return info;
		}

		Map::size_type Map::check_frame(size_type size) const
		{
			static SYSTEM_INFO info(get_system_info());
			size_type ret = (!size || size % info.dwAllocationGranularity) ? (size / info.dwAllocationGranularity + 1) * info.dwAllocationGranularity : size;
			return simstd::min(m_size, ret);
		}

	}
}
