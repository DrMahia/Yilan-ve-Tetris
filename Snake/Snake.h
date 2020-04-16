#pragma once
class _snake {
public:
// TEMEL DE�ERLER
	static const unsigned short int width = 50, height = 30; // Oynanabilir olan alan�n de�erleri
	static const unsigned short int area = width * height; // Haritadaki toplam kare say�s�

	bool map[width][height]{ false }; // Yeni yem koyarken, yemin olaca�� yerde y�lan�n olup olmad���n�n kontrol� bu dizi sayesinde kontrol edilir.
	_box tail[area]; // Y�lan�n b�t�n bilgileri bu struct i�erisindedir.
	_box food; // Yemin b�t�n bilgileri bu struct i�erisindedir.

	unsigned short int size = 0; // Y�lan�n b�y�kl���d�r.

// OYUN ��� DE�ERLER
	bool speed = false;
	short int move_state = 0;

	bool play_again = true;
	bool game_over = false;
	bool paused = false;

	string game_state;

// FONKS�YONLAR
	void create_food(); // Yeni yem olu�turur.
	void move_snake(_games*); // Y�lan�n b�t�n hareketleri bu fonksiyon i�erisinde ger�ekle�ir.
	
	void game_over_menu(bool); // Oyun kaybedildi�inde veya durduruldu�unda ekrana ��kan men�y� olu�turur veya siler.
	void clean_game(_games*); // Oyun yeniden ba�lat�ld���nda var olan b�t�n verileri s�f�rlar.
	
	void update_score(); // Ekrandaki skoru yeniler.
	void update_game_state(); // Oyunun durumunu 'Oynan�yor/Durduruldu/Oyun Bitti' olarak g�nceller.

	_snake(_games*); // Tetris oyununu ba�latan fonksiyondur.
};
