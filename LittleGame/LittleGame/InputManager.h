#pragma once

#include "Singleton.h"

#include <dinput.h>
#include <d3dx9math.h>
#include <functional>

#ifdef LE_PLATFORM_WINDOWS

typedef BYTE KeyCode;

// keycode defines

/****************************************************************************
*
*      Key codes
*
****************************************************************************/
#define LEMB_0 0x000000
#define LEMB_1 0x000001
#define LEMB_2 0x000002
#define LEMB_3 0x000003
#define LEMB_4 0x000004
#define LEMB_5 0x000005
#define LEMB_6 0x000006
#define LEMB_7 0x000007
#define LEMB_8 LEMB_0
#define LEMB_9 LEMB_0
#define LEMB_10 LEMB_0
#define LEMB_11 LEMB_0
#define LEMB_12 LEMB_0
#define LEMB_13 LEMB_0
#define LEMB_14 LEMB_0
#define LEMB_15 LEMB_0

#define LEK_ESCAPE DIK_ESCAPE          
#define LEK_1 DIK_1               
#define LEK_2 DIK_2               
#define LEK_3 DIK_3               
#define LEK_4 DIK_4               
#define LEK_5 DIK_5               
#define LEK_6 DIK_6               
#define LEK_7 DIK_7               
#define LEK_8 DIK_8               
#define LEK_9 DIK_9               
#define LEK_0 DIK_0               
#define LEK_MINUS DIK_MINUS               /* - on main keyboard */
#define LEK_EQUALS DIK_EQUALS          
#define LEK_BACK DIK_BACK                /* backspace */
#define LEK_TAB DIK_TAB             
#define LEK_Q DIK_Q               
#define LEK_W DIK_W               
#define LEK_E DIK_E               
#define LEK_R DIK_R               
#define LEK_T DIK_T               
#define LEK_Y DIK_Y               
#define LEK_U DIK_U               
#define LEK_I DIK_I               
#define LEK_O DIK_O               
#define LEK_P DIK_P               
#define LEK_LBRACKET DIK_LBRACKET        
#define LEK_RBRACKET DIK_RBRACKET        
#define LEK_RETURN DIK_RETURN              /* Enter on main keyboard */
#define LEK_CONTROL DIK_LCONTROL        
#define LEK_A DIK_A               
#define LEK_S DIK_S               
#define LEK_D DIK_D               
#define LEK_F DIK_F               
#define LEK_G DIK_G               
#define LEK_H DIK_H               
#define LEK_J DIK_J               
#define LEK_K DIK_K               
#define LEK_L DIK_L               
#define LEK_SEMICOLON DIK_SEMICOLON       
#define LEK_APOSTROPHE DIK_APOSTROPHE      
#define LEK_GRAVE DIK_GRAVE               /* accent grave */
#define LEK_LSHIFT DIK_LSHIFT          
#define LEK_BACKSLASH DIK_BACKSLASH
#define LEK_Z DIK_Z               
#define LEK_X DIK_X               
#define LEK_C DIK_C               
#define LEK_V DIK_V               
#define LEK_B DIK_B               
#define LEK_N DIK_N               
#define LEK_M DIK_M               
#define LEK_COMMA DIK_COMMA           
#define LEK_PERIOD DIK_PERIOD              /* . on main keyboard */
#define LEK_SLASH DIK_SLASH               /* / on main keyboard */
#define LEK_RSHIFT DIK_RSHIFT          
#define LEK_MULTIPLY DIK_MULTIPLY            /* * on numeric keypad */
#define LEK_LMENU DIK_LMENU               /* left Alt */
#define LEK_SPACE DIK_SPACE           
#define LEK_CAPITAL DIK_CAPITAL         
#define LEK_F1 DIK_F1              
#define LEK_F2 DIK_F2              
#define LEK_F3 DIK_F3              
#define LEK_F4 DIK_F4              
#define LEK_F5 DIK_F5              
#define LEK_F6 DIK_F6              
#define LEK_F7 DIK_F7              
#define LEK_F8 DIK_F8              
#define LEK_F9 DIK_F9              
#define LEK_F10 DIK_F10             
#define LEK_NUMLOCK DIK_NUMLOCK         
#define LEK_SCROLL DIK_SCROLL              /* Scroll Lock */
#define LEK_NUMPAD7 DIK_NUMPAD7         
#define LEK_NUMPAD8 DIK_NUMPAD8         
#define LEK_NUMPAD9 DIK_NUMPAD9         
#define LEK_SUBTRACT DIK_SUBTRACT            /* - on numeric keypad */
#define LEK_NUMPAD4 DIK_NUMPAD4         
#define LEK_NUMPAD5 DIK_NUMPAD5         
#define LEK_NUMPAD6 DIK_NUMPAD6         
#define LEK_ADD DIK_ADD                 /* + on numeric keypad */
#define LEK_NUMPAD1 DIK_NUMPAD1         
#define LEK_NUMPAD2 DIK_NUMPAD2         
#define LEK_NUMPAD3 DIK_NUMPAD3         
#define LEK_NUMPAD0 DIK_NUMPAD0         
#define LEK_DECIMAL DIK_DECIMAL             /* . on numeric keypad */
#define LEK_OEM_102 DIK_OEM_102             /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define LEK_F11 DIK_F11             
#define LEK_F12 DIK_F12             
#define LEK_F13 DIK_F13                 /*                     (NEC PC98) */
#define LEK_F14 DIK_F14                 /*                     (NEC PC98) */
#define LEK_F15 DIK_F15                 /*                     (NEC PC98) */
#define LEK_KANA DIK_KANA                /* (Japanese keyboard)            */
#define LEK_ABNT_C1 DIK_ABNT_C1             /* /? on Brazilian keyboard */
#define LEK_CONVERT DIK_CONVERT             /* (Japanese keyboard)            */
#define LEK_NOCONVERT DIK_NOCONVERT           /* (Japanese keyboard)            */
#define LEK_YEN DIK_YEN                 /* (Japanese keyboard)            */
#define LEK_ABNT_C2 DIK_ABNT_C2             /* Numpad . on Brazilian keyboard */
#define LEK_NUMPADEQUALS DIK_NUMPADEQUALS        /* = on numeric keypad (NEC PC98) */
#define LEK_PREVTRACK DIK_PREVTRACK           /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define LEK_AT DIK_AT                  /*                     (NEC PC98) */
#define LEK_COLON DIK_COLON               /*                     (NEC PC98) */
#define LEK_UNDERLINE DIK_UNDERLINE           /*                     (NEC PC98) */
#define LEK_KANJI DIK_KANJI               /* (Japanese keyboard)            */
#define LEK_STOP DIK_STOP                /*                     (NEC PC98) */
#define LEK_AX DIK_AX                  /*                     (Japan AX) */
#define LEK_UNLABELED DIK_UNLABELED           /*                        (J3100) */
#define LEK_NEXTTRACK DIK_NEXTTRACK           /* Next Track */
#define LEK_NUMPADENTER DIK_NUMPADENTER         /* Enter on numeric keypad */
#define LEK_RCONTROL DIK_RCONTROL        
#define LEK_MUTE DIK_MUTE                /* Mute */
#define LEK_CALCULATOR DIK_CALCULATOR          /* Calculator */
#define LEK_PLAYPAUSE DIK_PLAYPAUSE           /* Play / Pause */
#define LEK_MEDIASTOP DIK_MEDIASTOP           /* Media Stop */
#define LEK_VOLUMEDOWN DIK_VOLUMEDOWN          /* Volume - */
#define LEK_VOLUMEUP DIK_VOLUMEUP            /* Volume + */
#define LEK_WEBHOME DIK_WEBHOME             /* Web home */
#define LEK_NUMPADCOMMA DIK_NUMPADCOMMA         /* , on numeric keypad (NEC PC98) */
#define LEK_DIVIDE DIK_DIVIDE              /* / on numeric keypad */
#define LEK_SYSRQ DIK_SYSRQ           
#define LEK_RMENU DIK_RMENU               /* right Alt */
#define LEK_PAUSE DIK_PAUSE               /* Pause */
#define LEK_HOME DIK_HOME                /* Home on arrow keypad */
#define LEK_UP DIK_UP                  /* UpArrow on arrow keypad */
#define LEK_PRIOR DIK_PRIOR               /* PgUp on arrow keypad */
#define LEK_LEFT DIK_LEFT                /* LeftArrow on arrow keypad */
#define LEK_RIGHT DIK_RIGHT               /* RightArrow on arrow keypad */
#define LEK_END DIK_END                 /* End on arrow keypad */
#define LEK_DOWN DIK_DOWN                /* DownArrow on arrow keypad */
#define LEK_NEXT DIK_NEXT                /* PgDn on arrow keypad */
#define LEK_INSERT DIK_INSERT              /* Insert on arrow keypad */
#define LEK_DELETE DIK_DELETE              /* Delete on arrow keypad */
#define LEK_LWIN DIK_LWIN                /* Left Windows key */
#define LEK_RWIN DIK_RWIN                /* Right Windows key */
#define LEK_APPS DIK_APPS                /* AppMenu key */
#define LEK_POWER DIK_POWER               /* System Power */
#define LEK_SLEEP DIK_SLEEP               /* System Sleep */
#define LEK_WAKE DIK_WAKE                /* System Wake */
#define LEK_WEBSEARCH DIK_WEBSEARCH           /* Web Search */
#define LEK_WEBFAVORITES DIK_WEBFAVORITES        /* Web Favorites */
#define LEK_WEBREFRESH DIK_WEBREFRESH          /* Web Refresh */
#define LEK_WEBSTOP DIK_WEBSTOP             /* Web Stop */
#define LEK_WEBFORWARD DIK_WEBFORWARD          /* Web Forward */
#define LEK_WEBBACK DIK_WEBBACK             /* Web Back */
#define LEK_MYCOMPUTER DIK_MYCOMPUTER          /* My Computer */
#define LEK_MAIL DIK_MAIL                /* Mail */
#define LEK_MEDIASELECT DIK_MEDIASELECT         /* Media Select */

