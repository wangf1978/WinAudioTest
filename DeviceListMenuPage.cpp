#include "stdafx.h"
#include "DeviceListMenuPage.h"
#include <functiondiscoverykeys_devpkey.h>

CDeviceListMenuPage::CDeviceListMenuPage(CDeviceContext* pCtxDevice, IMenuNavigator* pNavigator, MENUPAGE_COOKIE cookieMenuPage)
	: CBaseMenuPage(NAVMENU_ID_DEVICE_LIST, cookieMenuPage)
	, m_pCtxDevice(pCtxDevice)
	, m_spNavigator(pNavigator)
{
}


CDeviceListMenuPage::~CDeviceListMenuPage()
{
}

STDMETHODIMP CDeviceListMenuPage::Process(const char* szInput)
{
	if (szInput == nullptr)
		return E_POINTER;

	HRESULT hr = S_OK;
	switch (szInput[0])
	{
	case 'l':
		m_dwDeviceStateMask = DEVICE_STATEMASK_ALL;
		Show();
	case 'a':
		m_dwDeviceStateMask = DEVICE_STATE_ACTIVE;
		Show();
		break;
	case 'd':
		m_dwDeviceStateMask = DEVICE_STATE_DISABLED;
		Show();
		break;
	case 'n':
		m_dwDeviceStateMask = DEVICE_STATE_NOTPRESENT;
		Show();
		break;
	case 'u':
		m_dwDeviceStateMask = DEVICE_STATE_UNPLUGGED;
		Show();
		break;
	case 'e':
		m_dwFlowFilter = eAll;
		Show();
	case 'r':
		m_dwFlowFilter = eRender;
		Show();
		break;
	case 'c':
		m_dwFlowFilter = eCapture;
		Show();
		break;
	case 'x':
		break;
	case 'b':
		break;
	case 'f':
		break;
	case 'p':
		break;
	default:
		hr = CBaseMenuPage::Process(szInput);
	}

	return hr;
}

HRESULT	CDeviceListMenuPage::OnListSelect(UINT nSelNo)
{
	HRESULT hr = S_OK;
	CDeviceCookie cookieDevice;

	if (nSelNo == 0 || nSelNo > m_DeviceIDList.size())
		return E_INVALIDARG;


	cookieDevice.pwstrDeviceID = m_DeviceIDList[nSelNo - 1].c_str();

	return m_spNavigator->Navigate(NAVMENU_ID_DEVICE, (MENUPAGE_COOKIE)&cookieDevice, this);
}

