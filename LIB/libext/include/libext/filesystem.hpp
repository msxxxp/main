#ifndef _LIBEXT_FILESYSTEM_HPP
#define _LIBEXT_FILESYSTEM_HPP

#include <libbase/std.hpp>
#include <libbase/filesystem.hpp>
#include <liblog/logger.hpp>

#include <memory>

namespace Fsys {
	Logger::Module_i * get_logger_module();

	namespace File {
		struct Facade;
	}

	bool is_exist(PCWSTR path);
	inline bool is_exist(const ustring & path) {
		return is_exist(path.c_str());
	}

	DWORD get_attr(PCWSTR path);
	inline DWORD get_attr(const ustring & path) {
		return get_attr(path.c_str());
	}

	void set_attr(PCWSTR path, DWORD attr);
	inline void set_attr(const ustring & path, DWORD attr) {
		return set_attr(path.c_str(), attr);
	}

	bool is_file(PCWSTR path);
	inline bool is_file(const ustring &path) {
		return is_file(path.c_str());
	}

	bool is_dir(PCWSTR path);
	inline bool is_dir(const ustring &path) {
		return is_dir(path.c_str());
	}

	void del(PCWSTR path);
	inline void del(const ustring &path) {
		del(path.c_str());
	}

	void del_sh(PCWSTR path);
	inline void del_sh(const ustring &path) {
		del_sh(path.c_str());
	}

	void del_recycle(PCWSTR path);
	inline void del_recycle(const ustring &path) {
		del_recycle(path.c_str());
	}

	void del_on_reboot(PCWSTR path);
	inline void del_on_reboot(const ustring & path) {
		del_on_reboot(path.c_str());
	}

	bool del_by_mask(PCWSTR mask);
	inline bool del_by_mask(const ustring &mask) {
		return del_by_mask(mask.c_str());
	}

	bool is_link(PCWSTR path);
	inline bool is_link(const ustring &path) {
		return is_link(path.c_str());
	}

	bool is_symlink(PCWSTR path);
	inline bool is_symlink(const ustring &path) {
		return is_symlink(path.c_str());
	}

	bool is_junction(PCWSTR path);
	inline bool is_junction(const ustring &path) {
		return is_junction(path.c_str());
	}

	ustring device_path_to_disk(PCWSTR path);

	ustring get_path(HANDLE path);

	HANDLE DuplicateHandle(HANDLE hndl);

	struct DeleteCmd: public Base::Command_p {
		DeleteCmd(const ustring &path):
			m_path(path) {
		}
		bool Execute() const {
			del(m_path);
			return true;
		}
	private:
		ustring m_path;
	};

	HANDLE HandleRead(PCWSTR path);

	HANDLE HandleWrite(PCWSTR path);

	inline HANDLE HandleRead(const ustring & path) {
		return HandleRead(path.c_str());
	}

	inline HANDLE HandleWrite(const ustring & path) {
		return HandleWrite(path.c_str());
	}

	///======================================================================================== Stat
	typedef struct _EXT_FILE_ID_128 {
	  ULONGLONG LowPart;
	  ULONGLONG HighPart;
	} EXT_FILE_ID_128, *PEXT_FILE_ID_128;

	typedef struct _FILE_ID_INFO {
	  ULONGLONG       VolumeSerialNumber;
	  EXT_FILE_ID_128 FileId;
	} FILE_ID_INFO, *PFILE_ID_INFO;

	struct Stat: private BY_HANDLE_FILE_INFORMATION {
		Stat(HANDLE hndl)
		{
			refresh(hndl);
		}

		Stat(PCWSTR path);

		Stat & operator =(HANDLE hndl);

		Stat & operator =(PCWSTR path);

		DWORD attr() const
		{
			return dwFileAttributes;
		}

		uint64_t ctime() const
		{
			return Base::make_uint64(ftCreationTime.dwHighDateTime, ftCreationTime.dwLowDateTime);
		}

		uint64_t atime() const
		{
			return Base::make_uint64(ftLastAccessTime.dwHighDateTime, ftLastAccessTime.dwLowDateTime);
		}

		uint64_t mtime() const
		{
			return Base::make_uint64(ftLastWriteTime.dwHighDateTime, ftLastWriteTime.dwLowDateTime);
		}

