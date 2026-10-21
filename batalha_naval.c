#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define agua ' '
#define navio '@'
#define explosao '*'
#define falha '-'

void libera_tabuleiro(char **tab, int nl)
{
    int l;
    for (l = 0; l < nl; l++)
    {
        free(tab[l]);
    }
    free(tab);
}

// Retorna NULL se nao houver memoria suficiente
char **cria_tabuleiro(int nl, int nc)
{
    char **tab = NULL;
    int i;

    tab = (char **)malloc(nl * sizeof(char *));
    if (tab == NULL)
    {
        return NULL;
    }

    for (i = 0; i < nl; i++)
    {
        tab[i] = (char *)malloc(nc * sizeof(char));
        if (tab[i] == NULL)
        {
            libera_tabuleiro(tab, i); // libera as linhas ja alocadas
            return NULL;
        }
    }

    return tab;
}

void init_tabuleiro(char **tab, int nl, int nc)
{
    int l, c;

    for (l = 0; l < nl; l++)
    {
        for (c = 0; c < nc; c++)
        {
            tab[l][c] = agua;
        }
    }
}

int sorteia(int min, int max)
{
    int n = rand();
    return n % (max - min + 1) + min;
}

void sorteia_tabuleiro(char **tab, int nl, int nc, int nv)
{
    int n = 0;
    int l, c;

    while (n < nv)
    {
        l = sorteia(0, nl - 1);
        c = sorteia(0, nc - 1);
        if (tab[l][c] == agua)
        {
            tab[l][c] = navio;
            n++;
        }
    }
}

void mostra_tabuleiro(char **tab, int nl, int nc, int mostrar_navios)
{
    int l, c;

    if (!mostrar_navios)
    {
        printf("  ");
        for (c = 0; c < nc; c++)
        {
            printf("%d ", c);
        }
        printf("\n");
    }

    for (l = 0; l < nl; l++)
    {
        printf("%c|", 'A' + l);
        for (c = 0; c < nc; c++)
        {
            if (tab[l][c] == navio && !mostrar_navios)
            {
                printf("%c|", agua);
            }
            else
            {
                printf("%c|", tab[l][c]);
            }
        }
        printf("\n");
    }

    if (mostrar_navios)
    {
        printf("  ");
        for (c = 0; c < nc; c++)
        {
            printf("%d ", c);
        }
        printf("\n");
    }
}

void screen(char **pc, char **j, int nl, int nc, int mostrar_pc)
{
    int c;
    mostra_tabuleiro(pc, nl, nc, mostrar_pc);
    printf("==");

    for (c = 0; c < nc; c++)
    {
        printf("==");
    }
    printf("\n");

    mostra_tabuleiro(j, nl, nc, 1);
}

// Retorna 1 se acertou, 0 se errou, -1 se a posicao ja tinha sido atingida
int atira(char **tab, int l, int c)
{
    if (tab[l][c] == navio)
    {
        tab[l][c] = explosao;
        return 1;
    }
    if (tab[l][c] == agua)
    {
        tab[l][c] = falha;
        return 0;
    }
    return -1;
}

void mostra_tiro(char *quem, int l, int c, int r)
{
    printf("%s atirou em %c%d: ", quem, 'A' + l, c);
    if (r == 1)
    {
        printf("ACERTOU!\n");
    }
    else
    {
        printf("agua.\n");
    }
}

