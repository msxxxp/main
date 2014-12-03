#ifndef HDLINK_GLOBAL_HPP_
#define HDLINK_GLOBAL_HPP_

#include <basis/configure.hpp>

#include <basis/std/memory>

#include <crypt.hpp>
#include <file_system.hpp>

struct Statistics {
	uint64_t FoundDirs;
	uint64_t FoundDirLink;
	uint64_t FoundFiles;
	uint64_t IgnoredDirLink;
	uint64_t IgnoredDirHidden;
	uint64_t IgnoredDirSystem;

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
	uint64_t headHashesCalculated;
	uint64_t tailHashesCalculated;
	uint64_t wholeHashesCalculated;
	uint64_t headHashCalculationFailed;
	uint64_t tailHashCalculationFailed;
	uint64_t wholeHashCalculationFailed;
	uint64_t filesOpened;
	uint64_t filesClosed;
	uint64_t fileOpenProblems;
	uint64_t folderObjCreated;
	uint64_t folderObjDestroyed;
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

	Statistics();
};

struct Options {
	uint64_t minFileSize;          // Minimum file size so that hard linking will be checked...
	uint64_t firstBlockHash;
	uint64_t showStatistics:1;
	uint64_t doRecursive   :1;
	uint64_t doHardlink    :1;
	uint64_t attrMustMatch :1;
	uint64_t timeMustMatch :1;
	uint64_t skipFileSmall :1;
	uint64_t skipFileHidden:1;
	uint64_t skipFileSystem:1;
	uint64_t skipDirLink   :1;
	uint64_t skipDirHidden :1;
	uint64_t skipDirSystem :1;

	Options();
};

struct Vars {
	simstd::unique_ptr<crypt::Provider> cryptProvider;
	file_system::Folders_t              folders;
	file_system::Files_t                files;
};

namespace global {
	Statistics & statistics();

	Options & options();

	Vars & vars();
}

#endif
