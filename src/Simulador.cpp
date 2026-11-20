// LeitorTracos.cpp - leitura do arquivo de traco.
#include "headers/LeitorTracos.h"

#include <cctype>
#include <fstream>
#include <sstream>

// Remove espacos (e o '\r' de arquivos do Windows) no inicio e no fim.
static std::string removerEspacos(const std::string& texto) {
    size_t inicio = 0;
    size_t fim = texto.size();
    while (inicio < fim && std::isspace(static_cast<unsigned char>(texto[inicio]))) {
        inicio++;
    }
    while (fim > inicio && std::isspace(static_cast<unsigned char>(texto[fim - 1]))) {
        fim--;
    }
    return texto.substr(inicio, fim - inicio);
}

// Converte "0x10000000" (ou "10000000") em numero. Retorna false se invalido.
static bool converterEnderecoHex(const std::string& texto, uint32_t& endereco) {
    std::string digitos = texto;
    if (digitos.size() >= 2 && digitos[0] == '0' && (digitos[1] == 'x' || digitos[1] == 'X')) {
        digitos = digitos.substr(2);
    }
    // 32 bits = no maximo 8 digitos hexadecimais
    if (digitos.empty() || digitos.size() > 8) {
        return false;
    }
    for (char c : digitos) {
        if (!std::isxdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    endereco = static_cast<uint32_t>(std::stoul(digitos, nullptr, 16));
    return true;
}

bool lerTraco(const std::string& caminho, std::vector<Acesso>& acessos, std::string& erro) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) {
        erro = "Nao foi possivel abrir o arquivo de traco: " + caminho;
        return false;
    }

    acessos.clear();
    std::string linha;
    int numeroLinha = 0;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;

        // Tudo depois de '#' e comentario
        size_t posicaoComentario = linha.find('#');
        if (posicaoComentario != std::string::npos) {
            linha = linha.substr(0, posicaoComentario);
        }
        linha = removerEspacos(linha);
        if (linha.empty()) {
            continue;
        }

        // Esperado: <R|W> <endereco> e mais nada
        std::istringstream partes(linha);
        std::string textoOperacao, textoEndereco, sobra;
        partes >> textoOperacao >> textoEndereco >> sobra;

        if (textoOperacao.empty() || textoEndereco.empty() || !sobra.empty()) {
            erro = "Linha " + std::to_string(numeroLinha) +
                   ": formato invalido (esperado 'R 0x...' ou 'W 0x...').";
            return false;
        }

        Acesso acesso;
        if (textoOperacao == "R" || textoOperacao == "r") {
            acesso.operacao = Operacao::Leitura;
        } else if (textoOperacao == "W" || textoOperacao == "w") {
            acesso.operacao = Operacao::Escrita;
        } else {
            erro = "Linha " + std::to_string(numeroLinha) +
                   ": operacao '" + textoOperacao + "' invalida (use R ou W).";
            return false;
        }

        if (!converterEnderecoHex(textoEndereco, acesso.endereco)) {
            erro = "Linha " + std::to_string(numeroLinha) +
                   ": endereco '" + textoEndereco + "' invalido (use hexadecimal de ate 32 bits).";
            return false;
        }

        acessos.push_back(acesso);
    }

    return true;
}