﻿#include <excis/filesystem.hpp>
#include <excis/exception.hpp>
#include <excis/priv.hpp>
#include <excis/reg.hpp>
#include <excis/sd.hpp>

#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>

#include <winioctl.h>

extern "C" {
	INT WINAPI SHCreateDirectoryExW(HWND, PCWSTR, PSECURITY_ATTRIBUTES);
}

namespace fsys {
	bool del_by_mask(PCWSTR mask) {
		bool Result = false;
		WIN32_FIND_DATAW wfd;
		HANDLE hFind = ::FindFirstFileW(mask, &wfd);
		if (hFind != INVALID_HANDLE_VALUE) {
			Result = true;
			ustring fullpath = path::extract_from_mask(mask);
			do {
				if (!path::filename::is_valid(wfd.cFileName))
					continue;
				ustring path = path::make(fullpath, wfd.cFileName);
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) {
					fsys::link::del(path);
				}
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					fsys::del_by_mask(path::make(path, L"*"));
					Result = fsys::directory::del_nt(path.c_str());
				} else {
					Result = fsys::file::del_nt(path.c_str());
				}
			} while (::FindNextFileW(hFind, &wfd));
			::FindClose(hFind);
		}
		return Result;
	}

	HANDLE HandleRead(PCWSTR path) {
		// Obtain backup/restore privilege in case we don't have it
		Ext::Privilege priv(SE_BACKUP_NAME);

		return CheckHandle(::CreateFileW(path, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ,
		                                 nullptr, OPEN_EXISTING,
		                                 FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
		                                 nullptr));
	}

	HANDLE HandleWrite(PCWSTR path) {
		Ext::Privilege priv(SE_RESTORE_NAME);

		return CheckHandle(::CreateFileW(path, GENERIC_READ | GENERIC_WRITE, 0, nullptr,
		                                 OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
		                                 nullptr));
	}

	void copy_file_security(PCWSTR path, PCWSTR dest) {
		Ext::WinSDW sd(path);
		set_security(dest, sd, SE_FILE_OBJECT);
	}

	namespace file {
		void replace(PCWSTR from, PCWSTR to, PCWSTR backup) {
			CheckApi(::ReplaceFileW(from, to, backup, 0, nullptr, nullptr));
		}
	}

	namespace directory {
		bool remove_dir(PCWSTR path, bool follow_links) {
			bool Result = false;
			if (path::is_mask(path)) {
				Result = fsys::del_by_mask(path);
			} else {
				if (!fsys::is_exist(path))
					return true;
				if (fsys::is_dir(path)) {
					if (!follow_links && fsys::is_link(path)) {
						fsys::link::del(path);
					} else {
						fsys::del_by_mask(path::make(path, L"*"));
						Result = fsys::directory::del_nt(path);
					}
				} else {
					Result = fsys::file::del_nt(path);
				}
			}
			return Result;
		}

		bool ensure_dir_exist(PCWSTR path, LPSECURITY_ATTRIBUTES lpsa) {
			if (directory::is_exist(path))
				return true;
			CheckApiError(::SHCreateDirectoryExW(nullptr, path, lpsa));
			return true;
		}
	}

	void SetOwnerRecur(const ustring &path, PSID owner, SE_OBJECT_TYPE type) {
		try {
			Ext::set_owner(path.c_str(), owner, type);
		} catch (...) {
		}
		if (fsys::is_dir(path)) {
			fsys::Sequence dir(path);
			for (auto it = dir.begin(); it != dir.end(); ++it) {
				if (it->is_dir()) {
					SetOwnerRecur(it->full_path(), owner, type);
				} else {
					try {
						Ext::set_owner(it->full_path().c_str(), owner, type);
					} catch (...) {
					}
				}
			}
		}
	}
}

namespace Ext {

	///========================================================================================== WinVol
	//void WinVol::Close() {
	//	if (m_hnd != INVALID_HANDLE_VALUE) {
	//		::FindVolumeClose(m_hnd);
	//		m_hnd = INVALID_HANDLE_VALUE;
	//	}
	//}
	//
	//bool WinVol::Next() {
	//	wchar_t buf[MAX_PATH];
	//	if (m_hnd != INVALID_HANDLE_VALUE) {
	//		ChkSucc(::FindNextVolumeW(m_hnd, buf, sizeofa(buf)));
	//	} else {
	//		m_hnd = ::FindFirstVolumeW(buf, sizeofa(buf));
	//		ChkSucc(m_hnd != INVALID_HANDLE_VALUE);
	//	}
	//	if (IsOK()) {
	//		name = buf;
	//	}
	//	return IsOK();
	//}
	//
	//ustring WinVol::GetPath() const {
	//	ustring Result;
	//	if (IsOK()) {
	//		DWORD size;
	//		::GetVolumePathNamesForVolumeNameW(name.c_str(), nullptr, 0, &size);
	//		if (::GetLastError() == ERROR_MORE_DATA) {
	//			auto_array<wchar_t> buf(size);
	//			::GetVolumePathNamesForVolumeNameW(name.c_str(), buf, size, &size);
	//			Result = buf.data();
	//			CutWord(Result, L"\\");
	//		}
	//	}
	//	return Result;
	//}
	//
	//ustring WinVol::GetDevice() const {
	//	auto_array<wchar_t> Result(MAX_PATH);
	//	::QueryDosDeviceW(GetPath().c_str(), Result, Result.size());
	//	return ustring(Result);
	//}
	//
	//bool WinVol::GetSize(uint64_t &uiUserFree, uint64_t &uiTotalSize, uint64_t &uiTotalFree) const {
	//	UINT mode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
	//	bool Result = ::GetDiskFreeSpaceExW(name.c_str(), (PULARGE_INTEGER)&uiUserFree,
	//	                                    (PULARGE_INTEGER)&uiTotalSize,
	//	                                    (PULARGE_INTEGER)&uiTotalFree);
	//	::SetErrorMode(mode);
	//	return Result;
	//}

	///=================================================================================================
	bool FileWipe(PCWSTR path) {
		//	{
		//		DWORD attr = get_attributes(path);
		//		if (!set_attributes(path, FILE_ATTRIBUTE_NORMAL))
		//			return false;
		//		WinFile WipeFile;
		//		if (!WipeFile.Open(path, GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ, nullptr,
		//		                   OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH | FILE_FLAG_SEQUENTIAL_SCAN)) {
		//			set_attributes(path, attr);
		//			return false;
		//		}
		//
		//		uint64_t size = WipeFile.size();
		//		{
		//			const uint64_t BufSize = 65536;
		//			char *buf[BufSize];
		//			WinMem::Fill(buf, BufSize, (char)'\0'); // используем символ заполнитель
		//
		//			DWORD Written;
		//			while (size > 0) {
		//				DWORD WriteSize = simstd::min(BufSize, size);
		//				WipeFile.Write(buf, WriteSize, Written);
		//				size -= WriteSize;
		//			}
		//			WipeFile.Write(buf, BufSize, Written);
		//		}
		//		WipeFile.Pointer(0, FILE_BEGIN);
		//		WipeFile.SetEnd();
		//	}
		//	ustring TmpName(TempFile(ExtractPath(path).c_str()));
		//	if (!move_file(path, TmpName.c_str(), MOVEFILE_REPLACE_EXISTING))
		//		return delete_file(path);
		//	return delete_file(TmpName);
		return path;
	}

}
