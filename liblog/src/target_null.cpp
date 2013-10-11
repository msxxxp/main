#include <liblog/logger.hpp>

namespace Logger {

	struct LogToNull: public Target_i {
		~LogToNull();

		void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

		void out(PCWSTR str, size_t size) const override;

		void lock() const override;

		void unlock() const override;
	};

	LogToNull::~LogToNull()
	{
	}

	void LogToNull::out(const Module_i * /*lgr*/, Level /*lvl*/, PCWSTR /*str*/, size_t /*size*/) const
	{
	}

	void LogToNull::out(PCWSTR /*str*/, size_t /*size*/) const
	{
	}

	void LogToNull::lock() const
	{
	}

	void LogToNull::unlock() const
	{
	}

	Target_t get_TargetToNull()
	{
		static Target_t instance(new LogToNull());
		return instance;
	}

}
