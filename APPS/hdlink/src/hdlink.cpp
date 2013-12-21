/**
 hdlink console tool
 Search duplicate files and make hardlinks

 © 2013 Andrew Grechkin

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <libext/exception.hpp>
#include <liblog/logger.hpp>
#include <libbase/cstr.hpp>
#include <hdlink.hpp>
#include <Statistics.hpp>

#include <memory>

struct ShowStatistics: public Base::Command_p {
	ssize_t execute()
	{
		if (Global::showStatistics) {
			Statistics & s = statistics();
			LogInfo(L"Processing statistics:\n");
			LogInfo(L"  Files     found: %i\n", s.FoundFiles);
			LogInfo(L"  Folders   found: %i\n", s.FoundDirs);
			LogInfo(L"  Junctions found: %i\n", s.FoundJuncs);
			LogInfo(L"\n");
			LogInfo(L"  Files unique by size and skipped: %i\n", s.filesFoundUnique);
			LogInfo(L"  Files were already linked: %i\n", s.fileAlreadyLinked);
			LogInfo(L"  Files   which were on different volumes: %i\n", s.filesOnDifferentVolumes);
			LogInfo(L"  Files   which were ignored by zero size: %i\n", s.IgnoredZero);
			LogInfo(L"  Files   which were filtered by size: %i\n", s.IgnoredSmall);
			LogInfo(L"  Files   which were filtered by system attribute: %i\n", s.IgnoredSystem);
			LogInfo(L"  Files   which were filtered by hidden attribute: %i\n", s.IgnoredHidden);
			LogInfo(L"  Folders which were filtered as junction: %i\n", s.IgnoredJunc);
			LogInfo(L"\n");
			LogInfo(L"  Number of file hashes calculated: %i\n", s.hashesCalculated);
			LogInfo(L"  Files compared: %i\n", s.fileCompares);
			LogInfo(L"  Files differ in first 65535 bytes: %i\n", s.fileContentDifferFirstBlock);
			LogInfo(L"  Files compared using a hash: %i\n", s.hashCompares);
			LogInfo(L"  Files content is same: %i\n", s.fileContentSame);
			LogInfo(L"  Link generations: %i\n", s.hardLinksSuccess);
			LogInfo(L"  Increase of free space: %I64i\n", s.FreeSpaceIncrease);
			LogInfo(L"  Summary size of files: %I64i\n", s.FoundFilesSize);

			LogDebug(L"\n");
			LogDebug(L"  Path objects created: %i\n", s.pathObjCreated);
			LogDebug(L"  Path objects destroyed: %i\n", s.pathObjDestroyed);
			LogDebug(L"  File objects created: %i\n", s.fileObjCreated);
			LogDebug(L"  File objects destroyed: %i\n", s.fileObjDestroyed);

//			LogInfo(L"  Files content differed in first 64 KiB: %i", s.fileContentDifferFirstBlock);
//			LogInfo(L"Files content differ after %i bytes: %i", FIRST_BLOCK_SIZE, s.fileContentDifferLater);
//			LogInfo(L"File compare problems: %i", s.fileCompareProblems);
//			LogInfo(L"Number of files opened: %i", s.filesOpened);
//			LogInfo(L"Number of files closed: %i", s.filesClosed);
//			LogInfo(L"Number of file open problems: %i", s.fileOpenProblems);
		}
		return true;
	}
};

struct ShowHelp: public Base::Command_p {
	ShowHelp(const wchar_t * prg) :
		m_prg(Base::filename_only(prg))
	{
	}

	ssize_t execute()
	{
		LogInfo(L"Search duplicate files and make hardlinks\n");
		LogInfo(L"© 2011 Andrew Grechkin, http://code.google.com/p/andrew-grechkin/\n");
		{
			ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
			LogInfo(L"NOTE:\n\tUse this tool on your own risk!\n");
		}
		{
			ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			LogInfo(L"Usage:\n");
		}
		LogInfo(L"\thdlink [options] [path] [path] [...]\n");
		{
			ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
			LogInfo(L"Options:\n");
		}
		LogInfo(L"\t/?\tShows this help screen\n");
		LogInfo(L"\t/l\tHardlink files, if not specified, tool will just search duplicates\n");
		LogInfo(L"\t/a\tFile attributes must match for linking\n");
		LogInfo(L"\t/t\tTime + Date of files must match\n");
		LogInfo(L"\t/h\tSkip hidden files\n");
		LogInfo(L"\t/s\tSkip system files\n");
		LogInfo(L"\t/j\tSkip junctions (reparse points)\n");
		LogInfo(L"\t/m\tLink small files <1024 bytes\n");
		LogInfo(L"\t/r\tRuns recursively through the given folder list\n");
		LogInfo(L"\t/q\tQuiet mode\n");
		LogInfo(L"\t/v\tVerbose mode\n");
		LogInfo(L"\t/d\tDebug mode\n");
		return true;
	}

private:
	const wchar_t * m_prg;
};

struct CmdParser: public Base::Command_p {
	CmdParser(wchar_t * cmdLine) :
		argv(::CommandLineToArgvW(cmdLine, &argc), ::LocalFree),
		action(new ShowHelp(argv[0]))
	{
		for (int i = 1; i < argc; ++i) {
			if (Cstr::compare(argv.get()[i], L"/?") == 0) {
				action.reset(new ShowHelp(argv[0]));
				break;
			}

			if (Cstr::compare(argv.get()[i], L"/i") == 0) {
				action.reset(new ArcInfo(arc_lib));
				break;
			}

			if (Cstr::compare(argv.get()[i], L"/l") == 0 && i < (argc - 1)) {
				action.reset(new ArcList(arc_lib, argv[i + 1]));
				break;
			}

			if (Cstr::compare(argv.get()[i], L"/t") == 0 && i < (argc - 1)) {
				action.reset(new ArcTest(arc_lib, argv[i + 1]));
				break;
			}

			if (Cstr::compare(argv.get()[i], L"/e") == 0 && i < (argc - 2)) {
				action.reset(new ArcExtract(arc_lib, argv[i + 1], argv[i + 2]));
				continue;
			}

			if (Cstr::compare(argv.get()[i], L"/a") == 0 && i < (argc - 3)) {
				action.reset(new ArcCompress(arc_lib, argv[i + 1], argv[i + 2], argv[i + 3]));
				continue;
			}

			if (Cstr::compare(argv.get()[i], L"/g") == 0 && i < (argc - 3)) {
				action.reset(new ArcCompressPiped(arc_lib, argv[i + 1], argv[i + 3], argv[i + 2]));
				continue;
			}
		}
	}

	ssize_t execute()
	{
		return action->execute();
	}

private:
	std::unique_ptr<wchar_t*> argv;
	std::shared_ptr<Command_p> action;
	int argc;
};

int main() try
{
	{
		cryptProvider.reset(new CryptProvider);

		CmdParser(::GetCommandLineW()).execute();
	}

	ShowStatistics().execute();

	return NO_ERROR;
} catch (Ext::AbstractError & e) {
	LogFatal(L"%s\n", e.what().c_str());
	LogFatal(L"%s\n", e.where());
	return e.code();
} catch (std::exception & e) {
	LogFatal(L"std::exception [%S]:\n", typeid(e).name());
	LogFatal(L"What: %S\n", e.what());
	return 1;
}
