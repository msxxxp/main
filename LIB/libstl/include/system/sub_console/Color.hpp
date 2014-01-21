#ifndef LIBBASE_CONSOLE_COLOR_HPP_
#define LIBBASE_CONSOLE_COLOR_HPP_

namespace console {

	struct Color {
		~Color();

		Color(WORD color);

		void restore();

	private:
		bool save();

		WORD m_color;
	};

}

#endif
