/** 7zip.h
 *	@interface to 7-zip
 *	@author © 2011 Andrew Grechkin
 *	@link (ole32 oleaut32 uuid)
 **/

#ifndef LIB7ZIP_HPP_
#define LIB7ZIP_HPP_

#include <libcom/std.hpp>
#include <libcom/guid.hpp>
#include <libcom/propvariant.hpp>
#include <libcom/unknown.hpp>
#include <libext/dll.hpp>
#include <libext/fileversion.hpp>
#include <libext/filesystem.hpp>
#include <libbase/std.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/logger.hpp>

#include <initguid.h>
#include <shobjidl.h>

#include <map>
#include <vector>
#include <memory>

namespace SevenZip {
#include <lib7zip/CPP/7zip/Archive/IArchive.h>
#include <lib7zip/CPP/7zip/IPassword.h>
	Logger::Module_i * get_logger_module();

	class Lib;

	const UInt64 max_check_size = 1024 * 1024;

	enum CompressMethod {
		metCopy = 0,
		metDelta = 3,
		met7zAES = 116459265,
		metARM = 50529537,
		metARMT = 50530049,
		metBCJ = 50528515,
		metBCJ2 = 50528539,
		metBZip2 = 262658,
		metDeflate = 262408,
		metDeflate64 = 262409,
		metIA64 = 50529281,
		metLZMA = 196865,
		metLZMA2 = 33,
		metPPC = 50528773,
		metPPMD = 197633,
		metRar1 = 262913,
		metRar2 = 262914,
		metRar3 = 262915,
		metSPARC = 50530309,
		metSwap2 = 131842,
		metSwap4 = 131844,
	};

	extern const Base::NamedValues<int> ArcItemPropsNames[63];
	extern const Base::NamedValues<CompressMethod> CompressMethodsNames[21];

	struct FailedFile {
		ustring path;
		HRESULT code;

		FailedFile(const ustring & p, HRESULT h) :
			path(p), code(h)
		{
		}
	};

	typedef std::vector<FailedFile> FailedFiles;

	struct DirItem: public Fsys::Stat {
		ustring path;
		ustring name;
		DirItem(const ustring & file_path, const ustring & file_name);
	};

	///====================================================================================== Method
	struct Method {
		uint64_t id;
		ustring name;
		std::vector<BYTE> start_sign;
		std::vector<BYTE> finish_sign;

		Method(const Lib & arc_lib, size_t idx);

	private:
		bool operator <(const Method & rhs) const;

		bool operator ==(const Method & rhs) const;

		bool operator !=(const Method & rhs) const;

		friend class Methods;
	};

	///===================================================================================== Methods
	struct Methods: private std::vector<Method> {
		typedef std::vector<Method> base_type;
		typedef base_type::const_iterator iterator;
		typedef base_type::const_iterator const_iterator;

		using base_type::begin;
		using base_type::end;
		using base_type::empty;
		using base_type::size;
		using base_type::at;

	private:
		Methods();

		void cache(const Lib & lib);

		friend class Lib;
	};

	///======================================================================================= Codec
	struct Codec {
		ustring name;
		ustring ext;
		ustring add_ext;
		ustring kAssociate;
		Com::WinGUID guid;
		std::vector<BYTE> start_sign, finish_sign;
		bool updatable;
		bool kKeepName;

//		ustring default_extension() const;

		Codec(const Lib & arc_lib, size_t idx);

		bool operator ==(const ustring & name) const;

	private:
		bool operator <(const Codec & rhs) const;
		bool operator ==(const Codec & rhs) const;
		bool operator !=(const Codec & rhs) const;

		friend class Codecs;
	};

	///====================================================================================== Codecs
	struct Codecs: private std::vector<Codec> {
		typedef std::vector<Codec> base_type;
		typedef base_type::const_iterator iterator;
		typedef base_type::const_iterator const_iterator;

		using base_type::begin;
		using base_type::rbegin;
		using base_type::end;
		using base_type::rend;
		using base_type::empty;
		using base_type::size;

		const_iterator at(const ustring & name) const;

//		ArcTypes find_by_ext(const ustring & ext) const;

	private:
		Codecs();

		void cache(const Lib & lib);

		friend class Lib;
	};

	///======================================================================================== Prop
	struct Prop {
		ustring name;
		PROPID id;
		Com::PropVariant prop;

