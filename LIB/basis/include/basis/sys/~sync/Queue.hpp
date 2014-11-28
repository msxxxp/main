#ifndef BASIS_SYS_SYNC_QUEUE_HPP_
#define BASIS_SYS_SYNC_QUEUE_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class Queue: private pattern::Uncopyable {
	public:
		~Queue();

		Queue();

		Queue(Queue&& right);

		Queue& operator =(Queue&& right);

		void swap(Queue& right);

		void put_message(const Message& message);

		WaitResult_t get_message(Message& message, Timeout_t timeout_msec = WAIT_FOREVER);

	private:
		struct Queue_impl;
		Queue_impl* m_impl;
	};

}

#endif
