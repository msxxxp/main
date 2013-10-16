#include <libbase/std.hpp>
#include <liblog/logger.hpp>
#include <libbase/messaging.hpp>

struct GlobalData
{
	~GlobalData()
	{
		LogTrace();
	}

	GlobalData()
	{
		LogTrace();
	}

	static GlobalData & inst()
	{
		static GlobalData instance;
		return instance;
	}



	Base::Queue * get_WorkerTasksQueue()
	{
		return & m_workerTasksQueue;
	}

private:
	Base::Queue m_workerTasksQueue;
};
