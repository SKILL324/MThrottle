#include "MSG.h"

HWND checkbox, Render_arrow, sens, sens_mult, buttons1, buttons2, buttons3, set_buttons, edge[3], pedals, zone_power[2][3], arrow[2];
HBITMAP BitUp, BitDown, Render_bit;
BOOL keyflag = FALSE;
BOOL icon_click = FALSE;
BOOL create1 = FALSE;
BOOL create2 = FALSE;
WCHAR buffer_zone[5];
Axis Axis_Index = BRAKE;
Axis Axis_Index_Flag = 0;
HBRUSH hBrush;

#define Case(a,b) case a: \
					{ \
						SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)b);\
					}break;\

HWND GetButtons3()
{
	return buttons3;
}

HWND GetRenderArrow()
{
	return Render_arrow;
}

VOID Set_Settings()
{
	WCHAR buffer[5];

	for (UINT i = ONPRESS; i < AXISSTATCOUNT; i++)
		for (UINT j = BEGIN; j < AXISPOSCOUNT; j++)
		{
			_itow_s(settings->axis_int[THROTTLE][i][j], buffer, 5, 10);
			Edit_SetText(zone_power[i][j], buffer);
		}
	_itow_s(settings->sens_mult, buffer, 5, 10);
	Edit_SetText(sens_mult, buffer);
	SendMessage(sens, TBM_SETPOS, TRUE, settings->sens);
	SendMessage(buttons1, CB_SETCURSEL, settings->steer_mode, 0);
	SendMessage(buttons2, CB_SETCURSEL, settings->cursor_pos, 0); 
	CheckDlgButton(WD->hWindow, IDC_CHECKBOX, settings->experimental);
	create1 = TRUE;
}

VOID Icontray_msg(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case WM_LBUTTONUP:
		{
			if (icon_click == FALSE)
			{
				SetWindowPos(WD->hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREPOSITION);
				ShowWindow(WD->hWindow, SW_SHOW);
				UpdateWindow(WD->hWindow);
				ShowWindow(WD->hWindow, SW_RESTORE);
				SetActiveWindow(WD->hWindow);
				
				icon_click = TRUE;
			}
			else
			{
				ShowWindow(WD->hWindow, SW_HIDE);
				icon_click = FALSE;
			}
		}break;

		case WM_RBUTTONUP:
		{
			DestroyWindow(WD->hWindow);
		}break;
	}
}

VOID WM_Close(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	DestroyWindow(hWnd);
}

VOID WM_Hscroll(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == TB_ENDTRACK)
	{
		settings->sens = (INT)SendMessage(sens, TBM_GETPOS, 0, 0);
	}
}

VOID WM_Keydown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (keyflag == TRUE)
	{
		INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
		if (Index == 0) { keyflag = FALSE; return; }
		UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
		if (Index >= 5)
			*psettings = settings->buttons[Index - 5] = (UCHAR)wParam;
		else
			*psettings = settings->axis[Index-1] = (UCHAR)wParam;

		WCHAR text1[2] = { (WCHAR)wParam, '\0' };

		switch (wParam)
		{
			Case('*', L"SET");
			Case(VK_SPACE, L"SPACE");
			Case(VK_SHIFT, L"SHIFT");
			Case(VK_CONTROL, L"CONTROL");
			Case(VK_CAPITAL, L"CAPITAL");
			Case(VK_TAB, L"TAB");
			case VK_ESCAPE:
			{
				*psettings = '*';
				SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L"SET");
			}break;
			default:
			{
				SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)text1);
			}break;
		}
		free(psettings);
		keyflag = FALSE;
	}
}

VOID WM_Lbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (keyflag == TRUE)
	{
		INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
		if (Index == 0) { keyflag = FALSE; return; }
		UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
		if (Index >= 5)
			*psettings = settings->buttons[Index - 5] = (UCHAR)wParam;
		else
			*psettings = settings->axis[Index - 1] = (UCHAR)wParam;

		switch (wParam)
		{
			Case('*', L"SET");
			Case(MK_LBUTTON, L"MLEFT");
			case VK_ESCAPE:
			{
				*psettings = '*';
				SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L"SET");
			}break;
	
		}
		free(psettings);
		keyflag = FALSE;
	}
}

VOID WM_Rbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (keyflag == TRUE)
	{
		INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
		if (Index == 0) { keyflag = FALSE; return; }
		UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
		if (Index >= 5)
			*psettings = settings->buttons[Index - 5] = (UCHAR)wParam;
		else
			*psettings = settings->axis[Index - 1] = (UCHAR)wParam;

		switch (wParam)
		{
			Case('*', L"SET");
			Case(MK_RBUTTON, L"MRIGTH");
		case VK_ESCAPE:
		{
			*psettings = '*';
			SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L"SET");
		}break;

		}
		free(psettings);
		keyflag = FALSE;
	}
}

