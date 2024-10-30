//Connect4 v.0001
//UA : 301024 - [mvfm]

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

//Struct pra os discos do tabuleiro, REFINAR DPS.
typedef struct disco
{
	SDL_Rect box;
	int posInit_X; int posInit_Y;
} disco;

// Menu bem básico, antes da inicialização do tabuleiro. Determina os modos de jogo Player X Player, Player X CPU.
int menuPrincipal();

// Identifica se o jogador clicou em uma área fora dos limites do tabuleiro, descartando jogadas inválidas.
//	Retorna 1 se a jogada é considera inválida, 0 caso contrário.
int foraDosLimites(int posMouseX, int posMouseY, SDL_Rect tabuleiro);



void main()
{
	//Inicialização de tudo, criação da Janela e determinação das texturas.
	SDL_Init(SDL_INIT_EVERYTHING);  
	SDL_Window* janela = SDL_CreateWindow("Teste Connect 4 - G.D.M.", 100, 100, 1500, 1024, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);
	SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./img/jogo_tabuleiro.png");
	SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./img/ficha_vermelha.png");
	SDL_Texture *ficha_amarela = IMG_LoadTexture(renderer, "./img/ficha_amarela.png");
  
 	// Criação própria do tabuleiro. 
 	// A altura e a largura são exatamente iguais às da imagem. FAVOR NÃO MEXER!
 	SDL_Rect tabuleiro;
 	tabuleiro.x = 200; tabuleiro.y = 150; tabuleiro.w = 1108; tabuleiro.h = 887;
 	
 	//Coisas do fundo.
 	SDL_RenderClear(renderer);
 	SDL_SetRenderDrawColor(renderer,23,20,33,255);
 	
 	// Determinações para o WAVLoader. Mesmas que c4.c
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("./wavs/secret.wav", &wavSpec, &wavBuffer, &wavLength);
 	
 	//Posições para 'preview' dos discos.
 	int discoPosInit_X = 275;
 	int discoPosInit_Y = 50;
 	
 	// 0 - Amarelo
 	// 1 - Vermelho
 	int determinaVez = 0;
 	
 	while(true)
 	{
 		//EventListener.
 		SDL_Event evento;
 		while(SDL_PollEvent(&evento))
 		{
 			//Clicado o 'X' vermelho gigante para matar o processo.
 			if(evento.type == SDL)_QUIT)
 			{
 				SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_FreeWAV(wavBuffer);
                SDL_Quit();
                return 0;
 			}else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
 			{
 				//Registra a posição clicada.
 				int mouse_x = event.button.x;
                int mouse_y = event.button.y;
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);
                
                //Toca o áudio ao clicar.
                SDL_QueueAudio(deviceId, wavBuffer, wavLength); 
                SDL_PauseAudioDevice(deviceId, 0);
 			}
 			
 		}
 	}
}
