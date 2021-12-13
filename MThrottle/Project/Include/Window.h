#pragma once
#ifndef WINDOW_H
#define WINDOW_H
#include <windows.h>

#define PrintError(text) MessageBox(NULL, text,L"ERROR", MB_ICONERROR);

struct 
{
	HWND hWindow;
	HDC dcWindow;
	BOOL running;
	WNDCLASS* window_class;
	BOOL(*processmsg)();
}typedef Window;

BOOL ProcessMsg();
Window* Window_Register(const WCHAR* class_name, LRESULT(WINAPI* wndproc)(HWND, UINT, WPARAM, LPARAM), Window* WD, UINT hbrush);
#endif