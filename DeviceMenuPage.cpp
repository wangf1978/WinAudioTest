#include "stdafx.h"
#include "DeviceMenuPage.h"
#include <functiondiscoverykeys_devpkey.h>
#include "MMDeviceUtility.h"

CDeviceMenuPage::CDeviceMenuPage(CDeviceContext* pCtxDevice, IMenuNavigator* pNavigator, MENUPAGE_COOKIE cookieMenuPage, IMenuPage* pUpperMenu)
	: CBaseMenuPage(NAVMENU_ID_DEVICE, cookieMenuPage, pUpperMenu)
	, m_pCtxDevice(pCtxDevice)
	, m_spNavigator(pNavigator)
{
	CDeviceCookie* pDeviceCookie = (CDeviceCookie*)cookieMenuPage;
	m_wstrDeviceID = pDeviceCookie->pwstrDeviceID;
}


CDeviceMenuPage::~CDeviceMenuPage()
{
}

STDMETHODIMP CDeviceMenuPage::Process(const char* szInput)
{
	if (szInput == nullptr)
		return E_INVALIDARG;

	HRESULT hr = S_OK;
	switch (szInput[0])
	{
	case 'x':
		break;
	case 'b':
		break;
	case 'f':
		break;
	case 'p':
		break;
	case 'i':
		if (FAILED(hr = ShowProperties(4)))
		{
			wprintf(L"Failed to show the properties of the current device.\n");
			ShowInputPrompt();
		}
		else
			Show();
		break;
	default:
		hr = CBaseMenuPage::Process(szInput);
	}

	return hr;
}

STDMETHODIMP CDeviceMenuPage::OnNotify(unsigned int message, NOTIFY_PARAM param1, NOTIFY_PARAM param2)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceMenuPage::Show()
{
	HRESULT hr = S_OK;
	ComPtr<IMMDevice> spDevice;
	hr = m_pCtxDevice->spDeviceEnumerator->GetDevice(m_wstrDeviceID.c_str(), &spDevice);
	if (FAILED(hr))
		return hr;

	DWORD nDeviceState = (DWORD)-1;
	hr = spDevice->GetState(&nDeviceState);

	ComPtr<IPropertyStore> spPropStore;
	std::wstring wstrDeviceIFName, wstrDeviceName;
	if (SUCCEEDED(hr = spDevice->OpenPropertyStore(STGM_READ, &spPropStore)))
	{
		PROPVARIANT pv;
		PropVariantInit(&pv);
		if (SUCCEEDED(spPropStore->GetValue(PKEY_Device_FriendlyName, &pv)) && pv.vt == VT_LPWSTR)
			wstrDeviceName = pv.pwszVal;
		PropVariantClear(&pv);

		PropVariantInit(&pv);
		if (SUCCEEDED(spPropStore->GetValue(PKEY_DeviceInterface_FriendlyName, &pv)) && pv.vt == VT_LPWSTR)
			wstrDeviceIFName = pv.pwszVal;
		PropVariantClear(&pv);
	}

	wprintf(L"--------------------------------------------------------------------------------\n");
	wprintf(L"%20s: %s\n", L"DeviceID", m_wstrDeviceID.c_str());
	wprintf(L"%20s: %s\n", L"DeviceName", wstrDeviceName.c_str());
	wprintf(L"%20s: %s\n", L"DeviceInterfaceName", wstrDeviceIFName.c_str());
	wprintf(L"%20s: %s\n", L"State", 
		nDeviceState == DEVICE_STATE_ACTIVE ? L"Active" : (
			nDeviceState == DEVICE_STATE_UNPLUGGED ? L"Unplugged" : (
				nDeviceState == DEVICE_STATE_DISABLED ? L"Disabled" : (
					nDeviceState == DEVICE_STATE_NOTPRESENT ? L"Not Present" : L""))));
	wprintf(L"--------------------------------------------------------------------------------\n");

	wprintf(L"[i] Show all properties    ");
	wprintf(L"\n");

	ShowGeneralNavigationMenu();

	ShowInputPrompt();

	return S_OK;
}

HRESULT CDeviceMenuPage::OnListSelect(UINT nSelIdx)
{
	return E_NOTIMPL;
}

HRESULT CDeviceMenuPage::ShowProperties(size_t indent)
{
	WCHAR wszTmp[2048];
	WCHAR wszIndent[1024];
	HRESULT hr = S_OK;

	ComPtr<IMMDevice> spDevice;
	hr = m_pCtxDevice->spDeviceEnumerator->GetDevice(m_wstrDeviceID.c_str(), &spDevice);
	if (FAILED(hr))
		return hr;

	size_t ccActualIndent = _countof(wszIndent) > (size_t)indent + 1 ? (size_t)indent : _countof(wszIndent) - 1;
	wmemset(wszIndent, L' ', ccActualIndent);
	wszIndent[ccActualIndent] = L'\0';

	ComPtr<IPropertyStore> spPropStore;
	hr = spDevice->OpenPropertyStore(STGM_READ, &spPropStore);
	if (FAILED(hr))
	{
		printf("Failed to open the property store of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return hr;
	}

	DWORD cProps = 0;
	hr = spPropStore->GetCount(&cProps);
	for (DWORD i = 0; i < cProps; i++)
	{
		PROPERTYKEY propkey;
		if (FAILED(hr = spPropStore->GetAt(i, &propkey)))
		{
			printf("Failed to get the property key at position: %d of property store {hr: 0X%X}.\n", i, hr);
			continue;
		}

		CMMDeviceUtility::GetDevicePropDesc(spPropStore.Get(), propkey, wszTmp, _countof(wszTmp), indent);
		wprintf(L"%s", wszTmp);
	}

	wprintf(L"\n");

	return S_OK;
}
