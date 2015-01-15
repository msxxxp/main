#include "logger_pvt.hpp"

//#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/url.hpp>

#include <basis/simstd/algorithm>

//#define TraceFunc(format, ...) //console::printf(L"%S " ## format ## L"/n", __PRETTY_FUNCTION__, ##__VA_ARGS__);

namespace logger {

	namespace {

		struct CompareWithDelim
		{
			CompareWithDelim(const wchar_t* delim): m_delim(delim)
			{
			}
			bool operator ()(const wchar_t& ch) const
			{
				return cstr::find(m_delim, ch);
			}
		private:
			const wchar_t* m_delim;
		};

		const wchar_t* find_first_of(const wchar_t* first, const wchar_t* last, const wchar_t* delim)
		{
			const wchar_t* found = simstd::find_if(first, last, CompareWithDelim(delim));
			return found == last ? nullptr : found;
		}

		void get_substr_in(wchar_t* dest, size_t size, const wchar_t* str, const wchar_t* delim)
		{
			const wchar_t* found1 = find_first_of(str, str + cstr::length(str), delim);

			if (found1) {
				++found1;
//				console::printf(L"%S '%s' -> found1: '%s'\n", __PRETTY_FUNCTION__, str, found1);
				const wchar_t* found2 = find_first_of(found1, found1 + cstr::length(found1), delim);
				if (found2) {
//					console::printf(L"%S '%s' -> found2: '%s'\n", __PRETTY_FUNCTION__, str, found2);
					cstr::copy(dest, found1, simstd::min(size, (size_t)(found2 - found1) + 1));
//					console::printf(L"%S '%s' -> '%s'\n", __PRETTY_FUNCTION__, str.c_str(), ret.c_str());
//					TraceFunc(L"'%s' -> '%s'", str.c_str(), ret.c_str());
				}
			}
		}

		Level convert_to_level(const wchar_t* str)
		{
			Level ret = defaults::get_level();
			if (cstr::compare_ci_1st_length(L"to", str) == 0 || cstr::compare_ci_1st_length(L"t3", str) == 0 || cstr::compare(L"0", str) == 0) {
				ret = Level::TrObj;
			} else if (cstr::compare_ci_1st_length(L"t2", str) == 0 || cstr::compare(L"1", str) == 0) {
				ret = Level::Trace2;
			} else if (cstr::compare_ci_1st_length(L"t", str) == 0 || cstr::compare(L"2", str) == 0) {
				ret = Level::Trace1;
			} else if (cstr::compare_ci_1st_length(L"d2", str) == 0 || cstr::compare(L"3", str) == 0) {
				ret = Level::Debug2;
			} else if (cstr::compare_ci_1st_length(L"d", str) == 0 || cstr::compare(L"4", str) == 0) {
				ret = Level::Debug1;
			} else if (cstr::compare_ci_1st_length(L"i", str) == 0 || cstr::compare(L"5", str) == 0) {
				ret = Level::Info;
			} else if (cstr::compare_ci_1st_length(L"r", str) == 0 || cstr::compare(L"6", str) == 0) {
				ret = Level::Report;
			} else if (cstr::compare_ci_1st_length(L"at", str) == 0 || cstr::compare(L"7", str) == 0) {
				ret = Level::Atten;
			} else if (cstr::compare_ci_1st_length(L"w", str) == 0 || cstr::compare(L"8", str) == 0) {
				ret = Level::Warn;
			} else if (cstr::compare_ci_1st_length(L"er", str) == 0 || cstr::compare(L"9", str) == 0) {
				ret = Level::Error;
			} else if (cstr::compare_ci_1st_length(L"f", str) == 0 || cstr::compare(L"10", str) == 0) {
				ret = Level::Fatal;
			} else if (cstr::compare_ci_1st_length(L"al", str) == 0 || cstr::compare(L"11", str) == 0) {
				ret = Level::Alert;
			} else if (cstr::compare_ci_1st_length(L"em", str) == 0 || cstr::compare(L"12", str) == 0) {
				ret = Level::Emerg;
			}
//			console::printf(L"%S '%s' -> %d(%s)\n", __PRETTY_FUNCTION__, str, ret, to_str(ret));
			return ret;
		}

