#include "stdafx.h"
#include "DeviceMenuNavigator.h"
#include "MMDeviceUtility.h"
#include "DeviceListMenuPage.h"
#include "DeviceMenuPage.h"

HRESULT CDeviceContext::UpdateFlowDeviceList()
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	if (spDeviceEnumerator == nullptr)
		return E_FAIL;

	m_DeviceFlowList.clear();

	// Check which data-flow the current added device belongs to
	for (int flowid = eRender; flowid <= eCapture; flowid++)
	{
		ComPtr<IMMDeviceCollection> spDevices;
		auto& flowDeviceList = m_DeviceFlowList[(EDataFlow)flowid];
		if (FAILED(spDeviceEnumerator->EnumAudioEndpoints((EDataFlow)flowid, DEVICE_STATEMASK_ALL, &spDevices)))
			continue;

		UINT cDevices = 0;
		if (FAILED(spDevices->GetCount(&cDevices)))
			continue;

		for (UINT idxDev = 0; idxDev < cDevices; idxDev++)
		{
			ComPtr<IMMDevice> spDevice;
			if (FAILED(spDevices->Item(idxDev, &spDevice)))
				continue;

			LPWSTR wsDeviceID = nullptr;
			if (FAILED(spDevice->GetId(&wsDeviceID)))
				continue;

			flowDeviceList.insert(wsDeviceID);
		}
	}

	return S_OK;
}

HRESULT CDeviceContext::UpdateDeviceList()
{
	HRESULT hr = S_OK;

	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	if (spDeviceEnumerator == nullptr)
		return E_POINTER;

	ComPtr<IMMDeviceCollection> spDevices;
	if (FAILED(hr = spDeviceEnumerator->EnumAudioEndpoints(eAll, DEVICE_STATEMASK_ALL, &spDevices)))
		return hr;

	UINT cDevices = 0;
	if (FAILED(hr = spDevices->GetCount(&cDevices)))
		return hr;

	m_DeviceList.clear();

	for (UINT idxDev = 0; idxDev < cDevices; idxDev++)
	{
		ComPtr<IMMDevice> spDevice;
		if (FAILED(spDevices->Item(idxDev, &spDevice)))
			continue;

		LPWSTR wsDeviceID = nullptr;
		if (FAILED(spDevice->GetId(&wsDeviceID)))
			continue;

		m_DeviceList.insert(wsDeviceID);
	}

	return S_OK;
}

HRESULT CDeviceContext::UpdateDefaultDeviceList()
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	if (spDeviceEnumerator == nullptr)
		return E_POINTER;

	for (int flowid = eRender; flowid <= eCapture; flowid++)
	{
		auto& flowDefaultDevices = m_DefaultDevices[(EDataFlow)flowid];
		for (int roleid = eConsole; roleid < ERole_enum_count; roleid++)
		{
			ComPtr<IMMDevice> spDevice;
			if (SUCCEEDED(spDeviceEnumerator->GetDefaultAudioEndpoint((EDataFlow)flowid, (ERole)roleid, &spDevice)))
			{
				LPWSTR wsDeviceID = nullptr;
				if (FAILED(spDevice->GetId(&wsDeviceID)))
					continue;

				flowDefaultDevices[(ERole)roleid] = wsDeviceID;
			}
		}
	}

	return S_OK;
}

HRESULT CDeviceContext::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	return S_OK;
}

