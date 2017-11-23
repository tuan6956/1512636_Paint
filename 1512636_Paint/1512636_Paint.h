#pragma once

#include "resource.h"

extern COLORREF _mainColor;
extern int _mainStyle;
extern int _mainWidth;
extern int _iTypeDraw;
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnDestroy(HWND hwnd);
void OnPaint(HWND hWnd);
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);


