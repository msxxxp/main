#include "../../logger_pvt.hpp"

#include <basis/sys/sync.hpp>

namespace logger {

	namespace {
		struct LogToNull: public Target_i {
			~LogToNull();

			LogToNull();

			void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const Module_i * lgr, WORD color, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const wchar_t * str, size_t size) const override;

			sync::ScopeGuard lock_scope() const override;
		};

		LogToNull::~LogToNull()
		{
			//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		}

		LogToNull::LogToNull()
		{
			//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		}

		void LogToNull::out(const Module_i * /*lgr*/, Level /*lvl*/, const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		void LogToNull::out(const Module_i * /*lgr*/, WORD /*color*/, Level /*lvl*/, const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		void LogToNull::out(const wchar_t * /*str*/, size_t /*size*/) const
		{
		}

		sync::ScopeGuard LogToNull::lock_scope() const
		{
			return sync::ScopeGuard();
		}
	}

	Target_t get_TargetToNull()
	{
		static Target_t instance(new LogToNull());
		return instance;
	}

}