		FILETIME ctime_ft() const
		{
			return ftCreationTime;
		}

		FILETIME atime_ft() const
		{
			return ftLastAccessTime;
		}

		FILETIME mtime_ft() const
		{
			return ftLastWriteTime;
		}

		uint64_t size() const
		{
			return Base::make_uint64(nFileSizeHigh, nFileSizeLow);
		}

		DWORD device() const
		{
			return dwVolumeSerialNumber;
		}

		size_t num_links() const
		{
			return nNumberOfLinks;
		}

		int64_t inode() const
		{
			return Base::make_uint64(nFileIndexHigh, nFileIndexLow) & 0x0000FFFFFFFFFFFFULL;
		}

		bool is_dir() const
		{
			return Fsys::is_dir(dwFileAttributes);
		}

		bool is_file() const
		{
			return Fsys::is_file(dwFileAttributes);
		}

		bool is_lnk() const
		{
			return Fsys::is_link(dwFileAttributes);
		}

		bool operator ==(const Stat & rhs) const
		{
			return device() == rhs.device() && inode() == rhs.inode();
		}

	protected:
		Stat()
		{
			Memory::zero((BY_HANDLE_FILE_INFORMATION*)this, sizeof(BY_HANDLE_FILE_INFORMATION));
		}

		void refresh(HANDLE hndl);

	private:
//		FILE_BASIC_INFO    m_basic_info;
//		FILE_STANDARD_INFO m_standard_info;
//		FILE_ID_INFO       m_id_info;

		friend struct File::Facade;
	};

	inline bool operator ==(const Stat & f1, const Stat & f2) {
		return f1.operator ==(f2);
	}

	///==================================================================================== Sequence
	class Sequence: private Base::Uncopyable {
		typedef Sequence this_type;
		class const_input_iterator;

	public:
		struct FindStat {
			const wchar_t * name() const
			{
				return m_stat.cFileName;
			}

			ustring full_path() const;

			uint64_t size() const
			{
				return Base::make_uint64(m_stat.nFileSizeHigh, m_stat.nFileSizeLow);
			}

			size_t attr() const
			{
				return m_stat.dwFileAttributes;
			}

			FILETIME ctime_ft() const
			{
				return m_stat.ftCreationTime;
			}

			FILETIME atime_ft() const
			{
				return m_stat.ftLastAccessTime;
			}

			FILETIME mtime_ft() const
			{
				return m_stat.ftLastWriteTime;
			}

			bool is_file() const
			{
				return !Fsys::is_dir(m_stat.dwFileAttributes);
			}

			bool is_dir() const
			{
				return Fsys::is_dir(m_stat.dwFileAttributes);
			}

			bool is_link() const
			{
				return Fsys::is_link(m_stat.dwFileAttributes);
			}

		private:
			WIN32_FIND_DATAW m_stat;
			ustring          m_path;
			friend struct const_input_iterator;
		};

	public:
		typedef FindStat value_type;
		typedef size_t   size_type;
		typedef size_t   flags_type;
		typedef const_input_iterator iterator;
		typedef const_input_iterator const_iterator;

		enum SearchFlags_t {
			incDots     = 0x0001,
			skipDirs    = 0x0002,
			skipFiles   = 0x0004,
			skipLinks   = 0x0008,
			skipHidden  = 0x0010,
			skipSystem  = 0x0020,
		};

		Sequence(const ustring & path, flags_type flags = 0);

		Sequence(const ustring & path, const ustring & mask, flags_type flags = 0);

		const_iterator begin() const;

		const_iterator end() const;

		bool empty() const;

		ustring path() const
		{
			return m_path;
		}

		ustring mask() const
		{
			return m_mask;
		}

		flags_type flags() const
		{
			return m_flags;
		}

	private:
		ustring    m_path;
		ustring    m_mask;
		flags_type m_flags;
	};

	class Sequence::const_input_iterator {
		typedef const_input_iterator this_type;

	public:
		this_type & operator ++();

		this_type operator ++(int);

		const value_type & operator *() const;

		const value_type * operator -> () const;

		bool operator ==(const this_type & rhs) const;

		bool operator !=(const this_type & rhs) const;

	private:
		const_input_iterator();

		const_input_iterator(const Sequence & seq);

