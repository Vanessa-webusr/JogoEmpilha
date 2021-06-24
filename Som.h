#pragma once
#include <iostream>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>



using namespace std;

class SOM
{
public:
	SOM(char* nome_arquivo);
	~SOM();
	void Play();
	void Stop();
	void setVolume(float Volume);
	void setLoop(bool Liga);
	char* getName();
	ALLEGRO_SAMPLE_INSTANCE* getInst_Som();

private:
	char* nome_arquivo;
	ALLEGRO_SAMPLE* Som;
	ALLEGRO_SAMPLE_INSTANCE* Inst_Som;
};

