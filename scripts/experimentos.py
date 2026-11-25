# experimentos.py - roda todas as configuracoes dos experimentos E1 a E6 (e Desafio),
# salva a saida completa de cada execucao em resultados/ e monta as tabelas do relatorio.
# Uso: python3 scripts/experimentos.py          (todos)
#      python3 scripts/experimentos.py E1 E3    (so alguns)

import os
import re
import subprocess
import sys

RAIZ = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
SIMULADOR = os.path.join(RAIZ, "simulador")
EXPERIMENTOS = os.path.join(RAIZ, "experimentos")
RESULTADOS = os.path.join(RAIZ, "resultados")

BLOCOS = [4, 8, 16, 32, 64]
VIAS = [1, 2, 4, 16]

# Colunas de cada tipo de tabela (iguais as do PDF)
COLUNAS_BLOCO = ["Bloco (B)", "Acertos", "Taxa de acerto", "Compuls.", "Conflito", "Capac.", "Bytes lidos"]
COLUNAS_VIAS = ["Vias", "Acertos", "Taxa de acerto", "Compuls.", "Conflito", "Capac."]
COLUNAS_ESCRITA = ["Politica", "Acertos", "Taxa de acerto", "Compuls.", "Conflito", "Capac.",
                   "Escritas", "Bytes escritos", "Bytes lidos"]

# Cada tabela: (experimento, titulo, traco, colunas, lista de (rotulo da linha, c, b, v, p))
TABELAS = [
    ("E1", "E1 - Tamanho do bloco numa varredura", "E1_varredura.txt", COLUNAS_BLOCO,
     [(b, 256, b, 1, "WB") for b in BLOCOS]),
    ("E2", "E2 - Tamanho do bloco com dados espalhados", "E2_registros.txt", COLUNAS_BLOCO,
     [(b, 256, b, 1, "WB") for b in BLOCOS]),
    ("E3", "E3 - Associatividade contra conflito", "E3_dois_vetores.txt", COLUNAS_VIAS,
     [(v, 256, 16, v, "WB") for v in VIAS]),
    ("E3", "E3 - Traco modificado (1 via)", "E3_modificado.txt", COLUNAS_VIAS,
     [(1, 256, 16, 1, "WB")]),
    ("E4", "E4 - Associatividade numa varredura", "E1_varredura.txt", COLUNAS_VIAS,
     [(v, 256, 16, v, "WB") for v in VIAS]),
    ("E5", "E5 - Laco de 80 palavras", "E5_laco_80.txt", COLUNAS_VIAS,
     [(v, 256, 16, v, "WB") for v in VIAS]),
    ("E5", "E5 - Laco de 60 palavras", "E5_laco_60.txt", COLUNAS_VIAS,
     [(v, 256, 16, v, "WB") for v in VIAS]),
    ("E6", "E6 - Acumulador", "E6_acumulador.txt", COLUNAS_ESCRITA,
     [(p, 256, 16, 2, p) for p in ["WT", "WB"]]),
    ("E6", "E6 - Copia", "E6_copia.txt", COLUNAS_ESCRITA,
     [(p, 256, 16, 2, p) for p in ["WT", "WB"]]),
    ("E6", "E6 - Escritas esparsas", "E6_esparsa.txt", COLUNAS_ESCRITA,
     [(p, 256, 16, 2, p) for p in ["WT", "WB"]]),
    ("Desafio", "Desafio", "desafio.txt", COLUNAS_VIAS,
     [(v, 256, 16, v, "WB") for v in [1, 2, 4]]),
]


# Roda o simulador e devolve a saida completa.
def rodar(traco, c, b, v, p):
    comando = [SIMULADOR, traco, "-c", str(c), "-b", str(b), "-v", str(v), "-p", p]
    resultado = subprocess.run(comando, capture_output=True, text=True)
    if resultado.returncode != 0:
        print(resultado.stderr)
        sys.exit(1)
    return resultado.stdout


# Le os numeros do resumo no fim da saida.
def ler_resumo(saida):
    faltas = re.search(r"compulsorias (\d+), conflito (\d+), capacidade (\d+)", saida)
    lidos = re.search(r"blocos lidos da mem\.\s+\d+\s+\((\d+) B\)", saida)
    escritas = re.search(r"escritas na memoria\s+(\d+)\s+\((\d+) B\)", saida)
    return {
        "Acertos": re.search(r"acertos \.+ (\d+)", saida).group(1),
        "Taxa de acerto": re.search(r"taxa de acerto \.+ ([\d.]+%)", saida).group(1),
        "Compuls.": faltas.group(1),
        "Conflito": faltas.group(2),
        "Capac.": faltas.group(3),
        "Bytes lidos": lidos.group(1),
        "Escritas": escritas.group(1),
        "Bytes escritos": escritas.group(2),
    }


# Monta uma tabela em Markdown.
def tabela_markdown(colunas, linhas):
    texto = "| " + " | ".join(colunas) + " |\n"
    texto += "|" + "---|" * len(colunas) + "\n"
    for linha in linhas:
        texto += "| " + " | ".join(str(x) for x in linha) + " |\n"
    return texto


def main():
    if subprocess.run(["make", "-s"], cwd=RAIZ).returncode != 0:
        sys.exit(1)

    escolhidos = sys.argv[1:]  # vazio = todos
    relatorio = ""

    for experimento, titulo, traco, colunas, configuracoes in TABELAS:
        if escolhidos and experimento not in escolhidos:
            continue

        caminho_traco = os.path.join(EXPERIMENTOS, traco)
        if not os.path.exists(caminho_traco):
            print(f"[pulando] {titulo}: {traco} ainda nao existe em experimentos/\n")
            continue

        pasta = os.path.join(RESULTADOS, experimento)
        os.makedirs(pasta, exist_ok=True)

        linhas = []
        for rotulo, c, b, v, p in configuracoes:
            saida = rodar(caminho_traco, c, b, v, p)

            # Saida completa (por acesso), para citar os acessos no relatorio
            nome = f"{os.path.splitext(traco)[0]}_c{c}_b{b}_v{v}_{p}.txt"
            with open(os.path.join(pasta, nome), "w") as f:
                f.write(saida)

            numeros = ler_resumo(saida)
            linhas.append([rotulo] + [numeros[col] for col in colunas[1:]])

        bloco = f"## {titulo}\nTraco: {traco}\n\n" + tabela_markdown(colunas, linhas)
        print(bloco)
        relatorio += bloco + "\n"

    # So grava o arquivo de tabelas quando roda todos (para nao apagar os outros)
    if not escolhidos:
        with open(os.path.join(RESULTADOS, "tabelas.md"), "w") as f:
            f.write(relatorio)
        print("Tabelas salvas em resultados/tabelas.md")
    print("Saidas completas salvas em resultados/<experimento>/")


if __name__ == "__main__":
    main()
