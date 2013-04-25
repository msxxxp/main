#include <libbase/std.hpp>
#include <libbase/logger.hpp>
#include <libbase/messaging.hpp>
#include <libbase/thread.hpp>
#include <libbase/ThreadPool.hpp>
#include <libbase/command_p.hpp>
#include <libbase/string.hpp>
#include <libext/filesystem.hpp>

#include <GlobalData.hpp>
#include <Settings.hpp>

#include <deque>

class Listener: public Base::ThreadRoutine_i {
	size_t run(void * data) override
	{
		LogTrace();
		return 0;
	}
};

struct Worker: public Base::ThreadRoutine_i {
	Worker(Base::Queue * queue) :
		m_queue(queue)
	{
	}

	size_t run(void * data) override
	{
		LogTrace();

		Base::Message msg;
		while (m_queue->get_message(msg) == Base::WaitResult_t::SUCCESS) {
			Base::Command_p * command = (Base::Command_p*)msg.get_data();
			command->execute();
		}

		return 0;
	}

private:
	Base::Queue * m_queue;
};

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
	WorkingTask(PCWSTR name, PathsArray paths) :
		requestedPaths(paths), m_name(name)
	{
		LogNoise(L"name: '%s'\n", m_name.c_str());
	}

protected:
	void expandAllPaths_depth()
	{
		for (auto it = requestedPaths.begin(); it != requestedPaths.end(); ++it) {
			expandedPaths.emplace_back(*it);
			if (Fsys::is_dir(*it))
				expand_dir_depth(*it);
		}

		for (auto it = expandedPaths.begin(); it != expandedPaths.end(); ++it)
			LogNoise(L"'%s'\n", it->c_str());
	}

	void expandAllPaths_breadth()
	{
//		for (auto it = requestedPaths.begin(); it != requestedPaths.end(); ++it) {
//			if (Fsys::is_dir(*it))
//				layer.emplace_back(*it);
//			else
//				expandedPaths.emplace_back(*it);
//		}
		PathsArray layer(requestedPaths);

		// remove all files from layer
		auto newEnd = copy_remove_if(layer.begin(), layer.end(),
		                             std::back_inserter(expandedPaths),
		                             [](const ustring & path){return !Fsys::is_dir(path);});
		layer.erase(newEnd, layer.end());

		// add all dirs
		expandedPaths.insert(expandedPaths.end(), layer.rbegin(), layer.rend());

		// process all dirs
		while (!layer.empty()) {
			expand_dir_breadth(layer);
		}

		for (auto it = expandedPaths.begin(); it != expandedPaths.end(); ++it)
			LogNoise(L"'%s'\n", it->c_str());
	}

protected:
	PathsArray requestedPaths;
	PathsArray expandedPaths;
	PathsArray processedPaths;

private:
	void expand_dir_depth(const ustring & path)
	{
		Fsys::Sequence seq(path);
		for (auto it = seq.begin(); it != seq.end(); ++it) {
//			LogInfo(L"'%20.20s' isDir: %d isLink: %d size: %I64u\n", it->name(), it->is_dir(), it->is_link(), it->size());
			expandedPaths.emplace_back(it->full_path());
			if (it->is_dir()) {
				expand_dir_depth(it->full_path());
			}
		}
	}

	void expand_dir_breadth(PathsArray & layer)
	{
		Fsys::Sequence seq(layer.back());
		for (auto it = seq.begin(); it != seq.end(); ++it) {
			expandedPaths.emplace_back(it->full_path());
			if (it->is_dir()) {
				layer.emplace_front(it->full_path());
			}
		}
		layer.pop_back();
	}

	ustring m_name;
};

struct Remove: public WorkingTask {
	Remove(WorkingTask::PathsArray paths) :
		WorkingTask(L"remove", paths)
	{
	}

	ssize_t execute() override
	{
		expandAllPaths_depth();
		return 0;
	}

private:

};

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place | Prefix::Thread | Prefix::Module);
//		set_default_target(get_TargetToConsole());
		set_default_target(get_TargetToFile(L".\\log.log"));

//		set_module_enabled(false, get_module(L"threads"));
	}
}

int main()
{
	setup_logger();

	LogTrace();

	Settings::inst().init(L"");
	GlobalData::inst();

	Listener listenerRoutine;
	Worker workerRoutine(GlobalData::inst().get_WorkerTasksQueue());

	Base::ThreadPool pool;
	for (size_t i = 0; i < Settings::inst().numberOfWorkers(); ++i) {
		// create workers
		pool.emplace_back(&workerRoutine);
		Base::Thread & thread = pool.back();
		thread.set_priority(Settings::inst().workerPriority());
		thread.set_io_priority(Settings::inst().workerIoPriority());
		thread.resume();
	}
	for (size_t i = 0; i < Settings::inst().numberOfListeners(); ++i) {
		// create listeners
		pool.emplace_back(&listenerRoutine);
		Base::Thread & thread = pool.back();
		thread.set_priority(Settings::inst().listenerPriority());
		thread.resume();
	}

	WorkingTask::PathsArray paths;
	paths.emplace_back(L"c:\\xld");
	paths.emplace_back(L"c:\\FaceProv.log");
	paths.emplace_back(L"c:\\tools");
	paths.emplace_back(L"c:\\Intel");
	Base::Command_p * task = new Remove(paths);
	Base::Message message;
	message.set_data(task);
	GlobalData::inst().get_WorkerTasksQueue()->put_message(message);

	pool.wait_all(1000);
	for (Base::Thread & thread : pool)
		thread.alert();
	pool.wait_all();
}