HRESULT CDeviceContext::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	m_DeviceList.insert(pwstrDeviceId);

	// Check which data-flow the current added device belongs to
	for (int flowid = eRender; flowid <= eCapture; flowid++)
	{
		ComPtr<IMMDeviceCollection> spDevices;
		auto& flowDeviceList = m_DeviceFlowList[(EDataFlow)flowid];
		if (FAILED(spDeviceEnumerator->EnumAudioEndpoints((EDataFlow)flowid, DEVICE_STATEMASK_ALL, &spDevices)))
			continue;

		UINT cDevices = 0;
		if (FAILED(spDevices->GetCount(&cDevices)))
			continue;

		for (UINT idxDev = 0; idxDev < cDevices; idxDev++)
		{
			ComPtr<IMMDevice> spDevice;
			if (FAILED(spDevices->Item(idxDev, &spDevice)))
				continue;

			LPWSTR wsDeviceID = nullptr;
			if (FAILED(spDevice->GetId(&wsDeviceID)))
				continue;

			if (_wcsicmp(wsDeviceID, pwstrDeviceId) == 0)
				flowDeviceList.insert(wsDeviceID);
		}
	}

	return S_OK;
}

HRESULT CDeviceContext::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	if (m_DeviceList.erase(pwstrDeviceId) != 1)
	{
		wprintf(L"[DeviceContext] Failed to remove device: %s from the device cache.\n", pwstrDeviceId);
	}

	for (int flowid = eRender; flowid <= eCapture; flowid++)
	{
		auto& flowDeviceList = m_DeviceFlowList[(EDataFlow)flowid];
		if (flowDeviceList.erase(pwstrDeviceId) != 1)
		{
			wprintf(L"[DeviceContext] Failed to remove device id: %s from the flow devices cache.", pwstrDeviceId);
		}
	}

	return S_OK;
}

HRESULT CDeviceContext::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	m_DefaultDevices[flow][role] = pwstrDefaultDeviceId;

	return S_OK;
}

HRESULT CDeviceContext::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
	std::lock_guard<std::recursive_mutex> guard(m_mutex);

	return S_OK;
}

CDeviceMenuNavigator::CDeviceMenuNavigator(HRESULT* phr)
{
	std::lock_guard<std::recursive_mutex> lock_guard(m_ctxDevice.m_mutex);

	HRESULT hr = S_OK;
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&m_ctxDevice.spDeviceEnumerator);

	if (FAILED(hr))
	{
		printf("[DeviceMenuNavigator] Failed to create a MMDeviceEnumerator {hr: 0X%X}.\n", hr);
	}

	AMP_SAFEASSIGN(phr, hr);

	if (m_ctxDevice.spDeviceEnumerator != nullptr)
		m_ctxDevice.spDeviceEnumerator->RegisterEndpointNotificationCallback((IMMNotificationClient*)this);

	m_ctxDevice.UpdateDeviceList();
	m_ctxDevice.UpdateFlowDeviceList();
	m_ctxDevice.UpdateDefaultDeviceList();
}


CDeviceMenuNavigator::~CDeviceMenuNavigator()
{
	if (m_ctxDevice.spDeviceEnumerator != nullptr)
		m_ctxDevice.spDeviceEnumerator->UnregisterEndpointNotificationCallback((IMMNotificationClient*)this);
}

STDMETHODIMP CDeviceMenuNavigator::NonDelegatingQueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == __uuidof(IMMNotificationClient))
		return GetCOMInterface(static_cast<IMMNotificationClient*>(this), ppvObject);
	else if (riid == IID_IInputConsumer)
		return GetCOMInterface(static_cast<IInputConsumer*>(this), ppvObject);

	return CMenuNavigator::NonDelegatingQueryInterface(riid, ppvObject);
}

STDMETHODIMP CDeviceMenuNavigator::IsSupport(
	/* [in]  */ NAVIMENU_ID idMenu)
{
	if (idMenu == NAVMENU_ID_DEVICE_LIST)
		return S_OK;
	else if (idMenu == NAVMENU_ID_DEVICE)
		return S_OK;

	return E_NOTIMPL;
}

STDMETHODIMP CDeviceMenuNavigator::RequestInput()
{
	if (m_spInputMgr == nullptr)
		return E_NOTIMPL;

	return m_spInputMgr->RequestInput();
}

