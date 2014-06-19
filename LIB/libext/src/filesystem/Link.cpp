#include <libext/filesystem.hpp>
#include <libext/exception.hpp>
//#include <libext/priv.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/cstr.hpp>

#include <basis/std/string>

namespace fsys {

//#ifndef SE_CREATE_SYMBOLIC_LINK_NAME
//#define SE_CREATE_SYMBOLIC_LINK_NAME      L"SeCreateSymbolicLinkPrivilege"
//#endif
//	Ext::Privilege CreateSymlinkPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME);
//	Ext::Privilege CreateSymlinkPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME);

#ifndef IO_REPARSE_TAG_VALID_VALUES
#define IO_REPARSE_TAG_VALID_VALUES 0xF000FFFF
#endif

#ifndef IsReparseTagValid
#define IsReparseTagValid(_tag) (!((_tag)&~IO_REPARSE_TAG_VALID_VALUES)&&((_tag)>IO_REPARSE_TAG_RESERVED_RANGE))
#endif

	namespace {
#ifndef REPARSE_DATA_BUFFER_HEADER_SIZE
		typedef struct _REPARSE_DATA_BUFFER {
			ULONG ReparseTag;
			USHORT ReparseDataLength;
			USHORT Reserved;
			union {
				struct {
					USHORT SubstituteNameOffset;
					USHORT SubstituteNameLength;
					USHORT PrintNameOffset;
					USHORT PrintNameLength;
					ULONG Flags;
					wchar_t PathBuffer[1];
				} SymbolicLinkReparseBuffer;
				struct {
					USHORT SubstituteNameOffset;
					USHORT SubstituteNameLength;
					USHORT PrintNameOffset;
					USHORT PrintNameLength;
					wchar_t PathBuffer[1];
				} MountPointReparseBuffer;
				struct {
					UCHAR DataBuffer[1];
				} GenericReparseBuffer;
			};
		} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

//#define IO_REPARSE_TAG_SYMLINK                  (0xA000000CL)
#define REPARSE_DATA_BUFFER_HEADER_SIZE FIELD_OFFSET(REPARSE_DATA_BUFFER,GenericReparseBuffer)
#endif

		HANDLE OpenLinkHandle(PCWSTR path, ACCESS_MASK acc = 0, DWORD create = OPEN_EXISTING)
		{
			return ::CreateFileW(path, acc, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, create, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, nullptr);
		}

		struct REPARSE_BUF {
			REPARSE_BUF(PCWSTR path);

			REPARSE_BUF(ULONG tag, PCWSTR PrintName, size_t PrintNameLength, PCWSTR SubstituteName, size_t SubstituteNameLength);

			void set_to(PCWSTR path) const;

			bool is_symlink() const;

			bool is_junction() const;

			size_t size() const;

			PREPARSE_DATA_BUFFER operator ->() const;

		private:
			union {
				BYTE buf[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
				REPARSE_DATA_BUFFER rdb;
			};
		};
	}

	REPARSE_BUF::REPARSE_BUF(PCWSTR path)
	{
		DWORD attr = fsys::get_attr(path);
		if (!(attr & FILE_ATTRIBUTE_REPARSE_POINT)) {
			CheckApiError(ERROR_NOT_A_REPARSE_POINT);
		}
		memory::auto_close<HANDLE> link(CheckHandle(OpenLinkHandle(path)));
		DWORD returned;
		CheckApi(::DeviceIoControl(link, FSCTL_GET_REPARSE_POINT, nullptr, 0, buf, size(),
		                           &returned, nullptr) && IsReparseTagValid(rdb.ReparseTag));
	}

