#ifndef PILHAINTER_H
#define PILHAINTER_H
#include "Pilha.h"


class PilhaInter : public Pilha {
public:
	PilhaInter(int tam_max);
	~PilhaInter();
	void Empilha(int numero, bool& deuCerto);
	void Desempilha(int& numero, bool& deuCerto);

};

#endif