		Prop(const Com::Object<IInArchive> & arc, size_t idx);
	};

	///======================================================================================= Props
	struct Props: private std::vector<Prop> {
		typedef std::vector<Prop> base_type;
		typedef base_type::const_iterator iterator;
		typedef base_type::const_iterator const_iterator;

		using base_type::begin;
		using base_type::end;
		using base_type::empty;
		using base_type::size;
		using base_type::at;

		void cache(const Com::Object<IInArchive> & arc);

	private:
		using base_type::push_back;

		Props();
		Props(const Com::Object<IInArchive> & arc);

		friend class Archive;
	};

	///========================================================================================= Lib
	class Lib: public Ext::FileVersion, private Ext::DynamicLibrary {
		typedef UInt32 (WINAPI *FCreateObject)(const GUID * clsID, const GUID * interfaceID, PVOID * outObject);
		typedef UInt32 (WINAPI *FGetHandlerProperty)(PROPID propID, PROPVARIANT * value);
		typedef UInt32 (WINAPI *FGetHandlerProperty2)(UInt32 index, PROPID propID, PROPVARIANT * value);
		typedef UInt32 (WINAPI *FGetMethodProperty)(UInt32 index, PROPID propID, PROPVARIANT * value);
		typedef UInt32 (WINAPI *FGetNumberOfMethods)(UInt32 *numMethods);
		typedef UInt32 (WINAPI *FGetNumberOfFormats)(UInt32 * numFormats);
		typedef UInt32 (WINAPI *FSetLargePageMode)();

	public:
		DEFINE_FUNC(CreateObject);
		DEFINE_FUNC(GetHandlerProperty);
		DEFINE_FUNC(GetHandlerProperty2);
		DEFINE_FUNC(GetMethodProperty);
		DEFINE_FUNC(GetNumberOfFormats);
		DEFINE_FUNC(GetNumberOfMethods);
		DEFINE_FUNC(SetLargePageMode);

		Lib(PCWSTR path);

		const Codecs & codecs() const;
		const Methods & methods() const;

		HRESULT get_prop(UInt32 index, PROPID prop_id, Com::PropVariant & prop) const;
		HRESULT get_prop(UInt32 index, PROPID prop_id, Com::WinGUID & guid) const;
		HRESULT get_prop(UInt32 index, PROPID prop_id, bool & value) const;
		HRESULT get_prop(UInt32 index, PROPID prop_id, ustring & value) const;
		HRESULT get_prop(UInt32 index, PROPID prop_id, std::vector<BYTE> & value) const;

		using Ext::DynamicLibrary::get_path;

	private:
		Codecs m_codecs;
		Methods m_methods;
	};

	///=================================================================================== SfxModule
//	struct SfxModule {
//		ustring path;
//		ustring description() const;
//		bool all_codecs() const;
//		bool install_config() const;
//	};
//
//	class SfxModules: public vector<SfxModule> {
//	public:
//		unsigned find_by_name(const ustring & name) const;
//	};

///============================================================================== FileReadStream
	struct FileReadStream: public IInStream, private Fsys::File::Facade, private Com::UnknownImp {
		~FileReadStream();

		FileReadStream(const ustring & path);

		// Unknown
		ULONG WINAPI AddRef() override;
		ULONG WINAPI Release() override;
		HRESULT WINAPI QueryInterface(REFIID riid, void ** object) override;

		// ISequentialInStream
		HRESULT WINAPI Read(void * data, UInt32 size, UInt32 * processedSize) override;

		// IInStream
		HRESULT WINAPI Seek(Int64 offset, UInt32 seekOrigin, UInt64 * newPosition) override;
	};

	///============================================================================= FileWriteStream
	struct FileWriteStream: public IOutStream, private Fsys::File::Facade, private Com::UnknownImp {
		~FileWriteStream();

		FileWriteStream(const ustring & path, DWORD creat = CREATE_NEW);

		// Unknown
		ULONG WINAPI AddRef() override;
		ULONG WINAPI Release() override;
		HRESULT WINAPI QueryInterface(REFIID riid, void ** object) override;

		// ISequentialOutStream
		HRESULT WINAPI Write(PCVOID data, UInt32 size, UInt32 * processedSize) override;

