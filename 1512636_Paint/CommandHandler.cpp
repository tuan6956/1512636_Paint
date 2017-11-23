// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include "stdafx.h"
#include <UIRibbon.h>

#include "1512636_Paint.h"
#include "CommandHandler.h"
#include "RibbonIDs.h"
#include "RibbonFramework.h"

#include <uiribbonpropertyhelpers.h>
#include "Propvarutil.h"
#include "stdlib.h"
#include "PropertySet.h"
#pragma comment (lib,"Propsys.lib")

//CCommandHandler::LineRibbonImageIds = { 
//IDB_BITMAP1,
//IDB_BITMAP2,
//IDB_BITMAP3,
//IDB_BITMAP4,
//IDB_BITMAP5,
//IDB_BITMAP6 
//};


int CCommandHandler::LineRibbonImageIds[7] = {
	IDB_BITMAP_LINE1,
	IDB_BITMAP_LINE2,
	IDB_BITMAP_LINE3,
	IDB_BITMAP_LINE4,
	IDB_BITMAP_LINE5,
	IDB_BITMAP_LINE6,
	IDB_BITMAP_LINE7
};
int CCommandHandler::StyleRibbonImageIds[5] = {
	IDB_BITMAP_SOILD,
	IDB_BITMAP_DOT,
	IDB_BITMAP_DASH,
	IDB_BITMAP_DASHDOT,
	IDB_BITMAP_DASHDOTDOT,
};

