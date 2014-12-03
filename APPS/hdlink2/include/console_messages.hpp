#ifndef HDLINK_CONSOLE_MESSAGES_HPP_
#define HDLINK_CONSOLE_MESSAGES_HPP_

#include <basis/ext/pattern.hpp>

struct ShowStatistics: public pattern::Command {
	ssize_t execute();
};

struct ShowHelp: public pattern::Command {
	ShowHelp(const wchar_t * prgName);

	ssize_t execute();
private:
	const wchar_t * m_prgName;
};

#endif
