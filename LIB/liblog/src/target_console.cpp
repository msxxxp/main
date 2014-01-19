#include <liblog/logger.hpp>

#include <libbase/console.hpp>
#include <libbase/lock.hpp>
#include <libbase/memory.hpp>

#include <patterns/Uncopyable.hpp>

namespace Logger {

	WORD LogLevelColors[static_cast<ssize_t>(Level::Force) + 1] = {
		FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN,
		0x00,
		FOREGROUND_BLUE | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		FOREGROUND_INTENSITY | FOREGROUND_RED,
		FOREGROUND_RED,
		0x00,
	};

	struct LogToConsole: public Target_i, private Pattern::Uncopyable {
		~LogToConsole();

		LogToConsole();

		void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

		void out(const wchar_t * str, size_t size) const override;

		Lock::ScopeGuard lock_scope() const override;

	private:
		Base::auto_destroy<Lock::SyncUnit_i*> m_sync;
	};

	LogToConsole::~LogToConsole()
	{
//		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
	}

	LogToConsole::LogToConsole() :
		m_sync(Lock::get_CritSection())
	{
//		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
	}

	void LogToConsole::out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
//		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		if (lgr->is_color_mode()) {
			Base::Console::Color color(LogLevelColors[static_cast<ssize_t>(lvl)]);
			Base::Console::out(str, size);
		} else {
			Base::Console::out(str, size);
		}
	}

	void LogToConsole::out(const wchar_t * str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
//		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		Base::Console::out(str, size);
	}

	Lock::ScopeGuard LogToConsole::lock_scope() const
	{
//		Base::Console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		return m_sync->lock_scope();
	}

	///=============================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
