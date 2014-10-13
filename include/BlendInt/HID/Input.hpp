/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_INPUT_HPP_
#define _BLENDINT_INPUT_HPP_

/*! @name Key and button actions
 *  @{ */
/*! @brief The key or button was released.
 *  @ingroup input
 */
#define GLFW_RELEASE                0
/*! @brief The key or button was pressed.
 *  @ingroup input
 */
#define GLFW_PRESS                  1
/*! @brief The key was held down until it repeated.
 *  @ingroup input
 */
#define GLFW_REPEAT                 2
/*! @} */

/*! @defgroup keys Keyboard keys
 *
 * These key codes are inspired by the *USB HID Usage Tables v1.12* (p. 53-60),
 * but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 * put in the 256+ range).
 *
 * The naming of the key codes follow these rules:
 *  - The US keyboard layout is used
 *  - Names of printable alpha-numeric characters are used (e.g. "A", "R",
 *    "3", etc.)
 *  - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *    "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *    correspond to the Unicode standard (usually for brevity)
 *  - Keys that lack a clear US mapping are named "WORLD_x"
 *  - For non-printable keys, custom names are used (e.g. "F4",
 *    "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

/* The unknown key */
#define GLFW_KEY_UNKNOWN            -1

/* Printable keys */
#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348
#define GLFW_KEY_LAST               GLFW_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 */
#define GLFW_MOD_SHIFT           0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 */
#define GLFW_MOD_CONTROL         0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 */
#define GLFW_MOD_ALT             0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 */
#define GLFW_MOD_SUPER           0x0008

/*! @} */

/*! @defgroup buttons Mouse buttons
 *  @ingroup input
 *  @{ */
