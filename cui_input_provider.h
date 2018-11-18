#ifndef __CLASS_CUI_INPUT_PROVIDER_H__
#define __CLASS_CUI_INPUT_PROVIDER_H__

#include "UI.h"

class CCUIInputProvider
	: public CComUnknown
	, public IInputProvider
{
public:
	CCUIInputProvider();
	virtual ~CCUIInputProvider();

	/**
	  * @interface IInputProvider
	  * @{ */
	DECLARE_IUNKNOWN
	/** @} */

	virtual HRESULT NonDelegatingQueryInterface(REFIID uuid, void** ppvObj);

	/**
	  * @interface IInputProvider
	  * @{ */
	STDMETHOD(Open)(
		/* [in] */ IInputManager* pMgr );
	STDMETHOD(Close)();
	STDMETHOD(ProcessInput)();
	STDMETHOD(ProcessLine)(const char *szInput);
	STDMETHOD_(BOOL, IsBusy)();
	/** @} */

protected:

	IInputManager* m_mgr;
	BOOL m_keyFlag;	
};

#endif//__CLASS_CUI_INPUT_PROVIDER_H__
