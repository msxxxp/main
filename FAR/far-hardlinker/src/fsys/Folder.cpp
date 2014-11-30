#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>

namespace fsys {

	Folder::~Folder()
	{
		LogTraceObj();
		LogNoise(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsDestroyed;
	}

	Folder::Folder(const ustring& name) :
		Node(name, Node_t(nullptr))
	{
		LogTraceObj();
		LogNoise(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

	Folder::Folder(const ustring& name, Node_t parent) :
		Node(name, parent)
	{
		LogTraceObj();
		LogNoise(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

}
