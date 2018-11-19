#pragma once

#include "MenuNavigator.h"
#include <mmdeviceapi.h>
#include <string>


// {253C6C10-74A1-4C83-9983-9A325324C02A}
AMP_DEFINE_GUID(NAVMENU_ID_DEVICE_LIST ,
	0x253c6c10, 0x74a1, 0x4c83, 0x99, 0x83, 0x9a, 0x32, 0x53, 0x24, 0xc0, 0x2a);

// {C92B9F27-90AC-444C-BA30-249093790729}
AMP_DEFINE_GUID(NAVMENU_ID_DEVICE ,
	0xc92b9f27, 0x90ac, 0x444c, 0xba, 0x30, 0x24, 0x90, 0x93, 0x79, 0x7, 0x29);

#define NAVMENU_NAMEW(x)	(\
	((x) == NAVMENU_ID_DEVICE_LIST?L"Device List":\
	((x) == NAVMENU_ID_DEVICE?L"Device":L"Uknown")))

using MMDeviceFlowList = std::unordered_map<EDataFlow, std::set<std::wstring>>;
using MMDefaultDevices = std::unordered_map<EDataFlow, std::unordered_map<ERole, std::wstring>>;

struct CDeviceContext
{
	std::recursive_mutex			m_mutex;

	ComPtr<IMMDeviceEnumerator>		spDeviceEnumerator;

	std::set<std::wstring>			m_DeviceList;
	MMDeviceFlowList				m_DeviceFlowList;
	MMDefaultDevices				m_DefaultDevices;

	HRESULT							UpdateFlowDeviceList();
	HRESULT							UpdateDeviceList();
	HRESULT							UpdateDefaultDeviceList();

	HRESULT							OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState);
	HRESULT							OnDeviceAdded(LPCWSTR pwstrDeviceId);
	HRESULT							OnDeviceRemoved(LPCWSTR pwstrDeviceId);
	HRESULT							OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId);
	HRESULT							OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key);
};

struct CDeviceCookie
{
	union
	{
		// For NAVMENU_ID_DEVICE
		struct
		{
			LPCWSTR					pwstrDeviceID;
		};
	};

	bool Equal(NAVIMENU_ID idMenu, const CDeviceCookie* cookieDevice)
	{
		if (idMenu == NAVMENU_ID_DEVICE_LIST)
		{
			return true;
		}
		else if (idMenu == NAVMENU_ID_DEVICE)
		{
			if (cookieDevice == nullptr)
				return false;

			return _wcsicmp(pwstrDeviceID, cookieDevice->pwstrDeviceID) == 0 ? true : false;
		}

		return false;
	}
};

class CDeviceMenuNavigator
	: public CMenuNavigator
	, public IMMNotificationClient
	, public IInputConsumer
{
public:
	CDeviceMenuNavigator(HRESULT* phr = nullptr);
	virtual ~CDeviceMenuNavigator();

	//
	// IUnknown interface
	//
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, void** ppvObject);

	//
	// CMenuNavigator
	//
	STDMETHOD(IsSupport)(/* [in]  */ NAVIMENU_ID idMenu);
	STDMETHOD(RequestInput)();
	virtual HRESULT ActivateMenuPage(
		IMenuNavigator* pNavigator, NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, 
		IMenuPage* pUpperMenuPage, IMenuPage** ppMenuPage);

	//
	// IInputConsumer interface
	//
	STDMETHOD(Open)(
		/* [in] */ IInputManager* pMgr);
	STDMETHOD(Close)();
	STDMETHOD(Process)(
		/* [in] */ const char* szInput);
	STDMETHOD(OnFocus)();
	STDMETHOD(OnLostFocus)();

	//
	// IMMNotificationClient interface
	//
	virtual HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
		_In_  LPCWSTR pwstrDeviceId,
		_In_  DWORD dwNewState);

	virtual HRESULT STDMETHODCALLTYPE OnDeviceAdded(
		_In_  LPCWSTR pwstrDeviceId);

	virtual HRESULT STDMETHODCALLTYPE OnDeviceRemoved(
		_In_  LPCWSTR pwstrDeviceId);

	virtual HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		_In_  EDataFlow flow,
		_In_  ERole role,
		_In_  LPCWSTR pwstrDefaultDeviceId);

	virtual HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
		_In_  LPCWSTR pwstrDeviceId,
		_In_  const PROPERTYKEY key);

	virtual HRESULT Run(NAVIMENU_ID idInitMenu, MENUPAGE_COOKIE cookieInitMenuPage);

protected:
	CDeviceContext					m_ctxDevice;

	BOOL							m_bClosed = TRUE;
	ComPtr<IInputManager>			m_spInputMgr;
};

