#include "MSG.h"

#define Case(a,b) case a: \
					{ \
						b(hWnd, Msg, wParam, lParam);\
					}break;\

#define Default(b) default: \
					{ \
						b(hWnd, Msg, wParam, lParam);\
					}break;\

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	IDC_Command(hWnd, Msg, wParam, lParam);
	switch (Msg)
	{
		Case(ICONTRAY_MSG, Icontray_msg);
		Case(WM_HSCROLL, WM_Hscroll);
		Case(WM_KEYDOWN, WM_Keydown);
		Case(WM_LBUTTONDOWN, WM_Lbuttondown);
		Case(WM_RBUTTONDOWN, WM_Rbuttondown);
		//Case(WM_MBUTTONDOWN, WM_Mbuttondown);
		//Case(WM_XBUTTONDOWN, WM_Xbuttondown); Not working, Check GetAsycKeyState() Doc
		Case(WM_COMMAND, WM_Command);
		Case(WM_SIZE, WM_Size);
		Case(WM_CREATE, WM_Create_WD);
		Case(WM_DESTROY, WM_Destroy);
		Case(WM_CLOSE, WM_Close);
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK RenderProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		Case(WM_DESTROY, WM_Destroy);
		Case(WM_CLOSE, WM_Close);
		Case(WM_CREATE, WM_Create_Render);
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

UINT Create_WD()
{
	WD = Window_Register(L"MThrottle", WndProc, WD, GRAY_BRUSH);
	Render = Window_Register(L"Render", RenderProc, Render, GRAY_BRUSH); //NULL_BRUSH

	WD->hWindow =
		CreateWindowEx(WS_EX_STATICEDGE, L"MThrottle", L"MThrottle",
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			1000, 200, 310, 420, NULL, NULL, NULL, NULL);

	if (WD->hWindow == NULL) { PrintError(L"NULL WINDOW HANDLE"); return 0; }

	WinRes();
	Resolution res = GetResoloution();

	Render->hWindow =
		CreateWindowEx(WS_EX_TRANSPARENT, L"Render", L"Render",
			WS_POPUP | WS_VISIBLE,
			0, (INT)(res.y / 1.05), (INT)(res.x), (INT)(res.y / 1.2), WD->hWindow, NULL, NULL, NULL);

	if (WD->hWindow == NULL) { PrintError(L"NULL WINDOW HANDLE"); return 0; }

	UpdateWindow(WD->hWindow);
	UpdateWindow(Render->hWindow);
	ShowWindow(Render->hWindow, SW_HIDE);
	return 1;
}

VOID IconTray_Init()
{
	nid.cbSize = sizeof(nid);
	nid.hWnd = FindWindow(L"MThrottle", L"MThrottle");
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = ICONTRAY_MSG;

	nid.hIcon = (HICON)LoadImage(NULL, L"ico.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	memcpy(nid.szTip, L"MThrottle", 20);
	Shell_NotifyIcon(NIM_ADD, &nid);
}

VOID IconTray_Destroy()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL Write_File()
{
	HANDLE sFile = CreateFile(
		L"Settings.dat",
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	if (sFile == INVALID_HANDLE_VALUE)
	{
		PrintError(L"Fail to Write on 'Settings.dat'!");
		return FALSE;
	}

	DWORD sBytesToRead = sizeof(*settings);
	DWORD sBytesWritten = 0;

	WriteFile(sFile,
		(VOID*)settings,
		sBytesToRead,
		&sBytesWritten,
		NULL);
	CloseHandle(sFile);
	return TRUE;
}

BOOL Read_File()
{
	
	HANDLE sFile = CreateFile(
		L"Settings.dat",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (sFile == INVALID_HANDLE_VALUE)
	{
		sFile = CreateFile(
			L"Settings.dat",
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		//Preset_Settings();
	}
	else
	{
		DWORD sBytesToRead = sizeof(*settings);
		DWORD sBytesRead = 0;

		BOOL sRead = ReadFile(sFile,
			(VOID*)settings,
			sBytesToRead,
			&sBytesRead,
			NULL);

		if (!sRead)
		{
			PrintError(L"Read File 'Settings.dat' Fail!")
			CloseHandle(sFile);
			return FALSE;
		}
	}
	Set_Settings();
	CloseHandle(sFile);
	return TRUE;
}