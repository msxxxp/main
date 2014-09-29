#ifndef BASIS_SYS_SYNC_HPP_
#define BASIS_SYS_SYNC_HPP_

#include <basis/configure.hpp>
#include <basis/ext/pattern.hpp>

namespace sync {

	typedef ssize_t Timeout_t;

	const Timeout_t WAIT_FOREVER = INFINITE;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};

}

#include <basis/sys/~sync/CriticalSection.hpp>
#include <basis/sys/~sync/Mutex.hpp>
#include <basis/sys/~sync/Semaphore.hpp>

namespace sync {
	struct ScopeGuard;

	///=============================================================================================
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

	///================================================================================== ScopeGuard
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

///=================================================================================================
//		struct SRWlock {
//			SRWlock() {
//				::InitializeSRWLock(&m_impl);
//			}
//		private:
//			SRWLOCK m_impl;
//		};
//
//		///=========================================================================================
//		struct SafeStack {
//			~SafeStack()
//			{
//				_aligned_free(m_impl);
//			}
//
//			SafeStack()
//			{
//				m_impl = (PSLIST_HEADER)_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT);
//				InitializeSListHead(m_impl);
//			}
//		private:
//			PSLIST_HEADER m_impl;
//		};
//

	///======================================================================== Forward declarations
	struct Message;
	class MessageManager;
	class Queue;
	class Observer;
	class Observable;

	///===================================================================================== Message
	struct Message {
		typedef ssize_t type_t;
		typedef ssize_t code_t;
		typedef ssize_t param_t;
		typedef void * data_t;

		static const type_t MASK_ALL_TYPES = ~static_cast<type_t>(0);
		static const code_t MASK_ALL_CODES = ~static_cast<code_t>(0);

		~Message();

		Message(const type_t & type = type_t(), const code_t & code = code_t(), const param_t & param = param_t(), const data_t & data = data_t());

		type_t get_type() const;

		code_t get_code() const;

		param_t get_param() const;

		data_t get_data() const;

		void set_type(const type_t & in);

		void set_code(const code_t & in);

		void set_param(const param_t & in);

		void set_data(const data_t & in);

	private:
		type_t  m_type;
		code_t  m_code;
		param_t m_param;
		data_t  m_data;
	};

	///======================================================================================= Queue
	class Queue: private pattern::Uncopyable {
	public:
		~Queue();

		Queue();

		Queue(Queue && right);

		Queue & operator = (Queue && right);

		void swap(Queue & right);

		void put_message(const Message & message);

		WaitResult_t get_message(Message & message, Timeout_t timeout_msec = WAIT_FOREVER);

	private:
		struct Queue_impl;
		Queue_impl * m_impl;
	};

	///============================================================================== MessageManager
	MessageManager * get_simple_message_manager();

	///==================================================================================== Observer
	class Observer {
	public:
		virtual ~Observer();

		virtual void notify(const Message & event) = 0;

		Observer():
			m_manager(get_simple_message_manager())
		{
		}

		Observer(MessageManager * manager) :
			m_manager(manager)
		{
		}

	private:
		MessageManager * m_manager;
	};

	///================================================================================== Observable
	class Observable {
	public:
		virtual ~Observable();

		Observable():
			m_manager(get_simple_message_manager()),
			m_changed(false)
		{
		}

		Observable(MessageManager * manager) :
			m_manager(manager),
			m_changed(false)
		{
		}

		void register_observer(Observer * observer);

		void unregister_observer(Observer * observer);

		void notify_all(const Message & event) const;

		void set_changed(bool changed) const;

		bool get_changed() const;

	private:
		MessageManager * m_manager;
		mutable bool m_changed;
	};

	///==================================================================================== Delivery
	namespace Delivery {
		typedef ssize_t SubscribtionId;

		typedef bool (*filter_t)(const Message & message);

		SubscribtionId Subscribe(Queue * queue, Message::type_t type_mask = Message::MASK_ALL_TYPES, Message::code_t code_mask = Message::MASK_ALL_CODES, filter_t filter = nullptr);

		void Unsubscribe(SubscribtionId id);

		void Unsubscribe(const Queue * queue);

		void SendRound(const Message & message);

		class Subscriber: private Queue {
		public:
			Subscriber(Message::type_t type_mask = Message::MASK_ALL_TYPES, Message::code_t code_mask = Message::MASK_ALL_CODES, filter_t filter = nullptr)
			{
				Subscribe(this, type_mask, code_mask, filter);
			}

			~Subscriber()
			{
				Unsubscribe(this);
			}
		};
	}

}

namespace totext {

	const wchar_t * c_str(sync::WaitResult_t waitResult);

}  // namespace totext

#endif
