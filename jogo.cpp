#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <cstdlib>
#include <ctime>
#include <cstddef>
#include "objetos.h"
#include "Pilha.h"
#include "PilhaInter.h"
#include "Som.h"

enum TECLAS { DIREITA, ESQUERDA, ESPACO , M , ESC};

// ---------- VARIÁVEIS GLOBAIS ---------
const int width_t = 1860;
const int height_t = 976;
const int FPS = 60;
const int NUM_SACAS = 2;
const int NUM_BIGORNAS = 1;
const int NUM_VASOS = 1;
const int NUM_PEIXES = 1;
const int QUANT_SONS = 10;
const int OBJETIVO_FASE_1 = 7;
const int LIMITE_PINGUIM = 6;	// Capacidade máxima =  n-1 =5

// ---------- GAME STATES --------
const int STATE_MENU = 0;
const int STATE_GAME = 1;
const int STATE_OVER = 2;
const int STATE_WIN = 3;

float s_x = 17, s_y = 27, s_w = 234, s_h = 279;

bool morte = false;
bool vitoria = false;


ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_BITMAP* background = NULL;
ALLEGRO_BITMAP* player = NULL;
ALLEGRO_BITMAP* cafe = NULL;
ALLEGRO_BITMAP* bigorna = NULL;
ALLEGRO_BITMAP* def = NULL;
ALLEGRO_BITMAP* fim_de_jogo = NULL;
ALLEGRO_BITMAP* game_menu = NULL;
ALLEGRO_BITMAP* ganhou_jogo = NULL;
ALLEGRO_BITMAP* vaso = NULL;
ALLEGRO_BITMAP* peixe = NULL;


#define BACKGROUND_FILE "fundo_neve_caminhao.png"
#define PLAYER_FILE "pinguim_sprite_v2.png"
#define CAFE_FILE "coffee_sprite_v1.png"
#define BIGORNA_FILE "anvil_v1.png"
#define FIM_DE_JOGO_FILE "game_over_v5.png"
#define GAME_MENU_FILE "game_menu_v3.png"
#define GANHA_JOGO_FILE "vitoria.png"
#define VASO_FILE "vaso.png"
#define PEIXE_FILE "peixe.png"


// --------- PROTÓTIPOS --------------------
void InitPinguim(Pinguim& p1);
void DesenhaPinguim(Pinguim& p1);
void MovePinguimEsquerda(Pinguim& p1);
void MovePinguimDireita(Pinguim& p1);

void InitSaca(Saca sacas[], int tamanho);
void LiberaSaca(Saca sacas[], int tamanho);
void AtualizarSaca(Saca sacas[], int tamanho);
void DesenhaSaca(Saca sacas[], int tamanho);
void ColideSacaPinguim(Saca sacas[], int s_tamanho, Pinguim p1, Pilha* pilha, bool &ColideSaca);
void DesenhaCrashedSaca(Saca sacas[], int s_tamanho);

void InitBigorna(Bigorna bigornas[], int tamanho);
void LiberaBigorna(Bigorna bigornas[], int tamanho);
void AtualizarBigorna(Bigorna bigornas[], int tamanho);
void DesenhaBigorna(Bigorna bigornas[], int tamanho);
void ColideBigornaPinguim(Bigorna bigornas[], int s_tamanho, Pinguim p1, Pilha* pilha);
void DesenhaCrashedBigorna(Bigorna bigornas[], int s_tamanho);

void InitVaso(Vaso vasos[], int tamanho);
void LiberaVaso(Vaso vasos[], int tamanho);
void AtualizarVaso(Vaso vasos[], int tamanho);
void DesenhaVaso(Vaso vasos[], int tamanho);
void ColideVasoPinguim(Vaso vasos[], int s_tamanho, Pinguim p1, Pilha* pilha);
void DesenhaCrashedVaso(Vaso vasos[], int s_tamanho);

