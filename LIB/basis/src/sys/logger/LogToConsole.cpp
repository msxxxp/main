#include "logger_pvt.hpp"

#include <basis/sys/console.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/memory.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/simstd/memory>

namespace logger {

	namespace {

		WORD LogLevelColors[static_cast<ssize_t>(Level::Force) + 1] = {
			// TraceObject
			FOREGROUND_INTENSITY,
			// Trace2
			FOREGROUND_INTENSITY,
			// Trace1
			FOREGROUND_INTENSITY,
			// Debug2
			FOREGROUND_INTENSITY,
			// Debug1
			FOREGROUND_INTENSITY,
			// Info
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			// Report
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
			// Attention
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
			// Warning
			FOREGROUND_RED | FOREGROUND_GREEN,
			// Error
			FOREGROUND_RED | FOREGROUND_INTENSITY,
			// Fatal
			FOREGROUND_RED,
			// Alert
			FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			// Emergency
			FOREGROUND_RED | FOREGROUND_BLUE,
			// Force
			FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN,
		};

		struct LogToConsole: public Target_i, private pattern::Uncopyable {
			~LogToConsole();

			LogToConsole();

			void out(const Module * lgr, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const Module * lgr, WORD color, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const wchar_t* str, size_t size) const override;

			lock_type lock_scope() const override;

		private:
			mutable sync_type m_sync;
		};

		LogToConsole::~LogToConsole()
		{
			TraceFunc();
		}

		LogToConsole::LogToConsole()
		{
			TraceFunc();
		}

		void LogToConsole::out(const Module * lgr, Level lvl, const wchar_t* str, size_t size) const
		{
//			console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
			auto lockScope(lock_scope());
			if (lgr->is_color_mode()) {
				console::Color color(LogLevelColors[static_cast<ssize_t>(lvl)]);
				if (lgr->is_utf8_mode())
					console::puts8(str, size);
				else
					console::puts(str, size);
			} else {
				if (lgr->is_utf8_mode())
					console::puts8(str, size);
				else
					console::puts(str, size);
			}
		}

		void LogToConsole::out(const Module * lgr, WORD color, Level lvl, const wchar_t* str, size_t size) const
		{
//			console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
			auto lockScope(lock_scope());

			if (lgr->is_color_mode() && color < 16) {
				if (color == 0)
					color = LogLevelColors[static_cast<ssize_t>(lvl)];
				console::Color clr(color);
				if (lgr->is_utf8_mode())
					console::puts8(str, size);
				else
					console::puts(str, size);
			} else {
				if (lgr->is_utf8_mode())
					console::puts8(str, size);
				else
					console::puts(str, size);
			}
		}

		void LogToConsole::out(const wchar_t* str, size_t size) const
		{
//			console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
			auto lockScope(lock_scope());
			console::puts(str, size);
		}

		lock_type LogToConsole::lock_scope() const
		{
//			console::printf(L"%S():%d\n", __FUNCTION__, __LINE__);
			return simstd::auto_lock(m_sync);
		}

	}

	///=============================================================================================
	Target_t get_TargetToConsole()
	{
		static Target_t instance(new LogToConsole());
		return instance;
	}

}
