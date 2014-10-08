#include "../logger_pvt.hpp"

#include <basis/sys/console.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/url.hpp>

#include <basis/simstd/string>

//#define TraceFunc(format, ...) //console::printf(L"%S " ## format ## L"/n", __PRETTY_FUNCTION__, ##__VA_ARGS__);

namespace logger {

	namespace {

		ustring get_substr_in(ustring str, const wchar_t * delim)
		{
			auto first = str.find_first_of(delim);
			if (first != ustring::npos) {
				first += 1;
				auto ret = str.substr(first, str.find_first_of(delim, first) - first);
//				console::printf(L"%S '%s' -> '%s'\n", __PRETTY_FUNCTION__, str.c_str(), ret.c_str());
//				TraceFunc(L"'%s' -> '%s'", str.c_str(), ret.c_str());
				return ret;
			}
			return ustring();
		}

		Level convert_to_level(const wchar_t * str)
		{
			Level ret = defaults::get_level();
			if (cstr::compare_ci_1st_length(L"t", str) == 0 || cstr::compare(L"0", str) == 0) {
				ret = Level::Trace;
			} else if (cstr::compare_ci_1st_length(L"d", str) == 0 || cstr::compare(L"1", str) == 0) {
				ret = Level::Debug;
			} else if (cstr::compare_ci_1st_length(L"i", str) == 0 || cstr::compare(L"2", str) == 0) {
				ret = Level::Info;
			} else if (cstr::compare_ci_1st_length(L"r", str) == 0 || cstr::compare(L"3", str) == 0) {
				ret = Level::Report;
			} else if (cstr::compare_ci_1st_length(L"at", str) == 0 || cstr::compare(L"4", str) == 0) {
				ret = Level::Atten;
			} else if (cstr::compare_ci_1st_length(L"w", str) == 0 || cstr::compare(L"5", str) == 0) {
				ret = Level::Warn;
			} else if (cstr::compare_ci_1st_length(L"er", str) == 0 || cstr::compare(L"6", str) == 0) {
				ret = Level::Error;
			} else if (cstr::compare_ci_1st_length(L"f", str) == 0 || cstr::compare(L"7", str) == 0) {
				ret = Level::Fatal;
			} else if (cstr::compare_ci_1st_length(L"al", str) == 0 || cstr::compare(L"8", str) == 0) {
				ret = Level::Alert;
			} else if (cstr::compare_ci_1st_length(L"em", str) == 0 || cstr::compare(L"9", str) == 0) {
				ret = Level::Emerg;
			}
			//console::printf(L"%S '%s' -> %d(%s)\n", __PRETTY_FUNCTION__, str, ret, to_str(ret));
			return ret;
		}

		Prefix::flags convert_to_prefix(const wchar_t * str)
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
			//console::printf(L"%S '%s' -> 0x%IX(%s)\n", __PRETTY_FUNCTION__, str, ret, to_str(ret));
			return ret;
		}

		Target_t convert_to_target(const wchar_t * str)
		{
			Target_t ret = defaults::get_target();
			if (cstr::compare_ci_1st_length(L"n", str) == 0) {
				ret = get_TargetToNull();
				//console::printf(L"%S '%s' -> null\n", __PRETTY_FUNCTION__, str);
			} else if (cstr::compare_ci_1st_length(L"c", str) == 0) {
				ret = get_TargetToConsole();
				//console::printf(L"%S '%s' -> console\n", __PRETTY_FUNCTION__, str);
			} else if (cstr::compare_ci_1st_length(L"f", str) == 0) {
				bool overwrite = cstr::compare_ci_1st_length(L"fo", str) == 0 || cstr::compare_ci_1st_length(L"fileo", str) == 0;
				ret = get_TargetToFile(get_substr_in(str, L"()").c_str(), overwrite);
				//console::printf(L"%S '%s' -> file, ow: %d\n", __PRETTY_FUNCTION__, str, overwrite);
				//		} else if (cstr::compare_ci_1st_length(L"s", str) == 0) {
				//			ret = get_TargetToSys(str); // TODO
				//			//console::printf(L"%S '%s' -> sys\n", __PRETTY_FUNCTION__, str);
			} else {
				//console::printf(L"%S '%s' -> not found\n", __PRETTY_FUNCTION__, str);
			}
			return ret;
		}

