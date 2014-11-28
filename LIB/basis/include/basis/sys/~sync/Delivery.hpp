#ifndef BASIS_SYS_SYNC_DELIVERY_HPP_
#define BASIS_SYS_SYNC_DELIVERY_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

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

#endif
