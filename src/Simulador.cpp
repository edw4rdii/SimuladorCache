// Simulador.cpp - fluxo de cada acesso, WT/WB, classificacao de faltas e saida.
#include "headers/Simulador.h"

#include <cstdio>
#include <iomanip>
#include <iostream>

// Verdadeiro se o valor e potencia de 2 (1, 2, 4, 8, ...).
static bool potenciaDe2(uint32_t valor) {
    return valor > 0 && (valor & (valor - 1)) == 0;
}

bool validarConfiguracao(const Configuracao& config, std::string& erro) {
    if (!potenciaDe2(config.capacidade) || !potenciaDe2(config.tamanhoBloco) ||
        !potenciaDe2(config.vias)) {
        erro = "capacidade, bloco e vias devem ser potencias de 2.";
        return false;
    }
    if (config.tamanhoBloco < 4) {
        erro = "o bloco deve ter pelo menos 4 bytes (uma palavra).";
        return false;
    }
    if (config.tamanhoBloco > config.capacidade) {
        erro = "o bloco nao pode ser maior que a capacidade.";
        return false;
    }
    if (config.vias > config.capacidade / config.tamanhoBloco) {
        erro = "mais vias do que blocos na cache.";
        return false;
    }
    return true;
}

// A totalmente associativa tem 1 conjunto (vias = numero de blocos).
Simulador::Simulador(const Configuracao& config)
    : config_(config),
      cache_(config.capacidade, config.tamanhoBloco, config.vias),
      cacheTotalmenteAssoc_(config.capacidade, config.tamanhoBloco,
                            config.capacidade / config.tamanhoBloco) {}

void Simulador::executar(const std::vector<Acesso>& acessos) {
    imprimirCabecalho();
    for (const Acesso& acesso : acessos) {
        ResultadoAcesso r = processarAcesso(acesso);
        if (!config_.apenasResumo) {
            imprimirAcesso(r);
        }
    }
    descargaFinal();
    imprimirResumo();
}

ResultadoAcesso Simulador::processarAcesso(const Acesso& acesso) {
    ResultadoAcesso r;
    r.numero = ++estat_.acessos;
    r.operacao = acesso.operacao;
    r.endereco = acesso.endereco;
    r.rotulo = cache_.calcularRotulo(acesso.endereco);
    r.indice = cache_.calcularIndice(acesso.endereco);
    r.offset = cache_.calcularOffset(acesso.endereco);

    // A totalmente associativa recebe todos os acessos.
    bool acertariaTotalmenteAssoc = acessarTotalmenteAssociativa(acesso.endereco);

    int via = cache_.buscar(r.indice, r.rotulo);
    r.acerto = (via != -1);

    if (r.acerto) {
        estat_.acertos++;
    } else {
        // Falta: classifica e traz o bloco para a via escolhida.
        uint32_t enderecoBloco = cache_.montarEnderecoBloco(r.rotulo, r.indice);
        r.tipoFalta = classificarFalta(enderecoBloco, acertariaTotalmenteAssoc);

        via = cache_.escolherVitima(r.indice);
        Bloco& b = cache_.bloco(r.indice, via);

        if (b.valido) {  // expulsao
            r.houveExpulsao = true;
            r.enderecoExpulso = cache_.montarEnderecoBloco(b.rotulo, r.indice);
            r.houveWriteback = b.sujo;
            if (b.sujo) {
                estat_.writebacks++;
            }
        }

        b.valido = true;
        b.sujo = false;
        b.rotulo = r.rotulo;
        estat_.blocosLidos++;
    }

    cache_.atualizarUso(r.indice, via);

    // Escrita (com alocacao nas duas politicas).
    if (acesso.operacao == Operacao::Escrita) {
        if (config_.politica == Politica::WriteBack) {
            cache_.bloco(r.indice, via).sujo = true;
        } else {
            estat_.escritasWriteThrough++;
        }
    }

    return r;
}

// Compulsoria: primeira vez que o bloco aparece no traco.
// Conflito: a totalmente associativa acertaria. Capacidade: ela tambem faltaria.
TipoFalta Simulador::classificarFalta(uint32_t enderecoBloco, bool acertariaTotalmenteAssoc) {
    estat_.faltas++;

    bool primeiraVez = blocosJaVistos_.insert(enderecoBloco).second;
    if (primeiraVez) {
        estat_.faltasCompulsorias++;
        return TipoFalta::Compulsoria;
    }
    if (acertariaTotalmenteAssoc) {
        estat_.faltasConflito++;
        return TipoFalta::Conflito;
    }
    estat_.faltasCapacidade++;
    return TipoFalta::Capacidade;
}