int le_jogada(char **tab, int nl, int nc, int pos[2])
{
    char ch;
    int x, digitos, fim, sobra;
    int valida = 0;

    do
    {
        printf("Jogada (ex: E7): ");
        if (scanf(" %c", &ch) == EOF)
        {
            return 0;
        }

        // Le o numero da coluna digito a digito ate o fim da linha.
        // Qualquer coisa alem de letra + numero (e espacos) torna a jogada invalida.
        pos[1] = 0;
        digitos = 0;
        fim = 0;
        sobra = 0;
        x = getchar();
        while (x != '\n' && x != EOF)
        {
            if (x >= '0' && x <= '9' && !fim)
            {
                if (pos[1] < 1000) // evita estouro do int; 1000 ja e fora do tabuleiro
                {
                    pos[1] = pos[1] * 10 + (x - '0');
                }
                digitos++;
            }
            else if (x == ' ')
            {
                if (digitos > 0)
                {
                    fim = 1;
                }
            }
            else
            {
                sobra = 1;
            }
            x = getchar();
        }

        if (ch >= 'a' && ch <= 'z')
        {
            ch = ch - 'a' + 'A';
        }
        pos[0] = ch - 'A';

        if (ch < 'A' || ch > 'Z' || digitos == 0 || sobra)
        {
            printf("Formato invalido. Use letra da linha + numero da coluna.\n");
        }
        else if (pos[0] < 0 || pos[0] >= nl || pos[1] < 0 || pos[1] >= nc)
        {
            printf("Fora do tabuleiro: linhas A-%c, colunas 0-%d.\n", 'A' + nl - 1, nc - 1);
        }
        else if (tab[pos[0]][pos[1]] == explosao || tab[pos[0]][pos[1]] == falha)
        {
            printf("Jogador ja atirou em %c%d. Escolha outra posicao.\n", 'A' + pos[0], pos[1]);
        }
        else
        {
            valida = 1;
        }
    } while (!valida);

    return 1;
}

int main()
{

    srand(time(0));

    char **tabpc = NULL;
    char **tabj = NULL;
    int nl = 5, nc = 8;
    int nv = 7; // numero de navios
    int acertosj = 0, acertospc = 0;
    int pos[2];
    int l, c, r;
    int sair = 0;

    // Sem essa verificacao, sorteia_tabuleiro ficaria em laco infinito procurando casa livre
    if (nv > nl * nc)
    {
        printf("Erro: %d navios nao cabem em um tabuleiro %dx%d.\n", nv, nl, nc);
        return 1;
    }

    tabpc = cria_tabuleiro(nl, nc);
    tabj = cria_tabuleiro(nl, nc);
    if (tabpc == NULL || tabj == NULL)
    {
        printf("Erro: memoria insuficiente.\n");
        if (tabpc != NULL)
        {
            libera_tabuleiro(tabpc, nl);
        }
        if (tabj != NULL)
        {
            libera_tabuleiro(tabj, nl);
        }
        return 1;
    }

    init_tabuleiro(tabpc, nl, nc);
    init_tabuleiro(tabj, nl, nc);

    sorteia_tabuleiro(tabpc, nl, nc, nv);
    sorteia_tabuleiro(tabj, nl, nc, nv);

    while (acertosj < nv && acertospc < nv && !sair)
    {
        printf("\n\n");
        screen(tabpc, tabj, nl, nc, 0);
        printf("Placar - Jogador: %d/%d  PC: %d/%d\n", acertosj, nv, acertospc, nv);

        if (le_jogada(tabpc, nl, nc, pos) == 0)
        {
            printf("\nJogo encerrado.\n");
            sair = 1;
        }
        else
        {
            // Jogador
            r = atira(tabpc, pos[0], pos[1]);
            mostra_tiro("Jogador", pos[0], pos[1], r);
            acertosj = acertosj + r;

            // PC sorteia so entre posicoes ainda nao atingidas
            if (acertosj < nv)
            {
                do
                {
                    l = sorteia(0, nl - 1);
                    c = sorteia(0, nc - 1);
                    r = atira(tabj, l, c);
                } while (r == -1);
                mostra_tiro("PC", l, c, r);
                acertospc = acertospc + r;
            }
        }
    }

    if (!sair)
    {
        printf("\n\n");
        screen(tabpc, tabj, nl, nc, 1);
        printf("Placar final - Jogador: %d/%d  PC: %d/%d\n", acertosj, nv, acertospc, nv);
        if (acertosj == nv)
        {
            printf("Jogador venceu!\n");
        }
        else
        {
            printf("O PC venceu!\n");
        }
    }

    libera_tabuleiro(tabpc, nl);
    libera_tabuleiro(tabj, nl);

    return 0;
}
