// WinAudioTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mmdeviceapi.h>
#include <wrl/client.h>
#include <tuple>
#include <propvarutil.h>
#include <string>
#include <Functiondiscoverykeys_devpkey.h>
#include <conio.h>

using namespace Microsoft::WRL;

#define DECL_TUPLE_A(x)		{x, #x}
#define DECL_TUPLE_W(x)		{x, _T(#x)}

#ifdef _UNICODE
#define DECL_TUPLE(x)		DECL_TUPLE_W(##x)
#else
#define DECL_TUPLE(x)		DECL_TUPLE_A(##x)
#endif

using FLAG_NAME_PAIR = const std::tuple<INT, const WCHAR*>;

static FLAG_NAME_PAIR Device_state_flag_names[] = {
	DECL_TUPLE_W(DEVICE_STATE_ACTIVE),
	DECL_TUPLE_W(DEVICE_STATE_DISABLED),
	DECL_TUPLE_W(DEVICE_STATE_NOTPRESENT),
	DECL_TUPLE_W(DEVICE_STATE_UNPLUGGED),
};

static FLAG_NAME_PAIR Data_flow_flag_names[] = {
	DECL_TUPLE_W(eRender),
	DECL_TUPLE_W(eCapture),
	DECL_TUPLE_W(eAll),
};

static FLAG_NAME_PAIR Role_flag_names[] = {
	DECL_TUPLE_W(eConsole),
	DECL_TUPLE_W(eMultimedia),
	DECL_TUPLE_W(eCommunications),
};

void GetFlagsDesc(INT nFlags, FLAG_NAME_PAIR* flag_names, size_t flag_count, WCHAR* szDesc, int ccDesc)
{
	bool bFirst = true;
	int ccWritten = 0;
	memset(szDesc, 0, ccDesc);
	for (size_t i = 0; i < flag_count; i++)
	{
		if (nFlags&(std::get<0>(flag_names[i])))
		{
			ccWritten += _stprintf_s(szDesc + ccWritten, ccDesc - ccWritten, L"%s%s", !bFirst ? L" | " : L"", std::get<1>(flag_names[i]));
			bFirst = false;
		}
	}
}

const WCHAR* GetEnumerateName(INT nEnumValue, FLAG_NAME_PAIR* enum_names, size_t enum_count)
{
	for (size_t i = 0; i < enum_count; i++)
		if (nEnumValue == std::get<0>(enum_names[i]))
			return std::get<1>(enum_names[i]);

	return NULL;
}

int GetDeviceTitleName(IMMDevice* pDevice, WCHAR* wszTitleName, int ccTitleName)
{
	if (pDevice == nullptr)
		return -1;

	LPWSTR strID = nullptr;
	if (FAILED(pDevice->GetId(&strID)))
		return -1;

	int ccWritten = swprintf_s(wszTitleName, ccTitleName, L"%s", strID);
	CoTaskMemFree(strID);

	ComPtr<IPropertyStore> spPropStore;
	if (SUCCEEDED(pDevice->OpenPropertyStore(STGM_READ, &spPropStore)))
	{
		PROPVARIANT propvar;
		PropVariantInit(&propvar);
		if (SUCCEEDED(spPropStore->GetValue(PKEY_DeviceInterface_FriendlyName, &propvar)))
		{
			ccWritten += swprintf_s(wszTitleName + ccWritten, ccTitleName - ccWritten, L"/");
			ccWritten += swprintf_s(wszTitleName + ccWritten, ccTitleName - ccWritten, L"%s", propvar.pwszVal);
		}
		PropVariantClear(&propvar);
	}

	return 0;
}