STDMETHODIMP CDeviceListMenuPage::OnNotify(unsigned int message, NOTIFY_PARAM param1, NOTIFY_PARAM param2)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceListMenuPage::Show()
{
	HRESULT hr = S_OK;
	UINT cDevices = 0;
	ComPtr<IMMDeviceCollection> spDevices;

	int key_number = 1;

	std::tuple<EDataFlow, const WCHAR*> DeviceFlows[] = {
		{eRender, L"Render"},
		{eCapture, L"Capture"}
	};

	std::tuple<ERole, const WCHAR*> DeviceRoles[] = {
		{eConsole, L"Games, system notification sounds, and voice commands"},
		{eMultimedia, L"Music, movies, narration, and live music recording"},
		{eCommunications, L"Voice communications"},
	};

	m_DeviceIDList.clear();

	for (size_t i = 0; i < _countof(DeviceFlows); i++)
	{
		if (m_dwFlowFilter != eAll && m_dwFlowFilter != std::get<0>(DeviceFlows[i]))
			continue;

		wprintf(L"Audio %s Devices:\n", std::get<1>(DeviceFlows[i]));
		hr = m_pCtxDevice->spDeviceEnumerator->EnumAudioEndpoints(std::get<0>(DeviceFlows[i]), m_dwDeviceStateMask, &spDevices);
		if (SUCCEEDED(hr) && SUCCEEDED(hr = spDevices->GetCount(&cDevices)))
		{
			ComPtr<IMMDevice> spDevice;
			for (UINT i = 0; i < cDevices; i++)
			{
				if (FAILED(spDevices->Item(i, &spDevice)))
					continue;

				LPWSTR lpwstrDeviceID = nullptr;
				if (FAILED(spDevice->GetId(&lpwstrDeviceID)))
					continue;

				DWORD nDeviceState = (UINT)-1;
				spDevice->GetState(&nDeviceState);

				ComPtr<IPropertyStore> spPropStores;
				PROPVARIANT propval;
				PropVariantInit(&propval);
				if (SUCCEEDED(spDevice->OpenPropertyStore(STGM_READ, &spPropStores)) && SUCCEEDED(spPropStores->GetValue(PKEY_DeviceInterface_FriendlyName, &propval)))
				{
					wprintf(L"    [%2d] %s - %s\n", key_number, propval.pwszVal,
						nDeviceState == DEVICE_STATE_ACTIVE ? L"Active" : (
							nDeviceState == DEVICE_STATE_UNPLUGGED ? L"Unplugged" : (
								nDeviceState == DEVICE_STATE_DISABLED ? L"Disabled" : (
									nDeviceState == DEVICE_STATE_NOTPRESENT ? L"Not Present" : L""))));
				}
				else
				{
					wprintf(L"    [%2d] %s - %s\n", key_number, lpwstrDeviceID,
						nDeviceState == DEVICE_STATE_ACTIVE ? L"Active" : (
							nDeviceState == DEVICE_STATE_UNPLUGGED ? L"Unplugged" : (
								nDeviceState == DEVICE_STATE_DISABLED ? L"Disabled" : (
									nDeviceState == DEVICE_STATE_NOTPRESENT ? L"Not Present" : L""))));
				}
				PropVariantClear(&propval);

				m_DeviceIDList.push_back(lpwstrDeviceID);

				CoTaskMemFree(lpwstrDeviceID);
				key_number++;
			}
		}
		else
		{
			wprintf(L"    No devices (hr: 0X%X)\n", hr);
		}

		wprintf(L"\n");

		for (size_t j = 0; j < _countof(DeviceRoles); j++)
		{
			wprintf(L"Default Audio %s Devices for %s:\n", std::get<1>(DeviceFlows[i]), std::get<1>(DeviceRoles[j]));

			ComPtr<IMMDevice> spDevice;
			hr = m_pCtxDevice->spDeviceEnumerator->GetDefaultAudioEndpoint(std::get<0>(DeviceFlows[i]), std::get<0>(DeviceRoles[j]), &spDevice);
			if (SUCCEEDED(hr))
			{
				LPWSTR lpwstrDeviceID = nullptr;
				if (FAILED(spDevice->GetId(&lpwstrDeviceID)))
					continue;

				DWORD nDeviceState = (UINT)-1;
				spDevice->GetState(&nDeviceState);

				ComPtr<IPropertyStore> spPropStores;
				PROPVARIANT propval;
				PropVariantInit(&propval);
				if (SUCCEEDED(spDevice->OpenPropertyStore(STGM_READ, &spPropStores)) && SUCCEEDED(spPropStores->GetValue(PKEY_DeviceInterface_FriendlyName, &propval)))
				{
					wprintf(L"    [%2d] %s - %s\n", key_number, propval.pwszVal,
						nDeviceState == DEVICE_STATE_ACTIVE ? L"Active" : (
							nDeviceState == DEVICE_STATE_UNPLUGGED ? L"Unplugged" : (
								nDeviceState == DEVICE_STATE_DISABLED ? L"Disabled" : (
									nDeviceState == DEVICE_STATE_NOTPRESENT ? L"Not Present" : L""))));
				}
				else
				{
					wprintf(L"    [%2d] %s - %s\n", key_number, lpwstrDeviceID,
						nDeviceState == DEVICE_STATE_ACTIVE ? L"Active" : (
							nDeviceState == DEVICE_STATE_UNPLUGGED ? L"Unplugged" : (
								nDeviceState == DEVICE_STATE_DISABLED ? L"Disabled" : (
									nDeviceState == DEVICE_STATE_NOTPRESENT ? L"Not Present" : L""))));
				}
				PropVariantClear(&propval);

				m_DeviceIDList.push_back(lpwstrDeviceID);

				CoTaskMemFree(lpwstrDeviceID);
				key_number++;
			}

			wprintf(L"\n");
		}
	}

	if (m_dwDeviceStateMask == DEVICE_STATE_ACTIVE)
		wprintf(L"[l] Show all devices    ");
	else
		wprintf(L"[a] Only show active devices    ");

	if (m_dwDeviceStateMask == DEVICE_STATE_DISABLED)
		wprintf(L"[l] Show all devices    ");
	else
		wprintf(L"[d] Only show disabled devices    ");

	if (m_dwDeviceStateMask == DEVICE_STATE_NOTPRESENT)
		wprintf(L"[l] Show all devices    ");
	else
		wprintf(L"[n] Only show not-present devices    ");

	if (m_dwDeviceStateMask == DEVICE_STATE_UNPLUGGED)
		wprintf(L"[l] Show all devices    ");
	else
		wprintf(L"[u] Only show unplugged devices");

	wprintf(L"\n");

	if (m_dwFlowFilter == eRender)
		wprintf(L"[e] Show all data-flow directions   ");
	else
		wprintf(L"[r] Only show audio render devices    ");

	if (m_dwFlowFilter == eCapture)
		wprintf(L"[e] Show all data-flow directions   ");
	else
		wprintf(L"[c] Only show audio capture devices");
	wprintf(L"\n");

	ShowGeneralNavigationMenu();

	ShowInputPrompt();

	return S_OK;
}
