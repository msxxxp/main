#ifndef BASIS_SYS_FSYS_MAP_NT_HPP_
#define BASIS_SYS_FSYS_MAP_NT_HPP_

namespace fsys {

	namespace file {

		struct Map_nt {
			typedef uint64_t size_type;

			~Map_nt();

			Map_nt(const wchar_t* path, size_type size = static_cast<size_type>(-1), bool write = false);

			size_type size() const;

			void* data() const;

			bool is_writeble() const;

			bool is_valid() const;

		private:
			uint64_t get_size(HANDLE file) const;

			void*     m_data;
			size_type m_size;
			bool      m_write;
		};

	}

}

#endif
