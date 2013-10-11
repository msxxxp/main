#include <liblog/logger.hpp>

namespace Logger {

	struct LogToMult: public Target_i {
		LogToMult(const Target_t & first, const Target_t & second);

		~LogToMult();

		void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

		void out(PCWSTR str, size_t size) const override;

		void lock() const override;

		void unlock() const override;
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

	void LogToMult::out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const
	{
		m_first->out(lgr, lvl, str, size);
		m_second->out(lgr, lvl, str, size);
	}

	void LogToMult::out(PCWSTR str, size_t size) const
	{
		m_first->out(str, size);
		m_second->out(str, size);
	}

	void LogToMult::lock() const
	{
		m_first->lock();
		m_second->lock();
	}

	void LogToMult::unlock() const
	{
		m_first->unlock();
		m_second->unlock();
	}

	Target_t get_TargetToMult(const Target_t & first, const Target_t & second)
	{
		return Target_t(new LogToMult(first, second));
	}

}
