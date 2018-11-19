#include "stdafx.h"
#include "input_manager.h"

CInputManager::CInputManager()
{
}

CInputManager::~CInputManager()
{
	Close();
}

HRESULT CInputManager::CreateInstance(REFIID riid, void** ppvObj)
{
	HRESULT hr = S_OK;
	if (ppvObj == nullptr)
		return E_POINTER;

	CInputManager* pInputMgr = new CInputManager();
	if (FAILED(hr = pInputMgr->QueryInterface(riid, ppvObj)))
	{
		delete pInputMgr;
		return hr;
	}

	return hr;
}

//
// interface IUnknown
//
STDMETHODIMP CInputManager::NonDelegatingQueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == IID_IInputManager)
		return GetCOMInterface(static_cast<IInputManager*>(this), ppvObject);

	return CComUnknown::NonDelegatingQueryInterface(riid, ppvObject);
}

//
// IVAInputManager
//
STDMETHODIMP CInputManager::Open()
{
	return S_OK;
}

STDMETHODIMP CInputManager::Close()
{
	for(size_t i = 0; i < m_vxCsmrs.size(); i++ )
	{
		m_vxCsmrs[i]->Close();
		m_vxCsmrs[i]->Release();
	}

	m_vxCsmrs.clear();

	return S_OK;
}

STDMETHODIMP CInputManager::AddConsumer(
		/* [in] */	IInputConsumer* pCsmr,
		/* [in] */	DWORD dwReserved )
{
	if( m_vxCsmrs.size() ) {
		m_vxCsmrs[0]->OnLostFocus();
	}

	m_vxCsmrs.insert( m_vxCsmrs.begin(), pCsmr );
	pCsmr->AddRef();
	pCsmr->OnFocus();

	return S_OK;
}

STDMETHODIMP CInputManager::RemoveConsumer(
		/* [in] */	IInputConsumer* pCsmr)
{
	for( std::vector<IInputConsumer*>::iterator it = m_vxCsmrs.begin();
		it != m_vxCsmrs.end(); it++ )
	{
		if( pCsmr == *it )
		{
			pCsmr->Close();

			BOOL bNewFocus = (it == m_vxCsmrs.begin());
			m_vxCsmrs.erase( it );
			pCsmr->Release();

			if( bNewFocus && m_vxCsmrs.size() ) {
				m_vxCsmrs[0]->OnFocus();
			}

			return S_OK;
		}
	}

	return E_FAIL;
}

STDMETHODIMP CInputManager::GetConsumerCount(
		/* [out] */	unsigned long* pCnt )
{
	*pCnt = m_vxCsmrs.size();
	return S_OK;
}

STDMETHODIMP CInputManager::GetFocusedConsumer(
		/* [out] */	IInputConsumer** ppCsmr )
{
	if( m_vxCsmrs.size() )
	{
		*ppCsmr = m_vxCsmrs[0];
		m_vxCsmrs[0]->AddRef();

		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP CInputManager::GetConsumer(
	/* [out] */	unsigned long nIdx,
	/* [out] */	IInputConsumer** ppCsmr )
{
	if( nIdx < m_vxCsmrs.size() )
	{
		*ppCsmr = m_vxCsmrs[nIdx];
		m_vxCsmrs[nIdx]->AddRef();

		return S_OK;
	}

	return E_FAIL;
}

STDMETHODIMP CInputManager::MoveToTop(
	/* [in] */	IInputConsumer* pCsmr)
{
	for( std::vector<IInputConsumer*>::iterator it = m_vxCsmrs.begin();
		it != m_vxCsmrs.end(); it++ )
	{
		if( pCsmr == *it )
		{
			if( it == m_vxCsmrs.begin() )
				return S_FALSE;

			m_vxCsmrs.erase( it );
			pCsmr->Release();

			m_vxCsmrs[0]->OnLostFocus();

			return AddConsumer( pCsmr );
		}
	}

	return E_FAIL;
}

STDMETHODIMP CInputManager::RequestInput()
{
	if (m_spMainThreadInputProvider == nullptr)
		return E_NOTIMPL;

	return m_spMainThreadInputProvider->ProcessInput();
}

HRESULT CInputManager::SetMainThreadInputProvider(IInputProvider* pMainThreadInputProvider)
{
	m_spMainThreadInputProvider = pMainThreadInputProvider;
	return S_OK;
}
