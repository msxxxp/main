#include <libbase/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/memory.hpp>
#include <libbase/console.hpp>

namespace Logger {

	WORD LogLevelColors[(int)Level::Logger + 1] = {
		0x8,
		0x6,
		0,
		0x3,
		0xA,
		0xE,
		0xC,
		0x4,
		0,
	};

	struct LogToConsole: public Target_i, private Base::Uncopyable {
		~LogToConsole();

		LogToConsole();

		void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

		void out(PCWSTR str, size_t size) const override;

		void lock() const override;

		void unlock() const override;

	private:
		Base::auto_destroy<Base::Lock::SyncUnit_i*> m_sync;
	};

	LogToConsole::~LogToConsole()
	{
	}

	LogToConsole::LogToConsole() :
		m_sync(Base::Lock::get_CritSection())
	{
	}

	void LogToConsole::out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const
	{
		auto lk(m_sync->lock_scope());
		if (lgr->is_color_mode()) {
			Base::ConsoleColor color(LogLevelColors[(int)lvl]);
			Base::consoleout(str, size);
		} else {
			Base::consoleout(str, size);
		}
	}

	void LogToConsole::out(PCWSTR str, size_t size) const
	{
		auto lk(m_sync->lock_scope());
		Base::consoleout(str, size);
	}

	void LogToConsole::lock() const
	{
		m_sync->lock();
	}

	void LogToConsole::unlock() const
	{
		m_sync->release();
	}

	///=========================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
