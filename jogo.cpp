#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <ctime>
#include "objetos.h"
#include "Pilha.h"
#include "PilhaInter.h"

enum TECLAS { DIREITA, ESQUERDA, ESPAÇO };

// ---------- VARIÁVEIS GLOBAIS ---------
const int width_t = 1860;
const int height_t = 976;
const int FPS = 60;
const int NUM_SACAS = 2;
const int NUM_BIGORNAS = 1;

// ---------- GAME STATES --------
const int STATE_MENU = 0;
const int STATE_GAME = 1;
const int STATE_OVER = 2;

float s_x = 17, s_y = 27, s_w = 234, s_h = 279;

bool morte = false;

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* player = NULL;
ALLEGRO_BITMAP* cafe = NULL;
ALLEGRO_BITMAP* bigorna = NULL;
ALLEGRO_BITMAP* def = NULL;
ALLEGRO_BITMAP* fim_de_jogo = NULL;
ALLEGRO_BITMAP* game_menu = NULL;


#define BACKGROUND_FILE "fundo_neve_caminhao.png"
#define PLAYER_FILE "pinguim_sprite_v2.png"
#define CAFE_FILE "coffee_sprite_v1.png"
#define BIGORNA_FILE "anvil_v1.png"
#define FIM_DE_JOGO_FILE "game_over_v5.png"
#define GAME_MENU_FILE "game_menu_v3.png"



// --------- PROTÓTIPOS --------------------
void InitPinguim(Pinguim& p1);
void DesenhaPinguim(Pinguim& p1);
void MovePinguimEsquerda(Pinguim& p1);
void MovePinguimDireita(Pinguim& p1);

void InitSaca(Saca sacas[], int tamanho);
void LiberaSaca(Saca sacas[], int tamanho);
void AtualizarSaca(Saca sacas[], int tamanho);
void DesenhaSaca(Saca sacas[], int tamanho);
void ColideSacaPinguim(Saca sacas[], int s_tamanho, Pinguim p1, Pilha* pilha);
void DesenhaCrashedSaca(Saca sacas[], int s_tamanho);

void InitBigorna(Bigorna bigornas[], int tamanho);
void LiberaBigorna(Bigorna bigornas[], int tamanho);
void AtualizarBigorna(Bigorna bigornas[], int tamanho);
void DesenhaBigorna(Bigorna bigornas[], int tamanho);
void ColideBigornaPinguim(Bigorna bigornas[], int s_tamanho, Pinguim p1, Pilha* pilha);


void Atualizarpilha(Pilha* pilha);
void DesenhapilhaDef(Pilha* pilha);


