#include <Path.hpp>
#include <global.hpp>

Path::~Path()
{
	++statistics().pathObjDestroyed;
}

Path::Path(const ustring & name, parent_type parent) :
	FsNode(name, parent)
{
	++statistics().pathObjCreated;
}
