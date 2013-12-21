#include <File.hpp>
#include <statistics.hpp>

File::~File()
{
	++statistics().fileObjDestroyed;
}

File::File(parent_type parent, const WIN32_FIND_DATAW & info) :
	FsNode(parent, info.cFileName),
	m_size(Base::make_uint64(info.nFileSizeHigh, info.nFileSizeLow)),
	m_time(Base::make_uint64(info.ftLastWriteTime.dwHighDateTime, info.ftLastWriteTime.dwLowDateTime)),
	m_attr(info.dwFileAttributes)
{
	++statistics().fileObjCreated;
}

DWORD File::get_attr() const
{
	return m_attr;
}

uint64_t File::time() const
{
	return m_time;
}

const Hash & File::hashFull() const
{
	return m_hashFull;
}

uint64_t File::size() const
{
	return m_size;
}

