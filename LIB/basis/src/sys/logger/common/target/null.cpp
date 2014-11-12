#include "../../logger_pvt.hpp"

#include <basis/sys/sync.hpp>

#include <basis/simstd/mutex>

namespace logger {

	namespace {
		struct LogToNull: public Target_i {
			~LogToNull() = default;

			LogToNull() = default;

			void out(const Module * lgr, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const Module * lgr, WORD color, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const wchar_t * str, size_t size) const override;

			lock_type lock_scope() const override;
		};

//		LogToNull::~LogToNull()
//		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		}

//		LogToNull::LogToNull()
//		{
//			console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		}

		void LogToNull::out(const Module * /*lgr*/, Level /*lvl*/, const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		void LogToNull::out(const Module * /*lgr*/, WORD /*color*/, Level /*lvl*/, const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		void LogToNull::out(const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		lock_type LogToNull::lock_scope() const
		{
			return simstd::move(lock_type(simstd::defer_lock));
		}
	}

	Target_t get_TargetToNull()
	{
		static Target_t instance(new LogToNull());
		return instance;
	}

}
