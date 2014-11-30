#ifndef _FSYS_STAT_HPP_
#define _FSYS_STAT_HPP_

#include <fsys.hpp>

namespace fsys {

	class Stat_i {
	public:
		virtual ~Stat_i() = default;

		bool operator ==(const Stat_i & other) const;

		virtual size_t attr() const = 0;
		virtual size_t volume_sn() const = 0;
		virtual size_t num_links() const = 0;

		virtual uint64_t size() const = 0;
		virtual uint64_t inode() const = 0;
		virtual uint64_t ctime() const = 0;
		virtual uint64_t atime() const = 0;
		virtual uint64_t mtime() const = 0;

		virtual bool is_dir() const = 0;
		virtual bool is_file() const = 0;
		virtual bool is_lnk() const = 0;
	};

	typedef simstd::unique_ptr<Stat_i> Stat;

	Stat stat(const char* path);

	Stat stat(const wchar_t* path);

}

#endif
