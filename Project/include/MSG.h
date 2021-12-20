#pragma once
#ifndef MSG_H
#define MSG_H
#include "MainWindow.h"
#include <commctrl.h>

//WNDPROC {

//} WNDPROC 

VOID Set_Settings();

HWND GetButtons3();
HWND GetRenderArrow();

VOID Icontray_msg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Close(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Hscroll(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Keydown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Lbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Rbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Mbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Xbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Command(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Size(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Create_WD(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Create_Render(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID WM_Destroy(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
VOID IDC_Command(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam); //Callback buttons
#endif