#include "MainWindow.h"
#include "Feeder.h"
#include <conio.h>
#include <time.h>
#include <stdio.h>

#define OCR_NORMAL 32512

static clock_t Begin = 0;
static clock_t End = 0;
static clock_t Delta;

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ PWSTR pCmdLine, 
	_In_ int nCmdShow)
{
	// DEBUG on Console
	//AllocConsole();
	//FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
	//freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	static BOOL altf10 = FALSE;
	if (Preset_Settings() == FALSE) return 0;
	if (Create_WD() == 0) return 0;
	IconTray_Init();
	if (Read_File() == FALSE) return 0;
	if (vJoy_Init() == FALSE) return 0;
	vJoy_ResetPos();
	
	while (WD->processmsg())
	{ 
		//Begin = clock();

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

		/*End = clock();
		Delta = (End - Begin) / CLOCKS_PER_SEC;
		if (Delta < 8)
			Sleep(8 - Delta);
		else
			Sleep(2000);*/
	}
	Write_File();
	vJoy_ReliquishVJD();
	IconTray_Destroy();
	return 0;
}