void InitPeixe(Peixe peixes[], int tamanho);
void LiberaPeixe(Peixe peixes[], int tamanho);
void AtualizarPeixe(Peixe peixes[], int tamanho);
void DesenhaPeixe(Peixe peixes[], int tamanho);
void ColidePeixePinguim(Peixe peixes[], int s_tamanho, Pinguim p1, Pilha* pilha);
void DesenhaCrashedPeixe(Peixe peixes[], int s_tamanho);

void Atualizarpilha(Pilha* pilha);
void DesenhapilhaDef(Pilha* pilha);


int main() {


	// ------- VARIÁVEIS DO JOGO ------------
	ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
	ALLEGRO_TIMER* timer = NULL;

    int fase = 1;
	// ------- VARIÁVEL FIM DO PROGRAMA PRINCIPAL ------
	int gamestate = STATE_MENU;
	bool fim = false;
	bool desenha = true;

	bool teclas[] = { false, false, false,false,false };

	// -------- VARIAVEIS PARA CONTROLE DO SOM ----------
	bool ColideSaca = false;
	bool Toca_Vitoria = false;
	bool Toca_GameOver = false;
	bool Audio = true;

	// ------- INICIALIZAÇÃO DE OBJETOS -----------
	Pinguim p1;
	Saca sacas[NUM_SACAS];
	Bigorna bigornas[NUM_BIGORNAS];
	Vaso vasos[NUM_VASOS];
	Peixe peixes[NUM_PEIXES];
	PilhaInter* pilhaInter = new PilhaInter(LIMITE_PINGUIM);
	Pilha* pilhaDef = new Pilha(OBJETIVO_FASE_1);
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
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(QUANT_SONS);
	

	// ------- INICIALIZAÇÃO DE SONS ----------------
	SOM Som_Menu((char*)"trilha_menu.ogg");
	SOM	Som_Fase1((char*)"trilha_sonora.ogg");
	SOM Som_GameOver((char*)"trilha_gameover.ogg");
	SOM Som_Empilha((char*)"trilha_empilha.ogg");
	SOM Som_Vitoria((char*)"trilha_vitoria.ogg");
	

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
	
	// ---------- INICIALIZAÇÃO DO VASO ---------
	
	
    vaso = al_load_bitmap(VASO_FILE);

	if (!vaso)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O VASO NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	
	// ---------- INICIALIZAÇÃO DO PEIXE ---------
	
	
    peixe = al_load_bitmap(PEIXE_FILE);

	if (!peixe)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O PEIXE NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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
	
	// ---------- INICIALIZAÇÃO DO GANHA JOGO -------
	ganhou_jogo = al_load_bitmap(GANHA_JOGO_FILE);

	if (!ganhou_jogo)
	{
		al_show_native_message_box(NULL, "AVISO!", "ERRO:", "O GANHA JOGO NÃO PODE SER CRIADO", NULL, ALLEGRO_MESSAGEBOX_ERROR);
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
	InitVaso(vasos, NUM_VASOS);
	InitPeixe(peixes, NUM_PEIXES);

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
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
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
				teclas[ESPACO] = true;
				break;
			case ALLEGRO_KEY_M:
				teclas[M] = true;
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
				teclas[ESPACO] = false;
				break;
			case ALLEGRO_KEY_M:
				teclas[M] = false;
				break;
			}

		}
		if (teclas[M]) {
			Audio = !Audio;
			teclas[M] = false;
		}

		switch (gamestate) {
		case STATE_MENU:

			al_draw_bitmap(game_menu, 0, 0, 0);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (Audio) {
				Som_Menu.setLoop(true);
				Som_Menu.setVolume(1);
				Som_Menu.Play();
			}
			else
				Som_Menu.Stop();

			if (teclas[ESPACO]) {
				gamestate = STATE_GAME;
				teclas[ESPACO] = false;
			}
			break;
		case STATE_GAME:
			Som_Menu.Stop();
			if (Audio) {
				Som_Fase1.setLoop(true);
				Som_Fase1.Play();
			}
			else
				Som_Fase1.Stop();

			if (morte) {
				gamestate = STATE_OVER;
				Toca_GameOver = true;
			}
			else if (vitoria){
				gamestate = STATE_WIN;
				Toca_Vitoria = true;
				fase = fase + 1;
				if(fase > 3){
				    fase = 1;
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

				if (teclas[ESPACO]) {
					if (p1.x < 300 && p1.x >= 0) {
						while (!pilhaInter->Vazia())
						{
							pilhaInter->Desempilha(aux, ok);
							if (aux != 0) {
								pilhaDef->Empilha(aux, ok);
								aux = 0;
							}
							Atualizarpilha(pilhaInter);
						}
					}
					teclas[ESPACO] = false;
				}
				


				LiberaSaca(sacas, NUM_SACAS);
				AtualizarSaca(sacas, NUM_SACAS);
				ColideSacaPinguim(sacas, NUM_SACAS, p1, pilhaInter, ColideSaca);
				if (ColideSaca && Audio) {
					Som_Empilha.Stop();
					Som_Empilha.Play();
					ColideSaca = false;
				}
				LiberaBigorna(bigornas, NUM_BIGORNAS);
				AtualizarBigorna(bigornas, NUM_BIGORNAS);
				ColideBigornaPinguim(bigornas, NUM_BIGORNAS, p1, pilhaInter);
				
				if(fase == 2){
				    LiberaVaso(vasos, NUM_VASOS);
				    AtualizarVaso(vasos, NUM_VASOS);
				    ColideVasoPinguim(vasos, NUM_VASOS, p1, pilhaInter);
				}
				if(fase == 3){
				    LiberaVaso(vasos, NUM_VASOS);
				    AtualizarVaso(vasos, NUM_VASOS);
				    ColideVasoPinguim(vasos, NUM_VASOS, p1, pilhaInter);
				    LiberaPeixe(peixes, NUM_PEIXES);
				    AtualizarPeixe(peixes, NUM_PEIXES);
				    ColidePeixePinguim(peixes, NUM_PEIXES, p1, pilhaInter);
                }
			}


			// ------- DESENHO -------

			if (desenha && al_is_event_queue_empty(fila_eventos))
			{

				desenha = false;

				al_draw_bitmap(background, 0, 0, 0);
				DesenhapilhaDef(pilhaDef);
				DesenhaPinguim(p1);
				DesenhaSaca(sacas, NUM_SACAS);
				DesenhaCrashedSaca(sacas, NUM_SACAS);
				DesenhaBigorna(bigornas, NUM_BIGORNAS);
				DesenhaCrashedBigorna(bigornas, NUM_BIGORNAS);
				
				if(fase == 2)
				{
				    DesenhaVaso(vasos, NUM_VASOS);
				    DesenhaCrashedVaso(vasos, NUM_VASOS);
				}
				if(fase == 3)
				{
				    DesenhaVaso(vasos, NUM_VASOS);
				    DesenhaCrashedVaso(vasos, NUM_VASOS);
				    DesenhaPeixe(peixes, NUM_PEIXES);
				    DesenhaCrashedPeixe(peixes, NUM_PEIXES);
                }
				al_flip_display();
				al_clear_to_color(al_map_rgb(0, 0, 0));
			}
			break;
		case STATE_OVER:
			Som_Fase1.Stop();
			if (Toca_GameOver && Audio) {
				Som_GameOver.Play();
				Toca_GameOver = false;
			}

			if(teclas[ESPACO]){
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
				teclas[ESPACO] = false;
			}

			al_draw_bitmap(background, 0, 0, 0);
			DesenhaPinguim(p1);
			DesenhapilhaDef(pilhaDef);
			al_draw_bitmap(fim_de_jogo, 720, 59, 0);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			break;
			
			case STATE_WIN:
				Som_Fase1.Stop();
				if (Toca_Vitoria && Audio) {
					Som_Vitoria.Play();
					Toca_Vitoria = false;
				}
			    if(teclas[ESPACO]){
					morte = false;
					vitoria = false;
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
					teclas[ESPACO] = false;
			    }

			al_draw_bitmap(background, 0, 0, 0);
			DesenhapilhaDef(pilhaDef);
			DesenhaPinguim(p1);
			al_draw_bitmap(ganhou_jogo, 720, 59, 0);

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
	al_destroy_bitmap(vaso);
	al_destroy_bitmap(peixe);
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

	al_draw_bitmap_region(player, s_x, s_y, s_w, s_h, p1.x, p1.y, 0);

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
			al_draw_bitmap_region(cafe, sacas[i].cafe_sx, sacas[i].cafe_sy, sacas[i].cafe_sw, sacas[i].cafe_sh, sacas[i].x, sacas[i].y, 0);
		}
	}
}

