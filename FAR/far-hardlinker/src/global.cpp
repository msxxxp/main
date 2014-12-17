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
		auto& filter = searchOptions.add_filter(fsys::Sequence::Filter::Type::IncludeOnly, L"qwe1");
		filter.set_size(1024, 1 * 1024 * 1024);
//		auto enabledAttr = ~(FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_REPARSE_POINT);
//		auto enabledAttr = ~static_cast<uint32_t>(0);
		filter.set_attr(0, FILE_ATTRIBUTE_READONLY);
//		filter.set_attr(FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);
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
