#pragma once

#include <iostream>

namespace Luha
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h

		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

}

#define LH_KEY_SPACE           ::Luha::Key::Space
#define LH_KEY_APOSTROPHE      ::Luha::Key::Apostrophe    /* ' */
#define LH_KEY_COMMA           ::Luha::Key::Comma         /* , */
#define LH_KEY_MINUS           ::Luha::Key::Minus         /* - */
#define LH_KEY_PERIOD          ::Luha::Key::Period        /* . */
#define LH_KEY_SLASH           ::Luha::Key::Slash         /* / */
#define LH_KEY_0               ::Luha::Key::D0
#define LH_KEY_1               ::Luha::Key::D1
#define LH_KEY_2               ::Luha::Key::D2
#define LH_KEY_3               ::Luha::Key::D3
#define LH_KEY_4               ::Luha::Key::D4
#define LH_KEY_5               ::Luha::Key::D5
#define LH_KEY_6               ::Luha::Key::D6
#define LH_KEY_7               ::Luha::Key::D7
#define LH_KEY_8               ::Luha::Key::D8
#define LH_KEY_9               ::Luha::Key::D9
#define LH_KEY_SEMICOLON       ::Luha::Key::Semicolon     /* ; */
#define LH_KEY_EQUAL           ::Luha::Key::Equal         /* = */
#define LH_KEY_A               ::Luha::Key::A
#define LH_KEY_B               ::Luha::Key::B
#define LH_KEY_C               ::Luha::Key::C
#define LH_KEY_D               ::Luha::Key::D
#define LH_KEY_E               ::Luha::Key::E
#define LH_KEY_F               ::Luha::Key::F
#define LH_KEY_G               ::Luha::Key::G
#define LH_KEY_H               ::Luha::Key::H
#define LH_KEY_I               ::Luha::Key::I
#define LH_KEY_J               ::Luha::Key::J
#define LH_KEY_K               ::Luha::Key::K
#define LH_KEY_L               ::Luha::Key::L
#define LH_KEY_M               ::Luha::Key::M
#define LH_KEY_N               ::Luha::Key::N
#define LH_KEY_O               ::Luha::Key::O
#define LH_KEY_P               ::Luha::Key::P
#define LH_KEY_Q               ::Luha::Key::Q
#define LH_KEY_R               ::Luha::Key::R
#define LH_KEY_S               ::Luha::Key::S
#define LH_KEY_T               ::Luha::Key::T
#define LH_KEY_U               ::Luha::Key::U
#define LH_KEY_V               ::Luha::Key::V
#define LH_KEY_W               ::Luha::Key::W
#define LH_KEY_X               ::Luha::Key::X
#define LH_KEY_Y               ::Luha::Key::Y
#define LH_KEY_Z               ::Luha::Key::Z
#define LH_KEY_LEFT_BRACKET    ::Luha::Key::LeftBracket   /* [ */
#define LH_KEY_BACKSLASH       ::Luha::Key::Backslash     /* \ */
#define LH_KEY_RIGHT_BRACKET   ::Luha::Key::RightBracket  /* ] */
#define LH_KEY_GRAVE_ACCENT    ::Luha::Key::GraveAccent   /* ` */
#define LH_KEY_WORLD_1         ::Luha::Key::World1        /* non-US #1 */
#define LH_KEY_WORLD_2         ::Luha::Key::World2        /* non-US #2 */