// Static method to create an instance of the object.
HRESULT CCommandHandler::CreateInstance(IUICommandHandler **ppCommandHandler)
{
	if (!ppCommandHandler)
	{
		return E_POINTER;
	}
	_mainStyle = 0;
	*ppCommandHandler = NULL;

	HRESULT hr = S_OK;

	CCommandHandler* pCommandHandler = new CCommandHandler();

	if (pCommandHandler != NULL)
	{
		*ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
	}
	else
	{
		hr = E_OUTOFMEMORY;
	}

	return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
	LONG cRef = InterlockedDecrement(&m_cRef);
	if (cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
	if (iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(this);
	}
	else if (iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(this);
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
	UINT nCmdID,
	REFPROPERTYKEY key,
	const PROPVARIANT* ppropvarCurrentValue,
	PROPVARIANT* ppropvarNewValue)
{
	HRESULT hr = E_FAIL;
	int item = 0;
	if (UI_PKEY_Categories == key)
	{
		hr = S_FALSE;
	}
	else if (UI_PKEY_ItemsSource == key)
	{
		int len = 5;
		if (nCmdID == ID_CMD_DROPDOWN_SIZE)
			len = 7;
		IUICollection* pCollection;
		hr = ppropvarCurrentValue->punkVal->QueryInterface(IID_PPV_ARGS(&pCollection));
		for (int i = 0; i < len; i++)
		{
			CPropertySet* pItem;
			hr = CPropertySet::CreateInstance(&pItem);
			if (FAILED(hr))
			{
				pCollection->Release();
				return hr;
			}

			// Create an IUIImage from a resource id.
			IUIImage* pImg;
			if (nCmdID == ID_CMD_DROPDOWN_SIZE)
				hr = CreateUIImageFromBitmapResource(MAKEINTRESOURCE(LineRibbonImageIds[i]), &pImg);
			else
				hr = CreateUIImageFromBitmapResource(MAKEINTRESOURCE(StyleRibbonImageIds[i]), &pImg);

			if (FAILED(hr))
			{
				pCollection->Release();
				pItem->Release();
				return hr;
			}
			pItem->InitializeItemProperties(pImg, L"", UI_COLLECTION_INVALIDINDEX);
			pCollection->Add(pItem);

			pItem->Release();
			pImg->Release();
		}
		pCollection->Release();
		hr = S_OK;
	}
	else if (UI_PKEY_SelectedItem == key)
	{
		hr = ::UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, item, ppropvarNewValue);
	}

	return hr;

}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
COLORREF colors[4] = { RGB(255,0,0), RGB(255,255,0),RGB(0,0,0),RGB(0,0,255) };
void checkToggle(const PROPVARIANT* ppropvarValue, int iCommand, int type )
{
	PROPVARIANT propvar;
	PROPVARIANT var;
	PROPVARIANT varNew;
	int cmdGroupShapeColor[2][5] = { { ID_CMD_DRAW_LINE , ID_CMD_DRAW_RECTANGLE, ID_CMD_DRAW_ELLIPSE, ID_CMD_DRAW_CURVE },{ ID_CMD_COLOR_BLACK , ID_CMD_COLOR_BLUE, ID_CMD_COLOR_RED, ID_CMD_COLOR_YELLOW, ID_CMD_COLOR_EDIT_COLOR } };

	//int cmdGroupShape[5] = { ID_CMD_DRAW_LINE , ID_CMD_DRAW_RECTANGLE, ID_CMD_DRAW_ELLIPSE };
	//int cmdGroupColor[5] = { ID_CMD_COLOR_BLACK , ID_CMD_COLOR_BLUE, ID_CMD_COLOR_RED, ID_CMD_COLOR_YELLOW, ID_CMD_COLOR_EDIT_COLOR };

	HRESULT hr;
	BOOL boolValue;

	int selected = ppropvarValue->uintVal;
	hr = g_pFramework->GetUICommandProperty(iCommand, UI_PKEY_BooleanValue, &var);
	hr = PropVariantToBoolean(var, &boolValue);

	//boolValue = !boolValue;
	
	
	PropVariantInit(&propvar);
	if (boolValue != 1)
	{
		boolValue = !boolValue;
		hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, boolValue, &varNew);

		hr = g_pFramework->SetUICommandProperty(iCommand, UI_PKEY_BooleanValue, varNew);
		return;
		
	}
	boolValue = !boolValue;
	hr = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, boolValue, &varNew);

	int len = 0;
	type == 0 ? len = 4 : len = 5;
	for (int i = 0; i < len; i++)
	{
		if(cmdGroupShapeColor[type][i] !=  iCommand)
			hr = g_pFramework->SetUICommandProperty(cmdGroupShapeColor[type][i],UI_PKEY_BooleanValue, varNew);
	}
	/*hr = g_pFramework->SetUICommandProperty(ID_CMD_DRAW_ELLIPSE,
		UI_PKEY_BooleanValue, varNew);
	hr = g_pFramework->SetUICommandProperty(ID_CMD_DRAW_RECTANGLE,
		UI_PKEY_BooleanValue, varNew);*/
}
STDMETHODIMP CCommandHandler::Execute(
	UINT nCmdID,
	UI_EXECUTIONVERB verb,
	const PROPERTYKEY* key,
	const PROPVARIANT* ppropvarValue,
	IUISimplePropertySet* pCommandExecutionProperties)
{
	HRESULT hr;
	HWND hwnd = GetForegroundWindow();
	BOOL boolValue;
	PROPVARIANT var, varNew;
	PROPVARIANT propvar;
	UINT selected;
	switch (nCmdID)
	{
	case ID_CMD_NEW:
		OnCommand(hwnd, ID_FILE_NEW, 0, 0);
		break;
	case ID_CMD_OPEN_BINARY:
		OnCommand(hwnd, ID_OPEN_BINARY, 0, 0);
		break;
	case ID_CMD_OPEN_BITMAP:
		OnCommand(hwnd, ID_OPEN_BITMAP, 0, 0);
		break;
	case ID_CMD_SAVE_BINARY:
		OnCommand(hwnd, ID_SAVE_BINARY, 0, 0);
		break;
	case ID_CMD_SAVE_BITMAP:
		OnCommand(hwnd, ID_SAVE_BITMAP, 0, 0);
		break;
	case ID_CMD_EXIT:
		PostMessage(hwnd, WM_CLOSE, NULL, NULL);
		break;
	case ID_CMD_DRAW_LINE:
	case ID_CMD_DRAW_RECTANGLE:
	case ID_CMD_DRAW_ELLIPSE:
	case ID_CMD_DRAW_CURVE:
		_iTypeDraw = nCmdID - ID_CMD_DRAW_LINE;
		checkToggle(ppropvarValue, nCmdID, 0);
		break;
	case ID_CMD_COLOR_RED:
	case ID_CMD_COLOR_YELLOW:
	case ID_CMD_COLOR_BLACK:
	case ID_CMD_COLOR_BLUE:
		_mainColor = colors[nCmdID - ID_CMD_COLOR_RED];
		checkToggle(ppropvarValue, nCmdID, 1);
		break;
	case ID_CMD_COLOR_EDIT_COLOR:
		checkToggle(ppropvarValue, nCmdID, 1);
		OnCommand(hwnd, ID_DESIGN_COLOR, 0, 0);
	}
	if (UI_EXECUTIONVERB_EXECUTE == verb)
	{
		if (nullptr != key && UI_PKEY_SelectedItem == (*key))
		{
			
			// Update the size
			unsigned int selectedIndex;
			hr = ::UIPropertyToUInt32(*key, *ppropvarValue, &selectedIndex);
			if (SUCCEEDED(hr))
			{
				if (nCmdID == ID_CMD_DROPDOWN_SIZE)
				{
					_mainWidth = selectedIndex;
				}
				else if (nCmdID == ID_CMD_DROPDOWN_STYLE)
				{
					_mainStyle = selectedIndex;
				}
				//MessageBox(0, m_reportFileName, 0, 0);
			}

		}
	}
	/*UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(nCmdID);*/

	return S_OK;
}
HRESULT CCommandHandler::CreateUIImageFromBitmapResource(LPCTSTR pszResource, __out IUIImage **ppimg)
{
	HRESULT hr = E_FAIL;

	*ppimg = NULL;
	this->m_pifbFactory = NULL;
	if (NULL == this->m_pifbFactory)
	{
		hr = CoCreateInstance(CLSID_UIRibbonImageFromBitmapFactory, NULL, CLSCTX_ALL, IID_PPV_ARGS(&this->m_pifbFactory));
		if (FAILED(hr))
		{
			return hr;
		}
	}

	// Load the bitmap from the resource file.
	HBITMAP hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), pszResource, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hbm)
	{
		// Use the factory implemented by the framework to produce an IUIImage.
		hr = this->m_pifbFactory->CreateImage(hbm, UI_OWNERSHIP_TRANSFER, ppimg);
		if (FAILED(hr))
		{
			DeleteObject(hbm);
		}
	}
	return hr;
}
