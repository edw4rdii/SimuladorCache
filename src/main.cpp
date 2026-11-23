// main.cpp - le os argumentos da linha de comando e chama o Simulador.
#include "headers/LeitorTracos.h"
#include "headers/Simulador.h"

#include <iostream>
#include <string>

static void mostrarUso() {
    std::cerr << "Uso: ./simulador <traco> -c <capacidade> -b <bloco> -v <vias> -p <WB|WT> [--resumo]\n"
              << "  -c  capacidade da cache em bytes\n"
              << "  -b  tamanho do bloco em bytes\n"
              << "  -v  numero de vias (1 = mapeamento direto)\n"
              << "  -p  politica de escrita: WB (write-back) ou WT (write-through)\n"
              << "  --resumo  mostra so o resumo, sem a saida por acesso\n"
              << "Ex.: ./simulador calibracao/C1_mapeamento_direto.txt -c 128 -b 16 -v 1 -p WB\n";
}

// Converte texto em numero positivo. Retorna false se nao for um numero valido.
static bool lerNumero(const std::string& texto, uint32_t& valor) {
    if (texto.empty() || texto.size() > 9) {
        return false;
    }
    for (char c : texto) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    valor = std::stoul(texto);
    return true;
}

int main(int argc, char* argv[]) {
    Configuracao config;
    bool temPolitica = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--resumo") {
            config.apenasResumo = true;
        } else if (arg == "-c" || arg == "-b" || arg == "-v" || arg == "-p") {
            if (i + 1 >= argc) {
                std::cerr << "Erro: falta o valor de " << arg << ".\n";
                return 1;
            }
            std::string valor = argv[++i];

            if (arg == "-p") {
                if (valor == "WB" || valor == "wb") {
                    config.politica = Politica::WriteBack;
                } else if (valor == "WT" || valor == "wt") {
                    config.politica = Politica::WriteThrough;
                } else {
                    std::cerr << "Erro: politica '" << valor << "' invalida (use WB ou WT).\n";
                    return 1;
                }
                temPolitica = true;
            } else {
                uint32_t numero;
                if (!lerNumero(valor, numero)) {
                    std::cerr << "Erro: valor '" << valor << "' de " << arg << " invalido.\n";
                    return 1;
                }
                if (arg == "-c") config.capacidade = numero;
                if (arg == "-b") config.tamanhoBloco = numero;
                if (arg == "-v") config.vias = numero;
            }
        } else if (arg[0] == '-') {
            std::cerr << "Erro: opcao '" << arg << "' desconhecida.\n";
            mostrarUso();
            return 1;
        } else if (config.arquivoTraco.empty()) {
            config.arquivoTraco = arg;
        } else {
            std::cerr << "Erro: mais de um arquivo de traco informado.\n";
            return 1;
        }
    }

    // Todos os parametros sao obrigatorios (menos --resumo).
    if (config.arquivoTraco.empty() || config.capacidade == 0 || config.tamanhoBloco == 0 ||
        config.vias == 0 || !temPolitica) {
        std::cerr << "Erro: faltam parametros.\n";
        mostrarUso();
        return 1;
    }

    std::string erro;
    if (!validarConfiguracao(config, erro)) {
        std::cerr << "Erro: configuracao invalida: " << erro << "\n";
        return 1;
    }

    std::vector<Acesso> acessos;
    if (!lerTraco(config.arquivoTraco, acessos, erro)) {
        std::cerr << "Erro: " << erro << "\n";
        return 1;
    }

    Simulador simulador(config);
    simulador.executar(acessos);
    return 0;
}
