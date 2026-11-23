// Cache.cpp - conjuntos x vias, divisao do endereco, busca e LRU.
#include "headers/Cache.h"

// log2 de uma potencia de 2 (ex.: 16 -> 4).
static uint32_t log2(uint32_t valor) {
    uint32_t bits = 0;
    while (valor > 1) {
        valor = valor / 2;
        bits++;
    }
    return bits;
}

Cache::Cache(uint32_t capacidade, uint32_t tamanhoBloco, uint32_t vias) {
    tamanhoBloco_ = tamanhoBloco;
    vias_ = vias;
    conjuntos_ = capacidade / (tamanhoBloco * vias);

    bitsOffset_ = log2(tamanhoBloco);
    bitsIndice_ = log2(conjuntos_);
    bitsRotulo_ = 32 - bitsOffset_ - bitsIndice_;

    relogio_ = 0;
    blocos_ = std::vector<std::vector<Bloco>>(conjuntos_, std::vector<Bloco>(vias_));  // cache vazia
}

// Divisao do endereco: rotulo | indice | offset
uint32_t Cache::calcularOffset(uint32_t endereco) const {
    return endereco % tamanhoBloco_;
}

uint32_t Cache::calcularIndice(uint32_t endereco) const {
    return (endereco / tamanhoBloco_) % conjuntos_;
}

uint32_t Cache::calcularRotulo(uint32_t endereco) const {
    return endereco / tamanhoBloco_ / conjuntos_;
}

// Endereco do inicio do bloco (offset = 0).
uint32_t Cache::montarEnderecoBloco(uint32_t rotulo, uint32_t indice) const {
    return (rotulo * conjuntos_ + indice) * tamanhoBloco_;
}

// Procura o rotulo no conjunto. Retorna a via, ou -1 se nao achar.
int Cache::buscar(uint32_t indice, uint32_t rotulo) const {
    for (uint32_t via = 0; via < vias_; via++) {
        const Bloco& b = blocos_[indice][via];
        if (b.valido && b.rotulo == rotulo) {
            return via;
        }
    }
    return -1;
}

// Marca o bloco como usado agora (vale tambem para acertos).
void Cache::atualizarUso(uint32_t indice, uint32_t via) {
    relogio_++;
    blocos_[indice][via].ultimoUso = relogio_;
}

// Via livre (invalida) ou, se nao houver, a menos recentemente usada.
uint32_t Cache::escolherVitima(uint32_t indice) const {
    for (uint32_t via = 0; via < vias_; via++) {
        if (!blocos_[indice][via].valido) {
            return via;
        }
    }

    uint32_t vitima = 0;
    for (uint32_t via = 1; via < vias_; via++) {
        if (blocos_[indice][via].ultimoUso < blocos_[indice][vitima].ultimoUso) {
            vitima = via;
        }
    }
    return vitima;
}

Bloco& Cache::bloco(uint32_t indice, uint32_t via) {
    return blocos_[indice][via];
}

const Bloco& Cache::bloco(uint32_t indice, uint32_t via) const {
    return blocos_[indice][via];
}
