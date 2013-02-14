#include <libbase/logger.hpp>

namespace Base {
	namespace Logger {

		struct LogToNull: public Target_i {
			~LogToNull();

			void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

			void out(PCWSTR str, size_t size) const override;
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

		Target_i * get_TargetToNull()
		{
			return new LogToNull();
		}

	}
}
