//============================================================================
// Name        : 7-zip.cpp
// Author      : Andrew Grechkin
// Version     :
// Copyright   : Your copyright notice
// Description : 7-zip example
//============================================================================

#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>
#include <libbase/console.hpp>
#include <liblog/logger.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/command_p.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Full/* | Prefix::Place*/);
		set_default_target(get_TargetToFile(L"test-7zip.log", true));
//
		set_module_prefix(Prefix::Full, get_module(L"SevenZip"));
		set_module_target(get_TargetToFile(L"test-SevenZip.log", true), get_module(L"SevenZip"));
		set_module_level(Level::Info, get_module(L"SevenZip"));
	}
}

struct ArcInfo: public Base::Command_p {
	ArcInfo(const SevenZip::Lib & lib) :
		arc_lib(lib)
	{
	}

	ssize_t execute()
	{
		Base::Console::printf(L"\nSupported methods (%Id):\n", arc_lib.methods().size());
		for (auto it = arc_lib.methods().begin(); it != arc_lib.methods().end(); ++it) {
			Base::Console::printf(L"%16I64d\tname: %-10.10s\n", it->id, it->name.c_str());
		}

		Base::Console::printf(L"\nSupported archive formats (%Id):\n", arc_lib.codecs().size());
		for (auto it = arc_lib.codecs().begin(); it != arc_lib.codecs().end(); ++it) {
			Base::Console::printf(L"%8s\tupd: %d\text: %16s\t add_ext: %s\tguid: %s\n", it->name.c_str(), it->updatable, it->ext.c_str(), it->add_ext.c_str(), it->guid.as_str().c_str());
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
};

struct ArcCompressPiped: public Base::Command_p {
	ArcCompressPiped(const SevenZip::Lib & lib, const ustring & destPath, const ustring & midName, const ustring & srcPath) :
		arc_lib(lib),
		m_destPath(destPath),
		m_midName(midName),
		m_srcPath(srcPath)
	{
		LogNoise(L"%p, '%s', '%s'\n", &lib, destPath.c_str(), srcPath.c_str());
	}

	ssize_t execute()
	{
		ustring midCodec = L"tar";
		ustring destCodec = L"gzip";

		ustring midName(m_midName + L"." + midCodec);
		ustring fullPath(m_destPath + L"." + midCodec + L"." + destCodec);
		if (!Fsys::File::is_exist(fullPath)) {
			Base::Console::printf(L"Compressing to '%s':\n", fullPath.c_str());
			SevenZip::CompressProperties midProps;
			midProps.add(m_srcPath);
			midProps.codec = midCodec;

			SevenZip::CompressProperties destProps;
			destProps.add_virtual(midName);
			destProps.codec = destCodec;
			destProps.level = SevenZip::CompressLevel::STORE;
			destProps.silent = false;
//			destProps.password = L"7z";
			destProps.encrypt_header = false;
			destProps.solid = true;

			SevenZip::CreateArchivePiped(arc_lib, midProps, fullPath, destProps).execute();
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
	ustring m_destPath;
	ustring m_midName;
	ustring m_srcPath;
};

struct ArcCompress: public Base::Command_p {
	ArcCompress(const SevenZip::Lib & lib, const ustring & path, const ustring & what, const ustring & codec) :
		arc_lib(lib),
		m_path(path),
		m_what(what),
		m_codec(codec)
	{
		LogNoise(L"%p, '%s', '%s', '%s'\n", &lib, path.c_str(), what.c_str(), codec.c_str());
	}

	ssize_t execute()
	{
		ustring full_path(m_path + L"." + m_codec);
		if (!Fsys::File::is_exist(full_path)) {
			Base::Console::printf(L"Compressing to '%s':\n", full_path.c_str());
			SevenZip::CompressProperties props;
			props.add(m_what);
			props.codec = m_codec;
			props.level = SevenZip::CompressLevel::FASTEST;
			props.silent = false;
//			props.password = L"7z";
			props.encrypt_header = false;
			props.solid = true;

			SevenZip::CreateArchive(arc_lib, full_path, props).execute();
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
	ustring m_path;
	ustring m_what;
	ustring m_codec;
};

struct ArcList: public Base::Command_p {
	ArcList(const SevenZip::Lib & lib, const ustring & path, bool full = false) :
		arc_lib(lib),
		m_path(path),
		m_full(full)
	{
	}

	ssize_t execute()
	{
		if (Fsys::File::is_exist(m_path)) {
			SevenZip::ArchiveSequence archive(arc_lib, m_path);
			Base::Console::printf(L"\nArchive information: %s\tCodec: %s\n", m_path.c_str(), archive.codec().name.c_str());
			Base::Console::printf(L"Number of archive properties: %Id\n", archive.props().size());
			for (auto it = archive.props().begin(); it != archive.props().end(); ++it) {
				Base::Console::printf(L"'%s'\t%s %d\t'%s'\n", it->name.c_str(), Base::NamedValues<int>::GetName(SevenZip::ArcItemPropsNames, Base::lengthof(SevenZip::ArcItemPropsNames), it->id), it->id, it->prop.as_str().c_str());
			}

			Base::Console::printf(L"\nListing(%Id):\n", archive.size());
			for (size_t i = 0; i < archive.size(); ++i) {
				auto it = archive.at(i);
				Base::Console::printf(L"%4Id IsDir: %d\tSize: %8Id\tPath: '%s'\n", i, it.is_dir(), it.size(), it.path().c_str());
				for (size_t j = 0; m_full && j < it.get_props_count(); ++j) {
					ustring name;
					PROPID id;
					if (it.get_prop_info(j, name, id)) {
						Base::Console::printf(L"'%s'\t%s %I64d\t%s\n", name.c_str(), Base::NamedValues<int>::GetName(SevenZip::ArcItemPropsNames, Base::lengthof(SevenZip::ArcItemPropsNames), id), id,
						         it.get_prop(id).as_str().c_str());
					}
				}
			}
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
	ustring m_path;
	bool m_full;
};

struct ArcTest: public Base::Command_p {
	ArcTest(const SevenZip::Lib & lib, const ustring & path) :
		arc_lib(lib),
		m_path(path)
	{
	}

	ssize_t execute()
	{
		if (Fsys::File::is_exist(m_path)) {
			Base::Console::printf(L"Testing:\n");
			ssize_t ret = SevenZip::ArchiveTest(arc_lib, m_path).execute();
			Base::Console::printf(L"Errors: %Id\n", ret);
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
	ustring m_path;
};

struct ArcExtract: public Base::Command_p {
	ArcExtract(const SevenZip::Lib & lib, const ustring & path, const ustring & where) :
		arc_lib(lib),
		m_path(path),
		m_where(where)
	{
	}

	ssize_t execute()
	{
		if (Fsys::File::is_exist(m_path)) {
			Base::Console::printf(L"\nExtracting:\n");
			ssize_t ret = SevenZip::ArchiveExtract(arc_lib, m_path, m_where).execute();
			Base::Console::printf(L"Errors: %Id\n", ret);
		}
		return true;
	}

private:
	const SevenZip::Lib & arc_lib;
	ustring m_path;
	ustring m_where;
};

struct ShowHelp: public Base::Command_p {
	ShowHelp(PCWSTR prg) :
		m_prg(Base::filename_only(prg))
	{
	}

	ssize_t execute()
	{
		Base::Console::printf(L"Простой пример работы с 7-zip\nAndrew Grechkin, 2012\n");
		Base::Console::printf(L"\nИнфо:\n");
		Base::Console::printf(L"\t%s /i\n", m_prg.c_str());
		Base::Console::printf(L"Лист:\n");
		Base::Console::printf(L"\t%s /l arc.name\n", m_prg.c_str());
		Base::Console::printf(L"Тест:\n");
		Base::Console::printf(L"\t%s /t arc.name\n", m_prg.c_str());
		Base::Console::printf(L"Распаковка:\n");
		Base::Console::printf(L"\t%s /e arc.name dest.path\n", m_prg.c_str());
		Base::Console::printf(L"Запаковка:\n");
		Base::Console::printf(L"\t%s /a arc.name src.path tar\n", m_prg.c_str());
		return true;
	}

private:
	ustring m_prg;
};

struct CmdParser: public Base::Command_p {
	CmdParser(PWSTR cmd_line, const SevenZip::Lib & arc_lib) :
		argv(::CommandLineToArgvW(cmd_line, &argc), LocalFree), action(new ShowHelp(argv[0]))
	{
		for (int i = 1; i < argc; ++i) {
			if (Base::Str::compare(argv[i], L"/?") == 0) {
				action.reset(new ShowHelp(argv[0]));
				break;
			}

			if (Base::Str::compare(argv[i], L"/i") == 0) {
				action.reset(new ArcInfo(arc_lib));
				break;
			}

			if (Base::Str::compare(argv[i], L"/l") == 0 && i < (argc - 1)) {
				action.reset(new ArcList(arc_lib, argv[i + 1]));
				break;
			}

			if (Base::Str::compare(argv[i], L"/t") == 0 && i < (argc - 1)) {
				action.reset(new ArcTest(arc_lib, argv[i + 1]));
				break;
			}

			if (Base::Str::compare(argv[i], L"/e") == 0 && i < (argc - 2)) {
				action.reset(new ArcExtract(arc_lib, argv[i + 1], argv[i + 2]));
				continue;
			}

			if (Base::Str::compare(argv[i], L"/a") == 0 && i < (argc - 3)) {
				action.reset(new ArcCompress(arc_lib, argv[i + 1], argv[i + 2], argv[i + 3]));
				continue;
			}

			if (Base::Str::compare(argv[i], L"/g") == 0 && i < (argc - 3)) {
				action.reset(new ArcCompressPiped(arc_lib, argv[i + 1], argv[i + 3], argv[i + 2]));
				continue;
			}
		}
	}

	ssize_t execute()
	{
		return action->execute();
	}

private:
	Base::auto_close<PWSTR*> argv;
	std::shared_ptr<Command_p> action;
	int argc;
};

int main()
try
{
	setup_logger();

	SevenZip::Lib arc_lib(L"7z.dll");
	Base::Console::printf(L"7-zip library version: %s, '%s'\n", arc_lib.get_version().c_str(), arc_lib.get_path().c_str());

	CmdParser(::GetCommandLineW(), arc_lib).execute();

	return 0;
}
catch (Ext::AbstractError & e) {
	LogFatal(L"%s\n", e.what().c_str());
	LogFatal(L"%s\n", e.where());
	return 2;
}
catch (std::exception & e) {
	LogFatal(L"std::exception [%S]:\n", typeid(e).name());
	LogFatal(L"What: %S\n", e.what());
	return 1;
}
