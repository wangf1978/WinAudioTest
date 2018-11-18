#include "stdafx.h"
#include "DeviceMenuNavigator.h"
#include "MMDeviceUtility.h"

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

HRESULT CDeviceMenuNavigator::NonDelegatingQueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == __uuidof(IMMNotificationClient))
		return GetCOMInterface(static_cast<IMMNotificationClient*>(this), ppvObject);

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

HRESULT CDeviceMenuNavigator::ActivateMenuPage(NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, IMenuPage** ppMenuPage)
{
	UNREFERENCED_PARAMETER(idMenu);
	UNREFERENCED_PARAMETER(cookieMenuPage);
	UNREFERENCED_PARAMETER(ppMenuPage);
	return E_NOTIMPL;
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
	
	m_ctxDevice.OnPropertyValueChanged(pwstrDeviceId, key);

	return S_OK;
}