		size_t convert_to_bit(const wchar_t * str)
		{
			size_t ret = 1;
			if (cstr::compare_ci_1st_length(L"f", str) == 0 || cstr::compare(L"0", str) == 0) {
				ret = 0;
			}
			//console::printf(L"%S '%s' -> %Iu\n", __PRETTY_FUNCTION__, str, ret);
			return ret;
		}

		struct ModuleOptionsSetter {
			ModuleOptionsSetter() :
				m_level(Level::Trace),
				m_prefix(Prefix::Medium),
				m_target(get_TargetToNull()),
				m_color(0),
				m_enabled(0)
			{
				set_level = set_prefix = set_target = set_color = set_enabled = 0;
			}

			void name(const wchar_t * str) {m_name = str;}

			void level(const wchar_t * str) {m_level = convert_to_level(str); set_level = 1;}

			void prefix(const wchar_t * str) {m_prefix = convert_to_prefix(str); set_prefix = 1;}

			void target(const wchar_t * str) {m_target = convert_to_target(str); set_target = 1;}

			void color(const wchar_t * str) {m_color = convert_to_bit(str); set_color = 1;}

			void enabled(const wchar_t * str) {m_enabled = convert_to_bit(str); set_enabled = 1;}

			void execute() {
				if (!m_name.empty()) {
					auto module = logger::get_module_impl(m_name.c_str());
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
			ustring       m_name;
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

		bool get_value(const wchar_t * name, const wchar_t * parameter, const wchar_t *& value)
		{
			auto len = cstr::length(name);

			if (len && cstr::compare_ci(name, parameter, len) == 0) {
				value = parameter + len;
				return true;
			}

			return false;
		}

		ustring get_parameter(ustring & str)
		{
			auto first = str.find_last_of(L"?;");
			if (first != ustring::npos) {
				ustring ret = str.substr(first + 1);
				str.erase(first);
				return ret;
			}
			return ustring();
		}

		void set_defaults(ustring options)
		{
			//console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, options.c_str());
			while (!options.empty()) {
				auto prm = get_parameter(options);
				//console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, prm.c_str());
				const wchar_t * value = nullptr;
				if (get_value(L"level=", prm.c_str(), value)) {
					defaults::set_level(convert_to_level(value));
				} else if (get_value(L"prefix=", prm.c_str(), value)) {
					defaults::set_prefix(convert_to_prefix(value));
				} else if (get_value(L"target=", prm.c_str(), value)) {
					defaults::set_target(convert_to_target(value));
				}
			}
		}

		void set_globals(ustring /*options*/)
		{
		}

		void set_module(ustring options)
		{
			//console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, options.c_str());
			ModuleOptionsSetter mos;
			while (!options.empty()) {
				auto prm = get_parameter(options);
				//console::printf(L"%S:%d '%s'\n", __PRETTY_FUNCTION__, __LINE__, prm.c_str());
				const wchar_t * value = nullptr;
				if (get_value(L"name=", prm.c_str(), value)) {
					mos.name(value);
				} else if (get_value(L"level=", prm.c_str(), value)) {
					mos.level(value);
				} else if (get_value(L"prefix=", prm.c_str(), value)) {
					mos.prefix(value);
				} else if (get_value(L"target=", prm.c_str(), value)) {
					mos.target(value);
				} else if (get_value(L"color=", prm.c_str(), value)) {
					mos.color(value);
				} else if (get_value(L"enable=", prm.c_str(), value)) {
					mos.enabled(value);
				}
			}
			mos.execute();
		}

	}

	void set_options(const wchar_t * url)
	{
		URL_COMPONENTSW info;
		bool res = url::crack(url, &info);

		if (res && cstr::compare_ci(L"logger", info.lpszScheme, info.dwSchemeLength) == 0) {
			if (cstr::compare_ci(L"/default", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_defaults(ustring(info.lpszExtraInfo, info.dwExtraInfoLength));
			} else if (cstr::compare_ci(L"/global", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_globals(ustring(info.lpszExtraInfo, info.dwExtraInfoLength));
			} else if (cstr::compare_ci(L"/module", info.lpszUrlPath, info.dwUrlPathLength) == 0) {
				set_module(ustring(info.lpszExtraInfo, info.dwExtraInfoLength));
			}
		}
	}

}
