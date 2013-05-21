#ifndef SOURCEONLYTASK_HPP_
#define SOURCEONLYTASK_HPP_

#include <WorkingTask.hpp>

struct SourceOnlyTask: public WorkingTask {
	SourceOnlyTask(const ustring & name, const WorkingTask::PathsArray & paths) :
		WorkingTask(name, paths)
	{
	}

	ssize_t execute() override
	{
		std::for_each(m_requestedPaths.begin(), m_requestedPaths.end(), [&] (ustring & item) {
			Base::Path::Inplace::ensure_prefix(item);
			process_path(item, Fsys::get_attr(item));
		});

		return 0;
	}

	ssize_t process_path(const ustring & sourcePath, DWORD attr) const
	{
		if (Fsys::is_dir(attr)) {
			if (!Fsys::is_link(attr) || is_follow_links()) {
				do_recursive_action(sourcePath);
			}
		}
		do_single_action(sourcePath, attr);
		return 0;
	}

	ssize_t do_recursive_action(const ustring & sourcePath) const
	{
		Fsys::Sequence seq(sourcePath);
		for (auto it = seq.begin(); it != seq.end(); ++it) {
			//			LogInfo(L"'s' isDir: %d isLink: %d size: %I64u\n", it->name(), it->is_dir(), it->is_link(), it->size());
			process_path(it->full_path(), it->attr());
		}
		return 0;
	}

	virtual ssize_t do_single_action(const ustring & sourcePath, DWORD attr) const = 0;

	virtual bool is_follow_links() const = 0;
};

struct DeleteTask: public SourceOnlyTask {
	DeleteTask(const WorkingTask::PathsArray & paths) :
		SourceOnlyTask(L"delete", paths)
	{
	}

	ssize_t do_single_action(const ustring & sourcePath, DWORD attr) const override
	{
		LogNoise(L"Delete '%s'\n", sourcePath.c_str());
		if (Fsys::is_dir(attr)) {
			LogErrorIf(!Fsys::Directory::del_nt(sourcePath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
		} else {
			LogErrorIf(!Fsys::File::del_nt(sourcePath.c_str()), L"%s\n", Base::ErrAsStr().c_str());
		}
		return 0;
	}

	bool is_follow_links() const override
	{
		return false;
	}
};

#endif
