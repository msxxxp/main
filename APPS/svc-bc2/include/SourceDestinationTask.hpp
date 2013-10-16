#ifndef SOURCEDESTINATIONTASK_HPP_
#define SOURCEDESTINATIONTASK_HPP_

#include <WorkingTask.hpp>

struct SourceDestinationTask: public WorkingTask {
	SourceDestinationTask(const ustring & name, const WorkingTask::PathsArray & paths, const ustring & destPath) :
		WorkingTask(name, paths), m_destPath(destPath)
	{
	}

	ssize_t execute() override
	{
		std::for_each(m_requestedPaths.begin(), m_requestedPaths.end(), [&] (ustring & item) {
			Base::Path::Inplace::ensure_prefix(item);
			process_path(item, Fsys::get_attr(item), Base::MakePath(m_destPath, Base::Filename::extract(item)));
		});

		return 0;
	}

	ssize_t process_path(const ustring & sourcePath, DWORD attr, const ustring & destPath) const
	{
		do_single_action(sourcePath, attr, destPath);
		if (Fsys::is_dir(attr)) {
			if (!Fsys::is_link(attr) || is_follow_links()) {
				do_recursive_action(sourcePath, attr, destPath);
			}
		}
		return 0;
	}

	ssize_t do_recursive_action(const ustring & sourcePath, DWORD /*attr*/, const ustring & destPath) const
	{
		Fsys::Sequence seq(sourcePath);
		for (auto it = seq.begin(); it != seq.end(); ++it) {
//			LogInfo(L"'%20.20s' isDir: %d isLink: %d size: %I64u\n", it->name(), it->is_dir(), it->is_link(), it->size());
			process_path(it->full_path(), it->attr(), Base::MakePath(destPath, it->name()));
		}
		return 0;
	}

	virtual ssize_t do_single_action(const ustring & sourcePath, DWORD attr, const ustring & destPath) const = 0;

	virtual bool is_follow_links() const = 0;

protected:
	ustring m_destPath;
};

struct CopyTask: public SourceDestinationTask {
	CopyTask(const WorkingTask::PathsArray & paths, const ustring & destPath) :
		SourceDestinationTask(L"copy", paths, Base::Path::ensure_prefix(destPath))
	{
		LogNoise(L"Destination: '%s'\n", m_destPath.c_str());
	}

private:
	ssize_t do_single_action(const ustring & sourcePath, DWORD attr, const ustring & destPath) const override
	{
		LogNoise(L"Copy '%s' to '%s'\n", sourcePath.c_str(), destPath.c_str());
		if (Fsys::is_dir(attr)) {
			if (Fsys::is_link(attr) && is_follow_links()) {
				LogErrorIf(!Fsys::Directory::create_nt(destPath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
			} else {
				LogErrorIf(!Fsys::Directory::copy_link(sourcePath.c_str(), destPath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
			}
		} else {
			if (Fsys::is_link(attr) && is_follow_links()) {
				LogErrorIf(!Fsys::File::copy(sourcePath.c_str(), destPath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
			} else {
				LogErrorIf(!Fsys::File::copy_link(sourcePath.c_str(), destPath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
			}
		}
		return 0;
	}

	bool is_follow_links() const override
	{
		return !Settings::inst().isCopyLinkItself();
	}
};

#endif
