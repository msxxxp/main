#include <liblog/logger.hpp>

#include <system/console.hpp>
#include <system/sync.hpp>
#include <simstd/memory>
#include <extra/pattern.hpp>

namespace logger {

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

	struct LogToConsole: public Target_i, private pattern::Uncopyable {
		~LogToConsole();

		LogToConsole();

		void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

		void out(const wchar_t * str, size_t size) const override;

		sync::ScopeGuard lock_scope() const override;

	private:
		memory::auto_destroy<sync::SyncUnit_i*> m_sync;
	};

	LogToConsole::~LogToConsole()
	{
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
	}

	LogToConsole::LogToConsole() :
		m_sync(sync::get_CritSection())
	{
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
	}

	void LogToConsole::out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		if (lgr->is_color_mode()) {
			console::Color color(LogLevelColors[static_cast<ssize_t>(lvl)]);
			console::puts(str, size);
		} else {
			console::puts(str, size);
		}
	}

	void LogToConsole::out(const wchar_t * str, size_t size) const
	{
		auto lockScope(m_sync->lock_scope());
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		console::puts(str, size);
	}

	sync::ScopeGuard LogToConsole::lock_scope() const
	{
//		console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
		return m_sync->lock_scope();
	}

	///=============================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
