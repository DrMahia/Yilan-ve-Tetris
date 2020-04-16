#pragma once
class _snake {
public:
// TEMEL DEÐERLER
	static const unsigned short int width = 50, height = 30; // Oynanabilir olan alanýn deðerleri
	static const unsigned short int area = width * height; // Haritadaki toplam kare sayýsý

	bool map[width][height]{ false }; // Yeni yem koyarken, yemin olacaðý yerde yýlanýn olup olmadýðýnýn kontrolü bu dizi sayesinde kontrol edilir.
	_box tail[area]; // Yýlanýn bütün bilgileri bu struct içerisindedir.
	_box food; // Yemin bütün bilgileri bu struct içerisindedir.

	unsigned short int size = 0; // Yýlanýn büyüklüðüdür.

// OYUN ÝÇÝ DEÐERLER
	bool speed = false;
	short int move_state = 0;

	bool play_again = true;
	bool game_over = false;
	bool paused = false;

	string game_state;

// FONKSÝYONLAR
	void create_food(); // Yeni yem oluþturur.
	void move_snake(_games*); // Yýlanýn bütün hareketleri bu fonksiyon içerisinde gerçekleþir.
	
	void game_over_menu(bool); // Oyun kaybedildiðinde veya durdurulduðunda ekrana çýkan menüyü oluþturur veya siler.
	void clean_game(_games*); // Oyun yeniden baþlatýldýðýnda var olan bütün verileri sýfýrlar.
	
	void update_score(); // Ekrandaki skoru yeniler.
	void update_game_state(); // Oyunun durumunu 'Oynanýyor/Durduruldu/Oyun Bitti' olarak günceller.

	_snake(_games*); // Tetris oyununu baþlatan fonksiyondur.
};