		struct impl {
			~impl() noexcept;
			impl() noexcept;
			impl(const Sequence & seq) noexcept;

			const Sequence * m_seq;
			HANDLE           m_find_handle;
			FindStat         m_stat;
		};

		std::shared_ptr<impl> m_impl;

		friend class Sequence;
	};

	namespace File {
		bool is_exist(PCWSTR path);
		inline bool is_exist(const ustring & path) {
			return is_exist(path.c_str());
		}

		uint64_t get_size(PCWSTR path);
		inline uint64_t get_size(const ustring & path) {
			return get_size(path.c_str());
		}

		uint64_t get_size(HANDLE hFile);

		uint64_t get_position(HANDLE hFile);

		void set_position(HANDLE hFile, uint64_t pos, DWORD m = FILE_BEGIN);

		void create(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);
		inline void create(const ustring &path, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			create(path.c_str(), lpsa);
		}

		void create(PCWSTR path, PCSTR content, LPSECURITY_ATTRIBUTES lpsa = nullptr);
		inline void create(const ustring &path, PCSTR content, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			create(path.c_str(), content, lpsa);
		}

		void create_hardlink(PCWSTR path, PCWSTR new_path);
		inline void create_hardlink(const ustring &path, const ustring &new_path) {
			create_hardlink(path.c_str(), new_path.c_str());
		}

		void del(PCWSTR path);
		inline void del(const ustring &path) {
			del(path.c_str());
		}

		bool wipe(PCWSTR path);

		void replace(PCWSTR from, PCWSTR to, PCWSTR backup = nullptr);
		inline void replace(const ustring & from, const ustring & to, PCWSTR backup = nullptr) {
			replace(from.c_str(), to.c_str(), backup);
		}

		uint64_t get_inode(PCWSTR path, size_t * nlink);

		size_t write(HANDLE file, PCVOID data, size_t bytesToWrite);
		inline size_t write(HANDLE file, const ustring & data) {
			return write(file, (PCVOID)data.c_str(), data.size() * sizeof(wchar_t));
		}

		void write(PCWSTR path, PCVOID data, size_t bytesToWrite, bool rewrite = false);
		inline void write(PCWSTR path, PCWSTR data, size_t size, bool rewrite = false) {
			write(path, data, size * sizeof(wchar_t), rewrite);
		}
		inline void write(const ustring & path, const ustring & data, bool rewrite = false) {
			write(path.c_str(), (PCVOID)data.c_str(), data.size() * sizeof(wchar_t), rewrite);
		}

		struct CopyCmd: public Base::Command_p {
			CopyCmd(const ustring & path, const ustring & dest):
				m_path(path),
				m_dest(dest) {
			}
			bool Execute() const {
				return Fsys::File::copy(m_path.c_str(), m_dest.c_str());
			}
		private:
			ustring m_path, m_dest;
		};

		struct MoveCmd: public Base::Command_p {
			MoveCmd(const ustring & path, const ustring & dest):
				m_path(path),
				m_dest(dest) {
			}
			bool Execute() const {
				return Fsys::File::move(m_path.c_str(), m_dest.c_str());
			}
		private:
			ustring m_path, m_dest;
		};

		///=========================================================================================
		struct Facade: private Fsys::Stat, private Base::Uncopyable {
			~Facade();

			explicit Facade(HANDLE hndl);

			Facade(const ustring & path, bool write = false);

			Facade(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags);

			uint64_t size() const;

			bool size_nt(uint64_t & size) const;

			DWORD read(PVOID data, size_t size);

			bool read_nt(PVOID buf, size_t size, DWORD & read);

			DWORD write(PCVOID buf, size_t size);

			bool write_nt(PCVOID buf, size_t size, DWORD & written);

			void set_attr(DWORD at);

			bool set_attr_nt(DWORD at);

			uint64_t get_position() const;

			void set_position(int64_t dist, DWORD method = FILE_BEGIN);

			bool set_position_nt(int64_t dist, DWORD method = FILE_BEGIN);

			void set_eof();

			bool set_eof_nt();

			void set_time(const FILETIME & ctime, const FILETIME & atime, const FILETIME & mtime);

			bool set_time_nt(const FILETIME & ctime, const FILETIME & atime, const FILETIME & mtime);

			void set_mtime(const FILETIME & mtime);

