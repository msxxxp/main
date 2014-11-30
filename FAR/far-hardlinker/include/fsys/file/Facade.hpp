#ifndef _FSYS_Facade_i_HPP_
#define _FSYS_Facade_i_HPP_

#include <fsys.hpp>

namespace fsys {

	namespace file {

		enum class Seek: ssize_t {
			FromBeginOfFile,
			FromCurrentPosition,
			FromEndOfFile,
		};

		struct OpenOptions {
			bool write;

			OpenOptions();
		};

		struct Facade_i {
			virtual ~Facade_i() = default;

			virtual ustring path() const = 0;

			virtual Stat stat() const = 0;

			virtual bool size(uint64_t & size) const = 0;

			virtual bool read(void * buf, size_t size, size_t & read) = 0;

			virtual bool write(const void* buf, size_t size, size_t & written) = 0;

			virtual bool position(uint64_t & pos) const = 0;

			virtual bool set_position(int64_t dist, Seek base) = 0;

			virtual bool set_eof() = 0;

			virtual bool set_time(const uint64_t & ctime, const uint64_t & atime, const uint64_t & mtime) = 0;

			virtual bool set_mtime(const uint64_t & mtime) = 0;

			virtual bool set_ctime(const uint64_t & ctime) = 0;

			virtual bool set_atime(const uint64_t & atime) = 0;
		};

		typedef simstd::unique_ptr<Facade_i> Facade;

//		Facade open(const astring& path, const OpenOptions & options = OpenOptions());

		Facade open(const ustring& path, const OpenOptions & options = OpenOptions());
	}

}

#endif