int main() {


	// ------- VARIÁVEIS DO JOGO ------------
	ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
	ALLEGRO_TIMER* timer = NULL;


	// ------- VARIÁVEL FIM DO PROGRAMA PRINCIPAL ------
	int gamestate = STATE_MENU;
	bool fim = false;
	bool desenha = true;

	bool teclas[] = { false, false, false };


	// ------- INICIALIZAÇÃO DE OBJETOS -----------
	Pinguim p1;
	Saca sacas[NUM_SACAS];
	Bigorna bigornas[NUM_BIGORNAS];
	PilhaInter* pilhaInter = new PilhaInter(6);
	Pilha* pilhaDef = new Pilha(25);
	int aux = 0;
	bool ok = false;


	// ------- INICIALIZAÇÃO DA ALLEGRO E DO DISPLAY -------
	if (!al_init())
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "ALLEGRO NÃO PODE SER INICIALIZADA", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	display = al_create_display(width_t, height_t);

	if (!display)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O DISPLAY NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DE ADDONS E INSTALAÇÕES -------
	al_install_keyboard();
	al_init_image_addon();

	// ------- INICIALIZAÇÃO DO FUNDO -------
	background = al_load_bitmap(BACKGROUND_FILE);

	if (!background)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O FUNDO NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DO PLAYER -------

	player = al_load_bitmap(PLAYER_FILE);

	if (!player)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O PINGUIM NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DA SACA DE CAFE -------

	cafe = al_load_bitmap(CAFE_FILE);

	if (!cafe)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "A SACA NÃO PODE SER CRIADA", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DA BIGORNA -------

	bigorna = al_load_bitmap(BIGORNA_FILE);

	if (!bigorna)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "A BIGORNA NÃO PODE SER CRIADA", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}


	// ------- INICIALIZAÇÃO DA PILHA DEF -------

	def = al_load_bitmap(CAFE_FILE);

	if (!def)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "A PILHA DEF NÃO PODE SER CRIADA", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DO FIM DE JOGO -------

	fim_de_jogo = al_load_bitmap(FIM_DE_JOGO_FILE);

	if (!fim_de_jogo)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O FIM DE JOGO NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- INICIALIZAÇÃO DO GAME MENU -------

	game_menu = al_load_bitmap(GAME_MENU_FILE);

	if (!game_menu)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O GAME MENU NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// ------- CRIAÇÃO DA FILA E DEMAIS DISPOSITIVOS -------
	fila_eventos = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	// ------- REGISTRO DE SOURCES -------
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_display_event_source(display));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

	// ------- FUNÇÕES INICIAIS ---------
	srand(time(NULL));

	InitPinguim(p1);
	InitSaca(sacas, NUM_SACAS);
	InitBigorna(bigornas, NUM_BIGORNAS);
	

	// ------- LOOP PRINCIPAL -------
	al_start_timer(timer);

	while (!fim)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(fila_eventos, &ev);

		// ------- EVENTOS E LÓGICA DO JOGO -------

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			fim = true;
		}

		switch (gamestate) {
		case STATE_MENU:
			teclas[DIREITA] = false;
			teclas[ESQUERDA] = false;
			teclas[ESPAÇO] = false;

			al_draw_bitmap(game_menu, 0, 0, NULL);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					fim = true;
					break;
				case ALLEGRO_KEY_SPACE:
					gamestate = STATE_GAME;
					break;
				}
			}
			break;
		case STATE_GAME:

			if (morte) {
				gamestate = STATE_OVER;
			}

			else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			{

				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_ESCAPE:
					fim = true;
					break;
				case ALLEGRO_KEY_RIGHT:
					teclas[DIREITA] = true;
					break;
				case ALLEGRO_KEY_LEFT:
					teclas[ESQUERDA] = true;
					break;
				case ALLEGRO_KEY_SPACE:
					teclas[ESPAÇO] = true;
					break;
				}

			}

			else if (ev.type == ALLEGRO_EVENT_KEY_UP)
			{
				switch (ev.keyboard.keycode)
				{
				case ALLEGRO_KEY_RIGHT:
					teclas[DIREITA] = false;
					break;
				case ALLEGRO_KEY_LEFT:
					teclas[ESQUERDA] = false;
					break;
				case ALLEGRO_KEY_SPACE:
					teclas[ESPAÇO] = false;
					break;
				}

			}

			else if (ev.type == ALLEGRO_EVENT_TIMER)
			{
				desenha = true;

				if (teclas[DIREITA]) {
					MovePinguimDireita(p1);
				}

				if (teclas[ESQUERDA]) {
					MovePinguimEsquerda(p1);
				}

				if (teclas[ESPAÇO]) {
					if (p1.x < 300 && p1.x >= 0) {
						pilhaInter->Desempilha(aux, ok);
						if (aux != 0) {
							pilhaDef->Empilha(aux, ok);
							aux = 0;
						}
						Atualizarpilha(pilhaInter);
					}
				}


				LiberaSaca(sacas, NUM_SACAS);
				AtualizarSaca(sacas, NUM_SACAS);
				ColideSacaPinguim(sacas, NUM_SACAS, p1, pilhaInter);
				LiberaBigorna(bigornas, NUM_BIGORNAS);
				AtualizarBigorna(bigornas, NUM_BIGORNAS);
				ColideBigornaPinguim(bigornas, NUM_BIGORNAS, p1, pilhaInter);

			}


			// ------- DESENHO -------

			if (desenha && al_is_event_queue_empty(fila_eventos))
			{

				desenha = false;

				al_draw_bitmap(background, 0, 0, NULL);
				DesenhapilhaDef(pilhaDef);
				DesenhaPinguim(p1);
				DesenhaSaca(sacas, NUM_SACAS);
				DesenhaCrashedSaca(sacas, NUM_SACAS);
				DesenhaBigorna(bigornas, NUM_BIGORNAS);

				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
			break;
		case STATE_OVER:

			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				fim = true;
				break;
			case ALLEGRO_KEY_SPACE:
				morte = false;	
				bool ok1 = true;
				bool ok2 = true;
				while (ok1) {
					pilhaInter->Desempilha(aux, ok1);
				}
				while (ok2) {
					pilhaDef->Desempilha(aux, ok2);
				}
				Atualizarpilha(pilhaInter);
				gamestate = STATE_MENU;
				break;
			}

			al_draw_bitmap(background, 0, 0, NULL);
			DesenhaPinguim(p1);
			DesenhapilhaDef(pilhaDef);
			al_draw_bitmap(fim_de_jogo, 720, 59, NULL);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			break;
		}
	}



	// ------- FINALIZAÇÕES DO PROGRAMA -------
	al_destroy_display(display);
	al_destroy_bitmap(background);
	al_destroy_bitmap(player);
	al_destroy_bitmap(cafe);
	al_destroy_bitmap(def);
	al_destroy_event_queue(fila_eventos);
	al_destroy_timer(timer);

	return 0;
}

