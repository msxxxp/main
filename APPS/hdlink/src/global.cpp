#include <global.hpp>

namespace Global {
	std::unique_ptr<CryptProvider> cryptProvider;
	size_t minFileSize = 1024;
	size_t firstBlockHash = 65 * 1024;
	bool showStatistics = true;
	bool isRecursive = false;
	bool doHardlink = false;
	bool attrMustMatch = false;
	bool timeMustMatch = false;
	bool linkSmall = false;
	bool skipJunct = false;
	bool skipHidden = false;
	bool skipSystem = false;
}

Statistics & statistics()
{
	static Statistics instance;
	return instance;
}
