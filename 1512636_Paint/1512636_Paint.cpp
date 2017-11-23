// 1512636_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512636_Paint.h"
#include <WindowsX.h>
#include <commctrl.h>
#include <memory.h>
#include <stdlib.h>
#include <commdlg.h>
#include <string>
#pragma comment(lib, "comctl32.lib")

#include "Shape.h"
#include "Line.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Curve.h"
#include <vector>
#include <fstream>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;

#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"
#define MAX_LOADSTRING 100

#define SHAPE_MODEL_LINE		0
#define SHAPE_MODEL_RECTANGLE	1
#define SHAPE_MODEL_ELLIPSE		2
#define SHAPE_MODEL_CURVE		3


#pragma region Default


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	HRESULT hr = CoInitialize(NULL);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MY1512636_PAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512636_PAINT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CoUninitialize();
	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0; //CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512636_PAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;//  MAKEINTRESOURCEW(IDC_MY1512636_PAINT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
#pragma endregion

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
bool _isClickDown = false;

POINT pStart;					
POINT pState;		

std::vector<CShape*> vShape;
int _iTypeDraw = 0;

COLORREF _mainColor = 0;
int _mainStyle = 0;
int _mainWidth = 0;

DashStyle _gDashStyle[5] = { DashStyleSolid, DashStyleDot, DashStyleDash, DashStyleDashDot, DashStyleDashDotDot };
bool _isChangeStyle = true;
bool _isOpenBitmap = false;

std::vector<CShape*> _ShapeModle;

HDC hdcOpenBitmap;
BITMAP bitmap;
HANDLE hOpenBitmap;

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

UINT _mainHeightRibbon = 0;
PBITMAPINFO CreateBitmapInfo(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;

	GetObject(hBmp, sizeof(BITMAP), &bmp);

	pbmi = static_cast<PBITMAPINFO>(LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)));
	UINT heightRibbon = GetRibbonHeight();

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	RECT rect;
	GetClientRect(hwnd, &rect);

	pbmi->bmiHeader.biWidth = rect.right - rect.left;
	pbmi->bmiHeader.biHeight = rect.bottom - rect.top ;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes; // we are assuming that there is only one plane
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

	// no compression this is an rgb bitmap
	pbmi->bmiHeader.biCompression = BI_RGB;

	// calculate size and align to a DWORD (8bit), we are assuming there is only one plane.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * bmp.bmBitsPixel + 31) & -31) * pbmi->bmiHeader.biHeight;

	// all device colours are important
	pbmi->bmiHeader.biClrImportant = 0;

	return pbmi;
}
int WriteBmpTofile(LPCWSTR pszFile, PBITMAPINFO pbi, HBITMAP hBmp, HDC hDC)
{
	HANDLE hFile;
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTemp;
	UINT heightRibbon = GetRibbonHeight();

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
	{
		return -1; // could not allocate bitmap
	}

	GetDIBits(hDC, hBmp, 0 , (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS);

	hFile = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return -1; // Could not open screenshot file
	}

	// type == BM
	hdr.bfType = 0x4d42;

	hdr.bfSize = (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

	// write the bitmap file header to file
	WriteFile(hFile, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), &dwTemp, NULL);

	// write the bitmap header to file
	WriteFile(hFile, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), &dwTemp, NULL);

	// copy the bitmap colour data into the file
	WriteFile(hFile, (LPSTR)lpBits, pbih->biSizeImage, &dwTemp, NULL);

	CloseHandle(hFile);

	GlobalFree((HGLOBAL)lpBits);

	return 1;
}
std::basic_string<TCHAR> getSaveFileName(HWND owner, int type)
{
	TCHAR buffer[260] = TEXT("");
	OPENFILENAME ofn = { 0 };
	ofn.lpstrFile = buffer;
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = L"Binary (*.bin) \0*.bin\0Bitmap (*.bmp)\0*.bmp\0All files (*.*)\0*.*\0";
	ofn.nFilterIndex = type; // Text | *.txt	ofn.lpstrFile = buffer;
	ofn.nMaxFile = 260;
	//ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ENABLEHOOK | OFN_NODEREFERENCELINKS | OFN_EXTENSIONDIFFERENT;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	ofn.lpstrInitialDir = NULL;
	BOOL check = GetSaveFileName(&ofn);
	if (!check)
		return TEXT("");
	std::wstring path = ofn.lpstrFile;
	if (type == 1)
	{
		int index = path.find(L".bin");
		if (index == 0 && ofn.nFilterIndex == type)
			path.clear();
		else if (index == -1 && ofn.nFilterIndex == type)
			path.append(L".bin");
	}
	else
	{
		int index = path.find(L".bmp");
		if (index == 0 && ofn.nFilterIndex == type)
			path.clear();
		else if (index == -1 && ofn.nFilterIndex == type)
			path.append(L".bmp");
	}
	return path;

}
std::basic_string<TCHAR> getOpenFileName(HWND owner,int type)
{
	TCHAR buffer[260] = TEXT("");
	OPENFILENAME ofn = { 0 };
	ofn.lpstrFile = buffer;
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = L"Binary (*.bin) \0*.bin\0Bitmap (*.bmp)\0*.bmp\0All files (*.*)\0*.*\0";
	ofn.nFilterIndex = type; // Text | *.txt	ofn.lpstrFile = buffer;
	ofn.nMaxFile = 260;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrInitialDir = NULL;
	if (!GetOpenFileName(&ofn))
		return TEXT("");
	std::wstring path = ofn.lpstrFile;
	
	return path;
}


