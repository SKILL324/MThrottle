#include "Window.h"

BOOL ProcessMsg()
{
	MSG Msg;
    
	while (PeekMessage(&Msg, NULL, 0u, 0u, PM_REMOVE))
	{ 
		if (Msg.message == WM_QUIT)
		{
			return FALSE;
		}
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return TRUE;
}

Window* Window_Register(const WCHAR* class_name, LRESULT(__stdcall* wndproc)(HWND, UINT, WPARAM , LPARAM), Window* WD, UINT hbrush)
{
	WD = (Window*)malloc(sizeof(Window));
	if (WD == NULL)return NULL;
	WD->window_class = (WNDCLASS*)calloc(1, sizeof(WNDCLASS));
	if (WD->window_class == NULL)return NULL;
	WD->processmsg = ProcessMsg;
	WD->window_class->hInstance = GetModuleHandle(NULL);
	WD->window_class->style = CS_VREDRAW | CS_HREDRAW;
	WD->window_class->lpszClassName = class_name;
	WD->window_class->lpfnWndProc = wndproc;
	WD->running = TRUE;
	WD->window_class->hIcon = (HICON)LoadImage(NULL, L"ico.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_SHARED);
	WD->window_class->hbrBackground = (HBRUSH)GetStockObject(hbrush); 
	WD->dcWindow = GetDC(WD->hWindow);
	
	if (RegisterClass(WD->window_class))
	{
		return WD;
	}
	else
	{ 
		PrintError(L"NULL WINDOW REGISTER");
		return NULL;
	}
}
