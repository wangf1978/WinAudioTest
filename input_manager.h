#ifndef __CLASS_INPUT_MANAGER_H__
#define __CLASS_INPUT_MANAGER_H__

#include "UI.h"

#include <vector>
#include "combase.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

class CInputManager
	: public CComUnknown
	, public IInputManager
{
private:
	CInputManager();

public:
	virtual ~CInputManager();

	static HRESULT CreateInstance(REFIID riid, void** ppvObj);

	/**
	  * @interface IUnknown
	  * @{ */
	DECLARE_IUNKNOWN
	/** @} */

	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, void** ppvObject);

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
	STDMETHOD(RequestInput)();
	/** @} */

	virtual HRESULT SetMainThreadInputProvider(IInputProvider* pMainThreadInputProvider);

protected:

	std::vector<IInputConsumer*>	m_vxCsmrs;
	ComPtr<IInputProvider>			m_spMainThreadInputProvider;
};

#endif//__CLASS_INPUT_MANAGER_H__