void fixCoordiSquare()
{
	if (abs(pStart.x - pState.x) > abs(pStart.y - pState.y))
	{
		if (pStart.x < pState.x)
			pState.x = pStart.x + abs(pStart.y - pState.y);
		else
			pState.x = pStart.x - abs(pStart.y - pState.y);
	}
	else
	{
		if (pStart.y < pState.y)
			pState.y = pStart.y + abs(pStart.x - pState.x);
		else
			pState.y = pStart.y - abs(pStart.x - pState.x);
	}

}
void CheckMenu(HWND hwnd, int typeParam, int param)
{
	HMENU hMenu = GetMenu(hwnd);
	if (typeParam == 1)
	{
		CheckMenuItem(hMenu, ID_TYPE_LINE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_TYPE_ELLIPSE, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_TYPE_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hMenu, param, MF_CHECKED);
	}
	else if (typeParam == 2)
	{
		CheckMenuItem(hMenu, ID_STYLE_SOILD, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_DASH, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_DOT, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_DASHDOT, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_DASHDOTDOT, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_NULL, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_STYLE_INSIDEFRAM, MF_UNCHECKED);
		CheckMenuItem(hMenu, param, MF_CHECKED);
		_isChangeStyle = true;

	}
	else
	{
		CheckMenuItem(hMenu, ID_WIDTH_1, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_2, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_3, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_4, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_5, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_6, MF_UNCHECKED);
		CheckMenuItem(hMenu, ID_WIDTH_7, MF_UNCHECKED);
		CheckMenuItem(hMenu, param, MF_CHECKED);
		_isChangeStyle = true;

	}

}
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	pStart.x = x;
	pStart.y = y;
	_isClickDown = true;
	SetCapture(hwnd);
}
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{

	pState.x = x;
	pState.y = y;
	if (keyFlags & MK_SHIFT && _iTypeDraw != SHAPE_MODEL_LINE)
		fixCoordiSquare();

	/*HBRUSH hbrush = GetStockBrush(NULL_BRUSH);
	HPEN pen_draw = CreatePen(_mainStyle, _mainWidth, _mainColor);*/
	Color color;
	color.SetFromCOLORREF(_mainColor);
	Pen* pen = new Pen(color, _mainWidth);
	pen->SetDashStyle(_gDashStyle[_mainStyle]);

	//pen->SetDashStyle(DashStyle(_mainStyle));
	_ShapeModle[_iTypeDraw]->setPen(pen);
	/*_ShapeModle[_iTypeDraw]->setBrush(hbrush);
	_ShapeModle[_iTypeDraw]->setPen(pen_draw);*/
	vShape.push_back(_ShapeModle[_iTypeDraw]->Clone());
	_ShapeModle[_iTypeDraw]->clearObject();
	InvalidateRect(hwnd, NULL, FALSE);

	ReleaseCapture();
	delete pen;
	_isClickDown = false;
}
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (_isClickDown)
	{

		pState.x = x;
		pState.y = y;
		if (keyFlags & MK_SHIFT && _iTypeDraw != SHAPE_MODEL_LINE)
			fixCoordiSquare();
		InvalidateRect(hwnd, NULL, FALSE);
	}
}
void OnPaint(HWND hWnd)
{
	RECT rect, rectribbon, rectwindow;
	PAINTSTRUCT ps;

	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);
	UINT heightRibbon = GetRibbonHeight();
	/*GetWindowRect(hWnd,&rectwindow);
	if (_mainHeightRibbon != heightRibbon)
	{
		GetClientRect(hWnd, &rectribbon);
		MoveWindow(hWnd, rectwindow.left, rectwindow.top , rectribbon.right - rectribbon.left, rectribbon.bottom - rectribbon.top,1);
	}*/
	GetClientRect(hWnd, &rect);


	HDC hdc_virtual = CreateCompatibleDC(hdc);
	HBITMAP hbm_virtual = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(hdc_virtual, hbm_virtual);

	

	FillRect(hdc_virtual, &rect, (HBRUSH)(COLOR_WINDOW + 1));

	if(_isOpenBitmap)
		BitBlt(hdc_virtual, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcOpenBitmap, 0, 0, SRCCOPY);


	//Graphics* graph = new Graphics(hdc_virtual);
	Color color;
	color.SetFromCOLORREF(_mainColor);
	Pen* pen = new Pen(color, _mainWidth);
	pen->SetDashStyle(_gDashStyle[_mainStyle]);

	for (int i = 0; i < vShape.size(); i++)
	{
		vShape[i]->Draw(hdc_virtual);
	}

	if (_isClickDown)
	{
		if (_isChangeStyle)
		{
			/*HBRUSH hbrush = GetStockBrush(NULL_BRUSH);
			HPEN pen_draw = CreatePen(_mainStyle, _mainWidth, _mainColor);
			_ShapeModle[_iTypeDraw]->setBrush(hbrush);
			_ShapeModle[_iTypeDraw]->setPen(pen_draw);*/
			
			color.SetFromCOLORREF(_mainColor);
			pen->SetColor(color);
			//pen->SetColor(Color(255, 0, 0, 0));
			pen->SetWidth(_mainWidth);
		
			pen->SetDashStyle(_gDashStyle[_mainStyle]);

			//_ShapeModle[_iTypeDraw]->setPen(pen);
			_isChangeStyle = false;
		}
		//_ShapeModle[_iTypeDraw]->Draw(hdc_virtual,pStart,pState);
		_ShapeModle[_iTypeDraw]->setPen(pen);
	/*	if (_iTypeDraw == SHAPE_MODEL_CURVE)
			pStart = pState;*/
		
		_ShapeModle[_iTypeDraw]->Draw(hdc_virtual,pStart,pState);

	}
	BitBlt(hdc, 0, heightRibbon, rect.right, rect.bottom, hdc_virtual, 0, heightRibbon, SRCCOPY);

	DeleteObject(hdc_virtual);
	/*DeleteObject(hdc_virtual);
	DeleteBitmap(hbm_virtual);*/
	//delete graph;
	// TODO: Add any drawing code here...
	EndPaint(hWnd, &ps);
}
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HMENU hMenu = GetMenu(hWnd);
	CHOOSECOLOR colorSelect;
	static COLORREF acrCustClr[16];
	static std::wstring _pathObject ;
	switch (id)
	{
	#pragma region Type
	case ID_TYPE_LINE:
		_iTypeDraw = SHAPE_MODEL_LINE;
		CheckMenu(hWnd, 1, ID_TYPE_LINE);
		break;
	case ID_TYPE_RECTANGLE:
		_iTypeDraw = SHAPE_MODEL_RECTANGLE;
		CheckMenu(hWnd, 1, ID_TYPE_RECTANGLE);
		break;
	case ID_TYPE_ELLIPSE:
		_iTypeDraw = SHAPE_MODEL_ELLIPSE;
		CheckMenu(hWnd, 1, ID_TYPE_ELLIPSE);
		break;
	#pragma endregion

	#pragma region Style

	case ID_STYLE_SOILD:
		_mainStyle = PS_SOLID;
		//_gDashStyle = DashStyle::DashStyleSolid;
		CheckMenu(hWnd, 2, ID_STYLE_SOILD);
		break;
	case ID_STYLE_DASH:
		_mainStyle = PS_DASH;
		//_gDashStyle = DashStyle::DashStyleDash;
		CheckMenu(hWnd, 2, ID_STYLE_DASH);
		break;
	case ID_STYLE_DASHDOT:
		_mainStyle = PS_DASHDOT;
		//_gDashStyle = DashStyle::DashStyleDashDot;
		CheckMenu(hWnd, 2, ID_STYLE_DASHDOT);
		break;
	case ID_STYLE_DASHDOTDOT:
		_mainStyle = PS_DASHDOTDOT;
		//_gDashStyle = DashStyle::DashStyleDashDotDot;
		CheckMenu(hWnd, 2, ID_STYLE_DASHDOTDOT);
		break;
	case ID_STYLE_NULL:
		_mainStyle = PS_NULL;
		//_gDashStyle = DashStyle::DashStyleCustom;
		CheckMenu(hWnd, 2, ID_STYLE_NULL);
		break;
	case ID_STYLE_DOT:
		_mainStyle = PS_DOT;
		//_gDashStyle = DashStyle::DashStyleDot;
		CheckMenu(hWnd, 2, PS_DOT);
		break;
	#pragma endregion

	#pragma region Width
	case ID_WIDTH_1:
		_mainWidth = 1;
		CheckMenu(hWnd, 3, ID_WIDTH_1);
		break;
	case ID_WIDTH_2:
		_mainWidth = 2;
		CheckMenu(hWnd, 3, ID_WIDTH_2);
		break;
	case ID_WIDTH_3:
		_mainWidth = 3;
		CheckMenu(hWnd, 3, ID_WIDTH_3);
		break;
	case ID_WIDTH_4:
		_mainWidth = 4;
		CheckMenu(hWnd, 3, ID_WIDTH_4);
		break;
	case ID_WIDTH_5:
		_mainWidth = 5;
		CheckMenu(hWnd, 3, ID_WIDTH_5);
		break;
	case ID_WIDTH_6:
		_mainWidth = 6;
		CheckMenu(hWnd, 3, ID_WIDTH_6);
		break;
	case ID_WIDTH_7:
		_mainWidth = 7;
		CheckMenu(hWnd, 3, ID_WIDTH_7);
		break;
	#pragma endregion
	case ID_DESIGN_COLOR:
		colorSelect.lStructSize = sizeof(CHOOSECOLOR);
		colorSelect.hwndOwner = hWnd;
		colorSelect.lpCustColors = (LPDWORD)acrCustClr;
		colorSelect.rgbResult = _mainColor;
		colorSelect.Flags = CC_FULLOPEN | CC_RGBINIT;
		//C: Hiển thị hộp thoại chọn màu
		if (ChooseColor(&colorSelect))
			_mainColor = colorSelect.rgbResult;
		break;
	case ID_FILE_NEW:
		vShape.clear();
		_mainColor = 0;
		_mainStyle = 0;
		_mainWidth = 0;
		_isOpenBitmap = false;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case ID_SAVE_BINARY:
		_pathObject = getSaveFileName(hWnd,1);
		if (_pathObject.empty())
			MessageBox(hWnd, L"You haven't chosen any file!", L"Cannot Save", MB_OK);
		else
		{
			std::fstream fs;
			int sizeShapes = vShape.size();
			fs.open(_pathObject, std::ios::out | std::ios::binary);
			fs.write((char*)&sizeShapes, sizeof(int));

			for (int i = 0; i < vShape.size(); i++)
				vShape[i]->SaveObject(fs);
			fs.close();
			MessageBox(hWnd, L"Save completed!", L"Save object", MB_OK);

		}
		break;
	case ID_SAVE_BITMAP:
		_pathObject = getSaveFileName(hWnd,2);
		if(_pathObject.empty())
			MessageBox(hWnd, L"You haven't chosen any file!", L"Cannot Save", MB_OK);
		else
		{
			UINT heightRibbon = GetRibbonHeight();

			RECT rect;
			GetClientRect(hWnd, &rect);
			HDC _hdc = GetDC(hWnd);
			HDC hdc_temp= CreateCompatibleDC(_hdc);

			HDC hdcTemp = GetDC(hWnd);
			HBITMAP hbm = CreateCompatibleBitmap(GetDC(hWnd), rect.right, rect.bottom);
			SelectObject(hdc_temp, hbm);
			BitBlt(hdc_temp, 0, heightRibbon, rect.right, rect.bottom, _hdc, 0, heightRibbon, SRCCOPY);

			PBITMAPINFO info = CreateBitmapInfo(hWnd, hbm);
			WriteBmpTofile(_pathObject.c_str(), info, hbm, _hdc);
			
			DeleteObject(_hdc);
			DeleteObject(hdc_temp);

			DeleteBitmap(hbm);
			
			MessageBox(hWnd, L"Save completed!", L"Save object", MB_OK);
		}	
		break;
	case ID_OPEN_BINARY:
		_pathObject = getOpenFileName(hWnd,1);
		if (_pathObject.empty())
			MessageBox(hWnd, L"You haven't chosen any file!", L"Cannot Open", MB_OK);
		else
		{
			std::fstream fs;
			fs.open(_pathObject, std::ios::in | std::ios::binary);
			if (fs.fail())
			{
				MessageBox(hWnd, L"Can't open file!", L"Open File ", MB_OK);
				return;
			}
			CShape* shape;
			int iIndex = 0;
			/*HPEN pen;
			HBRUSH hbrush;*/
			Pen* pen = NULL;
			vShape.clear();
			int sizeShapes = 0;
			fs.read((char*)&sizeShapes, sizeof(int));
			
			while (sizeShapes--)
			{
				fs.read((char*)&iIndex, sizeof(int));

				if (iIndex > 4 || iIndex < 1)
					continue;
				iIndex -= 1;
				_ShapeModle[iIndex]->ReadObject(fs);
				//fs.read((char*)&pen, sizeof(pen));

				////fs.read((char*)&hbrush, sizeof(hbrush));
				//fs.read((char*)&pStart, sizeof(pStart));
				//fs.read((char*)&pState, sizeof(pState));
				////_ShapeModle[iIndex]->setBrush(hbrush);
				//_ShapeModle[iIndex]->setPen(pen);
				//_ShapeModle[iIndex]->setPointStart(pStart);
				//_ShapeModle[iIndex]->setPointEnd(pState);

				vShape.push_back(_ShapeModle[iIndex]->Clone());
				_ShapeModle[iIndex]->clearObject();
			}
			fs.close();
		}
		break;
	case ID_OPEN_BITMAP:
		_pathObject = getOpenFileName(hWnd,2);
		if (_pathObject.empty())
			MessageBox(hWnd, L"You haven't chosen any file!", L"Cannot Open", MB_OK);
		else
		{
			_isOpenBitmap = true;
			RECT rect;
			GetClientRect(hWnd, &rect);
			int ret = 1;
			hOpenBitmap = LoadImage(NULL, _pathObject.c_str(), IMAGE_BITMAP, 0, 0,LR_LOADFROMFILE);

			hdcOpenBitmap = CreateCompatibleDC(NULL);

			if (hdcOpenBitmap == NULL || hOpenBitmap == NULL)
				ret = -1;

			if (NULL == SelectObject(hdcOpenBitmap, hOpenBitmap))
				ret = -1;

			if (ret == -1)
				MessageBox(hWnd, L"Can't open file!", L"Open File ", MB_OK);
			else if (ret == 1)
			{
				GetObject(hOpenBitmap, sizeof(bitmap), &bitmap);
				UINT heightRibbon = GetRibbonHeight();

				//BitBlt(hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcOpenBitmap, 0, 0, SRCCOPY);
				GetClientRect(hWnd, &rect);
				//rect.top += heightRibbon;
				InvalidateRect(hWnd, &rect, FALSE);
			}
			

		}
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}


}
void OnDestroy(HWND hwnd)
{
	DestroyFramework();
	GdiplusShutdown(gdiplusToken);
	PostQuitMessage(0);

}
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	bool initSuccess = false;
	initSuccess = InitializeFramework(hwnd);
	if (!initSuccess)
		return -1;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	_ShapeModle.push_back(new CLine());
	_ShapeModle.push_back(new CRectangle());
	_ShapeModle.push_back(new CEllipse());
	_ShapeModle.push_back(new CCurve());

	_mainHeightRibbon = GetRibbonHeight();
	return TRUE;
}
void OnSize(HWND hWnd, UINT message, int sx, int sy)
{
	UINT heightRibbon = GetRibbonHeight();
	if (_mainHeightRibbon != heightRibbon)
	{
		MoveWindow(hWnd, 0, 0, sx, sy, 1);
	}
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);

		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);


	}
	return DefWindowProc(hWnd, message, wParam, lParam);

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
