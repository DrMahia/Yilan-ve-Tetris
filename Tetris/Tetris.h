#pragma once
#include <Windows.h>

class _tetris {
public:
// TEMEL DEÐERLER
	static const unsigned short int width = 10, height = 26; // Oynanabilir olan alanýn deðerleri

	bool map[width][height]{ false }; // Bloklarýn hareket kabiliyeti gittikten sonra buradaki diziye lokasyonlarý belirtilerek veri eklenir.

	const bool blocks[5][4][4][4] = { // Blok tiplerini ve bloklarýn yönlerini içeren bir dizidir.
		{// T
			{ // Yön 1
				{ 0, 0, 0, 1 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 0, 1 },
				{ 0, 0, 0, 0 },
			},
			{ // Yön 2
				{ 0, 1, 1, 1 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
				{ 0, 0, 0, 0 },
			},
			{ // Yön 3
				{ 0, 0, 1, 0 },
				{ 0, 0, 1, 1 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 0 },
			},
			{ // Yön 4
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

	enum _direction { left, right, down, nil } direction; // Basýlan tuþlara göre 'direction' deðeri 'left', 'right' veya 'down' olabilir. Herhangi bir tuþa basýlmamýþsa 'direction' deðeri 'nil' yapýlýr.

// SÜRE ÝLE ÝLGÝLÝ DEÐERLER
	DWORD tick = GetTickCount(); // Sistemin baþlatýlmasýndan bu yana geçen sürenin milisaniye cinsinden deðerini verir.
	DWORD update_ms = 300; // Bloklarýn otomatik olarak aþaðýya inmesi için geçmesi gereken sürenin milisaniye cinsinden deðeridir.

// OYUN ÝÇÝ DEÐERLER
	bool play_again = true;
	bool game_over = false;
	bool paused = false;

	string game_state;

	short int score = 0;

	struct _block { // Yukarýdan aþaðýya düþen bloðun deðerlerini içeren struct yapýsýdýr.
		short int type = 0;

		short int rotation = 0;
		short int old_rotation = 0;

		short int x = 0, y = 0;

		short int start_x = 0, end_x = 0, start_y = 0, end_y = 0;
	} block;

// FONKSÝYONLAR
	void create_block(); // Yeni blok oluþturmayý saðlar.
	void move_block(_games*); // Bloðun saða, sola, aþaðýya hareket ettirilmesini ve bloðun yönünün deðiþtirilmesini saðlar.
	void calculate_position(); // Bloðun baþlangýç ve bitiþ pozisyonlarýný hesaplar.
	bool is_there_block(string); // Bloðun saðýnda, solunda, altýnda ve bulunduðu yerde daha önceden yerleþtirilmiþ olan blok var mý yok mu onu kontrol eder.

	void check_rows(); // Blok yerleþtirildikten sonra bütün satýrlarý kontrol eder. Eðer ki satýr tamamen dolu ise 'delete_row' fonksiyonunu çaðýrýr.
	void delete_row(short int); // Parametre olarak verilen satýrý komple siler ve diðer satýrlarý 1 sýra aþaðýya indirir.

	void game_over_menu(bool); // Oyun kaybedildiðinde veya durdurulduðunda ekrana çýkan menüyü oluþturur veya siler.
	void clean_game(_games*); // Oyun yeniden baþlatýldýðýnda var olan bütün verileri sýfýrlar.

	void update_score(); // Ekrandaki skoru yeniler.
	void update_game_state(); // Oyunun durumunu 'Oynanýyor/Durduruldu/Oyun Bitti' olarak günceller.


	_tetris(_games*); // Tetris oyununu baþlatan fonksiyondur.
};