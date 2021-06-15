#ifndef PILHA_H
#define PILHA_H

#include <iostream>

using namespace std;

class Pilha {
public:
	Pilha(int tamMax);
	virtual ~Pilha();
	bool Cheia();
	bool Vazia();
	virtual void Empilha(int numero, bool& deuCerto);
	virtual void Desempilha(int& numero, bool& deuCerto);
	int GetTamMax();
private:
	int topo;
	int* elementos;
	int tamMax;
};


#endif
