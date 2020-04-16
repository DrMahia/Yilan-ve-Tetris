#include <iostream>
#include <string>
#include <conio.h>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include "..\main\main.h"
#include "..\Snake\Snake.h"
using namespace std;

// ARAYÜZ FONKSİYONLARI
void _snake::update_score() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 10, 2, to_string((size-1)*10)+"    ");
}

void _snake::update_game_state() {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 11, 8, game_over ? "OYUN BITTI" : paused ? "DURDURULDU" : "OYNANIYOR ");
}

void _snake::game_over_menu(bool enable) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (enable) {
		console::createRectangle(&console, width + 2, 21, 22, 3);
		console::changeBox(&console, width + 3, 22, "Yeniden oyna (Y)");
		console::changeBox(&console, width + 3, 23, "Ana menuye don (M)");
		console::changeBox(&console, width + 3, 24, "Cik (K)");
		return;
	}

	string space; space.insert(0, 24, ' ');
	for(short int i = 21; i <= 26; i++)
		console::changeBox(&console, width + 2, i, space);
}

// OYUNU SIFIRLAMA
void _snake::clean_game(_games* games) {
	game_over = false;
	game_over_menu(false);

	paused = false;

	games->game[games->selected_game - 1].last_score = (size - 1) * 10;

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 4, 3, "ONCEKI SKOR: ");
	cout << games->game[games->selected_game - 1].last_score << (games->game[games->selected_game - 1].last_score < 1000 ? "   " : "  ");

	string space; space.insert(0, width, ' ');
	for(short int i = 1; i <= height; i++)
		console::changeBox(&console, 1, i, space);

	for (short int i = 0; i < size; i++)
		map[tail[i].x][tail[i].y] = false;

	tail[0] = { width / 2, height / 2 };
	map[width / 2][height / 2] = true;
	size = 1;

	update_game_state();
	update_score();
}

