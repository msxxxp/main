#include <libcom/unknown.hpp>
#include <libbase/logger.hpp>

namespace Com {

	UnknownImp::~UnknownImp()
	{
	}

	UnknownImp::UnknownImp() :
		m_ref_cnt(1)
	{
	}

	ULONG WINAPI UnknownImp::AddRef()
	{
		return InterlockedIncrement(&m_ref_cnt);
	}

	ULONG WINAPI UnknownImp::Release()
	{
		if (InterlockedDecrement(&m_ref_cnt) == 0) {
			delete this;
		}
//		LogNoise(L"%u\n", m_ref_cnt);
		return m_ref_cnt;
	}

	HRESULT WINAPI UnknownImp::QueryInterface(REFIID riid, void ** object)
	{
		if (object && IsEqualIID(riid, IID_IUnknown)) {
//			LogNoise(L"IID_IUnknown\n");
			*object = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		if (object)
			*object = nullptr;
		return E_NOINTERFACE;
	}

}