			bool set_mtime_nt(const FILETIME & mtime);

			void set_ctime(const FILETIME & ctime);

			bool set_ctime_nt(const FILETIME & ctime);

			void set_atime(const FILETIME & atime);

			bool set_atime_nt(const FILETIME & atime);

			ustring path() const {
				return m_path;
			}

			operator HANDLE() const {
				return m_hndl;
			}

			template<typename Type>
			bool io_control_out_nt(DWORD code, Type & data) throw() {
				DWORD size_ret;
				return ::DeviceIoControl(m_hndl, code, nullptr, 0, &data, sizeof(Type), &size_ret, nullptr) != 0;
			}

			static HANDLE Open(const ustring & path, bool write = false);

			static HANDLE Open(const ustring & path, ACCESS_MASK access, DWORD share, PSECURITY_ATTRIBUTES sa, DWORD creat, DWORD flags);

			void set_del_on_close(bool delOnClose);

		private:
			HANDLE m_hndl;
			ustring m_path;
			bool m_delOnClose;
		};

		///=============================================================================== File::Map
		/// Отображение файла в память блоками
		class Map: private Base::Uncopyable {
			class file_map_iterator;
			typedef Map this_type;

		public:
			typedef uint64_t size_type;
			typedef file_map_iterator iterator;
			typedef const file_map_iterator const_iterator;

			~Map();

			Map(const Facade & wf, size_type size = (size_type)-1, bool write = false);

			HANDLE map() const {
				return m_map;
			}

			size_type size() const {
				return m_size;
			}

			size_type get_frame() const {
				return m_frame;
			}

			size_type set_frame(size_type size);

			bool is_writeble() const {
				return m_write;
			}

			iterator begin();

			iterator end();

			const_iterator begin() const;

			const_iterator end() const;

			bool empty() const;

		private:
			static const size_type DEFAULT_FRAME = 1024 * 1024;

			size_type check_frame(size_type mul) const;

			size_type m_size;
			size_type m_frame;
			HANDLE m_map;
			bool m_write;
		};

		class Map::file_map_iterator {
			typedef file_map_iterator this_type;

		public:
			this_type & operator ++();

			this_type operator ++(int);

			void * operator *() const;

			void * data() const;

			size_type size() const;

			size_type offset() const;

			bool operator ==(const this_type & rhs) const;

			bool operator !=(const this_type & rhs) const;

		private:
			file_map_iterator();
			file_map_iterator(const Map * seq);

			struct impl;
			std::shared_ptr<impl> m_impl;

			friend class Map;
		};
	}

	///=================================================================================== Directory
	namespace Directory {
		bool is_exist(PCWSTR path);
		inline bool is_exist(const ustring & path) {
			return is_exist(path.c_str());
		}

		bool is_empty(PCWSTR path);
		inline bool is_empty(const ustring &path) {
			return is_empty(path.c_str());
		}

		void create(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);
		inline void create(const ustring &path, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			create(path.c_str(), lpsa);
		}

		bool create_full_nt(const ustring & p, LPSECURITY_ATTRIBUTES sa) throw();
		void create_full(const ustring & p, LPSECURITY_ATTRIBUTES sa = nullptr);

		bool create_dir(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);
		inline bool create_dir(const ustring &path, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			return create_dir(path.c_str(), lpsa);
		}

		void del(PCWSTR path);
		inline void del(const ustring &path) {
			del(path.c_str());
		}

		bool remove_dir(PCWSTR path, bool follow_links = false);
		inline bool remove_dir(const ustring &path, bool follow_links = false) {
			return remove_dir(path.c_str(), follow_links);
		}

		bool ensure_dir_exist(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa = nullptr);
		inline bool ensure_dir_exist(const ustring &path, LPSECURITY_ATTRIBUTES lpsa = nullptr) {
			return ensure_dir_exist(path.c_str(), lpsa);
		}
	}

	///======================================================================================== Link
	namespace Link {
		void copy(PCWSTR from, PCWSTR to);
		inline void copy(const ustring & from, const ustring & to) {
			copy(from.c_str(), to.c_str());
		}

		void create_sym(PCWSTR path, PCWSTR new_path);
		inline void	create_sym(const ustring & path, const ustring & new_path) {
			create_sym(path.c_str(), new_path.c_str());
		}