// ------------ FUNÇÕES E PROCEDIMENTOS SISTEMA -----------------

// ------- PINGUIM ----------

void InitPinguim(Pinguim& p1)
{
	p1.ID = PLAYER;
	p1.x = width_t / 2;
	p1.y = height_t / 2;
	p1.velocidade = 15;
	p1.borda_x = 218;
	p1.borda_y = 240;
}

void DesenhaPinguim(Pinguim& p1) {

	al_draw_bitmap_region(player, s_x, s_y, s_w, s_h, p1.x, p1.y, NULL);

}

void MovePinguimEsquerda(Pinguim& p1) {
	if (morte == false) {
		p1.x -= p1.velocidade;

		if (p1.x < 0)
			p1.x = 0;
	}
}

void MovePinguimDireita(Pinguim& p1) {
	if (morte == false) {
		p1.x += p1.velocidade;

		if (p1.x > 1700)
			p1.x = 1700;
	}
}

// -------- SACA DE CAFÉ ---------

void InitSaca(Saca sacas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		sacas[i].ID = CAFE;
		sacas[i].velocidade_x = rand() % 7 + 2;
		sacas[i].velocidade_y = 3;
		sacas[i].borda_x = 165;
		sacas[i].borda_y = 80;
		sacas[i].ativo = false;
		sacas[i].colide = false;
	}
}

void LiberaSaca(Saca sacas[], int tamanho) {
	if (morte == false) {
		for (int i = 0; i < tamanho; i++) {
			if (!sacas[i].ativo) {
				if (rand() % 100 == 0) {
					sacas[i].x = 1550;
					sacas[i].y = 200;
					sacas[i].velocidade_x = rand() % 7 + 2;
					sacas[i].velocidade_y = 3;
					sacas[i].cafe_sx = 7;
					sacas[i].cafe_sy = 9;
					sacas[i].cafe_sw = 172;
					sacas[i].cafe_sh = 100;
					sacas[i].colide = false;
					sacas[i].ativo = true;
					break;
				}
			}
		}
	}
}

void AtualizarSaca(Saca sacas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (sacas[i].ativo) {
			sacas[i].x -= sacas[i].velocidade_x;
			sacas[i].y += sacas[i].velocidade_y;

			if (sacas[i].x < 0) {
				sacas[i].ativo = false;
			}
			if (sacas[i].y > 810) {
				sacas[i].ativo = false;
			}
		}
	}
}

void DesenhaSaca(Saca sacas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (sacas[i].ativo) {
			al_draw_bitmap_region(cafe, sacas[i].cafe_sx, sacas[i].cafe_sy, sacas[i].cafe_sw, sacas[i].cafe_sh, sacas[i].x, sacas[i].y, NULL);
		}
	}
}

void ColideSacaPinguim(Saca sacas[], int s_tamanho, Pinguim p1, Pilha* pilha) {
	if (morte == false) {
		bool ok = false;
		for (int i = 0; i < s_tamanho; i++) {
			if (sacas[i].ativo) {
				if ((sacas[i].x - sacas[i].borda_x / 2) < (p1.x + p1.borda_x / 2) &&
					(sacas[i].x + sacas[i].borda_x / 2) > (p1.x - p1.borda_x / 2) &&
					(sacas[i].y + sacas[i].borda_y / 2) > (p1.y - p1.borda_y / 2) &&
					(sacas[i].y - sacas[i].borda_y / 2) < (p1.y + p1.borda_y / 2))
				{
					sacas[i].colide = true;
					pilha->Empilha(1, ok);
					Atualizarpilha(pilha);
					sacas[i].ativo = false;

				}
			}
		}
	}
}

void DesenhaCrashedSaca(Saca sacas[], int s_tamanho) {
	for (int i = 0; i < s_tamanho; i++) {
		int x_fixo = sacas[i].x;
		int y_fixo = sacas[i].y;
		if (!sacas[i].ativo && !sacas[i].colide) {
			al_draw_bitmap_region(cafe, 183, 9, 232, 100, x_fixo, y_fixo, NULL);
		}
	}
}

// --------- BIGORNA -----------

