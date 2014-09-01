#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/cstr.hpp>

namespace fsys {

	Folder::~Folder()
	{
		++global::statistics().folderObjectsDestroyed;
		LogDebug(L"'%s'\n", m_name.c_str());
	}

	Folder::Folder(const ustring & name) :
		Node(name, Node_t(nullptr))
	{
		LogDebug(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

	Folder::Folder(const ustring & name, Node_t parent) :
		Node(name, parent)
	{
		LogDebug(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

}
