#pragma once

#include "DeviceMenuNavigator.h"

class CDeviceMenuPage
	: public CBaseMenuPage
{
public:
	CDeviceMenuPage(CDeviceContext* pCtxDevice, IMenuNavigator* pNavigator, MENUPAGE_COOKIE cookieMenuPage, IMenuPage* pUpperMenu = nullptr);
	virtual ~CDeviceMenuPage();

	STDMETHOD(Process)(
		/* [in] */ const char* szInput);
	STDMETHOD(OnNotify)(
		/* [in] */ unsigned int message,
		/* [in[ */ NOTIFY_PARAM param1,
		/* [in] */ NOTIFY_PARAM param2);
	STDMETHOD(Show)();

protected:
	HRESULT							OnListSelect(UINT nSelIdx);
	UINT							GetListCount() { return 0; }
	HRESULT							ShowProperties(size_t indent = 0);

private:
	CDeviceContext*					m_pCtxDevice = nullptr;
	std::wstring					m_wstrDeviceID;
	ComPtr<IMenuNavigator>			m_spNavigator;
	
};

