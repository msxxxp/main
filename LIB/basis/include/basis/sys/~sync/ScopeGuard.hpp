#ifndef BASIS_SYS_SYNC_SCOPEGUARD_HPP_
#define BASIS_SYS_SYNC_SCOPEGUARD_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	struct ScopeGuard;

	struct SyncUnit_i: public pattern::Destroyable, private pattern::Uncopyable {
		ScopeGuard lock_scope();

		ScopeGuard lock_scope_read();

		void destroy() const override;

		void lock(bool readOnly = false) {readOnly ? _lock_read() : _lock();}

		bool try_lock(bool readOnly = false) {return readOnly ? _try_lock_read() : _try_lock();}

		void unlock() {_unlock();}

	protected:
		SyncUnit_i() = default;
		virtual ~SyncUnit_i() = default;

	private:
		virtual void _lock() = 0;

		virtual void _lock_read() = 0;

		virtual bool _try_lock() = 0;

		virtual bool _try_lock_read() = 0;

		virtual void _unlock() = 0;
	};

	SyncUnit_i * get_CritSection();

	SyncUnit_i * get_ReadWrite();

	struct ScopeGuard: private pattern::Uncopyable {
		~ScopeGuard();

		ScopeGuard();

		ScopeGuard(SyncUnit_i * unit, bool readOnly = false);

		ScopeGuard(ScopeGuard && right);

		ScopeGuard & operator =(ScopeGuard && right);

		void swap(ScopeGuard & right);

	private:
		SyncUnit_i * m_unit;
	};

}

#endif