VOID WM_Mbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (keyflag == TRUE)
	{
		INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
		if (Index == 0) { keyflag = FALSE; return; }
		UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
		if (Index >= 5)
			*psettings = settings->buttons[Index - 5] = (UCHAR)wParam;
		else
			*psettings = settings->axis[Index - 1] = (UCHAR)wParam;

		switch (wParam)
		{
			Case('*', L"SET");
			Case(MK_MBUTTON, L"MMIDDLE");
			case VK_ESCAPE:
			{
				*psettings = '*';
				SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L"SET");
			}break;

		}
		free(psettings);
		keyflag = FALSE;
	}
}

VOID WM_Xbuttondown(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (keyflag == TRUE)
	{
		INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
		if (Index == 0) { keyflag = FALSE; return; }
		UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
		if (Index >= 5)
			*psettings = settings->buttons[Index - 5] = (UCHAR)wParam;
		else
			*psettings = settings->axis[Index - 1] = (UCHAR)wParam;

		switch (wParam)
		{
			Case('*', L"SET");
			Case(MK_XBUTTON1, L"MX1");
			Case(MK_XBUTTON2, L"MX2");
		case VK_ESCAPE:
		{
			*psettings = '*';
			SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L"SET");
		}break;

		}
		free(psettings);
		keyflag = FALSE;
	}
}

VOID WM_Command(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
		case CBN_CLOSEUP:
		{
			if ((HWND)lParam == buttons1)
			{
				settings->steer_mode = (INT)SendMessage(buttons1, CB_GETCURSEL, 0, 0);
			}
			if ((HWND)lParam == buttons2)
			{
				settings->cursor_pos = (INT)SendMessage(buttons2, CB_GETCURSEL, 0, 0);
			}
			if ((HWND)lParam == buttons3)
			{
				INT Index = (INT)SendMessage(buttons3, CB_GETCURSEL, 0, 0);
				if (Index == 0) { SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)L""); break; }
				UCHAR* psettings = (UCHAR*)malloc(sizeof(UCHAR));
				if (Index >= 5)
					*psettings = settings->buttons[Index - 5];
				else
					*psettings = settings->axis[Index-1];
				WCHAR text1[2] = { (WCHAR)*psettings, '\0' };
				switch (*psettings)
				{
					Case('*', L"SET");
					Case(VK_SPACE, L"SPACE");
					Case(VK_SHIFT, L"SHIFT");
					Case(VK_CONTROL, L"CONTROL");
					Case(VK_CAPITAL, L"CAPITAL");
					Case(VK_TAB, L"TAB");
					Case(VK_LBUTTON, L"MLEFT");
					Case(VK_RBUTTON, L"MRIGTH");
					Case(VK_MBUTTON, L"MMIDDLE");
					Case(VK_XBUTTON1, L"MX1");
					Case(VK_XBUTTON2, L"MX2");
				default:
				{
					SendMessage(set_buttons, WM_SETTEXT, 0, (LPARAM)text1);
				}break;
				}
				free(psettings);
			}
		}break;

		case EN_CHANGE:
		{
			if (create1)
			{
				if ((HWND)lParam == sens_mult)
				{
					WCHAR buffer_sens_mult[2];
					buffer_sens_mult[1] = '\0';
					Edit_GetText(sens_mult, buffer_sens_mult, 2);
					settings->sens_mult = _wtoi(buffer_sens_mult);
				}
				else
				{
					for (INT i = ONPRESS; i < AXISSTATCOUNT; i++)
						for (INT j = BEGIN; j < AXISPOSCOUNT; j++)
						{
							if ((HWND)lParam == zone_power[i][j])
							{
								WCHAR buffer_axis_int[6];
								buffer_axis_int[5] = '\0';
								Edit_GetText(zone_power[i][j], buffer_axis_int, 5);

								switch (Axis_Index_Flag)
								{
								case THROTTLE:
								{
									settings->axis_int[THROTTLE][i][j] = _wtoi(buffer_axis_int);
								}break;
								case BRAKE:
								{
									settings->axis_int[BRAKE][i][j] = _wtoi(buffer_axis_int);
								}break;
								case CLUTH:
								{
									settings->axis_int[CLUTH][i][j] = _wtoi(buffer_axis_int);
								}break;
								case HANDBRAKE:
								{
									settings->axis_int[HANDBRAKE][i][j] = _wtoi(buffer_axis_int);
								}break;
								}
							}
						}
				}		
			}

		}break;
	}
}