int ShowDeviceInfo(IMMDevice* pDevice, size_t indent = 0)
{
	WCHAR wszTmp[1024];
	WCHAR wszIndent[1024];
	HRESULT hr = S_OK;

	LPWSTR strIdEndPoint = nullptr;
	hr = pDevice->GetId(&strIdEndPoint);
	if (FAILED(hr))
	{
		printf("Failed to get the id of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	wprintf(L"Default render/multimedia audio end-point id: %s\n", strIdEndPoint);
	CoTaskMemFree(strIdEndPoint);

	DWORD dwDeviceState = 0;
	hr = pDevice->GetState(&dwDeviceState);
	if (FAILED(hr))
	{
		printf("Failed to get the state of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	GetFlagsDesc(dwDeviceState, Device_state_flag_names, _countof(Device_state_flag_names), wszTmp, _countof(wszTmp));
	_tprintf(_T("Default render/multimedia audio end-point state: 0X%X(%s).\n"), dwDeviceState, wszTmp);

	ComPtr<IPropertyStore> spPropStore;
	hr = pDevice->OpenPropertyStore(STGM_READ, &spPropStore);
	if (FAILED(hr))
	{
		printf("Failed to open the property store of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	DWORD cProps = 0;
	hr = spPropStore->GetCount(&cProps);
	PROPERTYKEY propkey;
	PROPVARIANT propvalue;
	
	size_t ccActualIndent = _countof(wszIndent) > (size_t)indent + 1 ? (size_t)indent : _countof(wszIndent) - 1;
	wmemset(wszIndent, L' ', ccActualIndent);
	wszIndent[ccActualIndent] = L'\0';

	for (DWORD i = 0; i < cProps; i++)
	{
		if (FAILED(spPropStore->GetAt(i, &propkey)))
			continue;

		// https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/device-properties
		// https://docs.microsoft.com/en-us/windows/desktop/CoreAudio/audio-endpoint-properties
		PropVariantInit(&propvalue);
		if (FAILED(spPropStore->GetValue(propkey, &propvalue)))
			printf("Failed to get store value for property: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%u, {hr: 0X%X}.\n",
				propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
				propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
				propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid,
				hr);
		else
		{
			PropVariantToString(propvalue, wszTmp, _countof(wszTmp));
			wprintf(L"%s{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%u: %s\n", wszIndent,
				propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
				propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
				propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid,
				wszTmp);

		}
		PropVariantClear(&propvalue);
	}

	return 0;
}

void PrintDeviceTitle(IMMDevice* pDevice, size_t indent = 0)
{
	WCHAR wszTmp[1024];

	memset(wszTmp, 0, sizeof(wszTmp));
	GetDeviceTitleName(pDevice, wszTmp, _countof(wszTmp));

	WCHAR wszFmtStr[64];
	swprintf_s(wszFmtStr, _countof(wszFmtStr), L"%%%zus%%s:\n", indent);

	wprintf(wszFmtStr, L"", wszTmp);
}

int ShowDefaultAudioEndPoint(IMMDeviceEnumerator* pEnumerator, size_t indent = 0)
{
	HRESULT hr = S_OK;
	
	ComPtr<IMMDevice> spDevice;
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &spDevice);
	if (FAILED(hr))
	{
		printf("Failed to get the default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	PrintDeviceTitle(spDevice.Get(), indent);
	ShowDeviceInfo(spDevice.Get(), indent + 4);

	return 0;
}

int EnumerateAudioEndPoints(IMMDeviceEnumerator* pEnumerator, size_t indent = 0)
{
	HRESULT hr = S_OK;
	ComPtr<IMMDeviceCollection> spDeviceCollection;
	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATEMASK_ALL, &spDeviceCollection);
	if (FAILED(hr))
	{
		printf("Failed to enumerate the audio endpoints {hr: 0X%X}.\n", hr);
		return -1;
	}

	UINT cDevices = 0;
	hr = spDeviceCollection->GetCount(&cDevices);
	if (FAILED(hr))
	{
		printf("Failed to get the device counts {hr: 0X%X}.\n", hr);
		return -1;
	}

	ComPtr<IMMDevice> spDevice;
	for (UINT i = 0; i < cDevices; i++)
	{
		hr = spDeviceCollection->Item(i, &spDevice);
		if (FAILED(hr))
		{
			printf("Failed to get the MMDevice at the position: %u.\n", i);
			continue;
		}

		PrintDeviceTitle(spDevice.Get(), indent);
		ShowDeviceInfo(spDevice.Get(), indent + 4);
	}

	return 0;
}

class CMMNotificationClient : public IMMNotificationClient
{
protected:
	CMMNotificationClient(IMMDeviceEnumerator* pDeviceEnumerator) 
		: m_cRef(0)
		, m_spDeviceEnumerator(pDeviceEnumerator){

	}

public:
	static HRESULT CreateInstance(IMMDeviceEnumerator* pDeviceEnumerator, REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
			return E_POINTER;

		if (riid != __uuidof(IUnknown) && riid != __uuidof(IMMNotificationClient))
			return E_NOINTERFACE;

		CMMNotificationClient* pNotifyClient = new CMMNotificationClient(pDeviceEnumerator);
		if (pNotifyClient == nullptr)
			return E_OUTOFMEMORY;

		HRESULT hr = S_OK;
		if (FAILED(hr = pNotifyClient->QueryInterface(riid, ppvObject)))
		{
			delete pNotifyClient;
			return hr;
		}

		return hr;
	}

	virtual ~CMMNotificationClient()
	{

	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		if (ppvObject == nullptr)
			return E_POINTER;

		if (riid == __uuidof(IUnknown))
			*ppvObject = (IUnknown*)this;
		else if (riid == __uuidof(IMMNotificationClient))
			*ppvObject = (IMMNotificationClient*)this;
		else
			return E_NOINTERFACE;

		AddRef();

		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
#ifdef _DEBUG
		LONG lRef = InterlockedIncrement(&m_cRef);
		assert(lRef > 0);
#else
		InterlockedIncrement(&m_cRef);
#endif

		return m_cRef;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		LONG lRef = InterlockedDecrement(&m_cRef);
		assert(lRef >= 0);

		if (lRef == 0)
		{
			m_cRef++;
			delete this;
			return int(0);
		}

		return lRef;
	}

	virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
		/* [annotation][in] */
		_In_  LPCWSTR pwstrDeviceId,
		/* [annotation][in] */
		_In_  DWORD dwNewState)
	{
		WCHAR wszTmp[1024];
		GetFlagsDesc(dwNewState, Device_state_flag_names, _countof(Device_state_flag_names), wszTmp, _countof(wszTmp));
		wprintf(L"Device with id: %s change to new states: 0X%X (%s)\n",
			pwstrDeviceId, dwNewState, wszTmp);
		
		ComPtr<IMMDevice> spDevice;
		if (FAILED(m_spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		{
			printf("Failed to get the device instance.\n");
		}
		else
		{
			ShowDeviceInfo(spDevice.Get(), 4);
		}

		return S_OK;
	}

	virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnDeviceAdded(
		/* [annotation][in] */
		_In_  LPCWSTR pwstrDeviceId)
	{
		wprintf(L"New Device with id: %s is added.\n", pwstrDeviceId);

		ComPtr<IMMDevice> spDevice;
		if (FAILED(m_spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		{
			printf("Failed to get the device instance.\n");
		}
		else
		{
			ShowDeviceInfo(spDevice.Get(), 4);
		}

		return S_OK;
	}

	virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnDeviceRemoved(
		/* [annotation][in] */
		_In_  LPCWSTR pwstrDeviceId)
	{
		wprintf(L"New Device with id: %s is removed.\n", pwstrDeviceId);

		ComPtr<IMMDevice> spDevice;
		if (FAILED(m_spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		{
			printf("Failed to get the device instance.\n");
		}
		else
		{
			ShowDeviceInfo(spDevice.Get(), 4);
		}

		return S_OK;
	}

	virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
		/* [annotation][in] */
		_In_  EDataFlow flow,
		/* [annotation][in] */
		_In_  ERole role,
		/* [annotation][in] */
		_In_  LPCWSTR pwstrDefaultDeviceId)
	{
		wprintf(L"Default Device is changed to %s {flow: %s, role: %s}.\n", pwstrDefaultDeviceId,
			GetEnumerateName(flow, Data_flow_flag_names, _countof(Data_flow_flag_names)),
			GetEnumerateName(role, Role_flag_names, _countof(Role_flag_names)));

		ComPtr<IMMDevice> spDevice;
		if (FAILED(m_spDeviceEnumerator->GetDevice(pwstrDefaultDeviceId, &spDevice)))
		{
			printf("Failed to get the device instance.\n");
		}
		else
		{
			ShowDeviceInfo(spDevice.Get(), 4);
		}

		return S_OK;
	}

	virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
		/* [annotation][in] */
		_In_  LPCWSTR pwstrDeviceId,
		/* [annotation][in] */
		_In_  const PROPERTYKEY propkey)
	{
		wprintf(L"The property {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%u of device %s.\n", 
			propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
			propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
			propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid,
			pwstrDeviceId);

		return S_OK;
	}

private:
	LONG m_cRef;
	ComPtr<IMMDeviceEnumerator> m_spDeviceEnumerator;
};

void PrintCUI(int menu_id = 0)
{
	if (menu_id == 0)
	{
		printf("Please select the command:\n");
		printf("\t[l] List all endpoint devices\n");
		printf("\t[d] List the default audio endpoint device\n");
		printf("\t[x] Quit\n");
	}

}

int main()
{
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	ComPtr<IMMDeviceEnumerator> spEnumerator;

	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	HRESULT hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&spEnumerator);

	if (FAILED(hr))
	{
		printf("Failed to create a MMDeviceEnumerator {hr: 0X%X}.\n", hr);
		return -1;
	}

	ShowDefaultAudioEndPoint(spEnumerator.Get());

	EnumerateAudioEndPoints(spEnumerator.Get());

	ComPtr<IMMNotificationClient> spMMNotificationClient;
	hr = CMMNotificationClient::CreateInstance(spEnumerator.Get(), __uuidof(IMMNotificationClient), (void**)&spMMNotificationClient);

	if (FAILED(hr))
	{
		printf("Failed to create MMNotificationClient instance to receive the notification {hr: 0X%X}.\n", hr);
		return -1;
	}

	hr = spEnumerator->RegisterEndpointNotificationCallback(spMMNotificationClient.Get());
	if (FAILED(hr))
	{
		printf("Failed to register end-point notification callback {hr: 0X%X}.\n", hr);
		return -1;
	}

	PrintCUI();

	bool bExit = false;
	int ch = 0;
	while (!bExit)
	{
		ch = _getch();
		switch (ch)
		{
		case 'x':
		case 'X':
			bExit = true;
			break;
		case 'l':
			EnumerateAudioEndPoints(spEnumerator.Get());
			break;
		case 'd':
			ShowDefaultAudioEndPoint(spEnumerator.Get());
			break;
		}
	}

	hr = spEnumerator->UnregisterEndpointNotificationCallback(spMMNotificationClient.Get());
	if (FAILED(hr))
	{
		printf("Failed to un-register end-point nofication callback {hr: 0X%X}.\n", hr);
		return -1;
	}

    return 0;
}

