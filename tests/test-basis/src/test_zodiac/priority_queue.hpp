#ifndef PRIORITY_QUEUE_HPP_
#define PRIORITY_QUEUE_HPP_

#include <basis/configure.hpp>

#include <map>

/*
 * Класс - интерфейс для сообщений очереди
 */
class IMessageQueueEvents {
public:
	virtual ~IMessageQueueEvents() = default;

	virtual void on_start() = 0;

	virtual void on_stop() = 0;

	virtual void on_hwm() = 0;

	virtual void on_lwm() = 0;
};

/*
 * Коды возврата
 */
enum class RetCodes: ssize_t {
	OK = 0,
	HWM = -1,
	NO_SPACE = -2,
	STOPPED = -3,
};

/*
 * Шаблонный класс блокирующей очереди с приоритетами
 */
template<typename Type>
class MessageQueue: private std::multimap<int, Type> {
public:
	typedef Type MessageType;
	typedef typename std::multimap<int, Type>::value_type value_type;

	~MessageQueue()
	{
		stop();
	}

	/*
	 * Инициализация членов класса, синхронизирующих объектов ядра
	 */
	MessageQueue(int queue_size, int lwm = 0, int hwm = 0) :
		m_size(queue_size), m_lwm(lwm > 0 ? lwm : m_size * 0.1), m_hwm(hwm > 0 ? hwm : m_size * 0.9), m_events(NULL), m_mutex(m_hnd[0]), m_allow_read(m_hnd[1]), m_state(RetCodes::STOPPED)
	{
		m_mutex = CreateMutex(NULL, FALSE, NULL);
		m_allow_read = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	/*
	 * Помещение сообщения в очередь
	 * В зависимости от текущего состояния очереди она может быть переведена
	 * в состояние HWM или NO_SPACE При достижении верхней границы hwm происходит событие on_hwm()
	 */
	RetCodes put(const MessageType& msg, int priority)
	{
		DWORD res = WaitForSingleObject(m_mutex, INFINITE);
		RetCodes ret = m_state;
		if (res == WAIT_OBJECT_0) {
			if (m_state == RetCodes::OK) {
				this->insert(value_type(priority, msg));
				SetEvent(m_allow_read);
				if (this->size() == static_cast<size_t>(m_size)) {
					m_state = RetCodes::NO_SPACE;
				} else if (this->size() >= static_cast<size_t>(m_hwm)) {
					m_state = RetCodes::HWM;
					if (m_events)
						m_events->on_hwm();
				}
				ret = RetCodes::OK;
			}
			ReleaseMutex(m_mutex);
		}
		return ret;
	}

	/*
	 * Извлечение сообщения из очереди
	 * В зависимости при достижении порога сообщений lwm очередь может быть
	 * переведена в состояние OK. При этом происходит событие on_lwm()
	 *
	 */
	RetCodes get(MessageType & msg)
	{
		DWORD res = WaitForMultipleObjects(m_num_hnd, m_hnd, TRUE, INFINITE);
		RetCodes ret = RetCodes::STOPPED;
		if (res == WAIT_OBJECT_0) {
			if (m_state != RetCodes::STOPPED) {
				msg = this->begin()->second;
				this->erase(this->begin());
				if (m_state == RetCodes::NO_SPACE && this->size() <= static_cast<size_t>(m_hwm)) {
					m_state = RetCodes::HWM;
				}
				if (m_state == RetCodes::HWM && this->size() <= static_cast<size_t>(m_lwm)) {
					if (m_events)
						m_events->on_lwm();
					m_state = RetCodes::OK;
				}
				if (this->empty())
					ResetEvent(m_allow_read);
				ret = RetCodes::OK;
			}
			ReleaseMutex(m_mutex);
		}
		return ret;
	}

	/*
	 * Принудительное переведение очереди в состояние STOPPED
	 * При этом происходит событие on_stop()
	 */
	void stop()
	{
		DWORD res = WaitForSingleObject(m_mutex, INFINITE);
		if (res == WAIT_OBJECT_0) {
			if (m_state != RetCodes::STOPPED) {
				m_state = RetCodes::STOPPED;
				SetEvent(m_allow_read);
				ReleaseMutex(m_mutex);
				if (m_events)
					m_events->on_stop();
			} else {
				ReleaseMutex(m_mutex);
			}
		}
	}

	/*
	 * Переведение очереди в рабочее состояние
	 * При этом происходит событие on_start()
	 */
	void run()
	{
		DWORD res = WaitForSingleObject(m_mutex, INFINITE);
		if (res == WAIT_OBJECT_0) {
			if (m_state == RetCodes::STOPPED) {
				if (this->size() >= static_cast<size_t>(m_size))
					m_state = RetCodes::NO_SPACE;
				else if (this->size() < static_cast<size_t>(m_hwm))
					m_state = RetCodes::OK;
				else
					m_state = RetCodes::HWM;
				if (m_events)
					m_events->on_start();
			}
			ReleaseMutex(m_mutex);
		}
	}

	/*
	 * Установка обработчика событий очереди
	 */
	void set_events(IMessageQueueEvents * events)
	{
		DWORD res = WaitForSingleObject(m_mutex, INFINITE);
		if (res == WAIT_OBJECT_0) {
			m_events = events;
			ReleaseMutex(m_mutex);
		}
	}

private:
	int m_size, m_lwm, m_hwm;
	IMessageQueueEvents* m_events;

	/*
	 * объекты синхронизации
	 * Мьютекс для разграничения одновременного доступа к ресурсам очереди
	 * Событие для блокирования вызова get() при пустой очереди
	 */
	static const size_t m_num_hnd = 2; //
	HANDLE m_hnd[m_num_hnd];
	HANDLE &m_mutex, &m_allow_read;

	RetCodes m_state;
};

#endif
