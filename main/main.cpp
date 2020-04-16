#include <iostream>
#include <limits>
#include <time.h>

#include "..\main\main.h"
#include "..\Snake\Snake.h"
#include "..\Tetris\Tetris.h"
#include <Windows.h>
using namespace std;

string _games::getSelectedGame(_games* games) { // Oyunun adını döndürür.
	return games->selected_game <= 0 ? "" : games->game[games->selected_game-1].name;
}

void _games::openMainManu() { // Ana menüyü başlatır.
	system("Title Oyunlar");

	console::showCursor(true);
	console::changeSize(35, 10);

	while (selected_game < 1 or selected_game > 2) {
		system("cls");
		printf("Lutfen bir oyunu seciniz.\n");
		for (int i = 0; i < 2; i++)
			cout << i + 1 << "-) " << (game[i].name == "Snake" ? "Yilan" : game[i].name) << "	Onceki Skor: " << game[i].last_score << endl;

		cin >> this->selected_game;
		if (cin.fail()) {
			cin.clear(0, true);
			cin.ignore(SHRT_MAX, '\n');
		}
	}
}

_games::_games() {
	game[0].name = "Snake";
	game[1].name = "Tetris";
};

int main()
{
	srand((unsigned int) time(NULL));

	_games *games = new _games;
	while (games->selected_game != -1) {
		console::showCursor(true);
		if (games->getSelectedGame(games) == "") {
			games->openMainManu();
		}
		else if (games->getSelectedGame(games) == "Snake") {
			console::showCursor(false);
			_snake* snake = new _snake(games);
			delete snake;
		}
		else if (games->getSelectedGame(games) == "Tetris") {
			console::showCursor(false);
			_tetris* tetris = new _tetris(games);
			delete tetris;
		}
	};
	delete games;

	return 0;
}
