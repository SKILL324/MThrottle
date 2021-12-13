#pragma once
#pragma comment(lib, "vJoyInterface.lib")
#ifndef FEEDER_H
#define FEEDER_H
#include "public.h"
#include "WinUser.h"
#include "vjoyinterface.h"
#include "Window.h"
#include "MSG.h"

#define VJOY_MAX 32767
#define VJOY_MIN 0
#define SLEEP_TIME 1
#define SETTINGS_SIZE sizeof(settings)

enum Buttons
{
    BT9, BT10, BT11, BT12, BT13, BT14, BT15, BT16, BT17, BT18, BT19, BT20,
    BT21, BT22, BT23, BT24, BT25, BT26, BT27, BT28, BT29, BT30, BT31, BT32,
    BTCOUNT
};

enum Steering_Mode
{
    STEERING_NONE, STEERING_SINGLE, STEERING_DUAL, STEERING_DUAL1, STEERING_DUAL2,
    STEERMODECOUNT
};

typedef enum 
{
    THROTTLE, BRAKE, CLUTH, HANDBRAKE,
    AXISCOUNT
} Axis;

typedef enum
{
    TOP, BOTTOM,
    CURSORPOS
}Cursor_Pos;

enum Axis_Pos
{
    BEGIN, MIDDLE, END,
    AXISPOSCOUNT
};

enum Axis_Stat
{
    ONPRESS, OFFPRESS,
    AXISSTATCOUNT
};

enum Sens_Mult
{
    X1, X2, X3,
    SENSMULTCOUNT
};

typedef struct
{
    INT sens;
    INT sens_mult;
    UCHAR axis[AXISCOUNT+1];
    INT axis_int[AXISCOUNT][AXISSTATCOUNT][AXISPOSCOUNT];
    UCHAR buttons[BTCOUNT+1];
    INT cursor_pos;
    INT steer_mode;
    BOOL experimental;

}Settings; //DATA type
Settings* settings;//DATA Global

VOID(*ptrvJoyLoop1[2])(); // Organized functions about Steermode and CursorPos; 
VOID(*ptrvJoyLoop2)(INT); // Organized functions about OnlyButtons, OnlyAxis or AllButtons; 
VOID(*ptrvJoyLoop3)(); // Experimental

typedef struct
{
    UINT x, y;
}Resolution; 

VOID WinRes();
Resolution GetResoloution();

Preset_Settings();

LONG AXIS(Axis AXIS_INT);
LONG Steering();
LONG SteeringX();
LONG SteeringY();
BOOL Button(UCHAR BT);

VOID Steering_None();
VOID Steering_Single();
VOID Steering_Dual();
VOID Steering_Dual1();
VOID Steering_Dual2();
VOID Cursor_Top();
VOID Cursor_Bottom();
VOID AllAxisButtons(INT rint);
VOID OnlyAxis(INT Axis);
VOID OnlyButtons(INT button);
VOID Experimental();

BOOL vJoy_Init();
VOID vJoy_Loop();

VOID vJoy_ReliquishVJD();

VOID vJoy_ResetPos();
VOID SetPTRvJoyLoop();
#endif