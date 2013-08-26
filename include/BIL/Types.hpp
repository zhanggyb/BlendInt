/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BIL_TYPES_H_
#define _BIL_TYPES_H_

#include <GLFW/glfw3.h>

#include <string>	// typedef String

namespace BIL {

	enum MouseAction {
		MouseNone = -1,
		MousePress = 0,				/* GLFW_RELEASE */
		MouseRelease,			/* GLFW_PRESS */
		MouseMove				/* GLFW_REPEAT */
	};

	enum MouseButton {
		MouseButtonNone = -1,
		MouseButtonLeft = GLFW_MOUSE_BUTTON_1, /* 0 */
		MouseButtonRight = GLFW_MOUSE_BUTTON_2, /* 1 */
		MouseButtonMiddle = GLFW_MOUSE_BUTTON_3, /* 2 */
		MouseButtonScrollUp = GLFW_MOUSE_BUTTON_4, /* 3 */
		MouseButtonScrollDown = GLFW_MOUSE_BUTTON_5 /* 4 */
	};

	enum KeyAction {
		KeyNone = -1,
		KeyPress = 0,			/* GLFW_RELEASE */
		KeyRelease,				/* GLFW_PRESS */
		KeyRepeat				/* GLFW_REPEAT */
	};

	enum KeyModifier {
		ModifierNone = 0x0,					/* 0x0000 */
		ModifierShift = GLFW_MOD_SHIFT, /* 0x0001 */
		ModifierControl = GLFW_MOD_CONTROL, /* 0x0002 */
		ModifierAlt = GLFW_MOD_ALT,		 /* 0x0004 */
		ModifierSuper = GLFW_MOD_SUPER		 /* 0x0008 */
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


	/**
	 * flags to set which corners will become rounded:
	 *
	 * 1------2
	 * |      		|
	 * 8------4
	 */
	enum RoundBoxType
		{
			RoundBoxTopLeft = (1 << 0),
			RoundBoxTopRight = (1 << 1),
			RoundBoxBottomRight = (1 << 2),
			RoundBoxBottomLeft = (1 << 3),
			/* just for convenience */
			RoundBoxNone = 0,
			RoundBoxAll = (RoundBoxTopLeft | RoundBoxTopRight
						   | RoundBoxBottomRight | RoundBoxBottomLeft),
			UI_RB_ALPHA = RoundBoxAll + 1
		};

	enum ScrollState
		{
			SCROLL_PRESSED = (1 << 0),
			SCROLL_ARROW = (1 << 1),
			SCROLL_NO_OUTLINE = (1 << 2)
		};

}

#endif	/* _BIL_TYPES_H_ */