		// IOutStream
		HRESULT WINAPI Seek(Int64 offset, UInt32 seekOrigin, UInt64 * newPosition) override;
		HRESULT WINAPI SetSize(UInt64 newSize) override;

		using Fsys::File::Facade::set_mtime;
	};

	///================================================================================ OpenCallback
	struct OpenCallback: public IArchiveOpenCallback, public ICryptoGetTextPassword, private Com::UnknownImp {
		ustring Password;

		~OpenCallback();

		OpenCallback();

		// IUnknown
		ULONG WINAPI AddRef() override;
		ULONG WINAPI Release() override;
		HRESULT WINAPI QueryInterface(REFIID riid, void ** object) override;

		// IArchiveOpenCallback
		HRESULT WINAPI SetTotal(const UInt64 * files, const UInt64 * bytes) override;
		HRESULT WINAPI SetCompleted(const UInt64 * files, const UInt64 * bytes) override;

		// ICryptoGetTextPassword
		HRESULT WINAPI CryptoGetTextPassword(BSTR * password) override;
	};

	///===================================================================================== Archive
	class Archive: private Base::Uncopyable {
	public:
		typedef size_t size_type;

	public:
		Archive(const Lib & lib, const ustring & path);

		Archive(Com::Object<IInArchive> arc);

		const Codec & codec() const;

		const Props & props() const;

		Com::Object<IInArchive> operator ->() const;

		operator Com::Object<IInArchive>() const;

		bool empty() const;

		size_t size() const;

	private:
		void open_archive(const Lib & lib, const ustring & path);

		void init_props();

		Com::Object<IInArchive> m_arc;
		Codecs::const_iterator m_codec;
		Props m_props;
		UInt32 m_size;
	};

	///============================================================================= ArchiveSequence
	class ArchiveSequence: public Archive {
		class const_input_iterator;
		typedef Archive base_type;

	public:
		typedef ArchiveSequence this_type;
		typedef int flags_type;
		typedef const_input_iterator iterator;
		typedef const_input_iterator const_iterator;

		enum search_flags {
			incDots = 0x0001,
			skipDirs = 0x0002,
			skipFiles = 0x0004,
			skipLinks = 0x0008,
			skipHidden = 0x0010,
		};

		ArchiveSequence(const Lib & lib, const ustring & path, flags_type flags = 0);

		ArchiveSequence(const Lib & lib, const ustring & path, const ustring & mask, flags_type flags = 0);

		ArchiveSequence(Com::Object<IInArchive> arc, flags_type flags = 0);

		const_iterator begin() const;

		const_iterator end() const;

		const_iterator at(size_t index) const;

		flags_type flags() const;

	private:
		flags_type m_flags;
	};

	struct ArchiveSequence::const_input_iterator {
		typedef const_input_iterator this_type;

		this_type & operator ++();

		this_type operator ++(int);

		ustring path() const;

		uint64_t size() const;

		size_t attr() const;

		FILETIME mtime() const;

		bool is_file() const;

		bool is_dir() const;

		size_t get_props_count() const;

		bool get_prop_info(size_t index, ustring & name, PROPID & id) const;

		Com::PropVariant get_prop(PROPID id) const;

		bool operator ==(const this_type & rhs) const;

		bool operator !=(const this_type & rhs) const;

	private:
		const_input_iterator();
		const_input_iterator(const ArchiveSequence & seq);
		const_input_iterator(const ArchiveSequence & seq, UInt32 index);

		ArchiveSequence * m_seq;
		UInt32 m_index;
		bool m_end;

		friend class ArchiveSequence;
	};

	///============================================================================== ArchiveExtract
	class ArchiveExtract: public Base::Command_p {
	public:
		ArchiveExtract(const Lib & lib, const ustring & arcPath, const ustring & destPath);

		ssize_t execute() override;

	private:
		ArchiveSequence m_archive;
		ustring m_dest;
	};

	///================================================================================= ArchiveTest
	class ArchiveTest: public Base::Command_p {
	public:
		ArchiveTest(const Lib & lib, const ustring & path);

		ssize_t execute() override;

	private:
		ArchiveSequence m_archive;
	};

	///============================================================================= ExtractCallback
	struct ExtractCallback: public IArchiveExtractCallback, public ICryptoGetTextPassword, private Com::UnknownImp {
		FailedFiles failed_files;
		ustring Password;

		~ExtractCallback();

