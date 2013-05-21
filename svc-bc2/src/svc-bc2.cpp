#include <libbase/std.hpp>
#include <libbase/logger.hpp>
#include <libbase/messaging.hpp>
#include <libbase/thread.hpp>
#include <libbase/ThreadPool.hpp>
#include <libext/priv.hpp>

#include <GlobalData.hpp>
#include <Settings.hpp>
#include <SourceOnlyTask.hpp>
#include <SourceDestinationTask.hpp>

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

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium/* | Prefix::Place | Prefix::Thread | Prefix::Module*/);
		//		set_default_target(get_TargetToConsole());
		set_default_target(get_TargetToFile(L".\\log.log"));

		//		set_module_enabled(false, get_module(L"threads"));
	}
}

int main()
{
	setup_logger();

	LogTrace();
	Ext::Privilege CreateSymlinkPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME);
	Ext::Privilege BackupPrivilege(SE_BACKUP_NAME);
	Ext::Privilege RestorePrivilege(SE_RESTORE_NAME);

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

	{
		WorkingTask::PathsArray paths;
		paths.emplace_back(L"c:\\mingw");
		paths.emplace_back(L"c:\\Recovery");
		paths.emplace_back(L"c:\\tmp");
		paths.emplace_back(L"c:\\tools");
		paths.emplace_back(L"c:\\svc-bc2.cpp");
		paths.emplace_back(L"c:\\svc-bc2.cpp.link");
		Base::Command_p * task = new CopyTask(paths, L"c:\\tmp1\\");
		Base::Message message;
		message.set_data(task);
		GlobalData::inst().get_WorkerTasksQueue()->put_message(message);
	}

	{
		WorkingTask::PathsArray paths;
		paths.emplace_back(L"c:\\tmp1");
		Base::Command_p * task = new DeleteTask(paths);
		Base::Message message;
		message.set_data(task);
		GlobalData::inst().get_WorkerTasksQueue()->put_message(message);
	}

	pool.wait_all(5000);
	for (Base::Thread & thread : pool)
		thread.alert();
	pool.wait_all();
}
