#ifndef HDLINK_COMMAND_LINE_HPP_
#define HDLINK_COMMAND_LINE_HPP_

#include <basis/ext/pattern.hpp>

#include <basis/std/memory>

struct CommandLineParser: public pattern::Command {
//	typedef simstd::unique_ptr<wchar_t*, HLOCAL (*)(HLOCAL)> Argv_t;
	typedef simstd::shared_ptr<wchar_t*>         Argv_t;
	typedef simstd::shared_ptr<pattern::Command> Action_t;

	CommandLineParser(const wchar_t * cmdLine);

	ssize_t execute();

private:
	Argv_t   m_argv;
	Action_t m_action;
	int      m_argc;
};

#endif
