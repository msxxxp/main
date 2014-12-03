#include <global.hpp>

Statistics::Statistics()
{
	memset(this, 0, sizeof(*this));
}

Options::Options()
{
	minFileSize    = 1024;
	firstBlockHash = 65 * 1024;
	showStatistics = 1;
	doRecursive    = 1;
	doHardlink     = 0;
	attrMustMatch  = 0;
	timeMustMatch  = 0;
	skipFileSmall  = 1;
	skipFileHidden = 0;
	skipFileSystem = 0;
	skipDirLink    = 1;
	skipDirHidden  = 1;
	skipDirSystem  = 1;
}

namespace global {
	Statistics & statistics()
	{
		static Statistics instance;
		return instance;
	}

	Options & options()
	{
		static Options instance;
		return instance;
	}

	Vars & vars()
	{
		static Vars instance;
		return instance;
	}
}
