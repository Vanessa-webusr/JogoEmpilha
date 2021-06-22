// 1=Cafe /  2=Bigorna, Vaso e Peixe
#include "PilhaInter.h"



PilhaInter::PilhaInter(int tam_max) :Pilha(tam_max) {}

PilhaInter::~PilhaInter() {}

void PilhaInter::Empilha(int numero, bool& deuCerto) {
    if (numero == 1) {
        deuCerto = true;
        Pilha::Empilha(numero, deuCerto);
    }
    else
        deuCerto = false;

}

void PilhaInter::Desempilha(int& numero, bool& deuCerto) {
    Pilha::Desempilha(numero, deuCerto);
}
