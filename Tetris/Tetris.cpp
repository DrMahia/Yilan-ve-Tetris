#include <iostream>
#include <cmath>
#include <conio.h>

#include "..\main\main.h"
#include "..\Tetris\Tetris.h"
using namespace std;

// ARAYÜZ FONKSİYONLARI
void _tetris::update_score() { // Ekrandaki skoru yeniler. 
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 10, 2, to_string(score) + "    ");
}

void _tetris::update_game_state() { // Oyunun durumunu 'Oynanıyor/Durduruldu/Oyun Bitti' olarak günceller.
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 11, 8, game_over ? "OYUN BITTI" : paused ? "DURDURULDU" : "OYNANIYOR ");
}

void _tetris::game_over_menu(bool enable) { // Oyun kaybedildiğinde veya durdurulduğunda ekrana çıkan menüyü oluşturur veya siler.
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	if (enable) {
		console::createRectangle(&console, width + 2, 21, 21, 3);
		console::changeBox(&console, width + 3, 22, "Yeniden oyna (Y)");
		console::changeBox(&console, width + 3, 23, "Ana menuye don (M)");
		console::changeBox(&console, width + 3, 24, "Cik (K)");
		return;
	}

	string space; space.insert(0, 23, ' ');
	for (short int i = 21; i <= 26; i++)
		console::changeBox(&console, width + 2, i, space);
}

// OYUNU SIFIRLAMA
void _tetris::clean_game(_games* games) { // Oyun yeniden başlatıldığında var olan bütün verileri sıfırlar.
	game_over = false;
	game_over_menu(false);

	paused = false;

	games->game[games->selected_game - 1].last_score = score; // Bitirilen oyunun skorunu, önceki skor olarak kaydeder.
	score = 0;

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	console::changeBox(&console, width + 4, 3, "ONCEKI SKOR: ");
	cout << games->game[games->selected_game - 1].last_score << (games->game[games->selected_game - 1].last_score < 1000 ? "   " : "  ");

	string space; space.insert(0, width, ' ');
	for (short int i = 1; i <= height; i++)
		console::changeBox(&console, 1, i, space); // Oyun alanındaki karakterlerin hepsini siler.

	memset(map, false, sizeof map); // 'map' isimli dizinin bütün verilerini 'false' yapar.

	update_game_state();
	update_score();

	create_block();
}


// POZİSYON KONTROL FONKSİYONLARI
void _tetris::calculate_position() { // Bloğun başlangıç ve bitiş pozisyonlarını hesaplar.
	block.start_x = 1000, block.start_y = 1000, block.end_x = 0, block.end_y = 0; // fmax ve fmin fonksiyonlarının doğru çalışması için başlangıç değerlerini uç değerler yapar.

	for (short int y = 0; y <= 3; y++)
		for (short int x = 0; x <= 3; x++)
			if (blocks[block.type][block.rotation][y][x]) {
				block.start_x = (short int) fmin(x, block.start_x);
				block.end_x = (short int) fmax(x, block.end_x);

				block.start_y = (short int) fmin(y, block.start_y);
				block.end_y = (short int) fmax(y, block.end_y);
			}

	block.start_x += block.x, block.start_y += block.y, block.end_x += block.x, block.end_y += block.y;
}

bool _tetris::is_there_block(string pos) { // Bloğun sağında, solunda, altında ve bulunduğu yerde daha önceden yerleştirilmiş olan blok var mı yok mu onu kontrol eder.
	if (pos == "bottom") {
		for (short int x = 0; x <= 3; x++)
			for (short int y = 0; y <= 3; y++)
				if (block.y + y >= 0 and blocks[block.type][block.rotation][y][x] and map[block.x + x][block.y + y + 1])
					return true;
	}
	else if (pos == "current") {
		for (short int x = 0; x <= 3; x++)
			for (short int y = 0; y <= 3; y++)
				if (block.y + y >= 0 and blocks[block.type][block.rotation][y][x] and map[block.x + x][block.y + y])
					return true;
	}
	else
		for (short int x = 0; x <= 3; x++)
			for (short int y = 0; y <= 3; y++)
				if (block.y + y > 0
					and block.x + x > 0
					and block.x + x < width
					and blocks[block.type][block.rotation][y][x]
					and map[(pos == "left" ? block.x + x - 1 : block.x + x + 1)][block.y + y])
					return true;
	return false;
}