		Prefix::flags convert_to_prefix(const wchar_t* str)
		{
			Prefix::flags ret = defaults::get_prefix();
			if (cstr::compare_ci_1st_length(L"0", str) == 0) {
				ret = 0;
			} else if (cstr::compare_ci_1st_length(L"l", str) == 0) {
				ret = Prefix::Lite;
			} else if (cstr::compare_ci_1st_length(L"m", str) == 0) {
				ret = Prefix::Medium;
			} else if (cstr::compare_ci_1st_length(L"f", str) == 0) {
				ret = Prefix::Full;
			} else {
				uint64_t numPrefix = cstr::to_uint64(str);
				if (numPrefix)
					ret = static_cast<Prefix::flags>(numPrefix);
			}
//			console::printf(L"%S '%s' -> 0x%IX(%s)\n", __PRETTY_FUNCTION__, str, ret, to_str(ret));
			return ret;
		}

		Target_t convert_to_target(const wchar_t* str)
		{
			Target_t ret = defaults::get_target();
			if (cstr::compare_ci_1st_length(L"n", str) == 0) {
				ret = get_TargetToNull();
//				console::printf(L"%S '%s' -> null\n", __PRETTY_FUNCTION__, str);
			} else if (cstr::compare_ci_1st_length(L"c", str) == 0) {
				ret = get_TargetToConsole();
//				console::printf(L"%S '%s' -> console\n", __PRETTY_FUNCTION__, str);
			} else if (cstr::compare_ci_1st_length(L"f", str) == 0) {
				bool overwrite = cstr::compare_ci_1st_length(L"fo", str) == 0 || cstr::compare_ci_1st_length(L"fileo", str) == 0;
				wchar_t path[4096];
				get_substr_in(path, lengthof(path), str, L"()");
//				console::printf(L"%S '%s' -> file(%s), ow: %d\n", __PRETTY_FUNCTION__, str, path, overwrite);
				ret = get_TargetToFile(path, overwrite);
			} else if (cstr::compare_ci_1st_length(L"s", str) == 0) {
				CRT_ASSERT(false); // not implemented
//				ret = get_TargetToSys(str); // TODO
//				console::printf(L"%S '%s' -> sys\n", __PRETTY_FUNCTION__, str);
			} else {
//				console::printf(L"%S '%s' -> not found\n", __PRETTY_FUNCTION__, str);
			}
			return ret;
		}

		size_t convert_to_bit(const wchar_t* str)
		{
			size_t ret = 1;
			if (cstr::compare_ci_1st_length(L"f", str) == 0 || cstr::compare(L"0", str) == 0) {
				ret = 0;
			}
//			console::printf(L"%S '%s' -> %Iu\n", __PRETTY_FUNCTION__, str, ret);
			return ret;
		}

		struct ModuleOptionsSetter {
			ModuleOptionsSetter() :
				m_level(Level::Trace1),
				m_prefix(Prefix::Medium),
				m_target(get_TargetToNull()),
				m_color(0),
				m_enabled(0)
			{
				set_level = set_prefix = set_target = set_color = set_enabled = 0;
			}

			void name(const wchar_t* str) {cstr::copy(m_name1, str, lengthof(m_name1));}

			void level(const wchar_t* str) {m_level = convert_to_level(str); set_level = 1;}

			void prefix(const wchar_t* str) {m_prefix = convert_to_prefix(str); set_prefix = 1;}

			void target(const wchar_t* str) {m_target = convert_to_target(str); set_target = 1;}

			void color(const wchar_t* str) {m_color = convert_to_bit(str); set_color = 1;}

			void enabled(const wchar_t* str) {m_enabled = convert_to_bit(str); set_enabled = 1;}

