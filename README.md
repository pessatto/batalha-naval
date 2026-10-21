# Batalha Naval

Jogo de Batalha Naval no terminal, escrito em C, em que o Jogador enfrenta o computador (PC).

Projeto criado como **prática de lógica de programação**, usando apenas os conceitos básicos da linguagem:

- estrutura sequencial
- estruturas condicionais (`if`/`else`)
- estruturas de repetição (`for`, `while`, `do-while`)
- vetores e matrizes
- alocação dinâmica de memória (`malloc` e `free`)
- funções

## Como jogar

- Cada lado tem um tabuleiro de **5 linhas (A–E) × 8 colunas (0–7)** com **7 navios** posicionados aleatoriamente. Cada navio ocupa uma casa. Esses valores podem ser alterados nas constantes `LINHAS`, `COLUNAS` e `NAVIOS`, no início de `batalha_naval.c`.
- O Jogador e o PC atiram alternadamente. O Jogador digita a linha e a coluna do tiro, por exemplo `E7` (maiúscula ou minúscula).
- Jogadas fora do tabuleiro, em formato inválido ou em uma casa já atingida são recusadas, e o jogo pede outra.
- O PC sorteia seus tiros, sem repetir casas.
- Vence quem afundar os 7 navios do adversário primeiro.

### Legenda

| Símbolo | Significado |
|:-:|---|
| `@` | Navio (só aparece no tabuleiro do Jogador) |
| `*` | Navio atingido |
| `-` | Tiro na água |

A tela mostra o tabuleiro do PC em cima, com os navios escondidos, e o do Jogador embaixo:

```
  0 1 2 3 4 5 6 7
A| | | | | | | | |
B| | |-| | | | | |
C| | | | |*| | | |
D| | | | | | | | |
E| | | | | | | | |
==================
A|@| |@| | |@| | |
B| | | | | | |*| |
C| | |-| | | | |@|
D| | | | | |@| | |
E| | | | | | | |@|
  0 1 2 3 4 5 6 7
Placar - Jogador: 1/7  PC: 1/7
Jogada (ex: E7):
```

## Como executar

É preciso ter um compilador C instalado (`gcc` ou `clang`).

```bash
gcc batalha_naval.c -o batalha_naval
./batalha_naval
```

No Windows, o executável gerado é `batalha_naval.exe`.

## Estrutura do código

Todo o jogo está em [`batalha_naval.c`](batalha_naval.c):

| Função | O que faz |
|---|---|
| `cria_tabuleiro` | Aloca dinamicamente a matriz do tabuleiro |
| `init_tabuleiro` | Preenche o tabuleiro com água |
| `sorteia` | Sorteia um número entre um mínimo e um máximo. É usada para posicionar os navios e para escolher os tiros do PC |
| `sorteia_tabuleiro` | Posiciona os navios em casas aleatórias |
| `mostra_tabuleiro` / `screen` | Desenha os tabuleiros na tela |
| `le_jogada` | Lê e valida a jogada do Jogador |
| `atira` | Dá um tiro em uma casa e diz se acertou um navio, caiu na água ou se a casa já tinha sido atingida |
| `mostra_tiro` | Mostra o resultado de cada tiro |
| `libera_tabuleiro` | Libera a memória alocada |

## Autor

João Pedro Mendonça Pessatto · [github.com/pessatto](https://github.com/pessatto)
