﻿#include <lib7zip/7zip.hpp>
#include <libcom/bstr.hpp>
#include <libext/exception.hpp>

namespace SevenZip {
	Logger::Module_i * get_logger_module()
	{
		auto static module = Logger::get_module(L"SevenZip");
		return module;
	}

	///======================================================================================== Prop
	Prop::Prop(const Com::Object<IInArchive> & arc, size_t idx)
	{
		Com::BStr m_nm;
		VARTYPE type;
		CheckApiError(arc->GetArchivePropertyInfo(idx, &m_nm, &id, &type));
		CheckApiError(arc->GetArchiveProperty(id, prop.ref()));
		name = (m_nm) ? ustring(m_nm.c_str()) : ustring();
	}

	///======================================================================================= Props
	void Props::cache(const Com::Object<IInArchive> & arc)
	{
		UInt32 num_props = 0;
		CheckApiError(arc->GetNumberOfArchiveProperties(&num_props));
		clear();
		reserve(num_props);
		for (UInt32 idx = 0; idx < num_props; ++idx) {
			emplace_back(arc, idx);
		}
	}

	Props::Props()
	{
	}

	Props::Props(const Com::Object<IInArchive> & arc)
	{
		cache(arc);
	}

	///========================================================================================= Lib
	Lib::Lib(PCWSTR path) :
		FileVersion(path),
		DynamicLibrary(path)
	{
		GET_DLL_FUNC(CreateObject);
		GET_DLL_FUNC(GetMethodProperty);
		GET_DLL_FUNC(GetNumberOfFormats);
		GET_DLL_FUNC(GetNumberOfMethods);
		GET_DLL_FUNC_NT(GetHandlerProperty);
		GET_DLL_FUNC_NT(GetHandlerProperty2);
		GET_DLL_FUNC_NT(SetLargePageMode);

		CheckApiThrowError(GetHandlerProperty2 || GetHandlerProperty, ERROR_INVALID_LIBRARY);

		m_codecs.cache(*this);
		m_methods.cache(*this);
	}

	const Codecs & Lib::codecs() const
	{
		return m_codecs;
	}

	const Methods & Lib::methods() const
	{
		return m_methods;
	}

	HRESULT Lib::get_prop(UInt32 index, PROPID prop_id, Com::PropVariant & prop) const
	{
		return GetHandlerProperty2 ? GetHandlerProperty2(index, prop_id, prop.ref()) : GetHandlerProperty(prop_id, prop.ref());
	}

	HRESULT Lib::get_prop(UInt32 index, PROPID prop_id, Com::WinGUID & guid) const
	{
		Com::PropVariant prop;
		HRESULT res = get_prop(index, prop_id, prop);
		if (res == S_OK)
			guid.init(prop);
		return res;
	}

	HRESULT Lib::get_prop(UInt32 index, PROPID prop_id, bool & value) const
	{
		Com::PropVariant prop;
		HRESULT res = get_prop(index, prop_id, prop);
		return res == S_OK ? prop.as_bool_nt(value) : res;
	}

	HRESULT Lib::get_prop(UInt32 index, PROPID prop_id, ustring & value) const
	{
		Com::PropVariant prop;
		HRESULT res = get_prop(index, prop_id, prop);
		return res == S_OK ? prop.as_str_nt(value) : res;
	}

	HRESULT Lib::get_prop(UInt32 index, PROPID prop_id, std::vector<BYTE> & value) const
	{
		Com::PropVariant prop;
		HRESULT res = get_prop(index, prop_id, prop);
		if (res != S_OK)
			return res;
		if (prop.vt == VT_BSTR) {
			size_t len = ::SysStringByteLen(prop.bstrVal);
			BYTE* data = reinterpret_cast<BYTE*>(prop.bstrVal);
			value.assign(&data[0], &data[len]);
		} else
			return E_FAIL;
		return S_OK;
	}
}