HRESULT CDeviceMenuNavigator::ActivateMenuPage(
	IMenuNavigator* pNavigator, NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, 
	IMenuPage* pUpperMenuPage, IMenuPage** ppMenuPage)
{
	if (ppMenuPage == nullptr)
		return E_POINTER;

	HRESULT hr = S_OK;
	CDeviceCookie* pDeviceCookie = (CDeviceCookie*)cookieMenuPage;

	// Check whether the current page is the activated menu page or not
	if (m_currentPage != nullptr && m_currentPage->GetMenuID() == idMenu)
	{
		const CDeviceCookie* pIterDeviceCookie = (const CDeviceCookie*)(m_currentPage->GetCookie());

		if (idMenu == NAVMENU_ID_DEVICE_LIST && pDeviceCookie == nullptr && pIterDeviceCookie == 0)
			return m_currentPage.CopyTo(ppMenuPage);

		ComPtr<IMenuPage> spUpperPage;
		m_currentPage->GetUpperMenuPage(&spUpperPage);

		if (pDeviceCookie != nullptr && pDeviceCookie->Equal(idMenu, pIterDeviceCookie) == true && spUpperPage.Get() == pUpperMenuPage)
			return m_currentPage.CopyTo(ppMenuPage);
	}

	// Try to find the existed menu page in the navigation history
	std::list<ComPtr<IMenuPage>>* lists[2] = {&m_backPages, &m_forwardPages};

	for (int i = 0; i < _countof(lists); i++)
	{
		for (auto iter = lists[i]->cbegin(); iter != lists[i]->cend(); iter++)
		{
			if ((*iter) == nullptr || (*iter)->GetMenuID() != idMenu)
				continue;

			const CDeviceCookie* pIterDeviceCookie = (const CDeviceCookie*)((*iter)->GetCookie());

			if (idMenu == NAVMENU_ID_DEVICE_LIST && pDeviceCookie == nullptr && pIterDeviceCookie == 0)
				return (*iter).CopyTo(ppMenuPage);

			ComPtr<IMenuPage> spUpperPage;
			(*iter)->GetUpperMenuPage(&spUpperPage);

			if (pDeviceCookie != nullptr && pDeviceCookie->Equal(idMenu, pIterDeviceCookie) == true && spUpperPage.Get() == pUpperMenuPage)
				return (*iter).CopyTo(ppMenuPage);
		}
	}

	if (NAVMENU_ID_DEVICE_LIST == idMenu)
	{
		*ppMenuPage = new CDeviceListMenuPage(&m_ctxDevice, this, cookieMenuPage);
		(*ppMenuPage)->AddRef();
	}
	else if (NAVMENU_ID_DEVICE == idMenu)
	{
		*ppMenuPage = new CDeviceMenuPage(&m_ctxDevice, this, cookieMenuPage, pUpperMenuPage);
		(*ppMenuPage)->AddRef();
	}
	else
		hr = E_NOTIMPL;

	return hr;
}

