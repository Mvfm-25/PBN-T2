#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <time.h>

//Struct pra os sprites do jogo, REFINAR DPS. Deve funcionar tanto para as fichas quanto o tabuleiro.
typedef struct Sprite
{
    int id_time;
    SDL_Rect box;
    int posInit_X;
    int posInit_Y;
} Sprite;

// Identifica se o jogador clicou em uma área fora dos limites do tabuleiro, descartando jogadas inválidas.
// Retorna 1 se a jogada é considerada inválida, 0 caso contrário.
int foraDosLimites(int posMouseX, int posMouseY, SDL_Rect tabuleiro);

// Caso o determinaVez se mantenha com seu valor inicial '0' por qualquer razão quando o jogo é iniciado, a função é chamada para [...]
// Sortear quem fica com a vez atualmente. Da maneira implementada, ele sempre será chamado quando determinaVez = 0.
int sorteaVez();

// Preenche tabuleiro. Inicialmente serve apenas para determinar que espaços as fichas vão ocupar tanto no tabuleiro abstrato quanto no físico.
void preencheTabuleiro(Sprite tabuleiro[7][6], SDL_Renderer *renderer);

// Responsável pela verdadeira renderização das fichas no tabuleiro, segue as mesmas 'regras de conduta' para saber qual das fichas ele deve desenhar.
void desenhaFicha(int coluna, int jogador, Sprite tabuleiro[7][6], SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul);

//Tabuleiro abstrato.
Sprite tabuleiro[7][6];

int main()
{
    //Inicialização de tudo, criação da Janela e determinação das texturas.
    SDL_Init(SDL_INIT_EVERYTHING);  
    SDL_Window* janela = SDL_CreateWindow("Teste Connect 4 - G.D.M.", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);
    SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./img/FichaVermelha.png");
    SDL_Texture *ficha_azul = IMG_LoadTexture(renderer, "./img/FichaAzul.png");

    //Coisas do fundo.
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,23,20,33,255);

    // Determinações para o WAVLoader. Mesmas que c4.c
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("./wavs/secret.wav", &wavSpec, &wavBuffer, &wavLength);
    // 'Caminho' para o dispositivo de áudio disponível
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    
    // Tabuleiro físico (determina os limites na janela).
    SDL_Rect tabuleiroRect;
    tabuleiroRect.x = 200;
    tabuleiroRect.y = 150;
    tabuleiroRect.w = 1108;
    tabuleiroRect.h = 887;

    // Posições iniciais para 'preview' dos discos.
    int discoPosInit_X = 275;
    int discoPosInit_Y = 50;

    // 0 - Sem time determinado.
    // 1 - Vermelho
    // 2 - Azul
    int determinaVez = 0;
    srand(time(NULL));  // Inicializa o gerador de números aleatórios apenas uma vez

    // Loop principal do jogo
    while (true)
    {
        //Preenchendo tabuleiro com fichas invisíveis
        preencheTabuleiro(tabuleiro, renderer);

        // EventListener.
        SDL_Event evento;
        while (SDL_PollEvent(&evento))
        {
            // Clicado o 'X' vermelho gigante para matar o processo.
            if (evento.type == SDL_QUIT)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_FreeWAV(wavBuffer);
                SDL_Quit();
                return 0;
            }
            // Verifica a posição atual do mouse 
            else if (evento.type == SDL_MOUSEMOTION)
            {
                int mouse_x = evento.motion.x;
                int mouse_y = evento.motion.y;
                printf("Mouse em movimento: X = %d, Y = %d\n", mouse_x, mouse_y);
            }
            // Verifica se houve clique com o botão esquerdo do mouse
            else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
            {
                // Registra a posição clicada.
                int mouse_x = evento.button.x;
                int mouse_y = evento.button.y;
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);
                
                // Modularizar isso muito, muito mais. Queremos estes comportamentos funcionando para qualquer outra situação.
                // Focar principalmente no contexto de um jogo Player x CPU.
                if (foraDosLimites(mouse_x, mouse_y, tabuleiroRect))
                {
                    printf("Jogada inválida! Os turnos continuam iguais.\n");
                }
                else
                {
                    int coluna = (mouse_x - tabuleiroRect.x) / 192;  // Determinar em qual coluna foi o clique, usando o tamanho das fichas.
                    if (determinaVez == 0)
                    {
                        determinaVez = sorteaVez();
                    }
                    if (determinaVez == 1)
                    {
                        printf("Vez atual: VERMELHO\n");
                        desenhaFicha(coluna, 1, tabuleiro, renderer, ficha_vermelha, ficha_azul);
                        determinaVez = 2;  // Alterna a vez para o próximo jogador
                    }
                    else
                    {
                        printf("Vez atual: AZUL\n");
                        desenhaFicha(coluna, 2, tabuleiro, renderer, ficha_vermelha, ficha_azul);
                        determinaVez = 1;  // Alterna a vez para o próximo jogador
                    }

                    // Atualiza a tela com as mudanças
                    SDL_RenderPresent(renderer);

                    // Toca o áudio ao clicar.
                    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
                    SDL_PauseAudioDevice(deviceId, 0);
                }
            }
        }
        // Clear da página.
        SDL_RenderClear(renderer);
    }

    return 0;
}