// OYNANIŞ FONKSİYONLARI
void _snake::create_food() { // Yeni yem oluşturur.
	short int random = rand() % (area - size) + 1; // Haritada boşta kalan kare sayısını tespit edip arasından rastgele birini seçer.

	short int count = 0;
	for (short int i = 0; i < area; i++) {
		short int x = i % width; int y = (int)floor(i / width);
		if (!map[x][y])
			count++;

		if (count == random) { // Elde edilen rastgele sayıya 30 diyelim. Haritada 30'uncu boş alanı bulur ve oraya yem koyar.
			food.x = x; food.y = y;
			food.special = size != 1 ? (rand()%3 == 1 ? true : false) : false; // 3'te 1 oranla özel yem, 3'te 2 oranla normal yem koyar.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			console::changeBox(&food, food.special ? '@' : 254, true);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		}
	}
}

void _snake::move_snake(_games* games) { // Yılanın bütün hareketleri bu fonksiyon içerisinde gerçekleşir.
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'w':
			if (move_state == 1) speed = true;
			else speed = false;

			if (move_state != 3 or size == 1)
				move_state = 1;
			break;
		case 'd':
			if (move_state == 2) speed = true;
			else speed = false;

			if (move_state != 4 or size == 1)
				move_state = 2;
			break;
		case 's':
			if (move_state == 3) speed = true;
			else speed = false;

			if (move_state != 1 or size == 1)
				move_state = 3;
			break;
		case 'a':
			if (move_state == 4) speed = true;
			else speed = false;

			if (move_state != 2 or size == 1)
				move_state = 4;
			break;
		case 'p':
			if (!game_over) {
				speed = false;
				paused = !paused;
				if (paused) game_over_menu(true);
				else game_over_menu(false);
				update_game_state();
				break;
			}
		case 'y':
			if (paused) {
				game_over = true;
				break;
			}
		case 'm':
			if (paused) {
				game_over = true;
				play_again = false;
				games->game[games->selected_game - 1].last_score = (size - 1) * 10;
				games->selected_game = 0;
				break;
			}
		case 'k':
			if (paused) {
				game_over = true;
				play_again = false;
				games->selected_game = -1;
				break;
			}
		default:
			speed = false;
			break;
		}
	}
	else speed = false;

	if (paused)
		return; // Oyun durdurulmuş ise kod buradan aşağısına geçemez.

	short int end_of_tail = 0, last_food = 0;
	for (short int segment = size - 1; segment >= 0; segment--) // Kuyruğun sonunu arar.
	{
		if (tail[segment].special == false) {
			end_of_tail = segment; // Yenilen yemler kuyruğun sonuna ulaşmadan sisteme eklendiği için doğrudan hangi kısmın kuyruğun sonu olduğu anlaşılamaz. Bu nedenle böyle bir aramaya ihtiyaç duyulur.
			break;
		} else last_food = segment; // Kuyruğa en yakın olan yem bulunur.
	}

	if (last_food == 0 or tail[end_of_tail].x != tail[last_food].x or tail[end_of_tail].y != tail[last_food].y) {
		if (tail[end_of_tail].x != food.x or tail[end_of_tail].y != food.y) // Yeni yem kuyruğun sonunda değil ise kuyruğun sonunu günceller ve eski konumu siler.
			console::changeBox(&tail[end_of_tail], ' ', true);
		else { // Yeni yem kuyruğun sonunda ise kuyruğun eski konumunu yeme dönüştürür.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			console::changeBox(&tail[end_of_tail], food.special ? '@' : 254, true);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		map[tail[end_of_tail].x][tail[end_of_tail].y] = false;
	}
	
	short int x, y;
	if (move_state == 2) x = 1; else if (move_state == 4) x = -1; else x = 0;
	if (move_state == 1) y = -1; else if (move_state == 3) y = 1; else y = 0;
	
	// Yılanın hareketini ekrana yazdırır.
	if (size > 1) {
		for (short int segment = size - 1; segment >= 0; segment--)
		{
			if (tail[segment].special and tail[segment].x == tail[end_of_tail].x and tail[segment].y == tail[end_of_tail].y) 
			{
				tail[segment].special = false;
				console::changeBox(&tail[segment], 'o', true);
				map[tail[segment].x][tail[segment].y] = true;
			}
			else if (!tail[segment].special) {
				tail[segment].x = tail[segment - 1].x;
				tail[segment].y = tail[segment - 1].y;
			}
		}

		tail[0].x = tail[1].x + x;
		tail[0].y = tail[1].y + y;

		bool is_here_special = false;
		for (short int segment = size - 1; segment >= 0; segment--)
		{
			if (tail[segment].special and tail[segment].x == tail[1].x and tail[segment].y == tail[1].y) {
				is_here_special = true;
				break;
			}
			else break;
		}
		if (!is_here_special) {
			console::changeBox(&tail[1], 'o', true);
			map[tail[1].x][tail[1].y] = true;
		}
		else {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			console::changeBox(&tail[1], 'O', true);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
	}
	else {
		tail[0].x = tail[0].x + x;
		tail[0].y = tail[0].y + y;
	}

	if (tail[0].x > width - 1 or tail[0].x < 0 or tail[0].y > height - 1 or tail[0].y < 0) // Yılan haritanın dışına çıkmışsa oyunu sonlandırır.
	{
		game_over = true;
		return;
	}

	console::changeBox(&tail[0], 'O', true);
	map[tail[0].x][tail[0].y] = true;

	for (short int segment = 1; segment < size - 1; segment++)
	{
		if (tail[0].x == tail[segment].x and tail[0].y == tail[segment].y) { // Yılan kendini yediyse oyunu sonlandırır.
			game_over = true;
			return;
		}
	}

	if (tail[0].x == food.x and tail[0].y == food.y) // Yılanın kafası yeme ulaştıysa yemi yedirir.
	{
		if (food.special and size != 1) {
			size++;
			tail[size - 1].x = tail[1].x;
			tail[size - 1].y = tail[1].y;
			tail[size - 1].special = true;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
			console::changeBox(&tail[1], 'O', true);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}

		size++;
		tail[size - 1].x = tail[0].x;
		tail[size - 1].y = tail[0].y;
		tail[size - 1].special = true;

		create_food(); // Yeni yem oluşturulur.
		update_score();
	}
}

_snake::_snake(_games* games) { // Yılan oyununu başlatan fonksiyondur.
	system("Title Yilan");

	console::changeSize(width + 26, height+2);

	////// Yılan Arayüzü //////
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	// Oyun Ekranı
	console::createRectangle(&console, 0, 0, width, height);

	// Skor
	console::createRectangle(&console, width + 2, 0, 22, 4);
	console::changeBox(&console, width + 4, 2, "SKOR: ");
	console::changeBox(&console, width + 4, 3, "ONCEKI SKOR: "); cout << games->game[games->selected_game - 1].last_score;

	// Oyun Durumu
	console::createRectangle(&console, width + 2, 6, 22, 3);
	console::changeBox(&console, width + 4, 8, "DURUM: ");

	// Oynanış Bilgisi
	console::createRectangle(&console, width + 2, 11, 22, 8);

	console::changeBox(&console, width + 3, 12, "WASD tuslari yilani");
	console::changeBox(&console, width + 3, 13, "hareket ettirir");

	console::changeBox(&console, width + 3, 15, "Hizli gitmek icin WASD");
	console::changeBox(&console, width + 3, 16, "tuslarina basili tutun");

	console::changeBox(&console, width + 3, 18, "Durdurmak/Devam");
	console::changeBox(&console, width + 3, 19, "etmek icin P'ye basin");

	while (games->getSelectedGame(games) == "Snake") {
		while (play_again)
		{
			paused = false;
			move_state = rand() % 4 + 1;

			tail[0] = { width/2, height/2 };
			map[width/2][height/2] = true;
			size = 1;

			update_game_state();
			update_score();

			create_food();

			while (!game_over)
			{
				move_snake(games);
				Sleep(speed ? 30 : 100);
			}

			game_over_menu(true);
			update_game_state();

			while (game_over and play_again)
			{
				switch (paused ? 'y' : tolower(_getch()))
				{
				case 'y':
					clean_game(games);
					break;
				case 'm':
					play_again = false;
					games->game[games->selected_game - 1].last_score = (size - 1) * 10;
					games->selected_game = 0;
					break;
				case 'k':
					play_again = false;
					games->selected_game = -1;
					break;
				}
			}
		}
	}
}
