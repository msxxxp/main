#ifndef HDLINK_GLOBAL_HPP_
#define HDLINK_GLOBAL_HPP_

#include <CryptProvider.hpp>
#include <memory>

namespace Global {

	extern std::unique_ptr<CryptProvider> cryptProvider;
	extern size_t minFileSize; // Minimum file size so that hard linking will be checked...
	extern bool showStatistics;
	extern bool isRecursive;
	extern bool doHardlink;
	extern bool attrMustMatch;
	extern bool timeMustMatch;
	extern bool linkSmall;
	extern bool skipJunct;
	extern bool skipHidden;
	extern bool skipSystem;

}

#endif /* HDLINK_GLOBAL_HPP_ */
