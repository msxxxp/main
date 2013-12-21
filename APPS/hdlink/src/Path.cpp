#include <Path.hpp>
#include <statistics.hpp>

Path::~Path()
{
	++statistics().pathObjDestroyed;
}

Path::Path(parent_type parent, const ustring & name) :
	FsNode(parent, name)
{
	++statistics().pathObjCreated;
}
