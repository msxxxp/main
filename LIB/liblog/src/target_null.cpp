#include <liblog/logger.hpp>
//#include <libbase/console.hpp>

namespace Logger {

	struct LogToNull: public Target_i {
		~LogToNull();

		LogToNull();

		void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

		void out(const wchar_t * str, size_t size) const override;

		Lock::ScopeGuard lock_scope() const override;
	};

	LogToNull::~LogToNull()
	{
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	LogToNull::LogToNull()
	{
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}

	void LogToNull::out(const Module_i * /*lgr*/, Level /*lvl*/, const wchar_t * /*str*/, size_t /*size*/) const
	{
	}

	void LogToNull::out(const wchar_t * /*str*/, size_t /*size*/) const
	{
	}

	Lock::ScopeGuard LogToNull::lock_scope() const
	{
		return Lock::ScopeGuard();
	}

	Target_t get_TargetToNull()
	{
		static Target_t instance(new LogToNull());
		return instance;
	}

}
