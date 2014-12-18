#include <fsys.hpp>

#include <basis/simstd/algorithm>

const wchar_t* to_str(fsys::Sequence::FiltersBunch::Type type)
{
	auto ret = L"I";
	switch (type) {
		case fsys::Sequence::FiltersBunch::Type::IncludeOnly:
			break;
		case fsys::Sequence::FiltersBunch::Type::ExcludeAll:
			ret = L"E";
			break;
	}
	return ret;
}

fsys::Sequence::Filter::ByAttr::ByAttr(Attr include, Size exclude):
	include(include),
	exclude(exclude)
{
}

bool fsys::Sequence::Filter::ByAttr::operator ()(const FindStat& stat) const
{
	bool passed = (stat.attr() | include) == stat.attr() && (stat.attr() & exclude) == Attr();

	LogConsoleDebug2(-1, L"    %s [attr(0x%08X && !0x%08X)]: 0x%08X\n", passed ? L"passed" : L"ignore", include, exclude, stat.attr());

	return passed;
}

void fsys::Sequence::Filter::ByAttr::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByAttr::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::BySize::BySize(fsys::Sequence::Size from, fsys::Sequence::Size to):
	minSize(from),
	maxSize(to)
{
}

bool fsys::Sequence::Filter::BySize::operator ()(const Sequence::FindStat& stat) const
{
	bool passed = simstd::between(minSize, stat.size(), maxSize);

	LogConsoleDebug2(-1, L"    %s [size(%I64u, %I64u)]: %I64u\n", passed ? L"passed" : L"ignore", minSize, maxSize, stat.size());

	return passed;
}

void fsys::Sequence::Filter::BySize::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::BySize::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByMask::ByMask(const ustring& mask):
	mask(mask)
{
}

bool fsys::Sequence::Filter::ByMask::operator ()(const Sequence::FindStat& /*stat*/) const
{//FIXME
	bool passed = true;

	LogConsoleDebug2(-1, L"    %s [mask(%s)]\n", passed ? L"passed" : L"ignore", mask.c_str());

	return passed;
}

void fsys::Sequence::Filter::ByMask::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByMask::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByWrTime::ByWrTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByWrTime::operator ()(const FindStat& stat) const
{
	bool passed = simstd::between(minTime, stat.mtime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [wrtime(%I64d, %I64d)]: %I64d\n", passed ? L"passed" : L"ignore", minTime, maxTime, stat.mtime());

	return passed;
}

void fsys::Sequence::Filter::ByWrTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByWrTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByCrTime::ByCrTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByCrTime::operator ()(const FindStat& stat) const
{
	bool passed = simstd::between(minTime, stat.ctime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [crtime(%I64d, %I64d)]: %I64d\n", passed ? L"passed" : L"ignore", minTime, maxTime, stat.ctime());

	return passed;
}

void fsys::Sequence::Filter::ByCrTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByCrTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByAcTime::ByAcTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByAcTime::operator ()(const FindStat& stat) const
{
	bool passed = simstd::between(minTime, stat.atime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [actime(%I64d, %I64d)]: %I64d\n", passed ? L"passed" : L"ignore", minTime, maxTime, stat.atime());

	return passed;
}

void fsys::Sequence::Filter::ByAcTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByAcTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::FiltersBunch::~FiltersBunch()
{
	LogTraceObj();
}

fsys::Sequence::FiltersBunch::FiltersBunch(Type type, const ustring& name):
	name(name),
	type(type)
{
	LogTraceObj();
	for (auto it = bunch.cbegin(); it != bunch.cend(); ++it)
		(*it)->destroy();
}

bool fsys::Sequence::FiltersBunch::operator ()(const FindStat& stat, Statistics& /*statistics*/) const
{ // return true if skip this item
	LogConsoleDebug2(-1, L"   appply filter [%s, %s] on '%s'\n", to_str(type), name.c_str(), stat.name());

	bool passed = true;
	for (auto it = bunch.cbegin(); it != bunch.cend() && passed; ++it) {
		passed = (*it)->operator()(stat);
	}

	return (type == Type::IncludeOnly && !passed) || (type == Type::ExcludeAll && passed);
}

fsys::Sequence::FiltersBunch::Type fsys::Sequence::FiltersBunch::get_type() const
{
	return type;
}

void fsys::Sequence::FiltersBunch::add_filter(const Filter& filter)
{
	bunch.emplace_back(filter.clone());
}
