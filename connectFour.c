#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define LINHAS 6
#define COLUNAS 7

int tabuleiro[LINHAS][COLUNAS] = {0};

typedef struct disco
{
    int id_time;
    SDL_Rect box;
    int posInit_X;
    int posInit_Y;
} disco;

SDL_Rect botao_coluna[COLUNAS];  // Definindo um array de botões para cada coluna

// Função para verificar se o clique está dentro de algum botão de coluna
int clicouEmBotaoColuna(int posMouseX, SDL_Rect *botao_coluna);

// Função para fazer o disco "cair" na coluna selecionada
void cairDisco(int coluna, int time, SDL_Rect *tabuleiroRect, SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_amarela);

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* janela = SDL_CreateWindow("Teste Connect 4 - G.D.M.", 100, 100, 1500, 1024, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);

    SDL_Texture *tabuleiro_texture = IMG_LoadTexture(renderer, "./imgs/jogo_tabuleiro.png");
    SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./imgs/ficha_vermelha.png");
    SDL_Texture *ficha_amarela = IMG_LoadTexture(renderer, "./imgs/ficha_amarela.png");

    SDL_Rect tabuleiroRect;
    tabuleiroRect.x = 200; tabuleiroRect.y = 150; tabuleiroRect.w = 1108; tabuleiroRect.h = 887;

    // Inicializa a posição dos botões de coluna
    for (int i = 0; i < COLUNAS; i++) {
        botao_coluna[i].x = tabuleiroRect.x + (i * (tabuleiroRect.w / COLUNAS));
        botao_coluna[i].y = tabuleiroRect.y - 40; // Acima do tabuleiro
        botao_coluna[i].w = tabuleiroRect.w / COLUNAS;
        botao_coluna[i].h = 40;  // Altura do botão
    }

    int determinaVez = 0;
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
                    cairDisco(coluna, determinaVez, &tabuleiroRect, renderer, ficha_vermelha, ficha_amarela);
                    determinaVez = (determinaVez + 1) % 2;  // Alterna a vez entre os jogadores
                }
                else
                {
                    printf("Clique fora dos botões!\n");
                }
            }
        }
        
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, tabuleiro_texture, NULL, &tabuleiroRect);

        // Renderiza os botões de cada coluna
        for (int i = 0; i < COLUNAS; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Cor branca para os botões
            SDL_RenderFillRect(renderer, &botao_coluna[i]);
        }

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

void cairDisco(int coluna, int time, SDL_Rect *tabuleiroRect, SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_amarela)
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
        tabuleiro[linha][coluna] = time + 1; // 1 para Amarelo e 2 para Vermelho

        SDL_Rect discoRect;
        discoRect.w = tabuleiroRect->w / COLUNAS;
        discoRect.h = tabuleiroRect->h / LINHAS;
        discoRect.x = tabuleiroRect->x + coluna * discoRect.w;
        discoRect.y = tabuleiroRect->y + linha * discoRect.h;

        if (time == 0)
        {
            SDL_RenderCopy(renderer, ficha_amarela, NULL, &discoRect);
        }
        else
        {
            SDL_RenderCopy(renderer, ficha_vermelha, NULL, &discoRect);
        }
    }
}
