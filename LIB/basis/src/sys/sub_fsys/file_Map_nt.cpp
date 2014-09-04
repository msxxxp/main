#include <basis/sys/fsys.hpp>

#include <basis/std/algorithm>

namespace fsys {

	namespace file {

		Map_nt::~Map_nt()
		{
			if (m_data)
				::UnmapViewOfFile(m_data);
		}

		Map_nt::Map_nt(const wchar_t * path, size_type size, bool write) :
			m_data(nullptr), m_size(0), m_write(write)
		{
			ACCESS_MASK access = write ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ;
			DWORD share = write ? 0 : FILE_SHARE_DELETE | FILE_SHARE_READ;
			HANDLE file = ::CreateFileW(path, access, share, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			if (file == INVALID_HANDLE_VALUE)
				return;
			m_size = simstd::min(get_size(file), size);
			DWORD protect = write ? PAGE_READWRITE : PAGE_READONLY;
			HANDLE m_map = ::CreateFileMappingW(file, nullptr, protect, high_part_64(m_size), low_part_64(m_size), nullptr);
			if (m_map) {
				ACCESS_MASK access = m_write ? FILE_MAP_WRITE : FILE_MAP_READ;
				m_data = ::MapViewOfFile(m_map, access, 0, 0, static_cast<size_t>(size));
				::CloseHandle(m_map);
			}
			::CloseHandle(file);
		}

		Map_nt::size_type Map_nt::size() const
		{
			return m_size;
		}

		PVOID Map_nt::data() const
		{
			return m_data;
		}

		bool Map_nt::is_writeble() const
		{
			return m_write;
		}

		bool Map_nt::is_valid() const
		{
			return m_data;
		}

		uint64_t Map_nt::get_size(HANDLE file) const
		{
			LARGE_INTEGER size;
			size.QuadPart = 0ll;
			::GetFileSizeEx(file, &size);
			return size.QuadPart;
		}

	}
}