#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
 *  @ingroup input
 *  @{ */
#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16
/*! @} */

/*! @defgroup errors Error codes
 *  @ingroup error
 *  @{ */
/*! @brief GLFW has not been initialized.
 */
#define GLFW_NOT_INITIALIZED        0x00010001
/*! @brief No context is current for this thread.
 */
#define GLFW_NO_CURRENT_CONTEXT     0x00010002
/*! @brief One of the enum parameters for the function was given an invalid
 *  enum.
 */
#define GLFW_INVALID_ENUM           0x00010003
/*! @brief One of the parameters for the function was given an invalid value.
 */
#define GLFW_INVALID_VALUE          0x00010004
/*! @brief A memory allocation failed.
 */
#define GLFW_OUT_OF_MEMORY          0x00010005
/*! @brief GLFW could not find support for the requested client API on the
 *  system.
 */
#define GLFW_API_UNAVAILABLE        0x00010006
/*! @brief The requested client API version is not available.
 */
#define GLFW_VERSION_UNAVAILABLE    0x00010007
/*! @brief A platform-specific error occurred that does not match any of the
 *  more specific categories.
 */
#define GLFW_PLATFORM_ERROR         0x00010008
/*! @brief The clipboard did not contain data in the requested format.
 */
#define GLFW_FORMAT_UNAVAILABLE     0x00010009
/*! @} */

#define GLFW_FOCUSED                0x00020001
#define GLFW_ICONIFIED              0x00020002
#define GLFW_RESIZABLE              0x00020003
#define GLFW_VISIBLE                0x00020004
#define GLFW_DECORATED              0x00020005

#define GLFW_RED_BITS               0x00021001
#define GLFW_GREEN_BITS             0x00021002
#define GLFW_BLUE_BITS              0x00021003
#define GLFW_ALPHA_BITS             0x00021004
#define GLFW_DEPTH_BITS             0x00021005
#define GLFW_STENCIL_BITS           0x00021006
#define GLFW_ACCUM_RED_BITS         0x00021007
#define GLFW_ACCUM_GREEN_BITS       0x00021008
#define GLFW_ACCUM_BLUE_BITS        0x00021009
#define GLFW_ACCUM_ALPHA_BITS       0x0002100A
#define GLFW_AUX_BUFFERS            0x0002100B
#define GLFW_STEREO                 0x0002100C
#define GLFW_SAMPLES                0x0002100D
#define GLFW_SRGB_CAPABLE           0x0002100E
#define GLFW_REFRESH_RATE           0x0002100F

#define GLFW_CLIENT_API             0x00022001
#define GLFW_CONTEXT_VERSION_MAJOR  0x00022002
#define GLFW_CONTEXT_VERSION_MINOR  0x00022003
#define GLFW_CONTEXT_REVISION       0x00022004
#define GLFW_CONTEXT_ROBUSTNESS     0x00022005
#define GLFW_OPENGL_FORWARD_COMPAT  0x00022006
#define GLFW_OPENGL_DEBUG_CONTEXT   0x00022007
#define GLFW_OPENGL_PROFILE         0x00022008

#define GLFW_OPENGL_API             0x00030001
#define GLFW_OPENGL_ES_API          0x00030002

#define GLFW_NO_ROBUSTNESS                   0
#define GLFW_NO_RESET_NOTIFICATION  0x00031001
#define GLFW_LOSE_CONTEXT_ON_RESET  0x00031002

#define GLFW_OPENGL_ANY_PROFILE              0
#define GLFW_OPENGL_CORE_PROFILE    0x00032001
#define GLFW_OPENGL_COMPAT_PROFILE  0x00032002

#define GLFW_CURSOR                 0x00033001
#define GLFW_STICKY_KEYS            0x00033002
#define GLFW_STICKY_MOUSE_BUTTONS   0x00033003

#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
#define GLFW_CURSOR_DISABLED        0x00034003

#define GLFW_CONNECTED              0x00040001
#define GLFW_DISCONNECTED           0x00040002


namespace BlendInt {

	enum MouseAction {
		MouseNone = -1,                      //!< MouseNone
		MouseRelease = 0,				/* GLFW_RELEASE *///!< MousePress
		MousePress,			/* GLFW_PRESS */     //!< MouseRelease
		MouseMove				/* GLFW_REPEAT */       //!< MouseMove
	};

	enum MouseButton {
		MouseButtonNone = -1,
		MouseButtonLeft = 0, /* 0 GLFW_MOUSE_BUTTON_1 */
		MouseButtonRight, /* 1 GLFW_MOUSE_BUTTON_2 */
		MouseButtonMiddle, /* 2 GLFW_MOUSE_BUTTON_3 */
		MouseButtonScrollUp, /* 3 GLFW_MOUSE_BUTTON_4 */
		MouseButtonScrollDown/* 4 GLFW_MOUSE_BUTTON_5 */
	};

	enum KeyAction {
		KeyNone = -1,
		KeyPress = 0,			/* GLFW_RELEASE */
		KeyRelease,				/* GLFW_PRESS */
		KeyRepeat				/* GLFW_REPEAT */
	};

	enum KeyModifier {
		ModifierNone = 0x0,					/* 0x0000 */
		ModifierShift = 0x0001, /* GLFW_MOD_SHIFT */
		ModifierControl = 0x0002, /* GLFW_MOD_CONTROL */
		ModifierAlt = 0x0004,		 /* GLFW_MOD_ALT */
		ModifierSuper = 0x0008		 /* GLFW_MOD_SUPER */
	};

	enum Key {
		Key_Unknown = GLFW_KEY_UNKNOWN,  /* -1 */
		Key_Space = GLFW_KEY_SPACE,   /* 32 */
		Key_Apostrophe = GLFW_KEY_APOSTROPHE,   /* 39 ' */
		key_Comma = GLFW_KEY_COMMA,   /* 44 , */
		Key_Minus = GLFW_KEY_MINUS,   /* 45 - */
		Key_Period = GLFW_KEY_PERIOD,   /* 46 . */
		Key_Slash = GLFW_KEY_SLASH,   /* 47 / */
		Key_0 = GLFW_KEY_0,   /* 48 */
		Key_1 = GLFW_KEY_1,   /* 49 */
		Key_2 = GLFW_KEY_2,   /* 50 */
		Key_3 = GLFW_KEY_3,   /* 51 */
		Key_4 = GLFW_KEY_4,   /* 52 */
		Key_5 = GLFW_KEY_5,   /* 53 */
		Key_6 = GLFW_KEY_6,   /* 54 */
		Key_7 = GLFW_KEY_7,   /* 55 */
		Key_8 = GLFW_KEY_8,   /* 56 */
		Key_9 = GLFW_KEY_9,   /* 57 */
		Key_Semicolon = GLFW_KEY_SEMICOLON,   /* 59 ; */
		key_Equal = GLFW_KEY_EQUAL,   /* 61 = */
		Key_A = GLFW_KEY_A,   /* 65 */
		Key_B = GLFW_KEY_B,   /* 66 */
		Key_C = GLFW_KEY_C,   /* 67 */
		Key_D = GLFW_KEY_D,   /* 68 */
		Key_E = GLFW_KEY_E,   /* 69 */
		Key_F = GLFW_KEY_F,   /* 70 */
		Key_G = GLFW_KEY_G,   /* 71 */
		Key_H = GLFW_KEY_H,   /* 72 */
		Key_I = GLFW_KEY_I,   /* 73 */
		Key_J = GLFW_KEY_J,   /* 74 */
		Key_K = GLFW_KEY_K,   /* 75 */
		Key_L = GLFW_KEY_L,   /* 76 */
		Key_M = GLFW_KEY_M,   /* 77 */
		Key_N = GLFW_KEY_N,   /* 78 */
		Key_O = GLFW_KEY_O,   /* 79 */
		Key_P = GLFW_KEY_P,   /* 80 */
		Key_Q = GLFW_KEY_Q,   /* 81 */
		Key_R = GLFW_KEY_R,   /* 82 */
		Key_S = GLFW_KEY_S,   /* 83 */
		Key_T = GLFW_KEY_T,   /* 84 */
		Key_U = GLFW_KEY_U,   /* 85 */
		Key_V = GLFW_KEY_V,   /* 86 */
		Key_W = GLFW_KEY_W,   /* 87 */
		Key_X = GLFW_KEY_X,   /* 88 */
		Key_Y = GLFW_KEY_Y,   /* 89 */
		Key_Z = GLFW_KEY_Z,   /* 90 */
		Key_LeftBracket = GLFW_KEY_LEFT_BRACKET,   /* 91 [ */
		Key_Backslash = GLFW_KEY_BACKSLASH,   /* 92 \ */
		Key_RightBracket = GLFW_KEY_RIGHT_BRACKET,	/* 93  ] */
		Key_GraveAccent = GLFW_KEY_GRAVE_ACCENT,   /* 96 ` */
		Key_World_1 = GLFW_KEY_WORLD_1,   /* 161 non-US #1 */
		Key_World_2 = GLFW_KEY_WORLD_2,   /* 162 non-US #2 */
		Key_Escape = GLFW_KEY_ESCAPE,   /* 256 */
		Key_Enter = GLFW_KEY_ENTER,   /* 257 */
		Key_Tab = GLFW_KEY_TAB,   /* 258 */
		Key_Backspace = GLFW_KEY_BACKSPACE,   /* 259 */
		Key_Insert = GLFW_KEY_INSERT,   /* 260 */
		Key_Delete = GLFW_KEY_DELETE,   /* 261 */
		Key_Right = GLFW_KEY_RIGHT,   /* 262 */
		Key_Left = GLFW_KEY_LEFT,   /* 263 */
		Key_Down = GLFW_KEY_DOWN,   /* 264 */
		Key_Up = GLFW_KEY_UP,   /* 265 */
		Key_PageUp = GLFW_KEY_PAGE_UP,   /* 266 */
		Key_PageDown = GLFW_KEY_PAGE_DOWN,   /* 267 */
		Key_Home = GLFW_KEY_HOME,   /* 268 */
		Key_End = GLFW_KEY_END,   /* 269 */
		Key_CapsLock = GLFW_KEY_CAPS_LOCK,   /* 280 */
		Key_ScrollLock = GLFW_KEY_SCROLL_LOCK,   /* 281 */
		Key_NumLock = GLFW_KEY_NUM_LOCK,   /* 282 */
		Key_PrintScreen = GLFW_KEY_PRINT_SCREEN,   /* 283 */
		Key_Pause = GLFW_KEY_PAUSE,   /* 284 */
		Key_F1 = GLFW_KEY_F1,   /* 290 */
		Key_F2 = GLFW_KEY_F2,   /* 291 */
		Key_F3 = GLFW_KEY_F3,   /* 292 */
		Key_F4 = GLFW_KEY_F4,   /* 293 */
		Key_F5 = GLFW_KEY_F5,   /* 294 */
		Key_F6 = GLFW_KEY_F6,   /* 295 */
		Key_F7 = GLFW_KEY_F7,   /* 296 */
		Key_F8 = GLFW_KEY_F8,   /* 297 */
		Key_F9 = GLFW_KEY_F9,   /* 298 */
		Key_F10 = GLFW_KEY_F10,   /* 299 */
		Key_F11 = GLFW_KEY_F11,   /* 300 */
		Key_F12 = GLFW_KEY_F12,   /* 301 */
		Key_F13 = GLFW_KEY_F13,   /* 302 */
		Key_F14 = GLFW_KEY_F14,   /* 303 */
		Key_F15 = GLFW_KEY_F15,   /* 304 */
		Key_F16 = GLFW_KEY_F16,   /* 305 */
		Key_F17 = GLFW_KEY_F17,   /* 306 */
		Key_F18 = GLFW_KEY_F18,   /* 307 */
		Key_F19 = GLFW_KEY_F19,   /* 308 */
		Key_F20 = GLFW_KEY_F20,   /* 309 */
		Key_F21 = GLFW_KEY_F21,   /* 310 */
		Key_F22 = GLFW_KEY_F22,   /* 311 */
		Key_F23 = GLFW_KEY_F23,   /* 312 */
		Key_F24 = GLFW_KEY_F24,   /* 313 */
		Key_F25 = GLFW_KEY_F25,   /* 314 */
		Key_KP_0 = GLFW_KEY_KP_0,   /* 320 */
		Key_KP_1 = GLFW_KEY_KP_1,   /* 321 */
		Key_KP_2 = GLFW_KEY_KP_2,   /* 322 */
		Key_KP_3 = GLFW_KEY_KP_3,	/* 323 */
		Key_KP_4 = GLFW_KEY_KP_4,   /* 324 */
		Key_KP_5 = GLFW_KEY_KP_5,   /* 325 */
		Key_KP_6 = GLFW_KEY_KP_6,   /* 326 */
		Key_KP_7 = GLFW_KEY_KP_7,   /* 327 */
		Key_KP_8 = GLFW_KEY_KP_8,   /* 328 */
		Key_KP_9 = GLFW_KEY_KP_9,   /* 329 */
		Key_KP_Decimal = GLFW_KEY_KP_DECIMAL,   /* 330 */
		Key_KP_Devide = GLFW_KEY_KP_DIVIDE,   /* 331 */
		Key_KP_Multiply = GLFW_KEY_KP_MULTIPLY,   /* 332 */
		Key_KP_Subtract = GLFW_KEY_KP_SUBTRACT,   /* 333 */
		Key_KP_Add = GLFW_KEY_KP_ADD,   /* 334 */
		Key_KP_Enter = GLFW_KEY_KP_ENTER,   /* 335 */
		Key_KP_Equal = GLFW_KEY_KP_EQUAL,   /* 336 */
		Key_LeftShift = GLFW_KEY_LEFT_SHIFT,   /* 340 */
		Key_LeftControl = GLFW_KEY_LEFT_CONTROL,   /* 341 */
		Key_LeftAlt = GLFW_KEY_LEFT_ALT,   /* 342 */
		Key_LeftSuper = GLFW_KEY_LEFT_SUPER,   /* 343 */
		Key_RightShift = GLFW_KEY_RIGHT_SHIFT,   /* 344 */
		Key_RightControl = GLFW_KEY_RIGHT_CONTROL,   /* 345 */
		Key_RightAlt = GLFW_KEY_RIGHT_ALT,   /* 346 */
		Key_RightSuper = GLFW_KEY_RIGHT_SUPER,   /* 347 */
		Key_Menu = GLFW_KEY_MENU,   /* 348 */
		Key_Last = GLFW_KEY_MENU
	};

	enum InputMode {
		CURSOR = GLFW_CURSOR,
		STICKY_KEYS = GLFW_STICKY_KEYS,
		STICKY_MOUSE_BUTTONS = GLFW_STICKY_MOUSE_BUTTONS
	};

}

#endif /* INPUT_HPP_ */
