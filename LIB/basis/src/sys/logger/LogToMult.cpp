#include "logger_pvt.hpp"

#include <basis/sys/sync.hpp>

namespace logger {

	namespace {

		struct LogToMult: public Target_i {
			LogToMult(const Target_t & first, const Target_t & second);

			~LogToMult();

			void out(const Module * lgr, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const Module * lgr, WORD color, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const wchar_t* str, size_t size) const override;

			lock_type lock_scope() const override;

		private:
			Target_t m_first;
			Target_t m_second;
		};

		LogToMult::LogToMult(const Target_t & first, const Target_t & second) :
			m_first(first),
			m_second(second)
		{
		}

		LogToMult::~LogToMult()
		{
		}

		void LogToMult::out(const Module * lgr, Level lvl, const wchar_t* str, size_t size) const
		{
			m_first->out(lgr, lvl, str, size);
			m_second->out(lgr, lvl, str, size);
		}

		void LogToMult::out(const Module * lgr, WORD color, Level lvl, const wchar_t* str, size_t size) const
		{
			m_first->out(lgr, color, lvl, str, size);
			m_second->out(lgr, color, lvl, str, size);
		}

		void LogToMult::out(const wchar_t* str, size_t size) const
		{
			m_first->out(str, size);
			m_second->out(str, size);
		}

		lock_type LogToMult::lock_scope() const
		{
			return m_first->lock_scope();
			//		m_second->lock();
		}

	}

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second)
	{
		return Target_t(new LogToMult(first, second));
	}

}