	REPARSE_BUF::REPARSE_BUF(ULONG tag, PCWSTR PrintName, size_t PrintNameLength, PCWSTR SubstituteName, size_t SubstituteNameLength)
	{
		rdb.Reserved = 0;
		rdb.ReparseTag = tag;

		switch (rdb.ReparseTag) {
			case IO_REPARSE_TAG_MOUNT_POINT:
				rdb.MountPointReparseBuffer.SubstituteNameOffset = 0;
				rdb.MountPointReparseBuffer.SubstituteNameLength = static_cast<WORD>(SubstituteNameLength * sizeof(wchar_t));
				rdb.MountPointReparseBuffer.PrintNameOffset = rdb.MountPointReparseBuffer.SubstituteNameLength + 2;
				rdb.MountPointReparseBuffer.PrintNameLength = static_cast<WORD>(PrintNameLength * sizeof(wchar_t));
				rdb.ReparseDataLength = FIELD_OFFSET(REPARSE_DATA_BUFFER,MountPointReparseBuffer.PathBuffer) + rdb.MountPointReparseBuffer.PrintNameOffset
				    + rdb.MountPointReparseBuffer.PrintNameLength + 1 * sizeof(wchar_t) - REPARSE_DATA_BUFFER_HEADER_SIZE;

				if (rdb.ReparseDataLength + REPARSE_DATA_BUFFER_HEADER_SIZE <= static_cast<USHORT>(MAXIMUM_REPARSE_DATA_BUFFER_SIZE / sizeof(wchar_t))) {
					wmemcpy(&rdb.MountPointReparseBuffer.PathBuffer[rdb.MountPointReparseBuffer.SubstituteNameOffset / sizeof(wchar_t)], SubstituteName, SubstituteNameLength + 1);
					wmemcpy(&rdb.MountPointReparseBuffer.PathBuffer[rdb.MountPointReparseBuffer.PrintNameOffset / sizeof(wchar_t)], PrintName, PrintNameLength + 1);
				}

				break;
			case IO_REPARSE_TAG_SYMLINK:
				rdb.SymbolicLinkReparseBuffer.PrintNameOffset = 0;
				rdb.SymbolicLinkReparseBuffer.PrintNameLength = static_cast<WORD>(PrintNameLength * sizeof(wchar_t));
				rdb.SymbolicLinkReparseBuffer.SubstituteNameOffset = rdb.MountPointReparseBuffer.PrintNameLength;
				rdb.SymbolicLinkReparseBuffer.SubstituteNameLength = static_cast<WORD>(SubstituteNameLength * sizeof(wchar_t));
				rdb.ReparseDataLength = FIELD_OFFSET(REPARSE_DATA_BUFFER,SymbolicLinkReparseBuffer.PathBuffer) + rdb.SymbolicLinkReparseBuffer.SubstituteNameOffset
				    + rdb.SymbolicLinkReparseBuffer.SubstituteNameLength - REPARSE_DATA_BUFFER_HEADER_SIZE;

				if (rdb.ReparseDataLength + REPARSE_DATA_BUFFER_HEADER_SIZE <= static_cast<USHORT>(MAXIMUM_REPARSE_DATA_BUFFER_SIZE / sizeof(wchar_t))) {
					wmemcpy(&rdb.SymbolicLinkReparseBuffer.PathBuffer[rdb.SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(wchar_t)], SubstituteName, SubstituteNameLength);
					wmemcpy(&rdb.SymbolicLinkReparseBuffer.PathBuffer[rdb.SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(wchar_t)], PrintName, PrintNameLength);
				}
				break;
		}
	}

	void REPARSE_BUF::set_to(PCWSTR path) const
	{
		DWORD attr = fsys::get_attr(path);
		if (attr & FILE_ATTRIBUTE_READONLY) {
			fsys::set_attr(path, attr & ~FILE_ATTRIBUTE_READONLY);
		}

		memory::auto_close<HANDLE> hLink(CheckHandle(OpenLinkHandle(path, GENERIC_WRITE)));
		DWORD dwBytesReturned;
		CheckApi(::DeviceIoControl(hLink, FSCTL_SET_REPARSE_POINT, (PVOID)&rdb, rdb.ReparseDataLength + REPARSE_DATA_BUFFER_HEADER_SIZE, nullptr, 0, &dwBytesReturned, nullptr));

		if (attr & FILE_ATTRIBUTE_READONLY) {
			fsys::set_attr(path, attr);
		}
	}

	bool REPARSE_BUF::is_symlink() const
	{
		return rdb.ReparseTag == IO_REPARSE_TAG_SYMLINK;
	}

	bool REPARSE_BUF::is_junction() const
	{
		return rdb.ReparseTag == IO_REPARSE_TAG_MOUNT_POINT;
	}

	size_t REPARSE_BUF::size() const
	{
		return lengthof(buf);
	}

	PREPARSE_DATA_BUFFER REPARSE_BUF::operator ->() const
	{
		return (PREPARSE_DATA_BUFFER)&rdb;
	}

	bool is_link(PCWSTR path)
	{
		try {
			REPARSE_BUF rdb(path);
			return rdb->ReparseTag == IO_REPARSE_TAG_MOUNT_POINT || rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK;
		} catch (...) {
		}
		return false;
	}

	bool is_symlink(PCWSTR path)
	{
		return REPARSE_BUF(path).is_symlink();
	}

	bool is_junction(PCWSTR path)
	{
		return REPARSE_BUF(path).is_junction();
	}