VOID WM_Size(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case SIZE_MINIMIZED:
		{
			ShowWindow(hWnd, SW_HIDE);
			icon_click = FALSE;
		}break; 
			
	}
}

VOID WM_Create_WD(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)////////////////////////////////////////
{	
	sens = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, TRACKBAR_CLASS, L"", WM_HSCROLL | WS_VISIBLE | WS_CHILD | TBS_HORZ, 35, 30, 220, 25, hWnd, NULL, NULL, NULL);
	if (sens_mult = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 120, 10, 50, 15, hWnd, NULL, NULL, NULL))
	{
		SendMessage(sens_mult, EM_LIMITTEXT, 1, 0);
	}
	if (buttons1 = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_HSCROLL | WS_VSCROLL, 35, 70, 100, 100, hWnd, NULL, NULL, NULL))
	{
		TCHAR Buttons1[5][12] =
		{
			L"None", L"SingleAxis", L"DoubleAxis", L"DoubleLeft", L"DoubleRight"
		};
		TCHAR A[12];

		memset(&A, 0, sizeof(A));
		for (INT k = 0; k < 5; k++)
		{
			wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)Buttons1[k]);
			SendMessage(buttons1, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
		}
		SendMessage(buttons1, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
	if (buttons2 = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_HSCROLL | WS_VSCROLL, 155, 70, 100, 100, hWnd, NULL, NULL, NULL))
	{
		TCHAR Buttons2[2][13] =
		{
			TEXT("CursorTop"), TEXT("CursorBottom")
		};
		TCHAR C[13];

		memset(&C, 0, sizeof(C));
		for (INT l = 0; l < 2; l++)
		{
			wcscpy_s(C, sizeof(C) / sizeof(TCHAR), (TCHAR*)Buttons2[l]);
			SendMessage(buttons2, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)C);
		}
		SendMessage(buttons2, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
	if (buttons3 = CreateWindow(WC_COMBOBOX, L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_HSCROLL | WS_VSCROLL, 35, 130, 100, 100, hWnd, NULL, NULL, NULL))
	{
		TCHAR Buttons3[29][11] =
		{
			TEXT("All Enable"),
			TEXT("Throttle"), TEXT("Brake"),TEXT("Cluth"), TEXT("Handbrake"),
			TEXT("Button9"), TEXT("Button10"), TEXT("Button11"),  TEXT("Button12"),
			TEXT("Button13"), TEXT("Button14"), TEXT("Button15"), TEXT("Button16"),
			TEXT("Button17"), TEXT("Button18"), TEXT("Button19"), TEXT("Button20"),
			TEXT("Button21"), TEXT("Button22"), TEXT("Button23"), TEXT("Button24"),
			TEXT("Button25"), TEXT("Button26"), TEXT("Button27"), TEXT("Button28"),
			TEXT("Button29"), TEXT("Button30"), TEXT("Button31"), TEXT("Button32")
		};
		TCHAR B[29];

		memset(&B, 0, sizeof(B));
		for (INT j = 0; j < 29; j++)
		{
			wcscpy_s(B, sizeof(B) / sizeof(TCHAR), (TCHAR*)Buttons3[j]);
			SendMessage(buttons3, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)B);
		}
		SendMessage(buttons3, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	}
	 
	set_buttons = CreateWindow(WC_BUTTON, L"", WS_VISIBLE | WS_CHILD | ES_CENTER, 155, 130, 100, 25, hWnd, (HMENU)IDC_SET_BUTTON, NULL, NULL);
	edge[0] = CreateWindow(WC_STATIC, L"", WS_VISIBLE | WS_CHILD | ES_READONLY | SS_ETCHEDHORZ, 20, 110, 255, 25, hWnd, NULL, NULL, NULL);
	edge[1] = CreateWindow(WC_STATIC, L"", WS_VISIBLE | WS_CHILD | ES_READONLY | SS_ETCHEDHORZ, 20, 170, 255, 25, hWnd, NULL, NULL, NULL);
	edge[2] = CreateWindow(WC_STATIC, L"", WS_VISIBLE | WS_CHILD | ES_READONLY | SS_ETCHEDHORZ, 20, 320, 255, 25, hWnd, NULL, NULL, NULL);
	pedals = CreateWindow(WC_BUTTON, L"THROTTLE", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_READONLY, 95, 190, 100, 25, hWnd, (HMENU)IDC_PEDAL_BUTTON, NULL, NULL);
	arrow[0] = CreateWindowEx(0, WC_STATIC, L"UP", WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_CENTERIMAGE, 20, 220, 70, 100, hWnd, NULL, NULL, NULL);
	arrow[1] = CreateWindowEx(0, WC_STATIC, L"DOWN", WS_VISIBLE | WS_CHILD | SS_BITMAP | SS_CENTERIMAGE, 200, 220, 70, 100, hWnd, NULL, NULL, NULL);
	BitUp = (HBITMAP)LoadImage(NULL, L"UP.bmp", IMAGE_BITMAP, 70, 100, LR_LOADFROMFILE);
	BitDown = (HBITMAP)LoadImage(NULL, L"DOWN.bmp", IMAGE_BITMAP, 70, 100, LR_LOADFROMFILE);
	SendMessage(arrow[0], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BitUp);
	SendMessage(arrow[1], STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BitDown);
	zone_power[0][0] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 90, 230, 50, 15, hWnd, NULL, NULL, NULL);
	zone_power[0][1] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 90, 260, 50, 15, hWnd, NULL, NULL, NULL);
	zone_power[0][2] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 90, 290, 50, 15, hWnd, NULL, NULL, NULL);
	zone_power[1][0] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 150, 230, 50, 15, hWnd, NULL, NULL, NULL);
	zone_power[1][1] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 150, 260, 50, 15, hWnd, NULL, NULL, NULL);
	zone_power[1][2] = CreateWindow(WC_EDIT, L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_NUMBER, 150, 290, 50, 15, hWnd, NULL, NULL, NULL);
	for (INT i = ONPRESS; i < AXISSTATCOUNT; i++)
		for (INT j = BEGIN; j < AXISPOSCOUNT; j++)
		{
			SendMessage(zone_power[i][j], EM_LIMITTEXT, 4, 0);
		}
	
	checkbox = CreateWindow(WC_BUTTON, L"EXPERIMENTAL", WS_VISIBLE | WS_CHILD | BS_CHECKBOX | BS_CENTER | BS_FLAT, 80, 340, 120, 30, hWnd, (HMENU)IDC_CHECKBOX, NULL, NULL);
}

