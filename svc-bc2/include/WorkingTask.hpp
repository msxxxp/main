#ifndef WORKINGTASK_HPP_
#define WORKINGTASK_HPP_

#include <libbase/std.hpp>
#include <liblog/logger.hpp>
#include <libbase/command_p.hpp>
#include <libbase/string.hpp>
#include <libbase/path.hpp>
#include <libbase/err.hpp>
#include <libext/filesystem.hpp>

#include <deque>

template<class ForwardIt, class OutputIt, class UnaryPredicate>
ForwardIt copy_remove_if(ForwardIt first, ForwardIt last, OutputIt d_first, UnaryPredicate p)
{
	ForwardIt result = first;
	for (; first != last; ++first) {
		if (p(*first)) {
			*d_first = *first;
			++d_first;
		} else {
			*result = *first;
			++result;
		}
	}
	return result;
}

struct WorkingTask: public Base::Command_p {
	typedef std::deque<ustring> PathsArray;

	WorkingTask(const ustring & name, const PathsArray & requestedPaths) :
		m_requestedPaths(requestedPaths),
		m_name(name)
	{
		LogNoise(L"name: '%s'\n", m_name.c_str());
	}

protected:
	//	void expandAllPaths_depth()
	//	{
	//		for (auto it = requestedPaths.begin(); it != requestedPaths.end(); ++it) {
	//			expandedPaths.emplace_back(*it);
	//			if (Fsys::is_dir(*it))
	//				expand_dir_depth(*it);
	//		}
	//	}

	//	void expandAllPaths_breadth()
	//	{
	//		PathsArray layer(requestedPaths);
	//
	//		// remove all files from layer
	//		auto newEnd = copy_remove_if(layer.begin(), layer.end(),
	//		                             std::back_inserter(expandedPaths),
	//		                             [](const ustring & path){return !Fsys::is_dir(path);});
	//		layer.erase(newEnd, layer.end());
	//
	//		// add all dirs
	//		expandedPaths.insert(expandedPaths.end(), layer.rbegin(), layer.rend());
	//
	//		// process all dirs
	//		while (!layer.empty()) {
	//			expand_dir_breadth(layer);
	//		}
	//	}

protected:
	PathsArray m_requestedPaths;
	//	PathsArray expandedPaths;
	//	PathsArray processedPaths;

private:
	//	void expand_dir_depth(const ustring & path)
	//	{
	//		Fsys::Sequence seq(path);
	//		for (auto it = seq.begin(); it != seq.end(); ++it) {
	//			expandedPaths.emplace_back(it->full_path());
	//			if (it->is_dir()) {
	//				expand_dir_depth(it->full_path());
	//			}
	//		}
	//	}

	//	void expand_dir_breadth(PathsArray & layer)
	//	{
	//		Fsys::Sequence seq(layer.back());
	//		for (auto it = seq.begin(); it != seq.end(); ++it) {
	//			expandedPaths.emplace_back(it->full_path());
	//			if (it->is_dir()) {
	//				layer.emplace_front(it->full_path());
	//			}
	//		}
	//		layer.pop_back();
	//	}

	ustring m_name;
};

#endif
