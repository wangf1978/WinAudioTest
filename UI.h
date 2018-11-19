#pragma once

#include "combase.h"

interface IInputManager;
interface IInputConsumer;
interface IInputProvider;

using NAVIMENU_ID = const GUID &;
using MENUCONTEXT = void*;
using NOTIFY_PARAM = intptr_t;
using MENUPAGE_COOKIE = intptr_t;

// {977C793A-C193-4F85-B3DC-8DD911607BEF}
AMP_DEFINE_GUID(IID_IInputProvider,
	0x977c793a, 0xc193, 0x4f85, 0xb3, 0xdc, 0x8d, 0xd9, 0x11, 0x60, 0x7b, 0xef);

interface IInputProvider : public IUnknown
{
	STDMETHOD(Open)(
		/* [in] */ IInputManager* pMgr) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(ProcessInput)() PURE;
	STDMETHOD_(BOOL, IsBusy)() PURE;
	STDMETHOD(ProcessLine)(const char *szInput) PURE;
};

// {6369B077-9D00-46BA-B361-BBBF8C57B8C6}
AMP_DEFINE_GUID(IID_IInputConsumer,
	0x6369b077, 0x9d00, 0x46ba, 0xb3, 0x61, 0xbb, 0xbf, 0x8c, 0x57, 0xb8, 0xc6);

interface IInputConsumer : public IUnknown
{
	STDMETHOD(Open)(
		/* [in] */ IInputManager* pMgr) PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(Process)(
		/* [in] */ const char* szInput) PURE;
	STDMETHOD(OnFocus)() PURE;
	STDMETHOD(OnLostFocus)() PURE;
};

// {AEF206A8-7830-4F36-A5EE-F48104E62597}
AMP_DEFINE_GUID(IID_IInputManager,
	0xaef206a8, 0x7830, 0x4f36, 0xa5, 0xee, 0xf4, 0x81, 0x4, 0xe6, 0x25, 0x97);

interface IInputManager : public IUnknown
{
	STDMETHOD(Open)() PURE;
	STDMETHOD(Close)() PURE;
	STDMETHOD(AddConsumer)(
		/* [in] */	IInputConsumer* pCsmr,
		/* [in] */	DWORD dwReserved = 0) PURE;
	STDMETHOD(RemoveConsumer)(
		/* [in] */	IInputConsumer* pCsmr) PURE;
	STDMETHOD(GetConsumerCount)(
		/* [out] */	unsigned long* pCnt) PURE;
	STDMETHOD(GetFocusedConsumer)(
		/* [out] */	IInputConsumer** ppCsmr) PURE;
	STDMETHOD(GetConsumer)(
		/* [out] */	unsigned long nIdx,
		/* [out] */	IInputConsumer** ppCsmr) PURE;
	STDMETHOD(MoveToTop)(
		/* [in] */	IInputConsumer* pCsmr) PURE;
	STDMETHOD(RequestInput)() PURE;
};

// {12876A09-4D5A-407E-9781-8F4DD1312878}
AMP_DEFINE_GUID(IID_IMenuPage ,
	0x12876a09, 0x4d5a, 0x407e, 0x97, 0x81, 0x8f, 0x4d, 0xd1, 0x31, 0x28, 0x78);

interface IMenuPage : public IUnknown
{
	STDMETHOD(Process)(
		/* [in] */ const char* szInput) PURE;
	STDMETHOD(OnNotify)(
		/* [in] */ unsigned int message,
		/* [in[ */ NOTIFY_PARAM param1,
		/* [in] */ NOTIFY_PARAM param2) PURE;
	STDMETHOD(Show)() PURE;
	STDMETHOD(SetUpperMenuPage)(
		/* [in] */ IMenuPage* pMenuPage) PURE;
	STDMETHOD(GetUpperMenuPage)(
		/* [in] */ IMenuPage** ppMenuPage) PURE;
	STDMETHOD_(NAVIMENU_ID, GetMenuID)() PURE;
	STDMETHOD_(MENUPAGE_COOKIE, GetCookie)() PURE;
	STDMETHOD_(BOOL, HotkeyInput)() PURE;
	STDMETHOD(ShowInputPrompt)(
		/* [in]  */ const WCHAR* szPrompt = nullptr) PURE;
};

// {B80ECE3D-DBC1-43C3-9D28-9D6DAED2DDC6}
AMP_DEFINE_GUID(IID_IMenuNavigator,
	0xb80ece3d, 0xdbc1, 0x43c3, 0x9d, 0x28, 0x9d, 0x6d, 0xae, 0xd2, 0xdd, 0xc6);

interface IMenuNavigator : public IUnknown
{
	STDMETHOD(Back)() PURE;
	STDMETHOD(Forward)() PURE;
	STDMETHOD(GoUp)() PURE;
	STDMETHOD(Refresh)() PURE;
	STDMETHOD(GetCurrentPage)(
		/* [out] */ IMenuPage** ppMenuPage) PURE;
	STDMETHOD(Navigate)(
		/* [in]  */ NAVIMENU_ID idMenu,
		/* [in]  */ MENUPAGE_COOKIE cookieMenuPage,
		/* [in[  */ IMenuPage* pUpperMenu) PURE;
	STDMETHOD(IsSupport)(
		/* [in]  */ NAVIMENU_ID idMenu) PURE;
	STDMETHOD(RequestInput)() PURE;
};
