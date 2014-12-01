#ifndef _WIN_NET_VOLUME_H_
#define _WIN_NET_VOLUME_H_

#include <basis/configure.hpp>

namespace Ext {

const size_t MAX_VOL_LEN = 64;
const size_t MAX_MOUNT_POINT_LEN = 512;

/**
 * Callback-функция для перечисления томов/точек монтирования
 * @param[in] name - имя тома
 * @param[in,out] data - дополнительные данные, переданные при вызове перечислителя
 * return \c true - если нужно продолжить перечисление
 * @sa EnumVolumes
*/
typedef bool (*ENUMVOLUMECALLBACK)(const wchar_t* name, PVOID data);

void EnumVolumes(ENUMVOLUMECALLBACK callback, PVOID callbackData);

void EnumVolumeMountPoints(ENUMVOLUMECALLBACK callback, const wchar_t* volName, PVOID callbackData);

bool GetVolumePathNamesList(const wchar_t* volName, TCHAR *&mntPointsList, DWORD &listSize);

bool CheckDriveRemovable(const wchar_t* name, bool &isRemovable);

ustring GetVolumeByFileName(const wchar_t* fileName);

void GetDriveGeometry(const wchar_t* name, DISK_GEOMETRY &g);

void GetVolumeDiskExtents(const wchar_t* name, memory::auto_buf<PVOLUME_DISK_EXTENTS> &v);

ustring GetDrives();

}

#endif