HRESULT STDMETHODCALLTYPE CDeviceMenuNavigator::OnDeviceStateChanged(
	_In_  LPCWSTR pwstrDeviceId,
	_In_  DWORD dwNewState)
{
	WCHAR wszTmp[1024] = { 0 };

	ComPtr<IMMDevice> spDevice;
	if (m_ctxDevice.spDeviceEnumerator != nullptr &&
		SUCCEEDED(m_ctxDevice.spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		CMMDeviceUtility::GetDeviceTitleName(spDevice.Get(), wszTmp, _countof(wszTmp));

	CMMDeviceUtility::GetFlagsDesc(dwNewState, CMMDeviceUtility::Device_state_flag_names, _countof(CMMDeviceUtility::Device_state_flag_names), wszTmp, _countof(wszTmp));
	wprintf(L"\n\n!!-->[DeviceID: %s][%s] change to new states: 0X%X (%s)\n",
		pwstrDeviceId, wszTmp, dwNewState, 
		CMMDeviceUtility::GetEnumerateName(dwNewState, CMMDeviceUtility::Device_state_flag_names, _countof(CMMDeviceUtility::Device_state_flag_names)));

	m_ctxDevice.OnDeviceStateChanged(pwstrDeviceId, dwNewState);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDeviceMenuNavigator::OnDeviceAdded(
	_In_  LPCWSTR pwstrDeviceId)
{
	WCHAR wszTmp[1024] = { 0 };

	ComPtr<IMMDevice> spDevice;
	if (m_ctxDevice.spDeviceEnumerator != nullptr &&
		SUCCEEDED(m_ctxDevice.spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		CMMDeviceUtility::GetDeviceTitleName(spDevice.Get(), wszTmp, _countof(wszTmp));

	wprintf(L"\n\n!!-->[DeviceID: %s][%s] added.\n", pwstrDeviceId, wszTmp);

	m_ctxDevice.OnDeviceAdded(pwstrDeviceId);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDeviceMenuNavigator::OnDeviceRemoved(
	_In_  LPCWSTR pwstrDeviceId)
{
	WCHAR wszTmp[1024] = { 0 };

	ComPtr<IMMDevice> spDevice;
	if (m_ctxDevice.spDeviceEnumerator != nullptr &&
		SUCCEEDED(m_ctxDevice.spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
		CMMDeviceUtility::GetDeviceTitleName(spDevice.Get(), wszTmp, _countof(wszTmp));

	wprintf(L"\n\n!!-->[DeviceID: %s][%s] removed.\n", pwstrDeviceId, wszTmp);

	m_ctxDevice.OnDeviceRemoved(pwstrDeviceId);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDeviceMenuNavigator::OnDefaultDeviceChanged(
	_In_  EDataFlow flow,
	_In_  ERole role,
	_In_  LPCWSTR pwstrDefaultDeviceId)
{
	WCHAR wszTmp[1024] = { 0 };

	ComPtr<IMMDevice> spDevice;
	if (m_ctxDevice.spDeviceEnumerator != nullptr &&
		SUCCEEDED(m_ctxDevice.spDeviceEnumerator->GetDevice(pwstrDefaultDeviceId, &spDevice)))
		CMMDeviceUtility::GetDeviceTitleName(spDevice.Get(), wszTmp, _countof(wszTmp));

	wprintf(L"\n\n!!-->[DeviceID: %s][%s] becomes default %s %s device.\n", pwstrDefaultDeviceId, wszTmp, 
		CMMDeviceUtility::GetEnumerateName(flow, CMMDeviceUtility::Data_flow_flag_names, _countof(CMMDeviceUtility::Data_flow_flag_names)),
		CMMDeviceUtility::GetEnumerateName(role, CMMDeviceUtility::Role_flag_names, _countof(CMMDeviceUtility::Role_flag_names)));

	m_ctxDevice.OnDefaultDeviceChanged(flow, role, pwstrDefaultDeviceId);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDeviceMenuNavigator::OnPropertyValueChanged(
	_In_  LPCWSTR pwstrDeviceId,
	_In_  const PROPERTYKEY key)
{
	WCHAR wszTmp[1024] = { 0 };
	WCHAR wszPropKeyName[1024] = { 0 };
	WCHAR wszPropValueDesc[2048] = { 0 };

	ComPtr<IMMDevice> spDevice;
	if (m_ctxDevice.spDeviceEnumerator != nullptr &&
		SUCCEEDED(m_ctxDevice.spDeviceEnumerator->GetDevice(pwstrDeviceId, &spDevice)))
	{
		CMMDeviceUtility::GetDeviceTitleName(spDevice.Get(), wszTmp, _countof(wszTmp));
		ComPtr<IPropertyStore> spPropStore;
		if (SUCCEEDED(spDevice->OpenPropertyStore(STGM_READ, &spPropStore)))
			CMMDeviceUtility::GetDevicePropDesc(spPropStore.Get(), key, wszPropValueDesc, _countof(wszPropValueDesc), 4);
	}

	const WCHAR* wszPKeyname = nullptr;
	for (size_t i = 0; i < _countof(CMMDeviceUtility::prop_key_names); i++)
	{
		if (std::get<0>(CMMDeviceUtility::prop_key_names[i]) == key)
		{
			wszPKeyname = std::get<1>(CMMDeviceUtility::prop_key_names[i]);
			break;
		}
	}

	if (wszPKeyname != nullptr)
		swprintf_s(wszPropKeyName, _countof(wszPropKeyName), L"%s", wszPKeyname);
	else
		swprintf_s(wszPropKeyName, _countof(wszPropKeyName), L"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}/%02u",
			key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3,
			key.fmtid.Data4[0], key.fmtid.Data4[1], key.fmtid.Data4[2], key.fmtid.Data4[3],
			key.fmtid.Data4[4], key.fmtid.Data4[5], key.fmtid.Data4[6], key.fmtid.Data4[7], key.pid);

	wprintf(L"\n\n!!-->[DeviceID: %s][property: %s] value is changed to:\n", pwstrDeviceId, wszPropKeyName);
	wprintf(L"%s", wszPropValueDesc);

	m_ctxDevice.OnPropertyValueChanged(pwstrDeviceId, key);

	return S_OK;
}

STDMETHODIMP CDeviceMenuNavigator::Open(IInputManager* pMgr)
{
	m_bClosed = FALSE;
	m_spInputMgr = pMgr;

	return S_OK;
}

STDMETHODIMP CDeviceMenuNavigator::Close()
{
	m_bClosed = TRUE;
	m_spInputMgr = nullptr;

	return S_OK;
}

STDMETHODIMP CDeviceMenuNavigator::Process(const char* szInput)
{
	HRESULT hr = S_OK;
	ComPtr<IMenuPage> spCurMenuPage;
	if (FAILED(GetCurrentPage(&spCurMenuPage)))
	{
		wprintf(L"No active page to process the input.\n");
		return hr;
	}

	hr = spCurMenuPage->Process(szInput);

	if (szInput != nullptr)
	{
		switch (szInput[0])
		{
		case 'x':	//Quit
			hr = Close();
			break;
		case 'b':	// Backward
			if (FAILED(hr = Back()))
			{
				wprintf(L"Failed to go to the previous page {hr: 0X%X}.\n", hr);
				spCurMenuPage->ShowInputPrompt();
			}
			break;
		case 'f':	// Forward
			if (FAILED(hr = Forward()))
			{
				wprintf(L"Failed to go to the next page {hr: 0X%X}.\n", hr);
				spCurMenuPage->ShowInputPrompt();
			}
			break;
		case 'p':	// Go-up
			if (FAILED(hr = GoUp()))
			{
				wprintf(L"Failed to go to the upper page {hr: 0X%X}.\n", hr);
				spCurMenuPage->ShowInputPrompt();
			}
			break;
		}
	}

	return hr;
}

STDMETHODIMP CDeviceMenuNavigator::OnFocus()
{
	return E_NOTIMPL;
}

STDMETHODIMP CDeviceMenuNavigator::OnLostFocus()
{
	return E_NOTIMPL;
}

HRESULT CDeviceMenuNavigator::Run(NAVIMENU_ID idInitMenu, MENUPAGE_COOKIE cookieInitMenuPage)
{
	HRESULT hr = S_OK;
	if (FAILED(hr = Navigate(idInitMenu, cookieInitMenuPage, nullptr)))
	{
		wprintf(L"Failed to navigate to the \"%s\" menu page.\n", NAVMENU_NAMEW(idInitMenu));
		return hr;
	}

	ComPtr<IMenuPage> spCurrentMenuPage;
	if (FAILED(hr = GetCurrentPage(&spCurrentMenuPage)))
	{
		wprintf(L"No activate menu page.\n");
		return hr;
	}

	while (!m_bClosed)
	{
		hr = m_spInputMgr->RequestInput();

		switch (hr)
		{
		case E_NOTIMPL:
			::Sleep(100);
			break;
		case S_OK:
			break;
		case E_FAIL:
			break;
		}
	}

	return hr;
}
