## E1 - Tamanho do bloco numa varredura
Traco: E1_varredura.txt

| Bloco (B) | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. | Bytes lidos |
|---|---|---|---|---|---|---|
| 4 | 0 | 0.00% | 128 | 0 | 0 | 512 |
| 8 | 64 | 50.00% | 64 | 0 | 0 | 512 |
| 16 | 96 | 75.00% | 32 | 0 | 0 | 512 |
| 32 | 112 | 87.50% | 16 | 0 | 0 | 512 |
| 64 | 120 | 93.75% | 8 | 0 | 0 | 512 |

## E2 - Tamanho do bloco com dados espalhados
Traco: E2_registros.txt

| Bloco (B) | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. | Bytes lidos |
|---|---|---|---|---|---|---|
| 4 | 48 | 75.00% | 16 | 0 | 0 | 64 |
| 8 | 48 | 75.00% | 16 | 0 | 0 | 128 |
| 16 | 24 | 37.50% | 16 | 24 | 0 | 640 |
| 32 | 0 | 0.00% | 16 | 0 | 48 | 2048 |
| 64 | 0 | 0.00% | 16 | 0 | 48 | 4096 |

## E3 - Associatividade contra conflito
Traco: E3_dois_vetores.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 0 | 0.00% | 16 | 48 | 0 |
| 2 | 48 | 75.00% | 16 | 0 | 0 |
| 4 | 48 | 75.00% | 16 | 0 | 0 |
| 16 | 48 | 75.00% | 16 | 0 | 0 |

## E3 - Traco modificado (1 via)
Traco: E3_modificado.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 48 | 75.00% | 16 | 0 | 0 |

## E4 - Associatividade numa varredura
Traco: E1_varredura.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 96 | 75.00% | 32 | 0 | 0 |
| 2 | 96 | 75.00% | 32 | 0 | 0 |
| 4 | 96 | 75.00% | 32 | 0 | 0 |
| 16 | 96 | 75.00% | 32 | 0 | 0 |

## E5 - Laco de 80 palavras
Traco: E5_laco_80.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 204 | 85.00% | 20 | 0 | 16 |
| 2 | 196 | 81.67% | 20 | 0 | 24 |
| 4 | 180 | 75.00% | 20 | 0 | 40 |
| 16 | 180 | 75.00% | 20 | 0 | 40 |

## E5 - Laco de 60 palavras
Traco: E5_laco_60.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 165 | 91.67% | 15 | 0 | 0 |
| 2 | 165 | 91.67% | 15 | 0 | 0 |
| 4 | 165 | 91.67% | 15 | 0 | 0 |
| 16 | 165 | 91.67% | 15 | 0 | 0 |

## E6 - Acumulador
Traco: E6_acumulador.txt

| Politica | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. | Escritas | Bytes escritos | Bytes lidos |
|---|---|---|---|---|---|---|---|---|
| WT | 99 | 99.00% | 1 | 0 | 0 | 50 | 200 | 16 |
| WB | 99 | 99.00% | 1 | 0 | 0 | 1 | 16 | 16 |

## E6 - Copia
Traco: E6_copia.txt

| Politica | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. | Escritas | Bytes escritos | Bytes lidos |
|---|---|---|---|---|---|---|---|---|
| WT | 96 | 75.00% | 32 | 0 | 0 | 64 | 256 | 512 |
| WB | 96 | 75.00% | 32 | 0 | 0 | 16 | 256 | 512 |

## E6 - Escritas esparsas
Traco: E6_esparsa.txt

| Politica | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. | Escritas | Bytes escritos | Bytes lidos |
|---|---|---|---|---|---|---|---|---|
| WT | 0 | 0.00% | 32 | 0 | 0 | 32 | 128 | 512 |
| WB | 0 | 0.00% | 32 | 0 | 0 | 32 | 512 | 512 |

## Desafio
Traco: desafio.txt

| Vias | Acertos | Taxa de acerto | Compuls. | Conflito | Capac. |
|---|---|---|---|---|---|
| 1 | 0 | 0.00% | 4 | 28 | 0 |
| 2 | 0 | 0.00% | 4 | 28 | 0 |
| 4 | 28 | 87.50% | 4 | 0 | 0 |

