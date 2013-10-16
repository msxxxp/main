#ifndef _LIBBASE_MESSAGING_HPP_
#define _LIBBASE_MESSAGING_HPP_

#include <libbase/std.hpp>

namespace Base {

	///======================================================================== Forward declarations
	struct Message;
	struct MessageManager;
	class Queue;
	class Observer;
	class Observable;

	///===================================================================================== Message
	struct Message {
		typedef ssize_t type_t;
		typedef ssize_t code_t;
		typedef ssize_t param_t;
		typedef void * data_t;

		static const type_t ALL_TYPES = ~(type_t)0;
		static const code_t ALL_CODES = ~(code_t)0;

		~Message();

		Message(const type_t & type = type_t(), const code_t & code = code_t(), const param_t & param = param_t(), const data_t & data = nullptr);

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
	class Queue: private Base::Uncopyable {
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

		SubscribtionId Subscribe(Queue * queue, Message::type_t type_mask = Message::ALL_TYPES, Message::code_t code_mask = Message::ALL_CODES, filter_t filter = nullptr);

		void Unsubscribe(SubscribtionId id);

		void Unsubscribe(const Queue * queue);

		void SendRound(const Message & message);

		class Subscriber: private Queue {
		public:
			Subscriber(Message::type_t type_mask = Message::ALL_TYPES, Message::code_t code_mask = Message::ALL_CODES, filter_t filter = nullptr)
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

#endif