void InitBigorna(Bigorna bigornas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		bigornas[i].ID = BIGORNA;
		bigornas[i].velocidade_x = rand() % 7 + 2;
		bigornas[i].velocidade_y = 3;
		bigornas[i].borda_x = 165;
		bigornas[i].borda_y = 80;
		bigornas[i].ativo = false;
		bigornas[i].colide = false;
	}
}
void LiberaBigorna(Bigorna bigornas[], int tamanho) {
	if (morte == false) {
		for (int i = 0; i < tamanho; i++) {
			if (!bigornas[i].ativo) {
				if (rand() % 100 == 0) {
					bigornas[i].x = 1550;
					bigornas[i].y = 200;
					bigornas[i].velocidade_x = rand() % 7 + 2;
					bigornas[i].velocidade_y = 3;
					bigornas[i].colide = false;
					bigornas[i].ativo = true;
					break;
				}
			}
		}
	}
}
void AtualizarBigorna(Bigorna bigornas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (bigornas[i].ativo) {
			bigornas[i].x -= bigornas[i].velocidade_x;
			bigornas[i].y += bigornas[i].velocidade_y;

			if (bigornas[i].x < 0) {
				bigornas[i].ativo = false;
			}
			if (bigornas[i].y > 810) {
				bigornas[i].ativo = false;
			}
		}
	}
}
void DesenhaBigorna(Bigorna bigornas[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (bigornas[i].ativo) {
			al_draw_bitmap(bigorna, bigornas[i].x, bigornas[i].y, NULL);
		}
	}
}
void ColideBigornaPinguim(Bigorna bigornas[], int s_tamanho, Pinguim p1, Pilha* pilha) {
	if (morte == false) {
		bool ok = false;
		for (int i = 0; i < s_tamanho; i++) {
			if (bigornas[i].ativo) {
				if ((bigornas[i].x - bigornas[i].borda_x / 2) < (p1.x + p1.borda_x / 2) &&
					(bigornas[i].x + bigornas[i].borda_x / 2) > (p1.x - p1.borda_x / 2) &&
					(bigornas[i].y + bigornas[i].borda_y / 2) > (p1.y - p1.borda_y / 2) &&
					(bigornas[i].y - bigornas[i].borda_y / 2) < (p1.y + p1.borda_y / 2))
				{
					bigornas[i].colide = true;
					s_x = 742;
					s_y = 333;
					s_w = 241;
					s_h = 279;
					morte = true;
					bigornas[i].ativo = false;

				}
			}
		}
	}
}

// --------- PILHA ------------


void Atualizarpilha(Pilha* pilha) {

	int x;
	int cont = 0;
	bool Ok;
	Pilha* pAux = new Pilha(pilha->GetTamMax());

	while (pilha->Vazia() == false) {
		pilha->Desempilha(x, Ok);
		if (Ok) {
			cont++;
			pAux->Empilha(x, Ok);
		}
	}

	switch (cont)
	{
	case 0:
		s_x = 17;
		s_y = 27;
		s_w = 234;
		s_h = 279;
		break;
	case 1:
		s_x = 254;
		s_y = 27;
		s_w = 234;
		s_h = 279;
		break;
	case 2:
		s_x = 489;
		s_y = 27;
		s_w = 244;
		s_h = 279;
		break;
	case 3:
		s_x = 737;
		s_y = 27;
		s_w = 249;
		s_h = 279;
		break;
	case 4:
		s_x = 3;
		s_y = 332;
		s_w = 250;
		s_h = 304;
		break;
	case 5:
		s_x = 249;
		s_y = 332;
		s_w = 239;
		s_h = 304;
		break;
	case 6:
		s_x = 493;
		s_y = 332;
		s_w = 239;
		s_h = 304;
		morte = true;
		break;
	}

	while (pAux->Vazia() == false) {
		pAux->Desempilha(x, Ok);
		if (Ok) {
			cont--;
			pilha->Empilha(x, Ok);
		}
	}
}

void DesenhapilhaDef(Pilha* pilha) {
	int pos_x = 100;
	int pos_y = 675;

	int x;
	int cont = 0;
	bool Ok;
	Pilha* pAux = new Pilha(pilha->GetTamMax());

	while (pilha->Vazia() == false) {
		pilha->Desempilha(x, Ok);
		if (Ok) {
			cont++;
			pAux->Empilha(x, Ok);
		}
	}


	for (int i = 0; i < cont; i++) {
		al_draw_bitmap_region(def, 7, 9, 172, 100, pos_x, pos_y, NULL);
		pos_y -= 25;
	}

	while (pAux->Vazia() == false) {
		pAux->Desempilha(x, Ok);
		if (Ok) {
			cont--;
			pilha->Empilha(x, Ok);
		}
	}
}
