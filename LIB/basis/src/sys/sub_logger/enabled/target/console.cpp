#include "../../logger_pvt.hpp"

#include <basis/sys/console.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/memory.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/std/memory>

namespace logger {

	namespace {

		WORD LogLevelColors[static_cast<ssize_t>(Level::Force) + 1] = {
			// Trace
			FOREGROUND_INTENSITY,
			// Debug
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			// Info
			FOREGROUND_INTENSITY | FOREGROUND_GREEN,
			// Report
			FOREGROUND_GREEN,
			// Attention
			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
			// Warning
			FOREGROUND_RED | FOREGROUND_GREEN,
			// Error
			FOREGROUND_INTENSITY | FOREGROUND_RED,
			// Fatal
			FOREGROUND_RED,
			// Alert
			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
			// Emergency
			FOREGROUND_RED | FOREGROUND_BLUE,
			// Force
			FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN,
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

	}

	///=============================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