			void execute() {
				if (!cstr::is_empty(m_name1)) {
					auto module = logger::get_module_impl(m_name1);
					auto lock(module->lock_scope());
					if (set_level)
						module->set_level(m_level);
					if (set_prefix)
						module->set_prefix(m_prefix);
					if (set_target)
						module->set_target(m_target);
					if (set_color)
						module->set_color_mode(m_color);
					if (set_enabled)
						module->set_enabled(m_enabled);
				}
			}


		private:
			wchar_t       m_name1[16];
			Level         m_level;
			Prefix::flags m_prefix;
			Target_t      m_target;
			uint32_t      m_color:1;
			uint32_t      m_enabled:1;
			uint32_t      set_level:1;
			uint32_t      set_prefix:1;
			uint32_t      set_target:1;
			uint32_t      set_color:1;
			uint32_t      set_enabled:1;
		};

		bool get_value(const wchar_t* name, const wchar_t* parameter, const wchar_t *& value)
		{
			auto len = cstr::length(name);

			if (len && cstr::compare_ci(name, parameter, len) == 0) {
				value = parameter + len;
				return true;
			}

			return false;
		}

		const wchar_t* get_parameter(wchar_t* str)
		{
			wchar_t* found = nullptr;
			wchar_t* end = str + cstr::length(str);
			while (end > str)
				if (cstr::find(L"?;", *(--end))) {
					found = end;
					break;
				}
			if (found) {
				*found = static_cast<wchar_t>(0);
				++found;
				return found;
			}
			return L"";
		}

		void set_defaults(wchar_t* options, size_t size)
		{
			options[size] = static_cast<wchar_t>(0);
			while (!cstr::is_empty(options)) {
//				console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, options);
				auto prm = get_parameter(options);
//				console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, prm);
				const wchar_t* value = nullptr;
				if (get_value(L"level=", prm, value)) {
					defaults::set_level(convert_to_level(value));
				} else if (get_value(L"prefix=", prm, value)) {
					defaults::set_prefix(convert_to_prefix(value));
				} else if (get_value(L"target=", prm, value)) {
					defaults::set_target(convert_to_target(value));
				}
			}
		}

		void set_globals(wchar_t* /*options*/, size_t /*size*/)
		{
		}

		void set_module(wchar_t* options, size_t size)
		{
			options[size] = static_cast<wchar_t>(0);
			ModuleOptionsSetter mos;
			while (!cstr::is_empty(options)) {
//				console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, options);
				auto prm = get_parameter(options);
//				console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, prm);
				const wchar_t* value = nullptr;
				if (get_value(L"name=", prm, value)) {
					mos.name(value);
				} else if (get_value(L"level=", prm, value)) {
					mos.level(value);
				} else if (get_value(L"prefix=", prm, value)) {
					mos.prefix(value);
				} else if (get_value(L"target=", prm, value)) {
					mos.target(value);
				} else if (get_value(L"color=", prm, value)) {
					mos.color(value);
				} else if (get_value(L"enable=", prm, value)) {
					mos.enabled(value);
				}
			}
			mos.execute();
		}

	}

	void set_options(const wchar_t* url)
	{
		TraceFunc();
		auto urlCopy = cstr::dup(url);

		TraceFunc();
		URL_COMPONENTSW info;
		bool res = url::crack(urlCopy, &info);
		if (res && cstr::compare_ci(L"logger", info.lpszScheme, info.dwSchemeLength) == 0) {
			if (cstr::compare_ci(L"/default", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_defaults(info.lpszExtraInfo, info.dwExtraInfoLength);
			} else if (cstr::compare_ci(L"/global", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_globals(info.lpszExtraInfo, info.dwExtraInfoLength);
			} else if (cstr::compare_ci(L"/module", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_module(info.lpszExtraInfo, info.dwExtraInfoLength);
			}
		}

		memory::free(urlCopy);
		TraceFunc();
	}

}
