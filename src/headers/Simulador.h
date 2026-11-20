// Simulador.h - fluxo de cada acesso, WT/WB, classificacao de faltas e saida.
#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <cstdint>
#include <unordered_set>
#include <vector>
#include "Cache.h"
#include "Tipos.h"

class Simulador {
public:
    explicit Simulador(const Configuracao& config);

    // Executa todos os acessos, faz a descarga final (WB) e imprime a saida.
    void executar(const std::vector<Acesso>& acessos);

    const Estatisticas& estatisticas() const { return estat_; }

private:
    ResultadoAcesso processarAcesso(const Acesso& acesso);
    TipoFalta classificarFalta(uint32_t enderecoBloco, bool acertariaTotalmenteAssoc);
    bool acessarTotalmenteAssociativa(uint32_t endereco);  // true se acertou
    void descargaFinal();

    void imprimirCabecalho() const;
    void imprimirAcesso(const ResultadoAcesso& r) const;
    void imprimirResumo() const;

    Configuracao config_;
    Cache cache_;
    Cache cacheTotalmenteAssoc_;  // mesma capacidade e bloco, so para classificar
    std::unordered_set<uint32_t> blocosJaVistos_;  // para faltas compulsorias
    Estatisticas estat_;
};

#endif
