#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Window.h"
#include <Windows.h>
#include <windowsx.h>
#include "Feeder.h"

#define ICONTRAY_MSG (WM_APP + 1)
#define IDC_SET_BUTTON 102
#define IDC_PEDAL_BUTTON 103
#define IDC_CHECKBOX 104

Window* WD; // Main Window
Window* Render;
NOTIFYICONDATAW nid; // Tray info

UINT Create_WD();
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL Write_File();
BOOL Read_File();
VOID IconTray_Init();
VOID IconTray_Destroy();
#endif