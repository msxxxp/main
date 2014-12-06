#ifndef TEST_ZODIAC_HPP_
#define TEST_ZODIAC_HPP_

#include <basis/configure.hpp>
#include <basis/simstd/string>

#include "priority-queue.hpp"

class Reader;
class Writer;

/*
 * Тип сообщений
 */
typedef simstd::wstring MessageType;

/*
 * Главный класс приложения
 * инициализует все необходимые объекты и управляет их поведением
 * Так же является предоставляет обработчики Событий очереди
 */
class Main: public IMessageQueueEvents {
public:
	/*
	 * Разрушение объектов писателей, читателей и очереди
	 */
	~Main();

	/*
	 * Конструирование объектов очереди, писателей и читателей
	 * Установка обработчика событий очереди
	 */
	Main();

	/*
	 * запуск процесса тестирования очереди
	 */
	int main();

	/*
	 * Обработчик события запуска очереди
	 * запуск потоков писателей и читателей
	 */
	void on_start();

	/*
	 * Обработчик события остановки очереди
	 * ожидание завершения потоков писателей и читателей
	 */
	void on_stop();

	/*
	 * Обработчик события превышения количества сообщений в очереди
	 * перевод всех писателей в состояние ожидания
	 */
	void on_hwm();

	/*
	 * Обработчик события уменьшения сообщений в очереди до необходимого минимума
	 * запуск всех писателей
	 */
	void on_lwm();

private:
	MessageQueue<MessageType>* queue;

	static const int num_of_readers = 2;
	static const int num_of_writers = 3;
	Reader* m_readers[num_of_readers];
	Writer* m_writers[num_of_writers];
};

/*
 * Класс Reader
 * Получает сообщения с небольшой задержкой из очереди
 * m_thread - хэндл потока
 * m_que - очередь из которой объект получает сообщения
 * m_num - необязательный член, используется просто для порядковой идентификации потока
 */
class Reader {
public:
	Reader(MessageQueue<MessageType>& que, int num);

	/*
	 * Функция, запускающая поток на выполнение
	 */
	void run();

	/*
	 * Для возможности слежения за запущеными потоками, необходимо иметь возможность получить
	 * их HANDLE
	 */
	operator HANDLE() const;

protected:
	/*
	 * Полученное сообщение просто выводится на экран
	 */
	void handle_message(const MessageType& msg);

private:
	/*
	 * Собственно сама функция выполняемая в отдельном потоке
	 * В бесконечном цикле пытается получить сообщение из очереди
	 * При получении кода ошибки STOPPED - поток завершается
	 */
	static DWORD WINAPI thread_func(PVOID data);

	MessageQueue<MessageType>& m_que;
	HANDLE m_thread;
	int m_num;
};

/*
 * Класс Writer
 * Посылает сообщения с небольшой задержкой в очередь
 * Мной было принято решение для данного теста для управления состоянием всех Writer использовать
 * лишь одно событие. В задании это не оговорено.
 * m_sending - Event для управления состоянием всех объектов Writer sleep/sending
 * m_thread - хэндл потока
 * m_que - очередь в которую объект помещает сообщения
 * m_num - необязательный член, используется просто для порядковой идентификации потока
 */
class Writer {
public:
	Writer(MessageQueue<MessageType>& que, int num);

	/*
	 * Функция, запускающая поток на выполнение
	 */
	void run();

	/*
	 * Для возможности слежения за запущеными потоками, необходимо иметь возможность получить
	 * их HANDLE
	 */
	operator HANDLE() const;

	/*
	 * Перевод всех потоков Writer в состояние ожидания освобождения очереди
	 */
	static void sleep();

	/*
	 * Перевод всех потоков Writer в активное состояние
	 */
	static void sending();

private:
	/*
	 * Собственно сама функция выполняемая в отдельном потоке
	 * В бесконечном цикле пытается посылать сообщения в очередь
	 * при этом приоритет сообщения генерируется случайно
	 * При получении кода ошибки STOPPED - поток завершается
	 */
	static DWORD WINAPI thread_func(PVOID data);

	static HANDLE m_sending;

	MessageQueue<MessageType>& m_que;
	HANDLE m_thread;
	int m_num;
};

#endif
