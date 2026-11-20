// LeitorTracos.h - le o arquivo de traco (R/W + endereco hexadecimal).
#ifndef LEITOR_TRACOS_H
#define LEITOR_TRACOS_H

#include <string>
#include <vector>
#include "Tipos.h"

// Le todos os acessos do arquivo. Ignora comentarios (#) e linhas vazias.
// Retorna false e preenche 'erro' se o arquivo nao abrir ou houver linha invalida.
bool lerTraco(const std::string& caminho, std::vector<Acesso>& acessos, std::string& erro);

#endif
