//
//	ID Engine
//	ID_IN.h - Header file for Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

#ifndef	__ID_IN_H_
#define	__ID_IN_H_
#include "wl_def.h"

#ifdef	__DEBUG__
#define	__DEBUG_InputMgr__
#endif

#if defined(SWITCH) || defined (N3DS)
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
typedef enum
{
	sc_None		=	0
	sc_Bad		=	0xff,
	sc_Return	=	SDLK_MINUS,
	sc_Enter	=	sc_Return, // ZR
	sc_Escape	=   SDLK_PLUS, //SDLK_j // ZL
	sc_Space	=	SDLK_b,
	sc_BackSpace =	SDLK_BACKSPACE,
	sc_Tab		=	SDLK_TAB,
	sc_Alt		=	SDLK_x,
	sc_Control	=	SDLK_a,
	sc_CapsLock	=	SDLK_CAPSLOCK,
	sc_LShift	=	SDLK_y,
	sc_RShift	=	SDLK_RSHIFT,
	sc_UpArrow	=	SDLK_UP,
	sc_DownArrow =	SDLK_DOWN,
	sc_LeftArrow =	SDLK_LEFT,
	sc_RightArrow =	SDLK_RIGHT,
	sc_Insert	=	SDLK_INSERT,
	sc_Delete	=	SDLK_DELETE,
	sc_Home		=	SDLK_HOME,
	sc_End		=	SDLK_END,
	sc_PgUp		=	SDLK_PAGEUP,
	sc_PgDn		=	SDLK_PAGEDOWN,
	sc_F1		=	SDLK_F1,
	sc_F2		=	SDLK_F2,
	sc_F3		=	SDLK_F3,
	sc_F4		=	SDLK_F4,
	sc_F5		=	SDLK_F5,
	sc_F6		=	SDLK_F6,
	sc_F7		=	SDLK_F7,
	sc_F8		=	SDLK_F8,
	sc_F9		=	SDLK_F9,
	sc_F10		=	SDLK_F10,
	sc_F11		=	SDLK_F11,
	sc_F12		=	SDLK_F12,
	sc_ScrollLock	=	SDLK_SCROLLOCK,
	sc_PrintScreen	=	SDLK_PRINT,
	sc_1	=		SDLK_q,
	sc_2	=		SDLK_q,
	sc_3	=		SDLK_q,
	sc_4	=		SDLK_q,
	sc_5	=		SDLK_q,
	sc_6	=		SDLK_q,
	sc_7	=		SDLK_q,
	sc_8	=		SDLK_q,
	sc_9	=		SDLK_q,
	sc_0	=		SDLK_q,
	sc_A	=		SDLK_q,
	sc_B	=		SDLK_q,
	sc_C	=		SDLK_q,
	sc_D	=		SDLK_q,
	sc_E	=		SDLK_q,
	sc_F	=		SDLK_q,
	sc_G	=		SDLK_q,
	sc_H	=		SDLK_q,
	sc_I	=		SDLK_q,
	sc_J	=		SDLK_q,
	sc_K	=		SDLK_q,
	sc_L	=		SDLK_q,
	sc_M	=		SDLK_q,
	sc_N	=		SDLK_q,
	sc_O	=		SDLK_q,
	sc_P	=		SDLK_q,
	sc_Q	=		SDLK_q,
	sc_R	=		SDLK_q,
	sc_S	=		SDLK_q,
	sc_T	=		SDLK_q,
	sc_U	=		SDLK_q,
	sc_V	=		SDLK_q,
	sc_W	=		SDLK_q,
	sc_X	=		SDLK_q,
	sc_Y	=		SDLK_q,
	sc_Z	=		SDLK_q,
	sc_1	=		SDLK_1,
	sc_2	=		SDLK_2,
	sc_3	=		SDLK_3,
	sc_4	=		SDLK_4,
	sc_5	=		SDLK_5,
	sc_6	=		SDLK_6,
	sc_7	=		SDLK_7,
	sc_8	=		SDLK_8,
	sc_9	=		SDLK_9,
	sc_0	=		SDLK_0,
	key_None = 0,
	KEYCOUNT,
#if SDL_MAJOR_VERSION == 2
	bt_None = SDL_CONTROLLER_BUTTON_INVALID,
	bt_A = SDL_CONTROLLER_BUTTON_A,
	bt_B = SDL_CONTROLLER_BUTTON_B,
	bt_X = SDL_CONTROLLER_BUTTON_X,
	bt_Y = SDL_CONTROLLER_BUTTON_Y,
	bt_Back = SDL_CONTROLLER_BUTTON_BACK,
	bt_Guide = SDL_CONTROLLER_BUTTON_GUIDE,
	bt_Start = SDL_CONTROLLER_BUTTON_START,
	bt_LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
	bt_RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	bt_LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	bt_RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	bt_DpadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
	bt_DpadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	bt_DpadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	bt_DpadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	bt_touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	bt_Max = SDL_CONTROLLER_BUTTON_MAX,
	//Axis stuff
	gc_axis_invalid = SDL_CONTROLLER_AXIS_INVALID,
	gc_axis_leftx = SDL_CONTROLLER_AXIS_LEFTX,
	gc_axis_lefty = SDL_CONTROLLER_AXIS_LEFTY,
	gc_axis_rightx = SDL_CONTROLLER_AXIS_RIGHTX,
	gc_axis_righty = SDL_CONTROLLER_AXIS_RIGHTY,
	gc_trigger_left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
	gc_trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
	gc_axis_max = SDL_CONTROLLER_AXIS_MAX,
#endif
#if SDL_MAJOR_VERSION == 1
	KEYMAX = KEYCOUNT
#elif SDL_MAJOR_VERSION == 2
	GAMECONTROLLER_MAX = bt_Max + gc_axis_max,

	KEYMAX = KEYCOUNT + GAMECONTROLLER_MAX
#endif
}ScanCode;
#elif defined(PS2)
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDLK_SCROLLLOCK	SDLK_SCROLLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif
typedef enum
{
	sc_None = 0,
	sc_Bad = 0xff,
	sc_unknown = SDLK_UNKNOWN,
	sc_Return = SDLK_RETURN,
	sc_Enter = sc_Return,
	sc_Escape = SDLK_ESCAPE,
	sc_Space = SDLK_SPACE,
	sc_BackSpace = SDLK_BACKSPACE,
	sc_Tab = SDLK_TAB,
	sc_Alt = SDLK_LALT,
	sc_Control = SDLK_LCTRL,
	sc_CapsLock = SDLK_CAPSLOCK,
	sc_LShift = SDLK_LSHIFT,
	sc_RShift = SDLK_RSHIFT,
	sc_UpArrow = SDLK_UP,
	sc_DownArrow = SDLK_DOWN,
	sc_LeftArrow = SDLK_LEFT,
	sc_RightArrow = SDLK_RIGHT,
	sc_Insert = SDLK_INSERT,
	sc_Delete = SDLK_DELETE,
	sc_Home = SDLK_HOME,
	sc_End = SDLK_END,
	sc_PgUp = SDLK_PAGEUP,
	sc_PgDn = SDLK_PAGEDOWN,
	sc_F1 = SDLK_F1,
	sc_F2 = SDLK_F2,
	sc_F3 = SDLK_F3,
	sc_F4 = SDLK_F4,
	sc_F5 = SDLK_F5,
	sc_F6 = SDLK_F6,
	sc_F7 = SDLK_F7,
	sc_F8 = SDLK_F8,
	sc_F9 = SDLK_F9,
	sc_F10 = SDLK_F10,
	sc_F11 = SDLK_F11,
	sc_F12 = SDLK_F12,
	sc_ScrollLock = SDLK_SCROLLLOCK,
	sc_PrintScreen = SDLK_PRINTSCREEN,
	sc_1 = SDLK_1,
	sc_2 = SDLK_2,
	sc_3 = SDLK_3,
	sc_4 = SDLK_4,
	sc_5 = SDLK_5,
	sc_6 = SDLK_6,
	sc_7 = SDLK_7,
	sc_8 = SDLK_8,
	sc_9 = SDLK_9,
	sc_0 = SDLK_0,

	sc_A = SDLK_a,
	sc_B = SDLK_b,
	sc_C = SDLK_c,
	sc_D = SDLK_d,
	sc_E = SDLK_e,
	sc_F = SDLK_f,
	sc_G = SDLK_g,
	sc_H = SDLK_h,
	sc_I = SDLK_i,
	sc_J = SDLK_j,
	sc_K = SDLK_k,
	sc_L = SDLK_l,
	sc_M = SDLK_m,
	sc_N = SDLK_n,
	sc_O = SDLK_o,
	sc_P = SDLK_p,
	sc_Q = SDLK_q,
	sc_R = SDLK_r,
	sc_S = SDLK_s,
	sc_T = SDLK_t,
	sc_U = SDLK_u,
	sc_V = SDLK_v,
	sc_W = SDLK_w,
	sc_X = SDLK_x,
	sc_Y = SDLK_y,
	sc_Z = SDLK_z,
	key_None = 0,
	KEYCOUNT,
#if SDL_MAJOR_VERSION == 2
	bt_None = SDL_CONTROLLER_BUTTON_INVALID,
	bt_A = SDL_CONTROLLER_BUTTON_A,
	bt_B = SDL_CONTROLLER_BUTTON_B,
	bt_X = SDL_CONTROLLER_BUTTON_X,
	bt_Y = SDL_CONTROLLER_BUTTON_Y,
	bt_Back = SDL_CONTROLLER_BUTTON_BACK,
	bt_Guide = SDL_CONTROLLER_BUTTON_GUIDE,
	bt_Start = SDL_CONTROLLER_BUTTON_START,
	bt_LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
	bt_RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	bt_LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	bt_RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	bt_DpadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
	bt_DpadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	bt_DpadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	bt_DpadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	bt_touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	bt_Max = SDL_CONTROLLER_BUTTON_MAX,

	//Axis stuff
	gc_axis_invalid = SDL_CONTROLLER_AXIS_INVALID,
	gc_axis_leftx = SDL_CONTROLLER_AXIS_LEFTX,
	gc_axis_lefty = SDL_CONTROLLER_AXIS_LEFTY,
	gc_axis_rightx = SDL_CONTROLLER_AXIS_RIGHTX,
	gc_axis_righty = SDL_CONTROLLER_AXIS_RIGHTY,
	gc_trigger_left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
	gc_trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
	gc_axis_max = SDL_CONTROLLER_AXIS_MAX,
#endif
#if SDL_MAJOR_VERSION == 1
	KEYMAX = KEYCOUNT
#elif SDL_MAJOR_VERSION == 2
	GAMECONTROLLER_MAX = bt_Max + gc_axis_max,

	KEYMAX = KEYCOUNT + GAMECONTROLLER_MAX
#endif
} ScanCode;
#else
#if SDL_MAJOR_VERSION == 1
#define SDLK_SCROLLLOCK SDLK_SCROLLOCK
#define SDLK_KP_0 SDLK_KP0
#define SDLK_KP_1 SDLK_KP1
#define SDLK_KP_2 SDLK_KP2
#define SDLK_KP_3 SDLK_KP3
#define SDLK_KP_4 SDLK_KP4
#define SDLK_KP_5 SDLK_KP5
#define SDLK_KP_6 SDLK_KP6
#define SDLK_KP_7 SDLK_KP7
#define SDLK_KP_8 SDLK_KP8
#define SDLK_KP_9 SDLK_KP9
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#define SDL_Keymod SDLMod
#define SDL_Keysym SDL_keysym
#endif

