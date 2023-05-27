//============================================================================
// Name        : Flyffinator.cpp
// Author      : gaviolam
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <windows.h>
#include <winuser.h>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <chrono>
#include <cstdlib>

#define ENABLE_DEBUG 0 /*	0 = disable, 1 = enable	*/
#define DBG_LOG ENABLE_DEBUG
#define NORMAL_LOG 1

#define RED_THRESHOLD		180
#define GREEN_THRESHOLD		20
#define BLUE_THRESHOLD 		50

#define HP_PIXEL_X			193
#define HP_PIXEL_Y			308
#define TOGGLE_ATTACK_X		1440
#define TOGGLE_ATTACK_Y		440
#define MAIN_SCREEN_X		480
#define MAIN_SCREEN_Y		250
#define SUB_SCREEN_X		1440
#define SUB_SCREEN_Y		540
#define FLIGHT_KEY_HEX		0x30
#define ACTION_SLOT_KEY_HEX	0x31
#define BUFF_6_KEY_HEX		0x32
#define BUFF_7_KEY_HEX		0x33
#define BUFF_8_KEY_HEX		0x34
#define BUFF_9_KEY_HEX		0x35
#define BUFF_10_KEY_HEX		0x36
#define PERFECT_BLOCK_KEY	0X35
#define ENHCANT_POISON_KEY	0X36
#define CRITICAL_SWING_KEY	0X37
#define HEAL_KEY_HEX		0x38

/* MACRO functions */
#define myDisplay(x, ...)\
			if(x){				\
				printf(__VA_ARGS__);\
			}
/////////////////////

/* Function prototype */
static void press_key (unsigned char key);
static void alt_tab (void);
static void virtual_press (unsigned int x, unsigned int y);
static void use_pots (int delay);
static void rm_buff (int delay);
static void toggle_attack (void);

using namespace std;
std::mutex m;

int TOGGLE;

int main() {
	myDisplay(NORMAL_LOG, "Press any key to start...\n");
	getch();
	myDisplay(NORMAL_LOG, "Starting script...\n");
	Sleep(3000);

	/* Position cursor to main screen */
	virtual_press(MAIN_SCREEN_X, MAIN_SCREEN_Y);
	Sleep(500);

	TOGGLE = 0;

	/*std::thread th1(use_pots, 400);
	std::thread th2(rm_buff, 540000);
	th1.join();
	th2.join();*/
	use_pots(400);

	return 0;
}

static void press_key (unsigned char key)
{
	INPUT inputs[2] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = key;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = key;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs))
	{
		myDisplay(DBG_LOG, "[%s] failed: 0x%x\n", __func__, (unsigned int)HRESULT_FROM_WIN32(GetLastError()));
	}
	else {
		myDisplay(DBG_LOG, "[%s] Success!!!\n", __func__);
	}
}

static void alt_tab (void)
{
	INPUT inputs[4] = {};
	ZeroMemory(inputs, sizeof(inputs));

	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = VK_MENU;

	inputs[1].type = INPUT_KEYBOARD;
	inputs[1].ki.wVk = VK_TAB;

	inputs[2].type = INPUT_KEYBOARD;
	inputs[2].ki.wVk = VK_TAB;
	inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

	inputs[3].type = INPUT_KEYBOARD;
	inputs[3].ki.wVk = VK_MENU;
	inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

	myDisplay(NORMAL_LOG, "Using Alt Tab...\n");
	Sleep(500);
	UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	if (uSent != ARRAYSIZE(inputs))
	{
		myDisplay(DBG_LOG, "[%s] failed: 0x%x\n", __func__, (unsigned int)HRESULT_FROM_WIN32(GetLastError()));
	}
	else {
		myDisplay(DBG_LOG, "[%s] Success!!!\n", __func__);
	}
	Sleep(500);
}

static void virtual_press (unsigned int x, unsigned int y)
{
	SetCursorPos(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

static void use_pots (int delay)
{
	HDC dng = GetDC(NULL);
	int r, g, b;

	while (1) {
		COLORREF cr = GetPixel(dng, HP_PIXEL_X, HP_PIXEL_Y);

		r = (int)GetRValue(cr);
		g = (int)GetGValue(cr);
		b = (int)GetBValue(cr);

		if (r < RED_THRESHOLD && g < GREEN_THRESHOLD && b < BLUE_THRESHOLD) {
			myDisplay(NORMAL_LOG, "Using heal...\n");
			press_key(HEAL_KEY_HEX);
		}

		Sleep(delay);
	}

	ReleaseDC(NULL, dng);
}

static void rm_buff (int delay)
{
	while(1){
		m.lock();

		alt_tab();
		if (TOGGLE == 1) {
			toggle_attack();
			TOGGLE = !TOGGLE;
		}

		/* Go to main window */
		virtual_press(MAIN_SCREEN_X, MAIN_SCREEN_Y);
		Sleep(8000);

		myDisplay(NORMAL_LOG, "Using RM buffs...\n");

		/* Toggle Flight */
		press_key(FLIGHT_KEY_HEX);
		Sleep(500);
		press_key(FLIGHT_KEY_HEX);
		Sleep(4000);

		/* Go to sub window */
		virtual_press(SUB_SCREEN_X, SUB_SCREEN_Y);

		/* Use Action Slot */
		press_key(ACTION_SLOT_KEY_HEX);
		Sleep(7000);

		/* Buff 6 */
		press_key(BUFF_6_KEY_HEX);
		Sleep(500);
		press_key(BUFF_6_KEY_HEX);
		Sleep(2000);

		/* Buff 7 */
		press_key(BUFF_7_KEY_HEX);
		Sleep(500);
		press_key(BUFF_7_KEY_HEX);
		Sleep(2000);

		/* Buff 8 */
		press_key(BUFF_8_KEY_HEX);
		Sleep(500);
		press_key(BUFF_8_KEY_HEX);
		Sleep(2000);

		/* Buff 9 */
		press_key(BUFF_9_KEY_HEX);
		Sleep(500);
		press_key(BUFF_9_KEY_HEX);
		Sleep(2000);

		/* Buff 10 */
		press_key(BUFF_10_KEY_HEX);
		Sleep(500);
		press_key(BUFF_10_KEY_HEX);
		Sleep(2000);

		/* Go back to main window */
		virtual_press(MAIN_SCREEN_X, MAIN_SCREEN_Y);

		/* Use main buff */
		myDisplay(NORMAL_LOG, "Using perfect block buff...\n");
		press_key(PERFECT_BLOCK_KEY);
		Sleep(500);
		press_key(PERFECT_BLOCK_KEY);
		Sleep(2000);

		myDisplay(NORMAL_LOG, "Using enchant poison buff...\n");
		press_key(ENHCANT_POISON_KEY);
		Sleep(500);
		press_key(ENHCANT_POISON_KEY);
		Sleep(2000);

		myDisplay(NORMAL_LOG, "Using critical swing buff...\n");
		press_key(CRITICAL_SWING_KEY);
		Sleep(500);
		press_key(CRITICAL_SWING_KEY);
		Sleep(2000);

		/* Flight Mode Off */
		press_key(FLIGHT_KEY_HEX);
		press_key(FLIGHT_KEY_HEX);

		toggle_attack();
		TOGGLE = !TOGGLE;
		alt_tab();

		m.unlock();
		Sleep(delay);
	}
}

static void toggle_attack(void)
{
	virtual_press(TOGGLE_ATTACK_X, TOGGLE_ATTACK_Y);
	Sleep(500);
}
