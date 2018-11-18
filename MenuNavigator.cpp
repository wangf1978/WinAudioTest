#include "stdafx.h"
#include "MenuNavigator.h"

CMenuNavigator::CMenuNavigator()
{
}


CMenuNavigator::~CMenuNavigator()
{
}

HRESULT CMenuNavigator::NonDelegatingQueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == IID_IMenuNavigator)
		return GetCOMInterface(static_cast<IMenuNavigator*>(this), ppvObject);

	return CComUnknown::NonDelegatingQueryInterface(riid, ppvObject);
}

/*
Back pages:
	|---------------------|
	\__oldest             \_newest

Front pages:
	|---------------------|
	\__ newest            \_oldest

*/
STDMETHODIMP CMenuNavigator::Back()
{
	std::lock_guard<std::mutex> guard(m_mutexList);

	if (m_backPages.size() == 0)
		return E_FAIL;

	auto& back = m_backPages.back();
	m_forwardPages.push_front(m_currentPage);
	m_currentPage = back;
	m_backPages.pop_back();

	return m_currentPage->Show();
}

STDMETHODIMP CMenuNavigator::Forward()
{
	std::lock_guard<std::mutex> guard(m_mutexList);

	if (m_forwardPages.size() == 0)
		return E_FAIL;

	auto& front = m_forwardPages.front();
	m_backPages.push_back(m_currentPage);
	m_currentPage = front;
	m_forwardPages.pop_front();

	return m_currentPage->Show();
}

STDMETHODIMP CMenuNavigator::GoUp()
{
	std::lock_guard<std::mutex> guard(m_mutexList);

	if (m_currentPage == nullptr)
		return E_FAIL;

	HRESULT hr = S_OK;
	ComPtr<IMenuPage> spUpperPage;
	if (FAILED(hr = m_currentPage->GetUpperMenuPage(&spUpperPage)))
	{
		printf("[MenuNavigator] Failed to get the upper menu page.\n");
		return hr;
	}

	if (spUpperPage == nullptr)
	{
		printf("[MenuNavigator] the upper menu page does not exist.\n");
		return E_FAIL;
	}

	m_forwardPages.clear();
	m_backPages.push_back(m_currentPage);
	m_currentPage = spUpperPage;

	return m_currentPage->Show();
}

STDMETHODIMP CMenuNavigator::Refresh()
{
	std::lock_guard<std::mutex> guard(m_mutexList);
	if (m_currentPage == nullptr)
		return E_FAIL;

	return m_currentPage->Show();
}

STDMETHODIMP CMenuNavigator::GetCurrentPage(
	/* [out] */ IMenuPage** ppMenuPage)
{
	std::lock_guard<std::mutex> guard(m_mutexList);

	if (ppMenuPage == nullptr)
		return E_POINTER;

	return m_currentPage.CopyTo(ppMenuPage);
}

STDMETHODIMP CMenuNavigator::Navigate(
	/* [in]  */ NAVIMENU_ID idMenu,
	/* [in]  */ MENUPAGE_COOKIE cookieMenuPage,
	/* [in[  */ IMenuPage* pUpperMenu)
{
	std::lock_guard<std::mutex> guard(m_mutexList);

	// Try to activate the menu page with the specified menu_id and cookie
	ComPtr<IMenuPage> spNewMenuPage;
	HRESULT hr = ActivateMenuPage(idMenu, cookieMenuPage, &spNewMenuPage);

	if (FAILED(hr))
	{
		printf("[MenuNavigator] Failed to activate a MenuPage with MenuID: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}, cookieMenuPage: 0X%" PRIXPTR " {hr: 0X%X}.\n",
			idMenu.Data1, idMenu.Data2, idMenu.Data3,
			idMenu.Data4[0], idMenu.Data4[1], idMenu.Data4[2], idMenu.Data4[3], 
			idMenu.Data4[4], idMenu.Data4[5], idMenu.Data4[6], idMenu.Data4[7],
			cookieMenuPage, hr);
		return hr;
	}

	m_forwardPages.clear();
	m_backPages.push_back(m_currentPage);
	m_currentPage = spNewMenuPage;

	return m_currentPage->Show();
}

STDMETHODIMP CMenuNavigator::IsSupport(
	/* [in]  */ NAVIMENU_ID idMenu)
{
	return E_NOTIMPL;
}

CBaseMenuPage::CBaseMenuPage(NAVIMENU_ID idMenu, MENUPAGE_COOKIE cookieMenuPage, IMenuPage* pUpperMenu)
	: m_idMenu(idMenu)
	, m_cookieMenuPage(cookieMenuPage)
	, m_spUpperMenuPage(pUpperMenu)
{

}

CBaseMenuPage::~CBaseMenuPage()
{
}

HRESULT CBaseMenuPage::NonDelegatingQueryInterface(REFIID riid, void** ppvObject)
{
	if (riid == IID_IMenuPage)
		return GetCOMInterface(static_cast<IMenuPage*>(this), ppvObject);

	return CComUnknown::NonDelegatingQueryInterface(riid, ppvObject);
}

STDMETHODIMP CBaseMenuPage::Process(const char* szInput)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBaseMenuPage::OnNotify(unsigned int message, NOTIFY_PARAM param1, NOTIFY_PARAM param2)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBaseMenuPage::Show()
{
	return E_NOTIMPL;
}

STDMETHODIMP CBaseMenuPage::GetUpperMenuPage(IMenuPage** ppMenuPage)
{
	if (ppMenuPage == nullptr)
		return E_POINTER;

	*ppMenuPage = m_spUpperMenuPage.Get();
	if (*ppMenuPage != nullptr)
		(*ppMenuPage)->AddRef();

	return S_OK;
}

STDMETHODIMP_(NAVIMENU_ID) CBaseMenuPage::GetMenuID()
{
	return m_idMenu;
}

STDMETHODIMP_(MENUPAGE_COOKIE) CBaseMenuPage::GetCookie()
{
	return 0;
}

STDMETHODIMP_(BOOL) CBaseMenuPage::HotkeyInput()
{
	return TRUE;
}