typedef enum
{
	key_None = 0,
	sc_None	= 0,
	sc_Bad = 0xff,
	sc_unknown = SDLK_UNKNOWN,
	sc_Return =	   SDLK_RETURN,
	sc_Enter =     sc_Return,
	sc_Escape =    SDLK_ESCAPE,
    sc_Space =	   SDLK_SPACE,
    sc_BackSpace = SDLK_BACKSPACE,
	sc_Tab =       SDLK_TAB,
	sc_Alt =	   SDLK_LALT,
    sc_Control =   SDLK_LCTRL,
    sc_CapsLock	=  SDLK_CAPSLOCK,
    sc_LShift =	   SDLK_LSHIFT,
    sc_RShift =	   SDLK_RSHIFT,
	sc_UpArrow =   SDLK_UP,
    sc_DownArrow = SDLK_DOWN,
	sc_LeftArrow =	SDLK_LEFT,
    sc_RightArrow =	SDLK_RIGHT,
    sc_Insert =		SDLK_INSERT,
    sc_Delete =		SDLK_DELETE,
    sc_Home	=		SDLK_HOME, 
	sc_End  =			SDLK_END,
    sc_PgUp =			SDLK_PAGEUP,
    sc_PgDn =		SDLK_PAGEDOWN,
    sc_F1	=		SDLK_F1,
    sc_F2	=		SDLK_F2,
    sc_F3	=		SDLK_F3,
    sc_F4	=		SDLK_F4,
    sc_F5	=		SDLK_F5,
    sc_F6	=		SDLK_F6,
	sc_F7	=		SDLK_F7,
	sc_F8	=		SDLK_F8,
	sc_F9	=		SDLK_F9,
	sc_F10	=		SDLK_F10,
	sc_F11	=		SDLK_F11,
	sc_F12	=		SDLK_F12,
	sc_ScrollLock	=	SDLK_SCROLLLOCK,
	sc_PrintScreen	=	SDLK_PRINTSCREEN,
	sc_1	=		SDLK_1,
	sc_2	=		SDLK_2,
	sc_3	=		SDLK_3,
	sc_4	=		SDLK_4,
	sc_5	=		SDLK_5,
	sc_6	=		SDLK_6,
	sc_7	=		SDLK_7,
	sc_8	=		SDLK_8,
	sc_9	=		SDLK_9,
	sc_0	=		SDLK_0,

	sc_A	=		SDLK_a,
	sc_B	=		SDLK_b,
	sc_C	=		SDLK_c,
	sc_D	=		SDLK_d,
	sc_E	=		SDLK_e,
	sc_F	=		SDLK_f,
	sc_G	=		SDLK_g,
	sc_H	=		SDLK_h,
	sc_I	=		SDLK_i,
	sc_J	=		SDLK_j,
	sc_K	=		SDLK_k,
	sc_L	=		SDLK_l,
	sc_M	=		SDLK_m,
	sc_N	=		SDLK_n,
	sc_O	=		SDLK_o,
	sc_P	=		SDLK_p,
	sc_Q	=		SDLK_q,
	sc_R	=		SDLK_r,
	sc_S	=		SDLK_s,
	sc_T	=		SDLK_t,
	sc_U	=		SDLK_u,
	sc_V	=		SDLK_v,
	sc_W	=		SDLK_w,
	sc_X	=		SDLK_x,
	sc_Y	=		SDLK_y,
	sc_Z	=		SDLK_z,
	KEYCOUNT,
	
#if SDL_MAJOR_VERSION == 2
	bt_None = SDL_CONTROLLER_BUTTON_INVALID,
	bt_A = SDL_CONTROLLER_BUTTON_A,
	bt_B = SDL_CONTROLLER_BUTTON_B,
	bt_X = SDL_CONTROLLER_BUTTON_X,
	bt_Y = SDL_CONTROLLER_BUTTON_Y,
	bt_Back = SDL_CONTROLLER_BUTTON_BACK,
	bt_Guide = SDL_CONTROLLER_BUTTON_GUIDE,
	bt_Start = SDL_CONTROLLER_BUTTON_START,
	bt_LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
	bt_RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
	bt_LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
	bt_RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
	bt_DpadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
	bt_DpadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	bt_DpadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	bt_DpadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
	bt_touchpad = SDL_CONTROLLER_BUTTON_TOUCHPAD,
	bt_Max = SDL_CONTROLLER_BUTTON_MAX,

	//Axis stuff
	gc_axis_invalid = SDL_CONTROLLER_AXIS_INVALID,
	gc_axis_leftx = SDL_CONTROLLER_AXIS_LEFTX,
	gc_axis_lefty = SDL_CONTROLLER_AXIS_LEFTY,
	gc_axis_rightx = SDL_CONTROLLER_AXIS_RIGHTX,
	gc_axis_righty = SDL_CONTROLLER_AXIS_RIGHTY,
	gc_trigger_left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
	gc_trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
	gc_axis_max = SDL_CONTROLLER_AXIS_MAX,
#endif
#if SDL_MAJOR_VERSION == 1
	KEYMAX = KEYCOUNT
#elif SDL_MAJOR_VERSION == 2
	GAMECONTROLLER_MAX = bt_Max + gc_axis_max,

	KEYMAX = KEYCOUNT + GAMECONTROLLER_MAX,
#endif
} ScanCode;
#endif
#if SDL_MAJOR_VERSION == 2
extern bool GameControllerButtons[GAMECONTROLLER_MAX];
extern int GameControllerLeftStick[2];
extern int GameControllerRightStick[2];
extern SDL_GameController* GameController;
#endif