VOID WM_Create_Render(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	WinRes();
	Resolution res = GetResoloution();
	Render_arrow = CreateWindowEx(NULL, WC_STATIC, L"", SS_CENTERIMAGE | SS_BITMAP |  WS_CHILD | WS_VISIBLE, res.x/2.5, 0, res.x/30, res.y/1.2, hWnd, NULL, NULL, NULL);
	Render_bit = (HBITMAP)LoadImage(NULL, L"ARROW.bmp", IMAGE_BITMAP, 0, 100, LR_LOADFROMFILE);
	SendMessage(Render_arrow, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)Render_bit);
	create2 = TRUE;
}

VOID WM_Destroy(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	IconTray_Destroy();
	PostQuitMessage(0);
	WD->running = FALSE;
	Render->running = FALSE;
}

VOID IDC_Command(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case IDC_SET_BUTTON:
		{
			keyflag = TRUE;
			SetFocus(hWnd);
			
		}break;

		case IDC_CHECKBOX:
		{
			
			if (settings->experimental) {
				CheckDlgButton(hWnd, IDC_CHECKBOX, BST_UNCHECKED);
			}
			else {
				CheckDlgButton(hWnd, IDC_CHECKBOX, BST_CHECKED);
			}
			settings->experimental = IsDlgButtonChecked(hWnd, IDC_CHECKBOX);
		}break;

		case IDC_PEDAL_BUTTON:
		{
			create1 = FALSE;
			for (INT i = ONPRESS; i < AXISSTATCOUNT; i++)
				for (INT j = BEGIN; j < AXISPOSCOUNT; j++)
				{
					_itow_s((INT)settings->axis_int[Axis_Index][i][j], buffer_zone, sizeof(buffer_zone) / sizeof(WCHAR), 10);
					Edit_SetText(zone_power[i][j], buffer_zone);
				}
			switch (Axis_Index)
			{
				case THROTTLE:
				{
					SendMessage(pedals, WM_SETTEXT, 0, (LPARAM)TEXT("THROTTLE"));
					Axis_Index = BRAKE;
				}break;
				case BRAKE:
				{
					SendMessage(pedals, WM_SETTEXT, 0, (LPARAM)TEXT("BRAKE"));
					Axis_Index = CLUTH;
				}break;
				case CLUTH:
				{
					SendMessage(pedals, WM_SETTEXT, 0, (LPARAM)TEXT("CLUTH"));
					Axis_Index = HANDBRAKE;
				}break;
				case HANDBRAKE:
				{
					SendMessage(pedals, WM_SETTEXT, 0, (LPARAM)TEXT("HANDBRAKE"));
					Axis_Index = THROTTLE;
				}break;
			}
			Axis_Index_Flag++;
			if (Axis_Index_Flag == 4)
				Axis_Index_Flag = THROTTLE;
			create1 = TRUE;
		}break;
	}
}
