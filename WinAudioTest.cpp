// WinAudioTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define INITGUID
#include <mmdeviceapi.h>
#include <wrl/client.h>
#include <tuple>
#include <propvarutil.h>
#include <string>
#include <devpkey.h>
#include <propkey.h>
#include <conio.h>
#include "MMDeviceUtility.h"

using namespace Microsoft::WRL;

void PrintBuf(unsigned char* buf, int buf_size, int nLeadingSpace)
{
	WCHAR szTmp[2048];
	int nLeftChars = buf_size;
	int nLines = nLeftChars / 16 + (nLeftChars % 16 == 0 ? 0 : 1);

	if (nLeadingSpace > _countof(szTmp))
		return;

	for (int i = 0; i < nLeadingSpace; i++)
		szTmp[i] = L' ';

	for (int i = 0; i < nLines; i++)
	{
		int nWritten = nLeadingSpace;
		for (int j = 0; j < (nLeftChars < 16 ? nLeftChars : 16); j++)
			nWritten += swprintf_s(szTmp + nWritten, _countof(szTmp) - nWritten, L"%02X ", buf[i * 16 + j]);

		nWritten += swprintf_s(szTmp + nWritten, _countof(szTmp) - nWritten, L"- ");

		for (int j = 0; j < (nLeftChars < 16 ? nLeftChars : 16); j++)
			nWritten += swprintf_s(szTmp + nWritten, _countof(szTmp) - nWritten, L"%c",
				iswprint(buf[i * 16 + j]) ? buf[i * 16 + j] : L'.');

		nWritten += swprintf_s(szTmp + nWritten, _countof(szTmp) - nWritten, L"\n");

		if (nLeftChars < 16)
			nLeftChars = 0;
		else
			nLeftChars -= 16;

		wprintf(szTmp);
	}
}

int ShowDeviceProp(IPropertyStore* pPropStore, PROPERTYKEY propkey, size_t indent=0)
{
	HRESULT hr = S_OK;
	PROPVARIANT propval;

	int ccWritten = 0;
	WCHAR wszTmp[2048];
	WCHAR wszIndent[1024];
	size_t ccActualIndent = _countof(wszIndent) > (size_t)indent + 1 ? (size_t)indent : _countof(wszIndent) - 1;
	wmemset(wszIndent, L' ', ccActualIndent);
	wszIndent[ccActualIndent] = L'\0';

	const WCHAR* wszPKeyname = nullptr;
	for (size_t i = 0; i < _countof(CMMDeviceUtility::prop_key_names); i++)
	{
		if (std::get<0>(CMMDeviceUtility::prop_key_names[i]) == propkey)
		{
			wszPKeyname = std::get<1>(CMMDeviceUtility::prop_key_names[i]);
			break;
		}
	}

	if (wszPKeyname != nullptr)
		ccWritten = swprintf_s(wszTmp, _countof(wszTmp), L"%s%40s: ", wszIndent, wszPKeyname);
	else
		ccWritten = swprintf_s(wszTmp, _countof(wszTmp), L"%s{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%u: ", 
			wszIndent, 
			propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
			propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
			propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid);

	BLOB blob = { 0, 0 };
	PropVariantInit(&propval);
	if (FAILED(hr = pPropStore->GetValue(propkey, &propval)))
		ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"N/A (hr: 0X%X}", hr);
	else
	{
		switch (propval.vt)
		{
		case VT_I2:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.iVal);
			break;
		case VT_I4:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.lVal);
			break;
		case VT_R4:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%f", propval.fltVal);
			break;
		case VT_R8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%f", propval.dblVal);
			break;
		case VT_DATE:
			{
				SYSTEMTIME system_time;
				if (::VariantTimeToSystemTime(propval.date, &system_time))
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u/%u/%u", system_time.wMonth, system_time.wDay, system_time.wYear);
			}
			break;
		case VT_FILETIME:
			{
				SYSTEMTIME system_time;
				if (::FileTimeToSystemTime(&propval.filetime, &system_time))
					ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u/%u/%u", system_time.wMonth, system_time.wDay, system_time.wYear);
			}
			break;
		case VT_BOOL:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%s", propval.boolVal?L"TRUE":L"FALSE");
			break;
		case VT_I1:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.cVal);
			break;
		case VT_UI1:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.bVal);
			break;
		case VT_UI2:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.uiVal);
			break;
		case VT_UI4:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.ulVal);
			break;
		case VT_I8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%lld", propval.hVal.QuadPart);
			break;
		case VT_UI8:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%llu", propval.uhVal.QuadPart);
			break;
		case VT_INT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%d", propval.intVal);
			break;
		case VT_UINT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%u", propval.uintVal);
			break;
		case VT_VOID:
			break;
		case VT_HRESULT:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"0X%X", propval.lVal);
			break;
		case VT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.pintVal);
			break;
		case VT_INT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.pintVal);
			break;
		case VT_UINT_PTR:
			ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"%p", propval.puintVal);
			break;
		case VT_CLSID:
			if (propval.puuid != nullptr)
			{
				ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
					propval.puuid->Data1, propval.puuid->Data2, propval.puuid->Data3,
					propval.puuid->Data4[0], propval.puuid->Data4[1], propval.puuid->Data4[2], propval.puuid->Data4[3],
					propval.puuid->Data4[4], propval.puuid->Data4[5], propval.puuid->Data4[6], propval.puuid->Data4[7]);
			}
			break;
		case VT_BLOB:
			blob = propval.blob;
			break;
		case VT_STREAM:
		case VT_STORAGE:
		case VT_STREAMED_OBJECT:
		case VT_STORED_OBJECT:
		case VT_BLOB_OBJECT:
		case VT_CF:
		case VT_EMPTY:
		case VT_NULL:
		case VT_DECIMAL:
		case VT_VARIANT:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		case VT_ERROR:
		case VT_BSTR:
		case VT_CY:
		case VT_SAFEARRAY:
		case VT_CARRAY:
		case VT_USERDEFINED:
		case VT_LPSTR:
		case VT_LPWSTR:
		case VT_RECORD:
		case VT_VERSIONED_STREAM:
		case VT_BSTR_BLOB:
		case VT_VECTOR:
		case VT_ARRAY:
		case VT_BYREF:
		case VT_RESERVED:
		case VT_ILLEGAL:
		default:
			PropVariantToString(propval, wszTmp + ccWritten, _countof(wszTmp) - ccWritten);
			ccWritten += wcslen(wszTmp + ccWritten);
		}
	}

	ccWritten += swprintf_s(wszTmp + ccWritten, _countof(wszTmp) - ccWritten, L"\n");
	wprintf(L"%s", wszTmp);

	if (blob.cbSize > 0 && blob.pBlobData != nullptr)
	{
		PrintBuf(blob.pBlobData, blob.cbSize, indent + 42);
	}

	PropVariantClear(&propval);

	return 0;
}