typedef	enum
{
	demo_Off,
	demo_Record,
	demo_Playback,
	demo_PlayDone
} Demo;

typedef	enum
{
	ctrl_Keyboard,
	ctrl_Keyboard1 = ctrl_Keyboard,
	ctrl_Keyboard2,
	ctrl_Joystick,
	ctrl_Joystick1 = ctrl_Joystick,
	ctrl_Joystick2,
	ctrl_Mouse
} ControlType;

typedef	enum
{
	motion_Left = -1,
	motion_Up = -1,
	motion_None = 0,
	motion_Right = 1,
	motion_Down = 1
} Motion;

typedef	enum
{
	dir_North,
	dir_NorthEast,
	dir_East,
	dir_SouthEast,
	dir_South,
	dir_SouthWest,
	dir_West,
	dir_NorthWest,
	dir_None,
} Direction;

typedef	struct
{
	bool button0, button1, button2, button3;
	short x, y;
	Motion xaxis, yaxis;
	Direction dir;
} CursorInfo;

typedef	CursorInfo ControlInfo;

typedef	enum
{
	button0, 
	button1,
	upleft, 
	up, 
	upright,
	left, 
	right,
	downleft, 
	down, 
	downright
} KeyboardDef;

//Global variables
extern volatile bool KeyboardState[KEYCOUNT];
extern bool MousePresent;
extern volatile bool Paused;
extern volatile char LastASCII;
extern volatile ScanCode LastScan;
extern int JoyNumButtons;
extern bool forcegrabmouse;

