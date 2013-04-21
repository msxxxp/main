#include <libbase/std.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/memory.hpp>
#include <libext/dll.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

#include <wchar.h>

namespace Ext {

	namespace File {
		bool is_exist(PCWSTR path) {
			return FS::is_exist(path) && FS::is_file(path);
		}

		uint64_t get_size(PCWSTR path) {
			WIN32_FILE_ATTRIBUTE_DATA info;
			CheckApi(::GetFileAttributesExW(path, GetFileExInfoStandard, &info));
			return Base::make_uint64(info.nFileSizeHigh, info.nFileSizeLow);
		}

		uint64_t get_size(HANDLE hFile) {
			LARGE_INTEGER size;
			CheckApi(::GetFileSizeEx(hFile, &size));
			return size.QuadPart;
		}

		uint64_t get_position(HANDLE hFile) {
			LARGE_INTEGER pos;
			CheckApi(::SetFilePointerEx(hFile, pos, &pos, FILE_CURRENT));
			return pos.QuadPart;
		}

		void set_position(HANDLE hFile, uint64_t pos, DWORD m) {
			LARGE_INTEGER p;
			p.QuadPart = pos;
			CheckApi(::SetFilePointerEx(hFile, p, nullptr, m));
		}

		void create(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			Base::auto_close<HANDLE> file(CheckHandle(::CreateFileW(path, 0, 0, lpsa,
			                                                  CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr)));
		}

		void create(PCWSTR path, PCSTR content, LPSECURITY_ATTRIBUTES lpsa) {
			Base::auto_close<HANDLE> file(CheckHandle(::CreateFileW(path, GENERIC_WRITE, 0, lpsa,
			                                                  CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr)));
			DWORD bytesWritten = 0;
			DWORD bytesToWrite = Base::Str::length(content);
			CheckApi(::WriteFile(file, (PCVOID)content, bytesToWrite, &bytesWritten, nullptr) && bytesToWrite == bytesWritten);
		}

		void create_hardlink(PCWSTR path, PCWSTR new_path) {
			CheckApi(::CreateHardLinkW(new_path, path, nullptr));
		}

		bool del_simple_nt(PCWSTR path)
		{
			return ::DeleteFileW(path);
		}

		bool del_attrcheck_nt(PCWSTR path)
		{
			DWORD attr = ::GetFileAttributesW(path);
			if (attr != INVALID_FILE_ATTRIBUTES && ::SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL)) {
				if (del_simple_nt(path))
					return true;
				else
					::SetFileAttributesW(path, attr);
			}
			return false;
		}

		bool del_nt(PCWSTR path)
		{
			return del_simple_nt(path) || del_attrcheck_nt(path);
		}

		void del(PCWSTR path) {
			CheckApi(del_nt(path));
		}

		void read(PCWSTR path, astring &buf) {
			Base::auto_close<HANDLE> file(::CreateFileW(path, GENERIC_READ, 0, nullptr, OPEN_EXISTING,
			                                      FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, nullptr));
			if (file != INVALID_HANDLE_VALUE) {
				DWORD size = (DWORD)get_size(file);
				buf.reserve(size);
				CheckApi(::ReadFile(file, (PWSTR)buf.c_str(), buf.size(), &size, nullptr));
			}
		}

		uint64_t get_inode(PCWSTR path, size_t * nlink) {
			Base::auto_close<HANDLE> file(CheckHandle(::CreateFileW(path, FILE_READ_ATTRIBUTES,
			                                                  FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			                                                  nullptr, OPEN_EXISTING,
			                                                  FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
			                                                  nullptr)));
			BY_HANDLE_FILE_INFORMATION	info;
			CheckApi(::GetFileInformationByHandle(file, &info));
			if (nlink)
				*nlink = info.nNumberOfLinks;
			return Base::make_uint64(info.nFileIndexHigh, info.nFileIndexLow);
		}

		size_t write(HANDLE file, PCVOID data, size_t bytesToWrite) {
			DWORD bytesWritten = 0;
			CheckApi(::WriteFile(file, data, bytesToWrite, &bytesWritten, nullptr));
			return bytesWritten;
		}

