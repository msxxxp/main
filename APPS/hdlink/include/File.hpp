#ifndef HDLINK_FILE_HPP_
#define HDLINK_FILE_HPP_

#include <FsNode.hpp>
#include <Hash.hpp>

struct File: public FsNode {
	~File();

	File(parent_type parent, const WIN32_FIND_DATAW & info);

	DWORD get_attr() const;

	uint64_t time() const;

	uint64_t size() const;

	const Hash & hashFull() const;

	const WinFileId& inode() const
	{
		return m_inode;
	}

	bool operator <(const File & rhs) const
	{
		return m_size < rhs.m_size;
	}

	bool LoadHashMini() const
	{
		if (!m_hashMini.is_available()) {
			return m_hashMini(get_full_path().c_str(), FirstBlock);
		}
		return true;
	}

	bool LoadHashFull() const
	{
		if (!m_hashFull.is_available()) {
			return m_hashFull(get_full_path().c_str());
		}
		return true;
	}

	bool LoadInode()
	{
		if (m_inode.IsOK())
			return true;
		bool Result = m_inode.Load(get_full_path().c_str());
		if (!Result)
			LogError(L"Unable to load file inode info: %s\n", ErrAsStr().c_str());
		return Result;
	}

	bool hardlink(const File & other) const
	{
		++statistics().hardLinks;
		WinBuf<WCHAR> file1Name(MAX_PATH_LEN);
		WinBuf<WCHAR> file2Name(MAX_PATH_LEN);

		this->copyName(file1Name);
		rhs->copyName(file2Name);

		// Step 1: create hard link
		ustring file2hdlink(file2Name);
		file2hdlink += L".hdlink";
		if (!create_hardlink(file1Name, file2hdlink.c_str())) {
			logError(L"  Unable to create hard link: %i\n", ::GetLastError());
			return false;
		}

		// Step 2: move file to backup
		ustring file2backup(file2Name);
		file2backup += L".hdlink-backup";
		if (!move_file(file2Name, file2backup.c_str())) {
			delete_file(file2hdlink);
			logError(L"  Unable to backup file: %i\n", ::GetLastError());
			return false;
		}

		// Step 3: rename file
		if (!move_file(file2hdlink.c_str(), file2Name)) {
			move_file(file2backup.c_str(), file2Name);
			delete_file(file2hdlink);
			logError(L"  Unable to move file to backup: %i\n", ::GetLastError());
			return false;
		}

		if (file_exists(file2Name)) {
			delete_file(file2backup) || delete_on_reboot(file2backup);
		}

		{
			ConsoleColor col(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
			logVerbose(L"  Linked!\n");
		}

		++statistics().hardLinksSuccess;
		return true;
	}

	bool operator ==(const File & other) const
	{
		if (size() == other.size() && inode() == other.inode())
			return true;
		return false;
	}

	bool is_identical(const File & other) const
	{
		if (size() > FirstBlock) {
			if (LoadHashMini() && other.LoadHashMini()) {
				if (m_hashMini != other.m_hashMini) {
					++statistics().fileContentDifferFirstBlock;
					return false;
				}
			} else {
				return false;
			}
		}

		if (!LoadHashFull() || !other.LoadHashFull())
			return false;

		bool ret = (m_hashFull == other.m_hashFull);
		if (ret)
			++statistics().fileContentSame;
		else
			++statistics().fileContentDifferLater;

		return ret;
	}

private:
	parent_type m_parent;
	Hash mutable m_hashMini;
	Hash mutable m_hashFull;
	WinFileId m_inode;
	ustring m_name;
	uint64_t m_size;
	uint64_t m_time;
	DWORD m_attr;
};

#endif /* HDLINK_FILE_HPP_ */
