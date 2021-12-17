#include "MainWindow.h"
#include "Feeder.h"
#include <conio.h>

#define OCR_NORMAL 32512

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	static BOOL altf10 = FALSE;
	if (Preset_Settings() == FALSE) return 0;
	if (Create_WD() == 0) return 0;
	IconTray_Init();
	if (Read_File() == FALSE) return 0;
	if (vJoy_Init() == FALSE) return 0;
	vJoy_ResetPos();

	while (WD->processmsg())
	{
		Sleep(SLEEP_TIME); 

		if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_F10))
		{
			if (altf10 == FALSE)
			{
				WinRes();
				SetPTRvJoyLoop();
				vJoy_ResetPos();
				
			}
			else
			{
				ShowWindow(Render->hWindow, SW_HIDE);
			}
			Sleep(200);
			altf10 = !altf10;	
		}

		if (altf10 == TRUE)
		{
			vJoy_Loop();
		}
	}
	Write_File();
	vJoy_ReliquishVJD();
	IconTray_Destroy();
	return 0;
}