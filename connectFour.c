#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LINHAS 6
#define COLUNAS 7

int tabuleiro[LINHAS][COLUNAS] = {0};  // Tabuleiro inicializado com zeros
SDL_Rect botao_coluna[COLUNAS];  // Definindo um array de botões para cada coluna
int ultimaJogadaUsuario = -1;    // Armazena a última jogada do jogador humano

// Função para verificar se o clique está dentro de algum botão de coluna
int clicouEmBotaoColuna(int posMouseX, SDL_Rect *botao_coluna);

// Função para fazer o disco "cair" na coluna selecionada
void cairDisco(int coluna, int time);

// Função para a jogada da CPU
int jogadaCPU();

// Função para renderizar os discos jogados
void renderizarDiscos(SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul);

// Função recursiva para verificar a vitória
bool verificarVitoriaRecursiva(int linha, int coluna, int jogador, int direcaoX, int direcaoY, int contador);

// Função para verificar a vitória em todas as direções
bool verificarVitoria(int linha, int coluna, int jogador);

// Função principal para verificar se há um vencedor
bool verificarVencedor();

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* janela = SDL_CreateWindow("Connect 4 - G.D.M.", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);

    SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./img/fichaVermelha2.png");
    SDL_Texture *ficha_azul = IMG_LoadTexture(renderer, "./img/fichaAzul2.png");

    int window_width = 800;
    int window_height = 600;

    // Calcula as dimensões para os botões
    int botao_width = window_width / COLUNAS - 10;  // largura de cada botão com espaçamento
    int botao_height = 40;  // altura do botão

    // Inicializa a posição dos botões de coluna
    for (int i = 0; i < COLUNAS; i++) {
        botao_coluna[i].x = 50 + i * (botao_width + 10);
        botao_coluna[i].y = 50; // Acima do tabuleiro
        botao_coluna[i].w = botao_width;
        botao_coluna[i].h = botao_height;
    }

    int determinaVez = 0;  // 0 para ficha azul, 1 para ficha vermelha
    bool jogoAtivo = true;

    // Modo de jogo: 1 = Jogador x Jogador, 2 = Jogador x CPU
    int modoDeJogo;
    printf("Escolha o modo de jogo:\n1 - Jogador x Jogador\n2 - Jogador x CPU\n");
    scanf("%d", &modoDeJogo);

    while (jogoAtivo)
    {
        SDL_Event evento;
        while (SDL_PollEvent(&evento))
        {
            if (evento.type == SDL_QUIT)
            {
                jogoAtivo = false;
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_Quit();
            }
            else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT && determinaVez == 0)
            {
                int mouse_x = evento.button.x;

                // Verifica em qual coluna o jogador clicou
                int coluna = clicouEmBotaoColuna(mouse_x, botao_coluna);
                if (coluna != -1)
                {
                    printf("Coluna selecionada: %d\n", coluna);
                    cairDisco(coluna, determinaVez);

                    // Verifica se houve um vencedor
                    if (verificarVencedor())
                    {
                        printf("Fim de jogo! Jogador %d venceu!\n", determinaVez + 1);
                        SDL_Delay(5000);
                        jogoAtivo = false;
                    }

                    // Alterna a vez entre os jogadores
                    determinaVez = (determinaVez + 1) % 2;
                }
                else
                {
                    printf("Clique fora dos botões!\n");
                }
            }
        }

        // Jogada da CPU
        if (modoDeJogo == 2 && determinaVez == 1)
        {
            int coluna = jogadaCPU();
            if (coluna != -1) // Garantia de jogada válida
            {
                cairDisco(coluna, determinaVez);
                printf("CPU jogou na coluna %d\n", coluna);

                if (verificarVencedor())
                {
                    printf("Fim de jogo! Jogador %d venceu!\n", determinaVez + 1);
                    SDL_Delay(5000);
                    jogoAtivo = false;
                }

                determinaVez = (determinaVez + 1) % 2;
            }
        }

        // Renderiza o tabuleiro e as fichas jogadas
        SDL_RenderClear(renderer);

        // Renderiza as fichas no tabuleiro
        renderizarDiscos(renderer, ficha_vermelha, ficha_azul);

        SDL_RenderPresent(renderer);
    }

    return 0;
}

int clicouEmBotaoColuna(int posMouseX, SDL_Rect *botao_coluna)
{
    for (int i = 0; i < COLUNAS; i++) {
        if (posMouseX > botao_coluna[i].x && posMouseX < botao_coluna[i].x + botao_coluna[i].w) {
            return i;  // Retorna a coluna clicada
        }
    }
    return -1;  // Retorna -1 se não clicou em nenhum botão válido
}

void cairDisco(int coluna, int time)
{
    for (int i = LINHAS - 1; i >= 0; i--)
    {
        if (tabuleiro[i][coluna] == 0)
        {
            tabuleiro[i][coluna] = time + 1; // 1 para Azul e 2 para Vermelho
            if (time == 0) // Atualiza a última jogada do jogador humano
                ultimaJogadaUsuario = coluna;
            break;
        }
    }
}

int jogadaCPU()
{
    if (ultimaJogadaUsuario == -1)
    {
        printf("Aguardando jogada do usuário...\n");
        return -1;
    }

    int coluna = ultimaJogadaUsuario;
    if (tabuleiro[0][coluna] != 0)
    {
        if (coluna > 0 && tabuleiro[0][coluna - 1] == 0) {
            coluna = coluna - 1;
        } else if (coluna < COLUNAS - 1 && tabuleiro[0][coluna + 1] == 0) {
            coluna = coluna + 1;
        } else {
            do {
                coluna = rand() % COLUNAS;
            } while (tabuleiro[0][coluna] != 0);
        }
    }

    return coluna;
}

void renderizarDiscos(SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul)
{
    SDL_Rect discoRect;
    discoRect.w = 80;
    discoRect.h = 80;

    int offsetX = 50;
    int offsetY = 100;
    int spacingX = 90;
    int spacingY = 90;

    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (tabuleiro[i][j] != 0)
            {
                discoRect.x = offsetX + j * spacingX;
                discoRect.y = offsetY + i * spacingY;

                if (tabuleiro[i][j] == 1)
                {
                    SDL_RenderCopy(renderer, ficha_azul, NULL, &discoRect);
                }
                else if (tabuleiro[i][j] == 2)
                {
                    SDL_RenderCopy(renderer, ficha_vermelha, NULL, &discoRect);
                }
            }
        }
    }
}

bool verificarVitoriaRecursiva(int linha, int coluna, int jogador, int direcaoX, int direcaoY, int contador)
{
    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS)
        return false;

    if (tabuleiro[linha][coluna] != jogador)
        return false;

    contador++;
    if (contador == 4)
        return true;

    return verificarVitoriaRecursiva(linha + direcaoX, coluna + direcaoY, jogador, direcaoX, direcaoY, contador);
}

bool verificarVitoria(int linha, int coluna, int jogador)
{
    int direcoes[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (int i = 0; i < 4; i++)
    {
        if (verificarVitoriaRecursiva(linha, coluna, jogador, direcoes[i][0], direcoes[i][1], 0))
            return true;
    }
    return false;
}

bool verificarVencedor()
{
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (tabuleiro[i][j] != 0 && verificarVitoria(i, j, tabuleiro[i][j]))
            {
                return true;
            }
        }
    }
    return false;
}
