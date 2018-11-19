#include "stdafx.h"
#include "cui_input_provider.h"

CCUIInputProvider::CCUIInputProvider()
	:m_mgr(NULL)
{
}

CCUIInputProvider::~CCUIInputProvider()
{
}

//
// interface IUnknown
//
STDMETHODIMP CCUIInputProvider::NonDelegatingQueryInterface(REFIID riid, void** ppv )
{
	if (riid == IID_IInputProvider)
		return GetCOMInterface(static_cast<IInputProvider*>(this), ppv);

	return CComUnknown::NonDelegatingQueryInterface(riid, ppv);
}

//
// interface IVAInputProvider
//
STDMETHODIMP CCUIInputProvider::Open(
	/* [in] */ IInputManager* pMgr )
{
	m_mgr = pMgr;
	m_mgr->AddRef();
	m_keyFlag = FALSE;

	return S_OK;
}

STDMETHODIMP CCUIInputProvider::Close()
{
	m_mgr->Release();
	m_mgr = NULL;

	return S_OK;
}

STDMETHODIMP CCUIInputProvider::ProcessInput()
{
	m_keyFlag = TRUE;
	
	char szInputLine[256];
	if(NULL == fgets(szInputLine, 256, stdin))
	{
		Sleep(10);
		return S_OK;
	}

	ProcessLine(szInputLine);
	return S_OK;
}

STDMETHODIMP CCUIInputProvider::ProcessLine(const char *szInput)
{
	m_keyFlag = FALSE;

	IInputConsumer* focus;
	HRESULT	hr = m_mgr->GetFocusedConsumer( &focus );
	if( SUCCEEDED( hr ) )
	{
		focus->Process( szInput );
		focus->Release();
	}
	return S_OK;
}

STDMETHODIMP_(BOOL) CCUIInputProvider::IsBusy()
{
	return m_keyFlag;
}
