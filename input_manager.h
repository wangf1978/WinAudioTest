#ifndef __CLASS_INPUT_MANAGER_H__
#define __CLASS_INPUT_MANAGER_H__

#include "UI.h"

#include <vector>
#include "combase.h"

class CInputManager
	: CComUnknown
	, public IInputManager
{
public:
	CInputManager();
	virtual ~CInputManager();

	/**
	  * @interface IUnknown
	  * @{ */
	DECLARE_IUNKNOWN
	/** @} */

	virtual HRESULT NonDelegatingQueryInterface(REFIID riid, void** ppvObject);

	/**
	  * @interface IInputManager
	  * @{ */
	STDMETHOD(Open)();
	STDMETHOD(Close)();
	STDMETHOD(AddConsumer)(
		/* [in] */	IInputConsumer* pCsmr,
		/* [in] */	DWORD dwReserved = 0 );
	STDMETHOD(RemoveConsumer)(
		/* [in] */	IInputConsumer* pCsmr);
	STDMETHOD(GetConsumerCount)(
		/* [out] */	unsigned long* pCnt );
	STDMETHOD(GetFocusedConsumer)(
		/* [out] */	IInputConsumer** ppCsmr );
	STDMETHOD(GetConsumer)(
		/* [out] */	unsigned long nIdx,
		/* [out] */	IInputConsumer** ppCsmr );
	STDMETHOD(MoveToTop)(
		/* [in] */	IInputConsumer* pCsmr);
	/** @} */

protected:

	std::vector<IInputConsumer*> m_vxCsmrs;
};

#endif//__CLASS_INPUT_MANAGER_H__
