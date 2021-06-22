#include "Pilha.h"

// Construtor da pilha
Pilha::Pilha(int tamMax) :tamMax(tamMax) {  // Inicializa o tamanho máximo da fila
    elementos = new int[tamMax];           // Aloca um vetor de elementos
    topo = 0;                               // Inicializa a posição inicial do topo
}

// Destrutor da pilha
Pilha::~Pilha() {
    delete[]elementos;                     // Desaloca o vetor de elementos
}

// Implementação da função que empilha um elemento
void Pilha::Empilha(int numero, bool& deuCerto) {
    if (this->Cheia())
        deuCerto = false;
    else {
        deuCerto = true;
        elementos[topo] = numero;
        topo++;
    }
}

// Implementação da função para desempilhar um elemento
void Pilha::Desempilha(int& numero, bool& deuCerto) {
    if (this->Vazia()) {
        deuCerto = false;
    }
    else {
        deuCerto = true;
        numero = elementos[topo - 1];
        topo--;
    }
}

// Implementação da função que verifica se a pilha esta vazia
bool Pilha::Vazia() {
    if (topo == 0) {
        return true;
    }
    else return false;
}

bool Pilha::Cheia() {
    if (topo == tamMax) {
        return true;
    }
    else return false;
}

int Pilha::GetTamMax() {
    return tamMax;
}