		void write(PCWSTR path, PCVOID data, size_t bytesToWrite, bool rewrite) {
			DWORD creationDisposition = rewrite ? CREATE_ALWAYS : CREATE_NEW;
			Base::auto_close<HANDLE> file(CheckHandle(::CreateFileW(path, GENERIC_WRITE, 0, nullptr,
			                                                  creationDisposition, FILE_ATTRIBUTE_NORMAL, nullptr)));
			write(file, data, bytesToWrite);
		}
	}



	///================================================================================================
	/*
#define BUFF_SIZE (64*1024)

BOOL WipeFileW(wchar_t *filename) {
	DWORD Error = 0, OldAttr, needed;
	void *SD = nullptr;
	int correct_SD = FALSE;
	wchar_t dir[2*MAX_PATH], tmpname[MAX_PATH], *fileptr = wcsrchr(filename, L'\\');
	unsigned char *buffer = (unsigned char *)malloc(BUFF_SIZE);
	if (fileptr && buffer) {
		OldAttr = GetFileAttributesW(filename);
		SetFileAttributesW(filename, OldAttr&(~FILE_ATTRIBUTE_READONLY));
		if (!GetFileSecurityW(filename, DACL_SECURITY_INFORMATION, nullptr, 0, &needed))
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
				SD = malloc(needed);
				if (SD)
					if (GetFileSecurityW(filename, DACL_SECURITY_INFORMATION, SD, needed, &needed)) correct_SD = TRUE;
			}
		wcsncpy(dir, filename, fileptr - filename + 1);
		dir[fileptr-filename+1] = 0;
		if (GetTempFileNameW(dir, L"bc", 0, tmpname)) {
			if (MoveFileExW(filename, tmpname, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
				HANDLE f = CreateFileW(tmpname, FILE_GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
				if (f != INVALID_HANDLE_VALUE) {
					BY_HANDLE_FILE_INFORMATION info;
					if (GetFileInformationByHandle(f, &info)) {
						unsigned long long size = (unsigned long long)info.nFileSizeLow + (unsigned long long)info.nFileSizeHigh * 4294967296ULL;
						unsigned long long processed_size = 0;
						while (size) {
							unsigned long outsize = (unsigned long)((size >= BUFF_SIZE) ? BUFF_SIZE : size), transferred;
							WriteFile(f, buffer, outsize, &transferred, nullptr);
							size -= outsize;
							processed_size += outsize;
							if (UpdatePosInfo(0ULL, processed_size)) break;
						}
					}
					if ((SetFilePointer(f, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER) || (!SetEndOfFile(f))) Error = GetLastError();
					CloseHandle(f);
				}
				if (Error) MoveFileExW(tmpname, filename, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
				else if (!DeleteFileW(tmpname)) Error = GetLastError();
			} else {
				Error = GetLastError();
				DeleteFileW(tmpname);
			}
		} else Error = GetLastError();
		if (Error) {
			SetFileAttributesW(filename, OldAttr);
			if (correct_SD) SetFileSecurityW(filename, DACL_SECURITY_INFORMATION, SD);
		}
	}
	free(SD);
	free(buffer);
	if (Error) {
		SetLastError(Error);
		return FALSE;
	}
	return TRUE;
}
	 */
	/*
int			WipeDirectory(const wchar_t *Name) {

	string strTempName, strSavePath(Opt.strTempPath);
	BOOL usePath = FALSE;

	if (FirstSlash(Name)) {
		Opt.strTempPath = Name;
		DeleteEndSlash(Opt.strTempPath);
		CutToSlash(Opt.strTempPath);
		usePath = TRUE;
	}

	FarMkTempEx(strTempName, nullptr, usePath);
	Opt.strTempPath = strSavePath;

	if (!apiMoveFile(Name, strTempName)) {
		SetLastError((_localLastError = GetLastError()));
		return FALSE;
	}

	return apiRemoveDirectory(strTempName);

}
	 */
	/*
int			DeleteFileWithFolder(const wchar_t *FileName) {
	string strFileOrFolderName;
	strFileOrFolderName = FileName;
	Unquote(strFileOrFolderName);
	BOOL Ret = apiSetFileAttributes(strFileOrFolderName, FILE_ATTRIBUTE_NORMAL);

	if (Ret) {
		if (apiDeleteFile(strFileOrFolderName)) { //BUGBUG
			CutToSlash(strFileOrFolderName, true);
			return apiRemoveDirectory(strFileOrFolderName);
		}
	}

	SetLastError((_localLastError = GetLastError()));
	return FALSE;
}
	 */
	/*
void		DeleteDirTree(PCWSTR Dir) {
	if (*Dir == 0 ||
			(IsSlash(Dir[0]) && Dir[1] == 0) ||
			(Dir[1] == L':' && IsSlash(Dir[2]) && Dir[3] == 0))
		return;

	ustring strFullName;
	FAR_FIND_DATA_EX FindData;
	ScanTree ScTree(TRUE, TRUE, FALSE);
	ScTree.SetFindPath(Dir, L"*", 0);

	while (ScTree.GetNextName(&FindData, strFullName)) {
		apiSetFileAttributes(strFullName, FILE_ATTRIBUTE_NORMAL);

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (ScTree.IsDirSearchDone())
				apiRemoveDirectory(strFullName);
		} else
			apiDeleteFile(strFullName);
	}

	apiSetFileAttributes(Dir, FILE_ATTRIBUTE_NORMAL);
	apiRemoveDirectory(Dir);
}
	 */

}