#endif

struct MouseData
{
	bool buttonState[16];
	float x, y;
	int scroll;
};

class InputManager :
	public Singleton<InputManager>
{
	friend class Singleton<InputManager>;
protected:

	BYTE m_keyStates[256];
	BYTE m_prevKeyStates[256];
	MouseData m_mouseState;
	MouseData m_prevMouseState;
	
	IDirectInput8* m_directInput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;
	DIMOUSESTATE2 m_diMouseState;

	InputManager();

public:

	~InputManager();

	virtual void Initialize();
	virtual void Shutdown();
	virtual void Update();

	virtual bool GetKeyDown(KeyCode kc);
	virtual bool GetKey(KeyCode kc);
	virtual bool GetKeyUp(KeyCode kc);

	virtual bool GetMouseButton(KeyCode kc);
	virtual bool GetMouseButtonDown(KeyCode kc);
	virtual bool GetMouseButtonUp(KeyCode kc);
	virtual void GetMousePosition(D3DXVECTOR2* vec);
	virtual void GetMouseDirection(D3DXVECTOR2* vec);
	virtual int GetMouseScrollAbsolute();
	virtual int GetMouseScrollRelative();

	virtual void GetTouchPosition(D3DXVECTOR2* vec);
	virtual void GetTouchDirection(D3DXVECTOR2* vec);
	virtual bool GetTouchDown(KeyCode kc);
	virtual bool GetTouch(KeyCode kc);
	virtual bool GetTouchUp(KeyCode kc);
	virtual bool GetIsDoubleTouch();
	virtual bool GetIsPinch();
};