// Acessa a cache totalmente associativa. Retorna true se acertou.
bool Simulador::acessarTotalmenteAssociativa(uint32_t endereco) {
    Cache& c = cacheTotalmenteAssoc_;
    uint32_t rotulo = c.calcularRotulo(endereco);

    int via = c.buscar(0, rotulo);
    bool acertou = (via != -1);

    if (!acertou) {
        via = c.escolherVitima(0);
        c.bloco(0, via).valido = true;
        c.bloco(0, via).rotulo = rotulo;
    }
    c.atualizarUso(0, via);
    return acertou;
}

// No fim do traco (WB), os blocos ainda sujos sao gravados na memoria.
void Simulador::descargaFinal() {
    if (config_.politica == Politica::WriteBack) {
        for (uint32_t i = 0; i < cache_.numeroConjuntos(); i++) {
            for (uint32_t v = 0; v < cache_.numeroVias(); v++) {
                if (cache_.bloco(i, v).valido && cache_.bloco(i, v).sujo) {
                    estat_.descargaFinal++;
                }
            }
        }
        estat_.bytesEscritos = (estat_.writebacks + estat_.descargaFinal) * config_.tamanhoBloco;
    } else {
        estat_.bytesEscritos = estat_.escritasWriteThrough * 4;  // 1 palavra por escrita
    }
}

void Simulador::imprimirCabecalho() const {
    std::cout << "CONFIGURACAO  capacidade=" << config_.capacidade << " B"
              << "  bloco=" << config_.tamanhoBloco << " B"
              << "  vias=" << config_.vias
              << "  conjuntos=" << cache_.numeroConjuntos()
              << "  escrita=" << (config_.politica == Politica::WriteBack ? "WB" : "WT") << "\n";
    std::cout << "CAMPOS        rotulo=" << cache_.bitsRotulo() << " bits"
              << "  indice=" << cache_.bitsIndice() << " bits"
              << "  offset=" << cache_.bitsOffset() << " bits\n\n";
}

// Ex.:    5  R 0x10000080  rot=0x0200001  idx=0   off=0   FALTA  compulsoria expulsa 0x10000000
void Simulador::imprimirAcesso(const ResultadoAcesso& r) const {
    int digitosRotulo = (cache_.bitsRotulo() + 3) / 4;  // ex.: 25 bits -> 7 digitos hex
    char operacao = (r.operacao == Operacao::Leitura) ? 'R' : 'W';

    printf("%4d  %c 0x%08X  rot=0x%0*X  idx=%-3u off=%-3u ", (int)r.numero, operacao,
           r.endereco, digitosRotulo, r.rotulo, r.indice, r.offset);

    if (r.acerto) {
        printf("ACERTO\n");
        return;
    }

    const char* tipo = "compulsoria";
    if (r.tipoFalta == TipoFalta::Conflito) tipo = "conflito";
    if (r.tipoFalta == TipoFalta::Capacidade) tipo = "capacidade";

    if (r.houveExpulsao) {
        printf("FALTA  %-11s expulsa 0x%08X%s\n", tipo, r.enderecoExpulso,
               r.houveWriteback ? " (writeback)" : "");
    } else {
        printf("FALTA  %s\n", tipo);
    }
}

void Simulador::imprimirResumo() const {
    const Estatisticas& e = estat_;
    double taxa = (e.acessos > 0) ? 100.0 * e.acertos / e.acessos : 0.0;  // traco vazio

    if (!config_.apenasResumo) {
        std::cout << "\n";
    }
    std::cout << "acessos ............. " << e.acessos << "\n";
    std::cout << "acertos ............. " << e.acertos << "\n";
    std::cout << "faltas .............. " << e.faltas
              << "  (compulsorias " << e.faltasCompulsorias
              << ", conflito " << e.faltasConflito
              << ", capacidade " << e.faltasCapacidade << ")\n";
    std::cout << "taxa de acerto ...... " << std::fixed << std::setprecision(2) << taxa << "%\n";
    std::cout << "blocos lidos da mem.  " << e.blocosLidos
              << "  (" << e.blocosLidos * config_.tamanhoBloco << " B)\n";

    if (config_.politica == Politica::WriteBack) {
        std::cout << "escritas na memoria  " << e.writebacks + e.descargaFinal
                  << "  (" << e.bytesEscritos << " B)"
                  << "  [writebacks " << e.writebacks
                  << " + descarga final " << e.descargaFinal << "]\n";
    } else {
        std::cout << "escritas na memoria  " << e.escritasWriteThrough
                  << "  (" << e.bytesEscritos << " B)\n";
    }
}