//Bem direto o que essa função faz, seguir o nome.
int foraDosLimites(int posMouseX, int posMouseY, SDL_Rect tabuleiro)
{
    if (posMouseX > tabuleiro.x && posMouseX < (tabuleiro.x + tabuleiro.w) && posMouseY > tabuleiro.y && posMouseY < (tabuleiro.y + tabuleiro.h))
    {
        return 0;
    }
    return 1;
}

//Bem direto o que essa função faz, seguir o nome.
void preencheTabuleiro(Sprite tabuleiro[7][6], SDL_Renderer *renderer)
{
    for (int colunas = 0; colunas < 7; colunas++)
    {
        for (int linhas = 0; linhas < 6; linhas++)
        {
            // Criando Sprite local
            Sprite sprite;

            // Carregando textura inicial do sprite
            SDL_Texture *ficha = IMG_LoadTexture(renderer, "./img/FichaInvisivel.png");

            // Configurando o sprite
            sprite.id_time = 0;  // 0 significa NEUTRO
            sprite.box.w = 192;
            sprite.box.h = 192;

            // Determinando a posição no tabuleiro
            sprite.posInit_X = colunas * sprite.box.w;
            sprite.posInit_Y = linhas * sprite.box.h;

            // Salvando o sprite no tabuleiro
            tabuleiro[colunas][linhas] = sprite;
        }
    }
}

void desenhaFicha(int coluna, int jogador, Sprite tabuleiro[7][6], SDL_Renderer *renderer, SDL_Texture *ficha_vermelha, SDL_Texture *ficha_azul)
{
    // Procurar a linha mais baixa disponível na coluna
    for (int linha = 5; linha >= 0; linha--)
    {
        // Verifica se a posição está disponível (id_time == 0)
        if (tabuleiro[coluna][linha].id_time == 0)
        {
            // Atualiza o id_time dependendo do jogador (1 para vermelho, 2 para azul)
            tabuleiro[coluna][linha].id_time = jogador;

            // Determina qual textura usar para o jogador
            SDL_Texture *fichaAtual = (jogador == 1) ? ficha_vermelha : ficha_azul;

            // Configura o SDL_Rect para a posição correta da ficha
            SDL_Rect posicao = tabuleiro[coluna][linha].box;
            posicao.x = tabuleiro[coluna][linha].posInit_X;
            posicao.y = tabuleiro[coluna][linha].posInit_Y;

            // Renderiza a ficha na posição correta
            SDL_RenderCopy(renderer, fichaAtual, NULL, &posicao);

            // Saia do loop após desenhar a ficha
            break;
        }
    }
}

// Sorteia quem começa o jogo.
int sorteaVez()
{
    int rnd = rand();
    return (rnd > RAND_MAX / 2) ? 1 : 2;
}

