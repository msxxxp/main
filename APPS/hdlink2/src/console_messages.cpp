#include <console_messages.hpp>
#include <global.hpp>

ssize_t ShowStatistics::execute()
{
	using namespace global;
//	if (options().showStatistics) {
//		Statistics & s = statistics();
//		LogConsoleForce(-1, L"Processing statistics:\n");
//		LogConsoleForce(-1, L"  Files     found: %i\n", s.FoundFiles);
//		LogConsoleForce(-1, L"  Folders   found: %i\n", s.FoundDirs);
//		LogConsoleForce(-1, L"  Junctions found: %i\n", s.FoundDirLink);
//		LogConsoleForce(-1, L"\n");
//		LogConsoleForce(-1, L"  Files unique by size and skipped: %i\n", s.filesFoundUnique);
//		LogConsoleForce(-1, L"  Files were already linked: %i\n", s.fileAlreadyLinked);
//		LogConsoleForce(-1, L"  Files   which were on different volumes: %i\n", s.filesOnDifferentVolumes);
//		LogConsoleForce(-1, L"  Files   which were ignored by zero size: %i\n", s.IgnoredZero);
//		LogConsoleForce(-1, L"  Files   which were filtered by size: %i\n", s.IgnoredSmall);
//		LogConsoleForce(-1, L"  Files   which were filtered by system attribute: %i\n", s.IgnoredSystem);
//		LogConsoleForce(-1, L"  Files   which were filtered by hidden attribute: %i\n", s.IgnoredHidden);
//		LogConsoleForce(-1, L"  Folders which were filtered as junction: %i\n", s.IgnoredDirLink);
//		LogConsoleForce(-1, L"\n");
//		LogConsoleForce(-1, L"  Number of file hashes calculated: %i\n", s.wholeHashesCalculated);
//		LogConsoleForce(-1, L"  Files compared: %i\n", s.fileCompares);
//		LogConsoleForce(-1, L"  Files differ in first 65535 bytes: %i\n", s.fileContentDifferFirstBlock);
//		LogConsoleForce(-1, L"  Files compared using a hash: %i\n", s.hashCompares);
//		LogConsoleForce(-1, L"  Files content is same: %i\n", s.fileContentSame);
//		LogConsoleForce(-1, L"  Link generations: %i\n", s.hardLinksSuccess);
//		LogConsoleForce(-1, L"  Increase of free space: %I64i\n", s.FreeSpaceIncrease);
//		LogConsoleForce(-1, L"  Summary size of files: %I64i\n", s.FoundFilesSize);
//
//		LogConsoleForce(-1, L"\n");
//		LogConsoleForce(-1, L"  Path objects created: %i\n", s.folderObjCreated);
//		LogConsoleForce(-1, L"  Path objects destroyed: %i\n", s.folderObjDestroyed);
//		LogConsoleForce(-1, L"  File objects created: %i\n", s.fileObjCreated);
//		LogConsoleForce(-1, L"  File objects destroyed: %i\n", s.fileObjDestroyed);
//
////		LogConsoleForce(-1, L"  Files content differed in first 64 KiB: %i", s.fileContentDifferFirstBlock);
////		LogConsoleForce(-1, L"Files content differ after %i bytes: %i", FIRST_BLOCK_SIZE, s.fileContentDifferLater);
////		LogConsoleForce(-1, L"File compare problems: %i", s.fileCompareProblems);
////		LogConsoleForce(-1, L"Number of files opened: %i", s.filesOpened);
////		LogConsoleForce(-1, L"Number of files closed: %i", s.filesClosed);
////		LogConsoleForce(-1, L"Number of file open problems: %i", s.fileOpenProblems);
//	}
	return true;
}

ShowHelp::ShowHelp(const wchar_t * prgName) :
	m_prgName(filename_only(prgName))
{
}

ssize_t ShowHelp::execute()
{
	LogConsoleForce(-1, L"Search duplicate files and make hardlinks\n");
	LogConsoleForce(-1, L"© 2014 Andrew Grechkin, http://code.google.com/p/andrew-grechkin/\n");
	LogConsoleForce(-1, L"NOTE:\n");
	LogConsoleForce(-1, L"\tUse this tool on your own risk!\n");
	LogConsoleForce(-1, L"Usage:\n");
	LogConsoleForce(-1, L"\t%s [options] [path] [path] [...]\n", m_prgName);
	LogConsoleForce(-1, L"Options:\n");
	LogConsoleForce(-1, L"\t/?\tShows this help screen\n");
	LogConsoleForce(-1, L"\t/l\tHardlink files, if not specified, tool will just search duplicates\n");
	LogConsoleForce(-1, L"\t/a\tFile attributes must match for linking\n");
	LogConsoleForce(-1, L"\t/t\tTime + Date of files must match\n");
	LogConsoleForce(-1, L"\t/h\tSkip hidden files\n");
	LogConsoleForce(-1, L"\t/s\tSkip system files\n");
	LogConsoleForce(-1, L"\t/j\tSkip junctions (reparse points)\n");
	LogConsoleForce(-1, L"\t/m\tLink small files <1024 bytes\n");
	LogConsoleForce(-1, L"\t/r\tRuns recursively through the given folder list\n");
	LogConsoleForce(-1, L"\t/q\tQuiet mode\n");
	LogConsoleForce(-1, L"\t/v\tVerbose mode\n");
	LogConsoleForce(-1, L"\t/d\tDebug mode\n");

//	global::options().showStatistics = 1;

	return 0;
}
