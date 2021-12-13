#include "Feeder.h"

static LONG Steering_pos = VJOY_MAX / 2 - 1;
static VjdStat status;
static Resolution res; // Resolution
static INT index_buttons3;
static LONG axis[AXISCOUNT] = {0,0,0,0};
static INT Xpos = 0;


VOID WinRes()
{
    HMONITOR monitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTONEAREST);
    MONITORINFO w_info;
    w_info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &w_info);
    res.x = (INT)(w_info.rcMonitor.right - w_info.rcMonitor.left);
    res.y = (INT)(w_info.rcMonitor.bottom - w_info.rcMonitor.top);
}

Resolution GetResoloution()
{
    return res;
}

BOOL Preset_Settings() //Before Install, set default settings
{
    settings = (Settings*)malloc(sizeof(*settings));
    if (settings == NULL) { PrintError(L"Settings NULL!"); return FALSE; }
    settings->experimental = FALSE;
    settings->steer_mode = STEERING_NONE;
    settings->cursor_pos = TOP;
    settings->sens = 25;
    settings->sens_mult = 1;
    settings->axis_int[THROTTLE][ONPRESS][END] = 500;
    settings->axis_int[THROTTLE][ONPRESS][MIDDLE] = 1000;
    settings->axis_int[THROTTLE][ONPRESS][BEGIN] = 1250;
    settings->axis_int[THROTTLE][OFFPRESS][END] = 500;
    settings->axis_int[THROTTLE][OFFPRESS][MIDDLE] = 750;
    settings->axis_int[THROTTLE][OFFPRESS][BEGIN] = 500;
    settings->axis_int[BRAKE][ONPRESS][END] = 500;
    settings->axis_int[BRAKE][ONPRESS][MIDDLE] = 1000;
    settings->axis_int[BRAKE][ONPRESS][BEGIN] = 1500;
    settings->axis_int[BRAKE][OFFPRESS][END] = 1000;
    settings->axis_int[BRAKE][OFFPRESS][MIDDLE] = 900;
    settings->axis_int[BRAKE][OFFPRESS][BEGIN] = 900;
    settings->axis_int[CLUTH][ONPRESS][END] = 2000;
    settings->axis_int[CLUTH][ONPRESS][MIDDLE] = 2000;
    settings->axis_int[CLUTH][ONPRESS][BEGIN] = 2000;
    settings->axis_int[CLUTH][OFFPRESS][END] = 2000;
    settings->axis_int[CLUTH][OFFPRESS][MIDDLE] = 2000;
    settings->axis_int[CLUTH][OFFPRESS][BEGIN] = 2000;
    settings->axis_int[HANDBRAKE][ONPRESS][END] = 2000;
    settings->axis_int[HANDBRAKE][ONPRESS][MIDDLE] = 2000;
    settings->axis_int[HANDBRAKE][ONPRESS][BEGIN] = 2000;
    settings->axis_int[HANDBRAKE][OFFPRESS][END] = 2000;
    settings->axis_int[HANDBRAKE][OFFPRESS][MIDDLE] = 2000;
    settings->axis_int[HANDBRAKE][OFFPRESS][BEGIN] = 2000;
    for (INT i = BT9; i < BTCOUNT; i++)
        settings->buttons[i] = '*';
    for (INT i = THROTTLE; i < AXISCOUNT; i++)
        settings->axis[i] = '*';
    settings->axis[AXISCOUNT] = '\0';
    settings->buttons[BTCOUNT] = '\0';
    return TRUE;
}

LONG Steering()
{
    POINT Mousep;
    LONG mousedelta;
    GetCursorPos(&Mousep);
    mousedelta = Mousep.x - res.x / 2;
    Steering_pos += mousedelta * settings->sens / 8 * settings->sens_mult;
    if (Steering_pos > VJOY_MAX)
        Steering_pos = VJOY_MAX;
    else if (Steering_pos < VJOY_MIN)
        Steering_pos = VJOY_MIN;
    return Steering_pos;
}

LONG SteeringX()
{
    static LONG SteeringX;
    SteeringX = Steering_pos;
    if (SteeringX < VJOY_MAX / 2)
        return  ((VJOY_MAX / 2) - SteeringX) * 2;
    else return 0;
}
LONG SteeringY()
{
    static LONG SteeringY;
    SteeringY = Steering_pos;
    if (SteeringY > VJOY_MAX / 2)
        return (SteeringY - (VJOY_MAX / 2)) * 2;
    else return 0;
}

