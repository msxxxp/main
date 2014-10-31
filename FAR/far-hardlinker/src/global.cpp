#include <global.hpp>

namespace global {

	Statistics::Statistics()
	{
		LogTraceObj();
		memset(this, 0, sizeof(*this));
	}

	Options::Options()
	{
		LogTraceObj();
		firstBlockHash = 65 * 1024;
		showStatistics = 1;
		attrMustMatch = 0;
		timeMustMatch = 0;
		doHardlink = 0;
		searchOptions.fileMinSize = 1024;
		searchOptions.fileMaxSize = 3 * 1024 * 1024;
		searchOptions.set_flag(fsys::Sequence::folderSkipAll, false);
		searchOptions.set_flag(fsys::Sequence::folderSkipArchive, false);
		searchOptions.set_flag(fsys::Sequence::folderSkipReadOnly, false);
		searchOptions.set_flag(fsys::Sequence::folderSkipHidden, false);
		searchOptions.set_flag(fsys::Sequence::folderSkipSystem, true);
		searchOptions.set_flag(fsys::Sequence::folderSkipLink, false);
		searchOptions.set_flag(fsys::Sequence::fileSkipAll, false);
		searchOptions.set_flag(fsys::Sequence::fileSkipArchive, false);
		searchOptions.set_flag(fsys::Sequence::fileSkipReadOnly, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipHidden, false);
		searchOptions.set_flag(fsys::Sequence::fileSkipSystem, false);
		searchOptions.set_flag(fsys::Sequence::fileSkipLink, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipStreamed, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipCompressed, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipEncrypted, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipSparse, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipTemporary, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipOffline, true);
		searchOptions.set_flag(fsys::Sequence::fileSkipZeroSize, true);
	}

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
