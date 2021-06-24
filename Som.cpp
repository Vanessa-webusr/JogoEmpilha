#include "Som.h"

SOM::SOM(char* nome_arquivo) {
	Som = NULL;
	Inst_Som = NULL;
	Som = al_load_sample(nome_arquivo);
	Inst_Som = al_create_sample_instance(Som);
	al_attach_sample_instance_to_mixer(Inst_Som, al_get_default_mixer());
	nome_arquivo = nome_arquivo;
}

SOM::~SOM() {
	al_destroy_sample(Som);
	al_destroy_sample_instance(Inst_Som);
}

void SOM::Play() {
	al_play_sample_instance(Inst_Som);
}

void SOM::Stop() {
	al_stop_sample_instance(Inst_Som);
}

void SOM::setVolume(float Volume) {
	al_set_sample_instance_gain(Inst_Som, Volume);
}

void SOM::setLoop(bool Liga) {
	if (Liga)
		al_set_sample_instance_playmode(Inst_Som, ALLEGRO_PLAYMODE_LOOP);
	else
		al_set_sample_instance_playmode(Inst_Som, ALLEGRO_PLAYMODE_ONCE);
}

char* SOM::getName() {
	return nome_arquivo;
}

ALLEGRO_SAMPLE_INSTANCE* SOM::getInst_Som() {
	return Inst_Som;
}
