// Bloco.h - uma linha (bloco) da cache.
#ifndef BLOCO_H
#define BLOCO_H

#include <cstdint>

struct Bloco {
    bool valido = false;       // bit de validade (V)
    bool sujo = false;         // bit de modificacao (D)
    uint32_t rotulo = 0;       // tag
    uint64_t ultimoUso = 0;    // para o LRU: maior valor = usado mais recentemente
};

#endif
