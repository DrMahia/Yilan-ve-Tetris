#pragma once
#include <Windows.h>

class _tetris {
public:
// TEMEL DE�ERLER
	static const unsigned short int width = 10, height = 26; // Oynanabilir olan alan�n de�erleri

	bool map[width][height]{ false }; // Bloklar�n hareket kabiliyeti gittikten sonra buradaki diziye lokasyonlar� belirtilerek veri eklenir.

	const bool blocks[5][4][4][4] = { // Blok tiplerini ve bloklar�n y�nlerini i�eren bir dizidir.
		{// T
			{ // Y�n 1
				{ 0, 0, 0, 1 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 0, 1 },
				{ 0, 0, 0, 0 },
			},
			{ // Y�n 2
				{ 0, 1, 1, 1 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			},
			{ // Y�n 3
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{ // Y�n 4
				{ 0, 0, 1, 0 },
				{ 0, 1, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			}
		},
		{ // I
			{
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
			},
			{
				{ 0, 0, 0, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
			},
			{
				{ 0, 0, 0, 0 },
				{ 1, 1, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			}
		},
		{ // S
			{
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 0, 1 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 1, 1, 0 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 0, 1 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 1, 1 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			}
		},
		{ // L
			{
				{ 0, 1, 0, 0 },
				{ 0, 1, 0, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 0, 1 },
				{ 0, 1, 1, 1 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 1, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 1, 1, 1 },
				{ 0, 1, 0, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			}
		},
		{ // Kare
			{
				{ 0, 0, 0, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 0, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 0, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{
				{ 0, 0, 0, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 1, 1, 0 },
				{ 0, 0, 0, 0 },
			}
		}
	};

	enum _direction { left, right, down, nil } direction; // Bas�lan tu�lara g�re 'direction' de�eri 'left', 'right' veya 'down' olabilir. Herhangi bir tu�a bas�lmam��sa 'direction' de�eri 'nil' yap�l�r.

// S�RE �LE �LG�L� DE�ERLER
	DWORD tick = GetTickCount(); // Sistemin ba�lat�lmas�ndan bu yana ge�en s�renin milisaniye cinsinden de�erini verir.
	DWORD update_ms = 300; // Bloklar�n otomatik olarak a�a��ya inmesi i�in ge�mesi gereken s�renin milisaniye cinsinden de�eridir.

// OYUN ��� DE�ERLER
	bool play_again = true;
	bool game_over = false;
	bool paused = false;

	string game_state;

	short int score = 0;

	struct _block { // Yukar�dan a�a��ya d��en blo�un de�erlerini i�eren struct yap�s�d�r.
		short int type = 0;

		short int rotation = 0;
		short int old_rotation = 0;

		short int x = 0, y = 0;

		short int start_x = 0, end_x = 0, start_y = 0, end_y = 0;
	} block;

// FONKS�YONLAR
	void create_block(); // Yeni blok olu�turmay� sa�lar.
	void move_block(_games*); // Blo�un sa�a, sola, a�a��ya hareket ettirilmesini ve blo�un y�n�n�n de�i�tirilmesini sa�lar.
	void calculate_position(); // Blo�un ba�lang�� ve biti� pozisyonlar�n� hesaplar.
	bool is_there_block(string); // Blo�un sa��nda, solunda, alt�nda ve bulundu�u yerde daha �nceden yerle�tirilmi� olan blok var m� yok mu onu kontrol eder.

	void check_rows(); // Blok yerle�tirildikten sonra b�t�n sat�rlar� kontrol eder. E�er ki sat�r tamamen dolu ise 'delete_row' fonksiyonunu �a��r�r.
	void delete_row(short int); // Parametre olarak verilen sat�r� komple siler ve di�er sat�rlar� 1 s�ra a�a��ya indirir.

	void game_over_menu(bool); // Oyun kaybedildi�inde veya durduruldu�unda ekrana ��kan men�y� olu�turur veya siler.
	void clean_game(_games*); // Oyun yeniden ba�lat�ld���nda var olan b�t�n verileri s�f�rlar.

	void update_score(); // Ekrandaki skoru yeniler.
	void update_game_state(); // Oyunun durumunu 'Oynan�yor/Durduruldu/Oyun Bitti' olarak g�nceller.


	_tetris(_games*); // Tetris oyununu ba�latan fonksiyondur.
};