#include <liblog/logger.hpp>

#include <libbase/console.hpp>
#include <libbase/lock.hpp>
#include <libbase/memory.hpp>

namespace Logger {

	WORD LogLevelColors[static_cast<ssize_t>(Level::Force) + 1] = {
		0x08,
		0x06,
		0x00,
		0x03,
		0x0A,
		0x0E,
		0x0C,
		0x04,
		0x00,
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
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	LogToConsole::LogToConsole() :
		m_sync(Base::Lock::get_CritSection())
	{
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	void LogToConsole::out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
		if (lgr->is_color_mode()) {
			Base::Console::Color color(LogLevelColors[static_cast<ssize_t>(lvl)]);
			Base::Console::out(str, size);
		} else {
			Base::Console::out(str, size);
		}
	}

	void LogToConsole::out(PCWSTR str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
		Base::Console::out(str, size);
	}

	void LogToConsole::lock() const
	{
		m_sync->lock();
	}

	void LogToConsole::unlock() const
	{
		m_sync->release();
	}

	///=============================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