int ShowDeviceProp(IPropertyStore* pPropStore, UINT idxProp, size_t indent = 0)
{
	HRESULT hr = S_OK;
	PROPERTYKEY propkey;

	if (FAILED(hr = pPropStore->GetAt(idxProp, &propkey)))
	{
		printf("Failed to get the property key at position: %d of property store {hr: 0X%X}.\n", idxProp, hr);
		return -1;
	}

	return ShowDeviceProp(pPropStore, propkey, indent);
}

int ShowDeviceInfo(IMMDevice* pDevice, size_t indent = 0)
{
	WCHAR wszTmp[2048];
	WCHAR wszIndent[1024];
	HRESULT hr = S_OK;

	LPWSTR strIdEndPoint = nullptr;
	hr = pDevice->GetId(&strIdEndPoint);
	if (FAILED(hr))
	{
		printf("Failed to get the id of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	size_t ccActualIndent = _countof(wszIndent) > (size_t)indent + 1 ? (size_t)indent : _countof(wszIndent) - 1;
	wmemset(wszIndent, L' ', ccActualIndent);
	wszIndent[ccActualIndent] = L'\0';

	wprintf(L"%sDefault render/multimedia audio end-point id: %s\n", wszIndent, strIdEndPoint);
	CoTaskMemFree(strIdEndPoint);

	DWORD dwDeviceState = 0;
	hr = pDevice->GetState(&dwDeviceState);
	if (FAILED(hr))
	{
		printf("Failed to get the state of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	CMMDeviceUtility::GetFlagsDesc(dwDeviceState, CMMDeviceUtility::Device_state_flag_names, _countof(CMMDeviceUtility::Device_state_flag_names), wszTmp, _countof(wszTmp));
	_tprintf(_T("%sDefault render/multimedia audio end-point state: 0X%X(%s).\n"), wszIndent, dwDeviceState, wszTmp);

	ComPtr<IPropertyStore> spPropStore;
	hr = pDevice->OpenPropertyStore(STGM_READ, &spPropStore);
	if (FAILED(hr))
	{
		printf("Failed to open the property store of default render/multimedia audio end-point {hr: 0X%X}.\n", hr);
		return -1;
	}

	DWORD cProps = 0;
	hr = spPropStore->GetCount(&cProps);
	for (DWORD i = 0; i < cProps; i++)
	{
		//ShowDeviceProp(spPropStore.Get(), i, indent);

		PROPERTYKEY propkey;
		if (FAILED(hr = spPropStore->GetAt(i, &propkey)))
		{
			printf("Failed to get the property key at position: %d of property store {hr: 0X%X}.\n", i, hr);
			continue;
		}

		CMMDeviceUtility::GetDevicePropDesc(spPropStore.Get(), propkey, wszTmp, _countof(wszTmp), indent);
		wprintf(L"%s", wszTmp);
	}
	
	return 0;
}

void PrintDeviceTitle(IMMDevice* pDevice, size_t indent = 0)
{
	WCHAR wszTmp[1024];

	memset(wszTmp, 0, sizeof(wszTmp));
	CMMDeviceUtility::GetDeviceTitleName(pDevice, wszTmp, _countof(wszTmp));

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
		{
			*ppvObject = nullptr;
			return E_NOINTERFACE;
		}

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
		CMMDeviceUtility::GetFlagsDesc(dwNewState, CMMDeviceUtility::Device_state_flag_names, _countof(CMMDeviceUtility::Device_state_flag_names), wszTmp, _countof(wszTmp));
		wprintf(L"\n\n!!-->Device with id: %s change to new states: 0X%X (%s)\n",
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
		wprintf(L"\n\n!!-->New Device with id: %s is added.\n", pwstrDeviceId);

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
		wprintf(L"\n\n!!--> The Device with id: %s is removed.\n", pwstrDeviceId);

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
		wprintf(L"\n\n-->Default Device is changed to %s {flow: %s(%d), role: %s(%d)}.\n", pwstrDefaultDeviceId,
			CMMDeviceUtility::GetEnumerateName(flow, CMMDeviceUtility::Data_flow_flag_names, _countof(CMMDeviceUtility::Data_flow_flag_names)), flow,
			CMMDeviceUtility::GetEnumerateName(role, CMMDeviceUtility::Role_flag_names, _countof(CMMDeviceUtility::Role_flag_names)), role);

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
		wprintf(L"\n\n--> The property {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%u of device %s is changed.\n", 
			propkey.fmtid.Data1, propkey.fmtid.Data2, propkey.fmtid.Data3,
			propkey.fmtid.Data4[0], propkey.fmtid.Data4[1], propkey.fmtid.Data4[2], propkey.fmtid.Data4[3],
			propkey.fmtid.Data4[4], propkey.fmtid.Data4[5], propkey.fmtid.Data4[6], propkey.fmtid.Data4[7], propkey.pid,
			pwstrDeviceId);

		ComPtr<IMMDevice> spDevice;
		if (SUCCEEDED(m_spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		{
			ComPtr<IPropertyStore> spPropStore;
			if (SUCCEEDED(spDevice->OpenPropertyStore(STGM_READ, &spPropStore)))
			{
				ShowDeviceProp(spPropStore.Get(), propkey, 4);
			}
		}
		

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
	std::unordered_map<std::wstring, int> test;

	LPWSTR a = new WCHAR[100];
	wcscpy_s(a, 100, L"abc");

	LPWSTR b = new WCHAR[100];
	wcscpy_s(b, 100, L"abc");

	test[a] = 1;

	printf("unordered_map size: %d, count('abc'): %d.\n", test.size(), test.count(L"abc"));

	test[b] = 2;

	printf("unordered_map size: %d, count('abc'): %d.\n", test.size(), test.count(L"abc"));

	if (test.find(L"abc") != test.end())
		printf("test['abc']: %d.\n", test[L"abc"]);
	else
		printf("Failed to find 'abc'.\n");

	std::unordered_map<int, std::set<std::wstring>> strs;

	auto& va = strs[100];
	va.insert(L"abcd");

	va.insert(a);
	va.insert(b);

	for (auto iter = strs[100].cbegin(); iter != strs[100].cend(); iter++)
	{
		wprintf(L"%s\n", iter->c_str());
	}

	delete[] a;
	delete[] b;

	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	ComPtr<IMMDeviceEnumerator> spEnumerator;

	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		printf("Failed to initialize the COM environment.\n");
		return -1;
	}

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
		printf("Failed to un-register end-point notification callback {hr: 0X%X}.\n", hr);
		return -1;
	}

    return 0;
}