void ColideSacaPinguim(Saca sacas[], int s_tamanho, Pinguim p1, Pilha* pilha, bool &ColideSaca) {
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
					ColideSaca = true;
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
			al_draw_bitmap_region(cafe, 183, 9, 232, 100, x_fixo, y_fixo, 0);
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
			al_draw_bitmap(bigorna, bigornas[i].x, bigornas[i].y, 0);
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

void DesenhaCrashedBigorna(Bigorna bigornas[], int s_tamanho) {
	for (int i = 0; i < s_tamanho; i++) {
		int x_fixo = bigornas[i].x;
		int y_fixo = bigornas[i].y;
		if (!bigornas[i].ativo && !bigornas[i].colide) {
			al_draw_bitmap(bigorna, x_fixo, y_fixo, 0);
		}
	}
}


// --------- VASO -------------

void InitVaso(Vaso vasos[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		vasos[i].ID = VASO;
		vasos[i].velocidade_x = rand() % 7 + 2;
		vasos[i].velocidade_y = 3;
		vasos[i].borda_x = 165;
		vasos[i].borda_y = 80;
		vasos[i].ativo = false;
		vasos[i].colide = false;
	}
}
void LiberaVaso(Vaso vasos[], int tamanho) {
	if (morte == false) {
		for (int i = 0; i < tamanho; i++) {
			if (!vasos[i].ativo) {
				if (rand() % 100 == 0) {
					vasos[i].x = 1550;
					vasos[i].y = 200;
					vasos[i].velocidade_x = rand() % 7 + 2;
					vasos[i].velocidade_y = 3;
					vasos[i].colide = false;
					vasos[i].ativo = true;
					break;
				}
			}
		}
	}
}
void AtualizarVaso(Vaso vasos[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (vasos[i].ativo) {
			vasos[i].x -= vasos[i].velocidade_x;
			vasos[i].y += vasos[i].velocidade_y;

			if (vasos[i].x < 0) {
				vasos[i].ativo = false;
			}
			if (vasos[i].y > 750) {
				vasos[i].ativo = false;
			}
		}
	}
}
void DesenhaVaso(Vaso vasos[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (vasos[i].ativo) {
			al_draw_bitmap(vaso, vasos[i].x, vasos[i].y, 0);
		}
	}
}
void ColideVasoPinguim(Vaso vasos[], int s_tamanho, Pinguim p1, Pilha* pilha) {
	if (morte == false) {
		bool ok = false;
		for (int i = 0; i < s_tamanho; i++) {
			if (vasos[i].ativo) {
				if ((vasos[i].x - vasos[i].borda_x / 2) < (p1.x + p1.borda_x / 2) &&
					(vasos[i].x + vasos[i].borda_x / 2) > (p1.x - p1.borda_x / 2) &&
					(vasos[i].y + vasos[i].borda_y / 2) > (p1.y - p1.borda_y / 2) &&
					(vasos[i].y - vasos[i].borda_y / 2) < (p1.y + p1.borda_y / 2))
				{
					vasos[i].colide = true;
					s_x = 249;
					s_y = 630;
					s_w = 239;
					s_h = 329;
					morte = true;
					vasos[i].ativo = false;

				}
			}
		}
	}
}

void DesenhaCrashedVaso(Vaso vasos[], int s_tamanho) {
	for (int i = 0; i < s_tamanho; i++) {
		int x_fixo = vasos[i].x;
		int y_fixo = vasos[i].y;
		if (!vasos[i].ativo && !vasos[i].colide) {
			al_draw_bitmap(vaso, x_fixo, y_fixo, 0);
		}
	}
}

// --------- PEIXE -------------

void InitPeixe(Peixe peixes[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		peixes[i].ID = PEIXE;
		peixes[i].velocidade_x = rand() % 7 + 2;
		peixes[i].velocidade_y = 3;
		peixes[i].borda_x = 165;
		peixes[i].borda_y = 80;
		peixes[i].ativo = false;
		peixes[i].colide = false;
	}
}
void LiberaPeixe(Peixe peixes[], int tamanho) {
	if (morte == false) {
		for (int i = 0; i < tamanho; i++) {
			if (!peixes[i].ativo) {
				if (rand() % 100 == 0) {
					peixes[i].x = 1550;
					peixes[i].y = 200;
					peixes[i].velocidade_x = rand() % 7 + 2;
					peixes[i].velocidade_y = 3;
					peixes[i].colide = false;
					peixes[i].ativo = true;
					break;
				}
			}
		}
	}
}
void AtualizarPeixe(Peixe peixes[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (peixes[i].ativo) {
			peixes[i].x -= peixes[i].velocidade_x;
			peixes[i].y += peixes[i].velocidade_y;

			if (peixes[i].x < 0) {
				peixes[i].ativo = false;
			}
			if (peixes[i].y > 810) {
				peixes[i].ativo = false;
			}
		}
	}
}
void DesenhaPeixe(Peixe peixes[], int tamanho) {
	for (int i = 0; i < tamanho; i++) {
		if (peixes[i].ativo) {
			al_draw_bitmap(peixe, peixes[i].x, peixes[i].y, 0);
		}
	}
}
void ColidePeixePinguim(Peixe peixes[], int s_tamanho, Pinguim p1, Pilha* pilha) {
	if (morte == false) {
		bool ok = false;
		for (int i = 0; i < s_tamanho; i++) {
			if (peixes[i].ativo) {
				if ((peixes[i].x - peixes[i].borda_x / 2) < (p1.x + p1.borda_x / 2) &&
					(peixes[i].x + peixes[i].borda_x / 2) > (p1.x - p1.borda_x / 2) &&
					(peixes[i].y + peixes[i].borda_y / 2) > (p1.y - p1.borda_y / 2) &&
					(peixes[i].y - peixes[i].borda_y / 2) < (p1.y + p1.borda_y / 2))
				{
					peixes[i].colide = true;
					s_x = 3;
					s_y = 630;
					s_w = 250;
					s_h = 329;
					morte = true;
					peixes[i].ativo = false;

				}
			}
		}
	}
}

void DesenhaCrashedPeixe(Peixe peixes[], int s_tamanho) {
	for (int i = 0; i < s_tamanho; i++) {
		int x_fixo = peixes[i].x;
		int y_fixo = peixes[i].y;
		if (!peixes[i].ativo && !peixes[i].colide) {
			al_draw_bitmap(peixe, x_fixo, y_fixo, 0);
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
	int pos_x = 100; //width_t*0.0537;
	int pos_y = 675; //height_t*0.692;

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
		al_draw_bitmap_region(def, 7, 9, 172, 100, pos_x, pos_y, 0);
		pos_y -= 25;
	}

	while (pAux->Vazia() == false) {
		pAux->Desempilha(x, Ok);
		if (Ok) {
			cont--;
			pilha->Empilha(x, Ok);
		}
	}
	if(pilha->Cheia() == true){
	    vitoria = true;
	}
	
}
