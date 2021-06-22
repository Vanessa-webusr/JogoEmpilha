// ------- ARQUIVO OBJETOS ------
enum TIPO { PLAYER, CAFE, BIGORNA, VASO, PEIXE };

struct Pinguim {
	int ID;
	int x;
	int y;
	int velocidade;
	int borda_x;
	int borda_y;
};

struct Saca {
	int ID;
	int x;
	int y;
	int velocidade_x;
	int velocidade_y;
	int borda_x;
	int borda_y;
	float cafe_sx = 7, cafe_sy = 9, cafe_sw = 172, cafe_sh = 100;
	bool ativo;
	bool colide;
};

struct Bigorna {
	int ID;
	int x;
	int y;
	int velocidade_x;
	int velocidade_y;
	int borda_x;
	int borda_y;
	bool ativo;
	bool colide;
};

struct Vaso {
	int ID;
	int x;
	int y;
	int velocidade_x;
	int velocidade_y;
	int borda_x;
	int borda_y;
	bool ativo;
	bool colide;
};

struct Peixe {
	int ID;
	int x;
	int y;
	int velocidade_x;
	int velocidade_y;
	int borda_x;
	int borda_y;
	bool ativo;
	bool colide;
};
