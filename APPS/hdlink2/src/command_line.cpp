#include <command_line.hpp>
#include <console_messages.hpp>
#include <global.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

CommandLineParser::CommandLineParser(const wchar_t * cmdLine) :
	m_argv(::CommandLineToArgvW(cmdLine, &m_argc), ::LocalFree),
	m_action(new ShowHelp(m_argv.get()[0]))
{
	for (int i = 1; i < m_argc; ++i) {
		if (m_argv.get()[i] && cstr::length(m_argv.get()[i]) > 0 && (m_argv.get()[i][0] == L'/' || m_argv.get()[i][0] == L'-')) {
			const wchar_t * operation = m_argv.get()[i] + 1;

			if (cstr::compare_ci_1st_length(L"?", operation) == 0) {
				m_action.reset(new ShowHelp(m_argv.get()[0]));
				break;
			}
			if (cstr::compare_ci_1st_length(L"a", operation) == 0) {
				global::options().attrMustMatch = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"h", operation) == 0) {
				global::options().skipFileHidden = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"j", operation) == 0) {
				global::options().skipDirLink = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"l", operation) == 0) {
				global::options().doHardlink = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"m", operation) == 0) {
				global::options().skipFileSmall = 0;
				break;
			}
			if (cstr::compare_ci_1st_length(L"r", operation) == 0) {
				global::options().doRecursive = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"s", operation) == 0) {
				global::options().skipFileSystem = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"t", operation) == 0) {
				global::options().timeMustMatch = 1;
				break;
			}
			if (cstr::compare_ci_1st_length(L"w", operation) == 0) {
				global::options().showStatistics = 1;
				break;
			}
		}

//		if (cstr::compare(m_argv.get()[i], L"/l") == 0 && i < (m_argc - 1)) {
////				action.reset(new ArcList(arc_lib, argv[i + 1]));
//			break;
//		}
//
//		if (cstr::compare(m_argv.get()[i], L"/t") == 0 && i < (m_argc - 1)) {
////				action.reset(new ArcTest(arc_lib, argv[i + 1]));
//			break;
//		}
//
//		if (cstr::compare(m_argv.get()[i], L"/e") == 0 && i < (m_argc - 2)) {
////				action.reset(new ArcExtract(arc_lib, argv[i + 1], argv[i + 2]));
//			continue;
//		}
//
//		if (cstr::compare(m_argv.get()[i], L"/a") == 0 && i < (m_argc - 3)) {
////				action.reset(new ArcCompress(arc_lib, argv[i + 1], argv[i + 2], argv[i + 3]));
//			continue;
//		}
//
//		if (cstr::compare(m_argv.get()[i], L"/g") == 0 && i < (m_argc - 3)) {
////				action.reset(new ArcCompressPiped(arc_lib, argv[i + 1], argv[i + 3], argv[i + 2]));
//			continue;
//		}
	}

	LogTrace();
	global::vars().folders.emplace_back(file_system::Node_t(new file_system::Folder(L"c:\\sysint")));
	global::vars().folders.emplace_back(file_system::Node_t(new file_system::Folder(L"d:\\sysint")));
}

ssize_t CommandLineParser::execute()
{
	return m_action->execute();
}
