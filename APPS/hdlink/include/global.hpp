#ifndef HDLINK_GLOBAL_HPP_
#define HDLINK_GLOBAL_HPP_

#include <libbase/std.hpp>
#include <CryptProvider.hpp>
#include <memory>

namespace Global {

	extern std::unique_ptr<CryptProvider> cryptProvider;
	extern size_t minFileSize; // Minimum file size so that hard linking will be checked...
	extern size_t firstBlockHash;
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

struct Statistics {
	uint64_t FoundDirs;
	uint64_t FoundJuncs;
	uint64_t FoundFiles;
	uint64_t IgnoredJunc;

	uint64_t filesFoundUnique;
	uint64_t IgnoredHidden;
	uint64_t IgnoredSystem;
	uint64_t IgnoredSmall;
	uint64_t IgnoredZero;
	uint64_t filesOnDifferentVolumes;

	uint64_t bytesRead;
	uint64_t FoundFilesSize;
	uint64_t FreeSpaceIncrease;

	uint64_t fileCompares;
	uint64_t hashCompares;
	uint64_t hashComparesHit1;
	uint64_t hashComparesHit2;
	uint64_t hashComparesHit3;
	uint64_t fileMetaDataMismatch;
	uint64_t fileAlreadyLinked;
	uint64_t fileContentDifferFirstBlock;
	uint64_t fileContentDifferLater;
	uint64_t fileContentSame;
	uint64_t fileCompareProblems;
	uint64_t hashesCalculated;
	uint64_t filesOpened;
	uint64_t filesClosed;
	uint64_t fileOpenProblems;
	uint64_t pathObjCreated;
	uint64_t pathObjDestroyed;
	uint64_t fileObjCreated;
	uint64_t fileObjDestroyed;
	uint64_t unbufferedFileStreamObjCreated;
	uint64_t unbufferedFileStreamObjDestroyed;
	uint64_t fileSystemObjCreated;
	uint64_t fileSystemObjDestroyed;
	uint64_t hardLinks;
	uint64_t hardLinksSuccess;
	uint64_t collectionObjCreated;
	uint64_t collectionObjDestroyed;
	uint64_t itemObjCreated;
	uint64_t itemObjDestroyed;
	uint64_t referenceCounterObjCreated;
	uint64_t referenceCounterObjDestroyed;
	uint64_t sizeGroupObjCreated;
	uint64_t sizeGroupObjDestroyed;
	uint64_t sortedFileCollectionObjCreated;
	uint64_t sortedFileCollectionObjDestroyed;
	uint64_t duplicateEntryObjCreated;
	uint64_t duplicateEntryObjDestroyed;
	uint64_t duplicateFileCollectionObjCreated;
	uint64_t duplicateFileCollectionObjDestroyed;
	uint64_t duplicateFileHardLinkerObjCreated;
	uint64_t duplicateFileHardLinkerObjDestroyed;
};

Statistics & statistics();

#endif /* HDLINK_GLOBAL_HPP_ */
