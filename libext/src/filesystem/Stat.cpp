#include <libbase/std.hpp>
#include <libbase/logger.hpp>
#include <libbase/memory.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

namespace Fsys {
	Stat::Stat(PCWSTR path)
	{
		Base::auto_close<HANDLE> hndl(HandleRead(path));
		refresh(hndl);
	}

	void Stat::refresh(HANDLE hndl)
	{
		CheckApi(::GetFileInformationByHandle(hndl, this));
//		CheckApi(::GetFileInformationByHandleEx(hndl, FileBasicInfo, &m_basic_info, sizeof(m_basic_info)));
//		CheckApi(::GetFileInformationByHandleEx(hndl, FileStandardInfo, &m_standard_info, sizeof(m_standard_info)));
//		::GetFileInformationByHandleEx(hndl, (FILE_INFO_BY_HANDLE_CLASS)0x12, &m_id_info, sizeof(m_id_info));
//		LogTrace();
//		LogNoise(L"CreationTime:   %I64d\n", m_basic_info.CreationTime.QuadPart);
//		LogNoise(L"LastAccessTime: %I64d\n", m_basic_info.LastAccessTime.QuadPart);
//		LogNoise(L"LastWriteTime:  %I64d\n", m_basic_info.LastWriteTime.QuadPart);
//		LogNoise(L"ChangeTime:     %I64d\n", m_basic_info.ChangeTime.QuadPart);
//		LogNoise(L"FileAttributes: 0x%X\n", m_basic_info.FileAttributes);
//		LogNoise(L"AllocationSize: %I64d\n", m_standard_info.AllocationSize.QuadPart);
//		LogNoise(L"EndOfFile:      %I64d\n", m_standard_info.EndOfFile.QuadPart);
//		LogNoise(L"NumberOfLinks:  %u\n", m_standard_info.NumberOfLinks);
//		LogNoise(L"DeletePending:  %d\n", (int)m_standard_info.DeletePending);
//		LogNoise(L"Directory:      %d\n", (int)m_standard_info.Directory);
//		LogNoise(L"VolumeSNumber:  0x%I64X\n", m_id_info.VolumeSerialNumber);
//		LogNoise(L"FileId:         0x%I64X 0x%I64X\n", m_id_info.FileId.HighPart, m_id_info.FileId.LowPart);
	}

	Stat & Stat::operator =(HANDLE hndl)
	{
		refresh(hndl);
		return *this;
	}

	Stat & Stat::operator =(PCWSTR path)
	{
		Base::auto_close<HANDLE> hndl(HandleRead(path));
		refresh(hndl);
		return *this;
	}
}
