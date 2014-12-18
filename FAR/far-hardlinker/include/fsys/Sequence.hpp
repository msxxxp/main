#ifndef _FSYS_SEQUENCE_HPP_
#define _FSYS_SEQUENCE_HPP_

#include <fsys.hpp>

#include <basis/sys/fsys.hpp>
#include <basis/simstd/string>

namespace fsys {

	///=================================================================================================================
	class Sequence: private pattern::Uncopyable {
		typedef Sequence this_type;
		class ci_iterator;

	public:
		struct FindStat;
		class FiltersBunch;
		class Filter;
		class Options;
		class Statistics;

		enum SearchFlags {
			folderIncludeDots  = 0x0000001,

			folderSkipAll      = 0x0000010,
			folderSkipArchive  = 0x0000020,
			folderSkipReadOnly = 0x0000040,
			folderSkipHidden   = 0x0000080,
			folderSkipSystem   = 0x0000100,
			folderSkipLink     = 0x0000200,

			fileSkipAll        = 0x0001000,
			fileSkipArchive    = 0x0002000,
			fileSkipReadOnly   = 0x0004000,
			fileSkipHidden     = 0x0008000,
			fileSkipSystem     = 0x0010000,
			fileSkipLink       = 0x0020000,
			fileSkipStreamed   = 0x0040000,
			fileSkipCompressed = 0x0080000,
			fileSkipEncrypted  = 0x0100000,
			fileSkipSparse     = 0x0200000,
			fileSkipTemporary  = 0x0400000,
			fileSkipOffline    = 0x0800000,
			fileSkipZeroSize   = 0x1000000,
		};

		typedef FindStat    value_type;
		typedef size_t      size_type;
		typedef uint64_t    flags_type;
		typedef ci_iterator iterator;
		typedef ci_iterator const_iterator;

		typedef size_t   Attr;
		typedef int64_t  Time;
		typedef uint64_t Size;

		Sequence(const ustring& path, const ustring& mask, const Options& options);

		const_iterator begin() const;

		const_iterator end() const;

		bool empty() const;

		const ustring& path() const;

		const ustring& mask() const;

		const Options& options() const;

	private:
		ustring        m_path;
		ustring        m_mask;
		const Options& m_options;
	};

	inline const ustring& Sequence::path() const
	{
		return m_path;
	}

	inline const ustring& Sequence::mask() const
	{
		return m_mask;
	}

	///=================================================================================================================
	class Sequence::Statistics {
	public:
		Statistics();

		void init_statistics();
		void free_statistics();

		void folder_found(const FindStat& stat);
		void folder_ignored();
		void folder_ignored_archive();
		void folder_ignored_readonly();
		void folder_ignored_hidden();
		void folder_ignored_system();
		void folder_ignored_link();

		void file_found(const FindStat& stat);
		void file_ignored();
		void file_ignored_to_small();
		void file_ignored_to_big();
		void file_ignored_archive();
		void file_ignored_readonly();
		void file_ignored_hidden();
		void file_ignored_system();
		void file_ignored_link();
		void file_ignored_streamed();
		void file_ignored_compressed();
		void file_ignored_encrypted();
		void file_ignored_sparse();
		void file_ignored_temporary();
		void file_ignored_offline();
		void file_ignored_zero();

	private:
		struct Impl;
		Impl* m_impl;
	};

}

#include <fsys/Sequence/Stat.hpp>
#include <fsys/Sequence/FiltersBunch.hpp>
#include <fsys/Sequence/Options.hpp>
#include <fsys/Sequence/Iterator.hpp>

#endif