		void create_junc(PCWSTR path, PCWSTR new_path);
		inline void	create_junc(const ustring & path, const ustring & new_path) {
			return create_junc(path.c_str(), new_path.c_str());
		}

		void break_link(PCWSTR path);
		inline void break_link(const ustring & path) {
			break_link(path.c_str());
		}

		void del(PCWSTR path);
		inline void del(const ustring & path) {
			del(path.c_str());
		}

		ustring	read(PCWSTR path);
		inline ustring read(const ustring & path) {
			return read(path.c_str());
		}

		struct CreateSymCmd: public Base::Command_p {
			CreateSymCmd(const ustring & path, const ustring & new_path):
				m_path(path),
				m_new_path(new_path) {
			}
			bool Execute() const {
				create_sym(m_path, m_new_path);
				return true;
			}
		private:
			ustring m_path, m_new_path;
		};
	}

	///=============================================================================================
	void copy_file_security(PCWSTR path, PCWSTR dest);
	inline void copy_file_security(const ustring & path, const ustring & dest) {
		copy_file_security(path.c_str(), dest.c_str());
	}

}

namespace Ext {
	///========================================================================================== WinVol
	//class WinVol: private Uncopyable, public WinErrorCheck {
	//public:
	//	~WinVol() {
	//		Close();
	//	}
	//	WinVol(): m_hnd(INVALID_HANDLE_VALUE) {
	//	}
	//	bool 			Next();
	//
	//	ustring			GetName() const {
	//		return name;
	//	}
	//	ustring			GetPath() const;
	//	ustring			GetDevice() const;
	//
	//	uint64_t		GetSize() const {
	////		long long tmp = f_.nFileSizeHigh;
	////		tmp = tmp << (sizeof(f_.nFileSizeHigh) * 8);
	////		tmp |= f_.nFileSizeLow;
	//		return 0;
	//	}
	//
	//	DWORD			GetFlag() const {
	//		DWORD	Result = 0;
	////		::GetVolumeInformation(path.c_str(), nullptr, 0, nullptr, nullptr, &Result, nullptr, 0);
	//		return Result;
	//	}
	//	UINT			GetType() const {
	//		return ::GetDriveTypeW(name.c_str());
	//	}
	//
	//	bool			IsSuppCompress() const {
	//		return WinFlag::Check(GetFlag(), (DWORD)FILE_FILE_COMPRESSION);
	//	}
	//	bool			IsSuppEncrypt() const {
	//		return WinFlag::Check(GetFlag(), (DWORD)FILE_SUPPORTS_ENCRYPTION);
	//	}
	//	bool			IsSuppStreams() const {
	//		return WinFlag::Check(GetFlag(), (DWORD)FILE_NAMED_STREAMS);
	//	}
	//	bool			IsSuppACL() const {
	//		return WinFlag::Check(GetFlag(), (DWORD)FILE_PERSISTENT_ACLS);
	//	}
	//	bool			IsReadOnly() const {
	//		return WinFlag::Check(GetFlag(), (DWORD)FILE_READ_ONLY_VOLUME);
	//	}
	//
	//	bool			IsRemovable() const {
	//		return WinFlag::Check(GetType(), DRIVE_REMOVABLE);
	//	}
	//	bool			IsFixed() const {
	//		return WinFlag::Check(GetType(), DRIVE_FIXED);
	//	}
	//	bool			IsRemote() const {
	//		return WinFlag::Check(GetType(), (UINT)DRIVE_REMOTE);
	//	}
	//	bool			IsCdRom() const {
	//		return WinFlag::Check(GetType(), (UINT)DRIVE_CDROM);
	//	}
	//	bool			IsRamdisk() const {
	//		return WinFlag::Check(GetType(), (UINT)DRIVE_RAMDISK);
	//	}
	//
	//	bool			GetSize(uint64_t &uiUserFree, uint64_t &uiTotalSize, uint64_t &uiTotalFree) const;
	//	uint64_t		GetSizeTotal() const {
	//		uint64_t uf = 0, ts = 0, tf = 0;
	//		GetSize(uf, ts, tf);
	//		return ts;
	//	}
	//
	//private:
	//	void	Close();
	//
	//	HANDLE	m_hnd;
	//	ustring	name;
	//};

}

#endif
