#pragma once

#include "DeviceMenuNavigator.h"

class CDeviceListMenuPage: public CBaseMenuPage
{
public:
	CDeviceListMenuPage(CDeviceContext* pCtxDevice, IMenuNavigator* pNavigator, MENUPAGE_COOKIE cookieMenuPage);
	virtual ~CDeviceListMenuPage();

	STDMETHOD(Process)(
		/* [in] */ const char* szInput);
	STDMETHOD(OnNotify)(
		/* [in] */ unsigned int message,
		/* [in[ */ NOTIFY_PARAM param1,
		/* [in] */ NOTIFY_PARAM param2);
	STDMETHOD(Show)();

protected:
	HRESULT							OnListSelect(UINT nSelIdx);
	UINT							GetListCount() { return (UINT)m_DeviceIDList.size(); }

protected:
	CDeviceContext*					m_pCtxDevice = nullptr;
	std::vector<std::wstring>		m_DeviceIDList;
	ComPtr<IMenuNavigator>			m_spNavigator;
	DWORD							m_dwDeviceStateMask = DEVICE_STATEMASK_ALL;
	DWORD							m_dwFlowFilter = eAll;
};

