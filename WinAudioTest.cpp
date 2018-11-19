// WinAudioTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "combase.h"
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
#include "input_manager.h"
#include "DeviceMenuNavigator.h"
#include "cui_input_provider.h"

using namespace Microsoft::WRL;

int main()
{
	HRESULT hr = S_OK;
	CCUIInputProvider* pCUIInputProvider = nullptr;
	IInputManager* pInputMgr = nullptr;
	CDeviceMenuNavigator* pMenuNavigator = nullptr;

	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		printf("Failed to initialize the COM environment.\n");
		return -1;
	}

	// Create a Input Manager to coordinate input provider and input consumer
	if (FAILED(CInputManager::CreateInstance(IID_IInputManager, (void**)&pInputMgr)))
	{
		printf("Failed to create an input manager instance.\n");
		goto done;
	}

	// Create a Console User Interface keyboard input provider
	pCUIInputProvider = new CCUIInputProvider();
	((IUnknown*)pCUIInputProvider)->AddRef();
	if (FAILED(hr = pCUIInputProvider->Open(pInputMgr)))
	{
		printf("Failed to open a console user-interface input provider {hr: 0X%X}.\n", hr);
		goto done;
	}

	((CInputManager*)pInputMgr)->SetMainThreadInputProvider(pCUIInputProvider);

	// Create a Device Menu navigator
	pMenuNavigator = new CDeviceMenuNavigator();
	pMenuNavigator->AddRef();

	// Open the Menu navigator
	if (FAILED(hr = pMenuNavigator->Open(pInputMgr)))
	{
		pCUIInputProvider->Close();
		printf("Failed to open the menu navigator {hr: 0X%X}.\n", hr);
		goto done;
	}

	// Bind the menu navigator and begin to accept the input
	if (FAILED(pInputMgr->AddConsumer(pMenuNavigator)))
	{
		pCUIInputProvider->Close();
		printf("Failed to add the menu navigator into input manager {hr: 0X%X}.\n", hr);
		goto done;
	}
	
	// Run the application, and show the menu and accept the input
	pMenuNavigator->Run(NAVMENU_ID_DEVICE_LIST, 0);

	// Unbind the input provider, input manager and input consumer
	pInputMgr->RemoveConsumer(pMenuNavigator);
	pMenuNavigator->Close();
	pCUIInputProvider->Close();

done:
	AMP_SAFERELEASE(pMenuNavigator);
	AMP_SAFERELEASE(pCUIInputProvider);
	AMP_SAFERELEASE(pCUIInputProvider);

	CoUninitialize();

    return 0;
}

