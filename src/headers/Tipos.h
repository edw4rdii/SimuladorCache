// Tipos.h - enums e structs compartilhados por todo o simulador.
#ifndef TIPOS_H
#define TIPOS_H

#include <cstdint>
#include <string>

enum class Operacao { Leitura, Escrita };

enum class Politica { WriteThrough, WriteBack };

enum class TipoFalta { Nenhuma, Compulsoria, Capacidade, Conflito };

// Parametros informados pelo usuario na linha de comando.
struct Configuracao {
    std::string arquivoTraco;
    uint32_t capacidade = 0;    // bytes de dados da cache
    uint32_t tamanhoBloco = 0;  // bytes
    uint32_t vias = 0;          // 1 = mapeamento direto
    Politica politica = Politica::WriteBack;
    bool apenasResumo = false;  // true: nao imprime a saida por acesso
};

// Uma linha do arquivo de traco.
struct Acesso {
    Operacao operacao = Operacao::Leitura;
    uint32_t endereco = 0;
};

// Tudo o que e mostrado sobre um unico acesso.
struct ResultadoAcesso {
    uint64_t numero = 0;
    Operacao operacao = Operacao::Leitura;
    uint32_t endereco = 0;
    uint32_t rotulo = 0;
    uint32_t indice = 0;
    uint32_t offset = 0;
    bool acerto = false;
    TipoFalta tipoFalta = TipoFalta::Nenhuma;
    bool houveExpulsao = false;
    uint32_t enderecoExpulso = 0;
    bool houveWriteback = false;
};

// Contadores do resumo final.
struct Estatisticas {
    uint64_t acessos = 0;
    uint64_t acertos = 0;
    uint64_t faltas = 0;
    uint64_t faltasCompulsorias = 0;
    uint64_t faltasConflito = 0;
    uint64_t faltasCapacidade = 0;

    uint64_t blocosLidos = 0;
    uint64_t bytesLidos = 0;

    uint64_t escritasWriteThrough = 0;  // escritas de 4 bytes (WT)
    uint64_t writebacks = 0;            // writebacks durante a execucao (WB)
    uint64_t descargaFinal = 0;         // blocos sujos gravados no fim (WB)
    uint64_t bytesEscritos = 0;
};

// Retorna true se a configuracao e valida; senao preenche 'erro'.
bool validarConfiguracao(const Configuracao& config, std::string& erro);

#endif