VOID SteeringXY()
{
    SteeringX();
    SteeringY();
}
LONG AXIS(Axis AXIS)
{
    
    if ((GetAsyncKeyState(settings->axis[AXIS]) & 0x8000) != 0)
    {
        if (axis[AXIS] >= VJOY_MAX / 1.5)
            axis[AXIS] = axis[AXIS] + (LONG)(settings->axis_int[AXIS][ONPRESS][END]);
        else if (axis[AXIS] >= VJOY_MAX / 3)
            axis[AXIS] = axis[AXIS] + (LONG)(settings->axis_int[AXIS][ONPRESS][MIDDLE]);
        else
            axis[AXIS] = axis[AXIS] + (LONG)(settings->axis_int[AXIS][ONPRESS][BEGIN]);
    }
    else
    {
        if (axis[AXIS] >= VJOY_MAX / 1.5)
            axis[AXIS] = axis[AXIS] - (LONG)(settings->axis_int[AXIS][OFFPRESS][END]);
        else if (axis[AXIS] >= VJOY_MAX / 3)
            axis[AXIS] = axis[AXIS] - (LONG)(settings->axis_int[AXIS][OFFPRESS][MIDDLE]);
        else
            axis[AXIS] = axis[AXIS] - (LONG)(settings->axis_int[AXIS][OFFPRESS][BEGIN]);
    }
    if (axis[AXIS] > VJOY_MAX)
        axis[AXIS] = VJOY_MAX - 1;
    else if (axis[AXIS] < VJOY_MIN)
        axis[AXIS] = VJOY_MIN + 1;
    return axis[AXIS];
}

