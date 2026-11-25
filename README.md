# Simulador de Cache — Organização de Computadores

**Autores:** Eduardo Spezia e João Arnoldo Eger
**Disciplina:** Organização de Computadores — UNIVALI (Prof. Thiago Felski Pereira)

## Objetivo

Simulador de cache de dados usado como instrumento de medição: lê um traço de
acessos à memória (leituras e escritas) e simula uma cache configurável, para
estudar o efeito do tamanho do bloco, da associatividade e da política de
escrita (experimentos E1 a E6 e Desafio).

O simulador segue as regras da atividade: endereços de 32 bits, palavra de 4 bytes,
divisão rótulo | índice | offset, substituição LRU, write-allocate nas duas
políticas, Write-Through, Write-Back com bit de modificação e descarga final, e
classificação das faltas em compulsória, capacidade e conflito (usando uma segunda
cache totalmente associativa LRU que roda em paralelo).

## Linguagem e requisitos

- C++17, somente biblioteca padrão (STL), programa de console.
- Compilador GCC ou MinGW (`g++`) e `make`.
  No Windows com MinGW, use `mingw32-make` no lugar de `make`.
- Python 3 (somente para os scripts de calibração e de experimentos).

## Estrutura

```
SimuladorCache/
├── src/                 código-fonte (main, Cache, Bloco, Simulador, LeitorTracos)
│   └── headers/         arquivos .h
├── calibracao/          traços C1 a C4 e respostas esperadas
├── experimentos/        traços E1 a E6, traço do E3 modificado e traço do Desafio
├── resultados/          saídas completas de cada execução e tabelas
├── relatorio/           relatório final
├── scripts/             calibrar.py e experimentos.py
├── Makefile
└── README.md
```

## Compilar

```
make
```

Gera o executável `simulador` (`make clean` remove os arquivos compilados).

## Executar

```
./simulador <traco> -c <capacidade> -b <bloco> -v <vias> -p <WB|WT> [--resumo]
```

| Opção | Significado |
|---|---|
| `-c` | capacidade da cache em bytes (somente dados) |
| `-b` | tamanho do bloco em bytes |
| `-v` | número de vias (1 = mapeamento direto; nº de blocos = totalmente associativa) |
| `-p` | política de escrita: `WB` (write-back) ou `WT` (write-through) |
| `--resumo` | mostra só o resumo final, sem a saída acesso por acesso |

Exemplo (cache de 256 B, bloco de 16 B, 2 vias, write-back):

```
./simulador calibracao/C2_associativa_LRU.txt -c 256 -b 16 -v 2 -p WB
```

Capacidade, bloco e vias precisam ser potências de 2; o bloco deve ter pelo menos
4 bytes e não pode haver mais vias do que blocos. Configurações inválidas são
recusadas com uma mensagem explicando o motivo. O mesmo traço pode ser executado
com várias configurações sem alterar o código, apenas mudando os parâmetros.

### Formato do traço

Uma linha por acesso: `R` (leitura) ou `W` (escrita) e o endereço em hexadecimal.
Tudo depois de `#` é comentário; linhas vazias são ignoradas.

```
# exemplo
R 0x10000000
W 0x10000004
```

## Calibração (C1 a C4)

```
python3 scripts/calibrar.py
```

Compila o simulador, roda os quatro traços de `calibracao/` com as configurações
da atividade (o C3 é rodado em WB e em WT) e compara com os arquivos
`*_esperado.txt`. Cada teste mostra `OK` ou a diferença encontrada.
Os arquivos esperados não devem ser alterados.

## Experimentos

```
python3 scripts/experimentos.py            # roda todos
python3 scripts/experimentos.py E1 E3      # roda só os indicados (E1..E6, Desafio)
```

O script compila o simulador e, para cada configuração, grava em
`resultados/<experimento>/` a saída completa (por acesso), usada nas justificativas
do relatório, e imprime as tabelas no formato pedido na atividade. Quando roda
todos os experimentos, as tabelas também são salvas em `resultados/tabelas.md`.
Recomenda-se rodar a calibração antes (`python3 scripts/calibrar.py`).

Traços esperados em `experimentos/`: `E1_varredura.txt`, `E2_registros.txt`,
`E3_dois_vetores.txt`, `E5_laco_80.txt`, `E5_laco_60.txt`, `E6_acumulador.txt`,
`E6_copia.txt`, `E6_esparsa.txt`. Os traços de autoria do grupo são
`desafio.txt` e `E3_modificado.txt`. Traços ausentes são apenas avisados e
pulados.

| Experimento | Configuração |
|---|---|
| E1, E2 | 256 B, 1 via, WB; bloco = 4, 8, 16, 32, 64 B |
| E3, E4, E5 | 256 B, bloco 16 B, WB; vias = 1, 2, 4, 16 |
| E6 | 256 B, bloco 16 B, 2 vias; política = WT e WB |
| Desafio | 256 B, bloco 16 B, WB; vias = 1, 2, 4 |

## Saída do simulador

Para cada acesso: número, operação, endereço, rótulo, índice, offset, acerto ou
falta, tipo da falta e, quando houver, endereço do bloco expulso e indicação de
writeback. No fim: acessos, acertos, faltas por tipo, taxa de acerto, blocos e
bytes lidos da memória, escritas e bytes escritos (no write-back, separando
writebacks durante a execução e descarga final).