// DOLU SATIR KONTROL FONKSİYONLARI
void _tetris::delete_row(short int row) { // Parametre olarak verilen satırı komple siler ve diğer satırları 1 sıra aşağıya indirir.
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	for (short int x = 0; x < width; x++) {
		map[x][row] = false;
		console::changeBox(&console, x, row, ' ', true); // Dolu olan satırı siler.
	}

	for (short int y = row; y >= 0; y--) // Silinen satırın üstündeki satırları 1 sıra aşağıya indirir.
		for (short int x = 0; x < width; x++)
			if (y == 0) {
				if (map[x][y]) {
					map[x][y] = false;
					console::changeBox(&console, x, y, ' ', true);
				}
			}
			else {
				bool last_value = map[x][y];
				map[x][y] = map[x][y - 1];
				
				if (last_value and !map[x][y])
					console::changeBox(&console, x, y, ' ', true);
				else if (!last_value and map[x][y])
					console::changeBox(&console, x, y, '#', true);
			}

	score += width;
	update_score();
}

void _tetris::check_rows() { // Blok yerleştirildikten sonra bütün satırları kontrol eder. Eğer ki satır tamamen dolu ise 'delete_row' fonksiyonunu çağırır.
	for (short int y = height - 1; y >= 0; y--) {
		bool is_filled = true;

		for (short int x = 0; x < width; x++)
			if (!map[x][y])
				is_filled = false;

		if (is_filled) { // Satır tamamen dolmuş ise satır silme fonksiyonunu çağırır.
			delete_row(y);
			y++;
		}
	}
}

// OYNANIŞ FONKSİYONLARI
void _tetris::create_block() { // Yeni blok oluşturmayı sağlar.
	check_rows(); // Bütün satırları kontrol eder. Eğer ki dolu satır varsa orayı siler, diğer satırları kaydırır.

	block.type = rand() % 5; // 5 blok tipinden birisini rastgele seçer.
	block.rotation = rand() % 4; // Seçilen bloğun 4 yönünden birisini rastgele seçer.
	block.old_rotation = block.rotation;

	block.y = 10;

	for (short int x = 0; x <= 3; x++)
		for (short int y = 0; y <= 3; y++)
			if (blocks[block.type][block.rotation][y][x]) {
				block.x = (short int)floor((width - x) / 2); // Bloğun, oyun alanının tam ortasında oluşmasını sağlar.
				goto end;
			}
	end:;

	calculate_position(); // Yeni bloğun bütün pozisyonlarını hesaplar.
	block.y = block.start_y - block.end_y - 1; // Bloğu oyunun en üstüne taşır.
}

