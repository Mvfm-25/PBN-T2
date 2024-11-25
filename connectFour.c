#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define LINHAS 6
#define COLUNAS 7

int tabuleiro[LINHAS][COLUNAS] = {0};  // Tabuleiro inicializado com zeros

SDL_Rect botao_coluna[COLUNAS];  // Definindo um array de botões para cada coluna

// Função para verificar se o clique está dentro de algum botão de coluna
int clicouEmBotaoColuna(int posMouseX, SDL_Rect *botao_coluna);

// Função para fazer o disco "cair" na coluna selecionada
void cairDisco(int coluna, int time);

// Função para renderizar o tabuleiro com todas as fichas já jogadas
void renderizarTabuleiro(SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul, SDL_Rect *tabuleiroRect);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* janela = SDL_CreateWindow("Teste Connect 4 - G.D.M.", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);

    // Melhorando a representação do tabuleiro, não se preocupar com isso por enquanto.
    // Focar no comportamento das fichas por enquanto.
    SDL_Texture *tabuleiro_texture = IMG_LoadTexture(renderer, "./img/boardQuad2.png");
    SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./img/fichaVermelha2.png");
    SDL_Texture *ficha_azul = IMG_LoadTexture(renderer, "./img/fichaAzul2.png");

    SDL_Rect tabuleiroRect;
    tabuleiroRect.x = 50;
    tabuleiroRect.y = 50;
    tabuleiroRect.w = 700;
    tabuleiroRect.h = 600;

    // Inicializa a posição dos botões de coluna
    for (int i = 0; i < COLUNAS; i++) {
        botao_coluna[i].x = tabuleiroRect.x + (i * (tabuleiroRect.w / COLUNAS));
        botao_coluna[i].y = tabuleiroRect.y; // Acima do tabuleiro
        botao_coluna[i].w = tabuleiroRect.w / COLUNAS;
        botao_coluna[i].h = 40;  // Altura do botão
    }

    int determinaVez = 0;  // 0 para ficha azul, 1 para ficha vermelha
    bool jogoAtivo = true;

    while(jogoAtivo)
    {
        SDL_Event evento;
        while(SDL_PollEvent(&evento))
        {
            if(evento.type == SDL_QUIT)
            {
                jogoAtivo = false;
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_Quit();
            }
            else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
            {
                int mouse_x = evento.button.x;
                int mouse_y = evento.button.y;
                
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);

                // Verifica em qual coluna o jogador clicou
                int coluna = clicouEmBotaoColuna(mouse_x, botao_coluna);
                if (coluna != -1)
                {
                    printf("Coluna selecionada: %d\n", coluna);
                    cairDisco(coluna, determinaVez);
                    determinaVez = (determinaVez + 1) % 2;  // Alterna a vez entre os jogadores
                }
                else
                {
                    printf("Clique fora dos botões!\n");
                }
            }
        }

        // Renderiza o tabuleiro e as fichas jogadas
        SDL_RenderClear(renderer);

        // Renderiza as fichas no tabuleiro
        renderizarTabuleiro(renderer, ficha_vermelha, ficha_azul, &tabuleiroRect);

        // Renderiza o tabuleiro por último, cobrindo as fichas parcialmente
        SDL_RenderCopy(renderer, tabuleiro_texture, NULL, &tabuleiroRect);

        SDL_RenderPresent(renderer);
    }

    return 0;
}

// Função para verificar se o clique foi em uma das colunas
int clicouEmBotaoColuna(int posMouseX, SDL_Rect *botao_coluna)
{
    for (int i = 0; i < COLUNAS; i++) {
        if (posMouseX > botao_coluna[i].x && posMouseX < botao_coluna[i].x + botao_coluna[i].w) {
            return i;  // Retorna a coluna clicada
        }
    }
    return -1;  // Retorna -1 se não clicou em nenhum botão válido
}

// Função para simular o disco caindo e registrar o movimento no tabuleiro
void cairDisco(int coluna, int time)
{
    int linha = -1;
    for (int i = LINHAS - 1; i >= 0; i--)
    {
        if (tabuleiro[i][coluna] == 0)  // Verifica se a célula está vazia
        {
            linha = i;
            break;
        }
    }

    if (linha != -1)
    {
        tabuleiro[linha][coluna] = time + 1; // 1 para Azul e 2 para Vermelho
    }
}


// Função para renderizar o tabuleiro com todas as fichas já jogadas
void renderizarTabuleiro(SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul, SDL_Rect *tabuleiroRect)
{
    SDL_Rect discoRect;

    int disco_w = tabuleiroRect->w / COLUNAS;
    int disco_h = tabuleiroRect->h / LINHAS;

    discoRect.w = disco_w * 0.9;  // 90% do tamanho da célula
    discoRect.h = disco_h * 0.9;  // 90% do tamanho da célula

    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            if (tabuleiro[i][j] != 0)  // Se houver uma ficha na posição
            {
                // Corrigido para centralizar
                discoRect.x = tabuleiroRect->x + j * disco_w + (disco_w - discoRect.w) / 2;
                discoRect.y = tabuleiroRect->y + i * disco_h + (disco_h - discoRect.h) / 2;


                if (tabuleiro[i][j] == 1)  // Ficha azul
                {
                    SDL_RenderCopy(renderer, ficha_azul, NULL, &discoRect);
                }
                else if (tabuleiro[i][j] == 2)  // Ficha vermelha
                {
                    SDL_RenderCopy(renderer, ficha_vermelha, NULL, &discoRect);
                }
            }
        }
    }
}