		ExtractCallback(const ArchiveSequence & arc, const ustring & dest_path = ustring(), const ustring & pass = ustring());

		// IUnknown
		ULONG WINAPI AddRef() override;
		ULONG WINAPI Release() override;
		HRESULT WINAPI QueryInterface(REFIID riid, void ** object) override;

		// IProgress
		HRESULT WINAPI SetTotal(UInt64 size) override;
		HRESULT WINAPI SetCompleted(const UInt64 * completeValue) override;

		// IArchiveExtractCallback
		HRESULT WINAPI GetStream(UInt32 index, ISequentialOutStream ** outStream, Int32 askExtractMode) override;
		HRESULT WINAPI PrepareOperation(Int32 askExtractMode) override;
		HRESULT WINAPI SetOperationResult(Int32 resultEOperationResult) override;

		// ICryptoGetTextPassword
		HRESULT WINAPI CryptoGetTextPassword(BSTR * pass) override;

	private:
		const ArchiveSequence & m_wa;
		ustring m_dest;// Output directory

		struct CurrItem;
		std::shared_ptr<CurrItem> m_curr;
	};

	///=========================================================================== ArchiveProperties
	enum class CompressLevel: ssize_t {
		STORE   = 0,
		FASTEST = 1,
		FAST    = 3,
		NORMAL  = 5,
		MAX     = 7,
		ULTRA   = 9,
	};

	struct CompressProperties: private std::vector<DirItem> {
		typedef std::vector<DirItem> base_type;
		typedef base_type::const_iterator iterator;

		using base_type::begin;
		using base_type::end;
		using base_type::empty;
		using base_type::size;
		using base_type::at;

		std::vector<UInt64> VolumesSizes;
		ustring codec;
		ustring VolName;
		ustring VolExt;
		ustring password;
		CompressLevel level;
		bool solid;
		bool encrypt_header;
		bool AskPassword;
		bool IgnoreErrors;
		bool silent;

		CompressProperties();

		void add(const ustring & add_path);

		void writeln(PCWSTR str) const;

		void writeln(const ustring & str) const;

		void printf(PCWSTR format, ...) const;

	private:
		void base_add(const ustring & base_path, const ustring & name);
	};

	///============================================================================== UpdateCallback
	struct UpdateCallback: public IArchiveUpdateCallback2, public ICryptoGetTextPassword2, private Com::UnknownImp {
		~UpdateCallback();

		UpdateCallback(const CompressProperties & props, FailedFiles & ffiles);

		// Unknown
		ULONG WINAPI AddRef() override;
		ULONG WINAPI Release() override;
		HRESULT WINAPI QueryInterface(REFIID riid, void ** object) override;

		// IProgress
		HRESULT WINAPI SetTotal(UInt64 size) override;
		HRESULT WINAPI SetCompleted(const UInt64 * completeValue) override;

		// GetUpdateItemInfo
		HRESULT WINAPI GetUpdateItemInfo(UInt32 index, Int32 * newData, Int32 * newProperties, UInt32 * indexInArchive) override;
		HRESULT WINAPI GetProperty(UInt32 index, PROPID propID, PROPVARIANT * value) override;
		HRESULT WINAPI GetStream(UInt32 index, ISequentialInStream ** inStream) override;
		HRESULT WINAPI SetOperationResult(Int32 operationResult) override;

		// IArchiveUpdateCallback2
		HRESULT WINAPI GetVolumeSize(UInt32 index, UInt64 * size) override;
		HRESULT WINAPI GetVolumeStream(UInt32 index, ISequentialOutStream ** volumeStream) override;

		// ICryptoGetTextPassword2
		HRESULT WINAPI CryptoGetTextPassword2(Int32 * passwordIsDefined, BSTR * password) override;

	private:
		const CompressProperties & m_props;
		FailedFiles & m_ffiles;
	};

	///=============================================================================== CreateArchive
	struct CreateArchive: public Base::Command_p, private Base::Uncopyable {
		CreateArchive(const Lib & lib, const ustring & path, const CompressProperties & properties);

		ssize_t execute() override;

//		Object<IOutArchive> operator ->() const;

	private:
		void set_properties();

		const Lib & m_lib;
		const ustring m_path;
		const CompressProperties & m_properties;
		Com::Object<IOutArchive> m_archive;
		FailedFiles m_ffiles;
	};
}

#endif
