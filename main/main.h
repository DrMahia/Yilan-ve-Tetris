#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

// Her bir kutucuk
struct _box {
	short int x = 0;
	short int y = 0;
	bool special = false;
};


// Ana Menü
struct _game {
	string name;
	short int last_score = 0;
};

class _games {
public:
	_game game[2];
	short int selected_game = 0;

	static string getSelectedGame(_games*);

	void openMainManu();


	_games();
};

// Konsolu Değiştirmek İçin Kullanılan Fonksiyonlar
class console {
public:
	static void showCursor(bool enable) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.bVisible = enable;
		info.dwSize = enable ? 0 : 100;
		SetConsoleCursorInfo(console, &info);
	};

	static void changeSize(short width, short height) {
		char text[50];
		snprintf(text, 50, "mode CON: COLS=%d LINES=%d", width, height);
		system(text);

		RECT rectWindow;
		HWND console = GetConsoleWindow();
		GetWindowRect(console, &rectWindow);
		auto x = (GetSystemMetrics(SM_CXSCREEN) - (rectWindow.right - rectWindow.left))/2;
		auto y = (GetSystemMetrics(SM_CYSCREEN) - (rectWindow.bottom - rectWindow.top))/2;

		MoveWindow(console, x, y, 1000, 1000, true);
	};

	static void changeBox(_box* box, unsigned char character) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD mouse_coord = { box->x, box->y };
		SetConsoleCursorPosition(console, mouse_coord);
		cout << character;
	};
	static void changeBox(_box* box, unsigned char character, bool in_game) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD mouse_coord = { box->x + 1, box->y + 1 };
		SetConsoleCursorPosition(console, mouse_coord);
		cout << character;
	};
	static void changeBox(HANDLE* console, short int x, short int y, unsigned char character) {
		COORD mouse_coord = { x, y };
		SetConsoleCursorPosition(*console, mouse_coord);
		cout << character;
	};
	static void changeBox(HANDLE* console, short int x, short int y, unsigned char character, bool in_game) {
		COORD mouse_coord = { x + 1, y + 1 };
		SetConsoleCursorPosition(*console, mouse_coord);
		cout << character;
	};
	static void changeBox(HANDLE* console, short int x, short int y, string str) {
		COORD mouse_coord = { x, y };
		SetConsoleCursorPosition(*console, mouse_coord);
		cout << str;
	};

	static void createRectangle(HANDLE *console, short int x, short int y, short int w, short int h) {
		changeBox(console, x, y, 218);
		changeBox(console, x + w + 1, y, 191);
		changeBox(console, x + w + 1, y + h + 1, 217);
		changeBox(console, x, y + h + 1, 192);

		for (short int i = x + 1; i < x + w + 1; i++) {
			changeBox(console, i, y, 196);
			changeBox(console, i, y + h + 1, 196);
		}

		for (short int i = y + 1; i < y + h + 1; i++) {
			changeBox(console, x, i, 179);
			changeBox(console, x + w + 1, i, 179);
		}
	};

	static void setCursorPosition(short int x, short int y) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		COORD mouse_coord = { x, y };
		SetConsoleCursorPosition(console, mouse_coord);
	};
};