	namespace Link {
		void copy(PCWSTR from, PCWSTR to)
		{
			Stat stat(from);
			REPARSE_BUF rdb(from);
			if (!(stat.attr() & FILE_ATTRIBUTE_DIRECTORY)) {
				File::create(to);
			}
			rdb.set_to(to);
			fsys::set_attr(to, stat.attr());
		}

		void create_sym(PCWSTR path, PCWSTR new_path)
		{
			if (fsys::is_dir(path))
				Directory::create(new_path);
			else
				File::create(new_path);

			memory::auto_close<HANDLE> hLink(CheckHandle(OpenLinkHandle(new_path, GENERIC_WRITE)));
			ustring SubstituteName(ustring(REPARSE_PREFIX) + fsys::Path::remove_prefix(path));
			REPARSE_BUF rdb(IO_REPARSE_TAG_SYMLINK, path, cstr::length(path), SubstituteName.c_str(), SubstituteName.size());
			try {
				rdb.set_to(new_path);
			} catch (...) {
				fsys::del_nt(new_path);
				throw;
			}
		}

		void create_junc(PCWSTR path, PCWSTR new_path)
		{
			Directory::create(new_path);
			memory::auto_close<HANDLE> hLink(CheckHandle(OpenLinkHandle(new_path, GENERIC_WRITE)));
			ustring SubstituteName(ustring(REPARSE_PREFIX) + fsys::Path::remove_prefix(path));
			REPARSE_BUF rdb(IO_REPARSE_TAG_MOUNT_POINT, path, cstr::length(path), SubstituteName.c_str(), SubstituteName.size());
			try {
				rdb.set_to(new_path);
			} catch (...) {
				Directory::del_nt(new_path);
				throw;
			}
		}

		void break_link(PCWSTR path)
		{
			DWORD attr = fsys::get_attr(path);
			if (!(attr & FILE_ATTRIBUTE_REPARSE_POINT)) {
				CheckApiError(ERROR_NOT_A_REPARSE_POINT);
			}

			REPARSE_BUF rdb(path);
			if (attr & FILE_ATTRIBUTE_READONLY) {
				fsys::set_attr(path, attr & ~FILE_ATTRIBUTE_READONLY);
			}

			memory::auto_close<HANDLE> hLink(CheckHandle(OpenLinkHandle(path, GENERIC_WRITE)));

			REPARSE_GUID_DATA_BUFFER rgdb;
			memory::zero(rgdb);
			rgdb.ReparseTag = rdb->ReparseTag;
			DWORD dwBytesReturned;
			CheckApi(::DeviceIoControl(hLink, FSCTL_DELETE_REPARSE_POINT, &rgdb, REPARSE_GUID_DATA_BUFFER_HEADER_SIZE, nullptr, 0, &dwBytesReturned, 0));

			if (attr & FILE_ATTRIBUTE_READONLY) {
				fsys::set_attr(path, attr);
			}
		}

		void del(PCWSTR path)
		{
			break_link(path);
			fsys::del(path);
		}

		ustring read(PCWSTR path)
		{
			ustring ret;
			REPARSE_BUF rdb(path);
			size_t NameLength = 0;
			if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK) {
				NameLength = rdb->SymbolicLinkReparseBuffer.PrintNameLength / sizeof(wchar_t);
				if (NameLength) {
					ret.assign(&rdb->SymbolicLinkReparseBuffer.PathBuffer[rdb->SymbolicLinkReparseBuffer.PrintNameOffset / sizeof(wchar_t)], NameLength);
				} else {
					NameLength = rdb->SymbolicLinkReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
					ret.assign(&rdb->SymbolicLinkReparseBuffer.PathBuffer[rdb->SymbolicLinkReparseBuffer.SubstituteNameOffset / sizeof(wchar_t)], NameLength);
				}
			} else {
				NameLength = rdb->MountPointReparseBuffer.PrintNameLength / sizeof(wchar_t);
				if (NameLength) {
					ret.assign(&rdb->MountPointReparseBuffer.PathBuffer[rdb->MountPointReparseBuffer.PrintNameOffset / sizeof(wchar_t)], NameLength);
				} else {
					NameLength = rdb->MountPointReparseBuffer.SubstituteNameLength / sizeof(wchar_t);
					ret.assign(&rdb->MountPointReparseBuffer.PathBuffer[rdb->MountPointReparseBuffer.SubstituteNameOffset / sizeof(wchar_t)], NameLength);
				}
			}
			if (ret.find(L"\\??\\") == 0) {
				ret.erase(0, 4);
			}
			return ret;
		}
	}

}
