# calibrar.py - roda os 4 tracos de calibracao e compara com as respostas esperadas.
# Uso: python3 scripts/calibrar.py

import difflib
import os
import subprocess
import sys

RAIZ = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
SIMULADOR = os.path.join(RAIZ, "simulador")
CALIBRACAO = os.path.join(RAIZ, "calibracao")

# (nome, traco, arquivo esperado, configuracoes a rodar)
TESTES = [
    ("C1 mapeamento direto", "C1_mapeamento_direto.txt", "C1_mapeamento_direto_esperado.txt",
     [["-c", "128", "-b", "16", "-v", "1", "-p", "WB"]]),
    ("C2 associativa LRU", "C2_associativa_LRU.txt", "C2_associativa_LRU_esperado.txt",
     [["-c", "64", "-b", "16", "-v", "2", "-p", "WB"]]),
    # C3: o arquivo esperado tem a saida WB e a WT, separadas por uma linha de '='
    ("C3 escrita (WB + WT)", "C3_escrita.txt", "C3_escrita_esperado.txt",
     [["-c", "64", "-b", "16", "-v", "1", "-p", "WB"],
      ["-c", "64", "-b", "16", "-v", "1", "-p", "WT"]]),
    ("C4 classificacao", "C4_classificacao.txt", "C4_classificacao_esperado.txt",
     [["-c", "64", "-b", "16", "-v", "1", "-p", "WB"]]),
]

SEPARADOR = "\n" + "=" * 70 + "\n\n"


# Roda o simulador e devolve a saida como texto.
def rodar(traco, parametros):
    resultado = subprocess.run([SIMULADOR, traco] + parametros, capture_output=True, text=True)
    if resultado.returncode != 0:
        print(resultado.stderr)
        sys.exit(1)
    return resultado.stdout


def main():
    # Compila antes de testar
    if subprocess.run(["make", "-s"], cwd=RAIZ).returncode != 0:
        sys.exit(1)

    todos_ok = True
    for nome, traco, esperado, configuracoes in TESTES:
        caminho_traco = os.path.join(CALIBRACAO, traco)
        saida = SEPARADOR.join(rodar(caminho_traco, p) for p in configuracoes)

        with open(os.path.join(CALIBRACAO, esperado)) as f:
            resposta = f.read()

        if saida == resposta:
            print(f"{nome:22} ... OK")
        else:
            todos_ok = False
            print(f"{nome:22} ... DIFERENTE")
            diferenca = difflib.unified_diff(resposta.splitlines(), saida.splitlines(),
                                             "esperado", "simulador", lineterm="")
            print("\n".join(diferenca))

    sys.exit(0 if todos_ok else 1)


if __name__ == "__main__":
    main()
