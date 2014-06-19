#include <basis/sys/console.hpp>

namespace console {

	Color::Color(WORD color) :
		m_color(0)
	{
		if (color && save())
			::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), color);
	}

	void Color::restore()
	{
		if (m_color) {
			::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), m_color);
			m_color = 0;
		}
	}

	bool Color::save()
	{
		CONSOLE_SCREEN_BUFFER_INFO tmp;
		if (::GetConsoleScreenBufferInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &tmp)) {
			m_color = tmp.wAttributes;
		}
		return m_color;
	}

}
