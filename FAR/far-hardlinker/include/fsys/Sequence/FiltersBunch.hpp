#ifndef _FSYS_SEQUENCE_FILTERSBUNCH_HPP_
#define _FSYS_SEQUENCE_FILTERSBUNCH_HPP_

#include <basis/simstd/vector>

class fsys::Sequence::Filter {
public:
	class ByAttr;
	class BySize;
	class ByMask;
	class ByWrTime;
	class ByCrTime;
	class ByAcTime;

	virtual ~Filter() = default;

	virtual bool operator ()(const FindStat& stat) const = 0;

private:
	virtual void destroy() = 0;
	virtual Filter* clone() const = 0;

	friend class fsys::Sequence::FiltersBunch;
};

class fsys::Sequence::Filter::ByAttr: public Filter {
	typedef ByAttr this_type;
public:
	ByAttr(Attr include, Size exclude);

	bool operator ()(const FindStat& stat) const override;

private:
	Attr include, exclude;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::Filter::BySize: public Filter {
	typedef BySize this_type;
public:
	BySize(Size from, Size to);

	bool operator ()(const FindStat& stat) const override;

private:
	Size minSize, maxSize;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::Filter::ByMask: public Filter {
	typedef ByMask this_type;
public:
	ByMask(const ustring& mask);

	bool operator ()(const FindStat& stat) const override;

private:
	ustring mask;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::Filter::ByWrTime: public Filter {
	typedef ByWrTime this_type;
public:
	ByWrTime(Time from, Time to);

	bool operator ()(const FindStat& stat) const override;

private:
	Time minTime, maxTime;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::Filter::ByCrTime: public Filter {
	typedef ByCrTime this_type;
public:
	ByCrTime(Time from, Time to);

	bool operator ()(const FindStat& stat) const override;

private:
	Time minTime, maxTime;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::Filter::ByAcTime: public Filter {
	typedef ByAcTime this_type;
public:
	ByAcTime(Time from, Time to);

	bool operator ()(const FindStat& stat) const override;

private:
	Time minTime, maxTime;

	void destroy() override;
	fsys::Sequence::Filter* clone() const override;
};

class fsys::Sequence::FiltersBunch {
public:
	enum class Type: ssize_t
	{
		IncludeOnly,
		ExcludeAll,
	};

	~FiltersBunch();
	FiltersBunch(Type type, const ustring& name);

	bool operator ()(const FindStat& stat, Statistics& statistics) const;

	Type get_type() const;

	void add_filter(const Filter& filter);

private:
	ustring name;
	Type    type;

	typedef simstd::vector<Filter*> Filters;
	Filters bunch;
};

#endif