void _tetris::move_block(_games* games) { // Bloğun sağa, sola, aşağıya hareket ettirilmesini ve bloğun yönünün değiştirilmesini sağlar.
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'd':
			direction = right;
			break;
		case 's':
			direction = down;
			break;
		case 'a':
			direction = left;
			break;
		case 'q':
			block.old_rotation = block.rotation; // Eğer ki döndürme işlemi için pozisyon uygun değilse döndürmeyi engellemek için kullanılmaktadır.
			block.rotation = (block.rotation + 1) % 4;
			break;
		case 'e':
			block.old_rotation = block.rotation;
			block.rotation = (block.rotation - 1) < 0 ? 3 : (block.rotation - 1);
			break;
		case 'p':
			if (!game_over) {
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
				games->game[games->selected_game - 1].last_score = score;
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
			break;
		}
	}
	else direction = nil;
	
	if (paused)
		return; // Oyun durdurulmuş ise kod buradan aşağısına geçemez.

	bool is_new = false; // Blok yerleştirildikten sonra son hareket hamlesinin yapılabilmesi için kullanılmaktadır.
	if (block.end_y == height - 1 and GetTickCount() - tick > update_ms) { // Blok oyun alanının tabanına ulaşmış ise...
		confirm_block:;
		for (short int x = 0; x <= 3; x++)
			for (short int y = 0; y <= 3; y++)
				if (blocks[block.type][block.rotation][y][x]) {
					if (block.y + y < 0) {
						game_over = true;
						return;
					}
					else
						map[block.x + x][block.y + y] = true;
				}

		create_block();
		calculate_position();
		tick = GetTickCount();
		is_new = true;
	}
	else if (is_there_block("bottom") and GetTickCount() - tick > update_ms) // Blokğun hemen altında daha önceden yerleştirilmiş başka bir blok var ise...
		goto confirm_block;

	calculate_position();
		

	if (block.old_rotation != block.rotation) {
		if (block.end_x > width - 1 or block.start_x < 0 or block.end_y > height - 1 or is_there_block("current")) // Eğer ki bloğu döndürmek için pozisyon uygun değilse
			block.rotation = block.old_rotation; // Yönü eski haline getirir.

	}

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	short int px = direction == right ? ( block.end_x < width - 1 ? 1 : 0) : direction == left ? (block.start_x > 0 ? -1 : 0) : 0;
	short int py = is_there_block("bottom") ? 0 : block.end_y == height - 1 ? 0 : direction == down ? 1 : GetTickCount() - tick > update_ms ? 1 : 0;

	if (px != 0)
		if (is_there_block(px == 1 ? "right" : "left")) // Bloğun hareket yönü müsait değilse
			px = 0; // Hareketi engeller.

	if (py == 1) tick = GetTickCount();
	else if (px == 0 and py == 0 and block.rotation == block.old_rotation and !is_new) return;

	
	for (short int x = 0; x <= 3; x++) { // Bloğun hareketini ekrana yazdırır.
		for (short int y = 0; y <= 3; y++) {

			if (block.y + y + py >= 0 and blocks[block.type][block.rotation][y][x])
				console::changeBox(&console, block.x + x + px, block.y + y + py, '#', true);

			if (block.y + y >= 0 and (x - px >= 0 or x - px <= 3)
				and (y - py >= 0 or y - py <= 3)
				and blocks[block.type][block.old_rotation][y][x]
				and !blocks[block.type][block.rotation][y - py][x - px]

				or (block.y + y >= 0) and 
					(
						   (x - px < 0) and blocks[block.type][block.old_rotation][y][0]
						or (x - px > 3) and blocks[block.type][block.old_rotation][y][3]
						or (y - py < 0) and blocks[block.type][block.old_rotation][0][x]
						or (y - py > 3) and blocks[block.type][block.old_rotation][3][x]
					)
				)
				console::changeBox(&console, block.x + x, block.y + y, ' ', true);
		}
	}

	block.old_rotation = block.rotation;
	
	block.x += px; block.y += py;
}


_tetris::_tetris(_games* games) { // Tetris oyununu başlatan fonksiyondur.
	system("Title Tetris");

	console::changeSize(width + 25, height + 2);

	////// Tetris Arayüzü //////
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	// Oyun Ekranı
	console::createRectangle(&console, 0, 0, width, height);

	// Skor
	console::createRectangle(&console, width + 2, 0, 21, 4);
	console::changeBox(&console, width + 4, 2, "SKOR: 0");
	console::changeBox(&console, width + 4, 3, "ONCEKI SKOR: "); cout << games->game[games->selected_game - 1].last_score;

	// Oyun Durumu
	console::createRectangle(&console, width + 2, 6, 21, 3);
	console::changeBox(&console, width + 4, 8, "DURUM: ");

	// Oynanış Bilgisi
	console::createRectangle(&console, width + 2, 11, 21, 8);

	console::changeBox(&console, width + 3, 12, "ASD tuslari blogu");
	console::changeBox(&console, width + 3, 13, "hareket ettirir");

	console::changeBox(&console, width + 3, 15, "QE tuslari blogu");
	console::changeBox(&console, width + 3, 16, "dondurur");

	console::changeBox(&console, width + 3, 18, "Durdurmak/Devam");
	console::changeBox(&console, width + 3, 19, "etmek icin P'ye basin");

	create_block();

	while (games->getSelectedGame(games) == "Tetris") {
		while (play_again)
		{
			paused = false;

			update_game_state();
			update_score();

			while (!game_over)
			{
				move_block(games);
				Sleep(20);
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
					games->game[games->selected_game - 1].last_score = score;
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