/* Function keys */
#define LH_KEY_ESCAPE          ::Luha::Key::Escape
#define LH_KEY_ENTER           ::Luha::Key::Enter
#define LH_KEY_TAB             ::Luha::Key::Tab
#define LH_KEY_BACKSPACE       ::Luha::Key::Backspace
#define LH_KEY_INSERT          ::Luha::Key::Insert
#define LH_KEY_DELETE          ::Luha::Key::Delete
#define LH_KEY_RIGHT           ::Luha::Key::Right
#define LH_KEY_LEFT            ::Luha::Key::Left
#define LH_KEY_DOWN            ::Luha::Key::Down
#define LH_KEY_UP              ::Luha::Key::Up
#define LH_KEY_PAGE_UP         ::Luha::Key::PageUp
#define LH_KEY_PAGE_DOWN       ::Luha::Key::PageDown
#define LH_KEY_HOME            ::Luha::Key::Home
#define LH_KEY_END             ::Luha::Key::End
#define LH_KEY_CAPS_LOCK       ::Luha::Key::CapsLock
#define LH_KEY_SCROLL_LOCK     ::Luha::Key::ScrollLock
#define LH_KEY_NUM_LOCK        ::Luha::Key::NumLock
#define LH_KEY_PRINT_SCREEN    ::Luha::Key::PrintScreen
#define LH_KEY_PAUSE           ::Luha::Key::Pause
#define LH_KEY_F1              ::Luha::Key::F1
#define LH_KEY_F2              ::Luha::Key::F2
#define LH_KEY_F3              ::Luha::Key::F3
#define LH_KEY_F4              ::Luha::Key::F4
#define LH_KEY_F5              ::Luha::Key::F5
#define LH_KEY_F6              ::Luha::Key::F6
#define LH_KEY_F7              ::Luha::Key::F7
#define LH_KEY_F8              ::Luha::Key::F8
#define LH_KEY_F9              ::Luha::Key::F9
#define LH_KEY_F10             ::Luha::Key::F10
#define LH_KEY_F11             ::Luha::Key::F11
#define LH_KEY_F12             ::Luha::Key::F12
#define LH_KEY_F13             ::Luha::Key::F13
#define LH_KEY_F14             ::Luha::Key::F14
#define LH_KEY_F15             ::Luha::Key::F15
#define LH_KEY_F16             ::Luha::Key::F16
#define LH_KEY_F17             ::Luha::Key::F17
#define LH_KEY_F18             ::Luha::Key::F18
#define LH_KEY_F19             ::Luha::Key::F19
#define LH_KEY_F20             ::Luha::Key::F20
#define LH_KEY_F21             ::Luha::Key::F21
#define LH_KEY_F22             ::Luha::Key::F22
#define LH_KEY_F23             ::Luha::Key::F23
#define LH_KEY_F24             ::Luha::Key::F24
#define LH_KEY_F25             ::Luha::Key::F25

/* Keypad */
#define LH_KEY_KP_0            ::Luha::Key::KP0
#define LH_KEY_KP_1            ::Luha::Key::KP1
#define LH_KEY_KP_2            ::Luha::Key::KP2
#define LH_KEY_KP_3            ::Luha::Key::KP3
#define LH_KEY_KP_4            ::Luha::Key::KP4
#define LH_KEY_KP_5            ::Luha::Key::KP5
#define LH_KEY_KP_6            ::Luha::Key::KP6
#define LH_KEY_KP_7            ::Luha::Key::KP7
#define LH_KEY_KP_8            ::Luha::Key::KP8
#define LH_KEY_KP_9            ::Luha::Key::KP9
#define LH_KEY_KP_DECIMAL      ::Luha::Key::KPDecimal
#define LH_KEY_KP_DIVIDE       ::Luha::Key::KPDivide
#define LH_KEY_KP_MULTIPLY     ::Luha::Key::KPMultiply
#define LH_KEY_KP_SUBTRACT     ::Luha::Key::KPSubtract
#define LH_KEY_KP_ADD          ::Luha::Key::KPAdd
#define LH_KEY_KP_ENTER        ::Luha::Key::KPEnter
#define LH_KEY_KP_EQUAL        ::Luha::Key::KPEqual

#define LH_KEY_LEFT_SHIFT      ::Luha::Key::LeftShift
#define LH_KEY_LEFT_CONTROL    ::Luha::Key::LeftControl
#define LH_KEY_LEFT_ALT        ::Luha::Key::LeftAlt
#define LH_KEY_LEFT_SUPER      ::Luha::Key::LeftSuper
#define LH_KEY_RIGHT_SHIFT     ::Luha::Key::RightShift
#define LH_KEY_RIGHT_CONTROL   ::Luha::Key::RightControl
#define LH_KEY_RIGHT_ALT       ::Luha::Key::RightAlt
#define LH_KEY_RIGHT_SUPER     ::Luha::Key::RightSuper
#define LH_KEY_MENU            ::Luha::Key::Menu

// Mouse
#define LH_MOUSE_BUTTON_0      ::Luha::Mouse::Button0
#define LH_MOUSE_BUTTON_1      ::Luha::Mouse::Button1
#define LH_MOUSE_BUTTON_2      ::Luha::Mouse::Button2
#define LH_MOUSE_BUTTON_3      ::Luha::Mouse::Button3
#define LH_MOUSE_BUTTON_4      ::Luha::Mouse::Button4
#define LH_MOUSE_BUTTON_5      ::Luha::Mouse::Button5
#define LH_MOUSE_BUTTON_6      ::Luha::Mouse::Button6
#define LH_MOUSE_BUTTON_7      ::Luha::Mouse::Button7
#define LH_MOUSE_BUTTON_LAST   ::Luha::Mouse::ButtonLast
#define LH_MOUSE_BUTTON_LEFT   ::Luha::Mouse::ButtonLeft
#define LH_MOUSE_BUTTON_RIGHT  ::Luha::Mouse::ButtonRight
#define LH_MOUSE_BUTTON_MIDDLE ::Luha::Mouse::ButtonMiddle