BOOL Button(UCHAR BT)
{
    if ((GetAsyncKeyState(BT) & 0x8000) != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL vJoy_Init()
{
    if (!vJoyEnabled())
    {
        MessageBoxA(NULL, "Failed Getting vJoy attributes.", "vJoyInit_Error", MB_OK);
        return FALSE;
    }
    status = GetVJDStatus(1);

    switch (status)
    {
        case VJD_STAT_OWN:
            MessageBoxA(NULL, "vJoy Device 1 is already owned by this feeder", "vJoy_Status", MB_OK);
            return FALSE;
            break;
        case VJD_STAT_FREE:
            break;
        case VJD_STAT_BUSY:
            MessageBoxA(NULL, "vJoy Device 1 is already owned by another feeder\n Cannot continue", "vJoy_Status", MB_OK);
            return FALSE;
            break;
        case VJD_STAT_MISS:
            MessageBoxA(NULL, "vJoy Device 1 is not installed or disabled\n Cannot continue", "vJoy_Status", MB_OK);
            return FALSE;
            break;
        default:
            MessageBoxA(NULL, "vJoy Device 1 general error \nCannot continue", "vJoy_Status", MB_OK);
            return FALSE;
            break;
    }

    if ((status == VJD_STAT_OWN) || (status == VJD_STAT_FREE) && (!AcquireVJD(status)))
    {
        MessageBoxA(NULL, "Failed to acquire vJoy device number 1. \n", "vJoy_Status", MB_OK);
        return FALSE;
    }
}
VOID vJoy_Loop()
{
    ptrvJoyLoop1[0](); // Steering
    ptrvJoyLoop1[1](); // Cursor pos
    ptrvJoyLoop2(index_buttons3);   // Axis Buttons   
    ptrvJoyLoop3();
}

VOID SetPTRvJoyLoop()
{
    switch (settings->steer_mode)
    {
        case STEERING_NONE:
        {
            ptrvJoyLoop1[0] = Steering_None;
        }break;
        case STEERING_SINGLE:
        {
            ptrvJoyLoop1[0] = Steering_Single;
        }break;
        case STEERING_DUAL:
        {
            ptrvJoyLoop1[0] = Steering_Dual;
        }break;
        case STEERING_DUAL1:
        {
            ptrvJoyLoop1[0] = Steering_Dual1;
        }break;
        case STEERING_DUAL2:
        {
            ptrvJoyLoop1[0] = Steering_Dual2;
        }break;
            
    }
    switch (settings->cursor_pos)
    {
        case TOP:
        {
            ptrvJoyLoop1[1] = Cursor_Top;
        }break;
        case BOTTOM:
        {
            ptrvJoyLoop1[1] = Cursor_Bottom;
        }break;
    }
    index_buttons3 = (INT)SendMessage(GetButtons3(), CB_GETCURSEL, 0, 0);
    
    if (index_buttons3 == 0)
    {
        ptrvJoyLoop2 = AllAxisButtons;
    }     
    else if (index_buttons3 <= 4)
    {
        index_buttons3 -= 1;
        ptrvJoyLoop2 = OnlyAxis;
    }
    else if (index_buttons3 >= 5)
    {
        index_buttons3 -= 5;
        ptrvJoyLoop2 = OnlyButtons;
    }

    switch (settings->experimental)
    {
        case TRUE:
        {
            ptrvJoyLoop3 = Experimental;
            ShowWindow(Render->hWindow, SW_SHOW);
        }break;  

        case FALSE:
        {
            ptrvJoyLoop3 = Steering_None;
        }break;
    }
}

VOID Experimental()
{
    FLOAT a1 = res.x - res.x/30;
    FLOAT a2 = VJOY_MAX;
    FLOAT a3 = Steering_pos;
    Xpos = (INT)(a1 / a2 * a3);
    if (Xpos >= res.x - res.x/30)
        Xpos = (INT)(res.x - (res.x/30)-1);
        
    SetWindowPos(GetRenderArrow(), (HWND)1, Xpos, 0, res.x / 30, res.y / 1.2, SWP_NOSIZE | SWP_NOZORDER);
    SetWindowPos(Render->hWindow, HWND_TOPMOST, 0, 0, res.x / 30, res.y / 1.2, SWP_NOSIZE | SWP_NOMOVE | SWP_NOREPOSITION);
}

VOID AllAxisButtons(INT rint)
{
    SetAxis(AXIS(THROTTLE), status, HID_USAGE_RX);
    SetAxis(AXIS(BRAKE), status, HID_USAGE_RY);
    SetAxis(AXIS(CLUTH), status, HID_USAGE_RZ);
    SetAxis(AXIS(HANDBRAKE), status, HID_USAGE_SL0);
    for (INT i = BT9; i < BTCOUNT; i++)
    {
        if (settings->buttons[i] != '*')
            SetBtn(Button(settings->buttons[i]), status, (UCHAR)(i+1));
    }
}

VOID OnlyAxis(INT Axis)
{
    SetAxis(AXIS(Axis), status, (UINT)(51+Axis));
}

VOID OnlyButtons(INT button)
{
    SetBtn(Button(settings->buttons[button]), status, (UCHAR)(button+1));
}

VOID vJoy_ReliquishVJD()
{
    ResetVJD(status);
    RelinquishVJD(status);
}

VOID vJoy_ResetPos()
{
    switch (settings->steer_mode)
    {
        case STEERING_SINGLE:
        {
            SetAxis(VJOY_MAX / 2, status, HID_USAGE_X);
        }break;
        default:
        {
            SetAxis(0, status, HID_USAGE_X);
        }break;
    }
    Steering_pos = VJOY_MAX / 2;
    SetAxis(0, status, HID_USAGE_Y);
    SetAxis(0, status, HID_USAGE_Z);
    SetAxis(0, status, HID_USAGE_RX);
    SetAxis(0, status, HID_USAGE_RY);
    SetAxis(0, status, HID_USAGE_RZ);
    SetAxis(0, status, HID_USAGE_SL0);
    SetAxis(0, status, HID_USAGE_SL1);
}

VOID Cursor_Top()
{
    SetCursorPos(res.x / 2, 0);
}

VOID Cursor_Bottom()
{
    SetCursorPos(res.x / 2, res.y);
}

VOID Steering_None()
{

}

VOID Steering_Single()
{
    SetAxis(Steering(), status, HID_USAGE_X);
}

VOID Steering_Dual()
{
    Steering();
    SetAxis(SteeringX(), status, HID_USAGE_Y);
    SetAxis(SteeringY(), status, HID_USAGE_Z);
    return;
}

VOID Steering_Dual1()
{
    Steering();
    SetAxis(SteeringX(), status, HID_USAGE_Y);
}

VOID Steering_Dual2()
{
    Steering();
    SetAxis(SteeringX(), status, HID_USAGE_Z);
}