#include <liblog/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/memory.hpp>
#include <libbase/console.hpp>

namespace Logger {

	WORD LogLevelColors[(int)Level::Force + 1] = {
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
		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	LogToConsole::LogToConsole() :
		m_sync(Base::Lock::get_CritSection())
	{
		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	void LogToConsole::out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const
	{
		auto lk(m_sync->lock_scope());
		if (lgr->is_color_mode()) {
			Base::Console::Color color(LogLevelColors[(int)lvl]);
			Base::Console::out(str, size);
		} else {
			Base::Console::out(str, size);
		}
	}

	void LogToConsole::out(PCWSTR str, size_t size) const
	{
		auto lk(m_sync->lock_scope());
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