#define	IN_KeyDown(code)	(Keyboard((code)))
#define	IN_ClearKey(code)	{ KeyboardSet(code, false);\
						      if (code == LastScan) LastScan = sc_None;}


bool Keyboard(int key);
void KeyboardSet(int key, bool state);
int KeyboardLookup(int key);


// DEBUG - put names in prototypes
extern	void		IN_Startup(void), IN_Shutdown(void);
extern	void		IN_ClearKeysDown(void);
extern	void		IN_ReadControl(int, ControlInfo*);
extern	void		IN_GetJoyAbs(word joy, word* xp, word* yp);
extern	void		IN_SetupJoy(word joy, word minx, word maxx,
	word miny, word maxy);
extern	void		IN_StopDemo(void), IN_FreeDemoBuffer(void),
IN_Ack(void);
extern	bool		IN_UserInput(longword delay);
extern	char		IN_WaitForASCII(void);
extern	ScanCode	IN_WaitForKey(void);
extern	word		IN_GetJoyButtonsDB(word joy);
extern	const char* IN_GetScanName(ScanCode);

void    IN_WaitAndProcessEvents();
void    IN_ProcessEvents();

int     IN_MouseButtons(void);

bool IN_JoyPresent();
void    IN_SetJoyCurrent(int joyIndex);
int     IN_JoyButtons(void);
void    IN_GetJoyDelta(int* dx, int* dy);
void    IN_GetJoyFineDelta(int* dx, int* dy);

void    IN_StartAck(void);
bool IN_CheckAck(void);
bool    IN_IsInputGrabbed();
void    IN_CenterMouse();

#endif