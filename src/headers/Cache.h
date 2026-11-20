// Cache.h - conjuntos x vias, divisao do endereco, busca e LRU.
// A cache totalmente associativa (classificacao de faltas) e esta mesma
// classe criada com vias = numero de blocos (1 conjunto).
#ifndef CACHE_H
#define CACHE_H

#include <cstdint>
#include <vector>
#include "Bloco.h"

class Cache {
public:
    Cache(uint32_t capacidade, uint32_t tamanhoBloco, uint32_t vias);

    // Divisao do endereco: rotulo | indice | offset
    uint32_t calcularOffset(uint32_t endereco) const;
    uint32_t calcularIndice(uint32_t endereco) const;
    uint32_t calcularRotulo(uint32_t endereco) const;
    uint32_t montarEnderecoBloco(uint32_t rotulo, uint32_t indice) const;

    // Procura o rotulo no conjunto. Retorna a via, ou -1 se nao achar.
    int buscar(uint32_t indice, uint32_t rotulo) const;

    // Marca o bloco como usado agora (vale tambem para acertos).
    void atualizarUso(uint32_t indice, uint32_t via);

    // Via livre (invalida) ou, se nao houver, a menos recentemente usada.
    uint32_t escolherVitima(uint32_t indice) const;

    Bloco& bloco(uint32_t indice, uint32_t via);
    const Bloco& bloco(uint32_t indice, uint32_t via) const;

    uint32_t numeroConjuntos() const { return conjuntos_; }
    uint32_t numeroVias() const { return vias_; }
    uint32_t bitsRotulo() const { return bitsRotulo_; }
    uint32_t bitsIndice() const { return bitsIndice_; }
    uint32_t bitsOffset() const { return bitsOffset_; }

private:
    uint32_t tamanhoBloco_;
    uint32_t vias_;
    uint32_t conjuntos_;
    uint32_t bitsOffset_;
    uint32_t bitsIndice_;
    uint32_t bitsRotulo_;
    uint64_t relogio_;  // contador crescente usado como "hora" do LRU
    std::vector<std::vector<Bloco>> blocos_;  // blocos_[indice][via]
};

#endif
