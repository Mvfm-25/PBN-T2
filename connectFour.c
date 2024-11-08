//Connect4 v.0002 (não sei se mudei o suficiente para justificar uma mudança de versão)
//UA : 3081124 - [mvfm]

//Favor atualizar o UA com a data em que modificaste o código, assim como mudar o nome dentro dos colchetes.
//Fica bonito pra quem está lendo.


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

//Struct pra os sprites do jogo, REFINAR DPS. Deve funcionar tanto para as fichas quanto o tabuleiro.
typedef struct Sprite
{
	int id_time;
	SDL_Rect box;
	int posInit_X; int posInit_Y;
} Sprite;


// Menu bem básico, antes da inicialização do tabuleiro. Determina os modos de jogo Player X Player, Player X CPU.
//void menuPrincipal();

// Identifica se o jogador clicou em uma área fora dos limites do tabuleiro, descartando jogadas inválidas.
//	Retorna 1 se a jogada é considera inválida, 0 caso contrário.
int foraDosLimites(int posMouseX, int posMouseY, SDL_Rect tabuleiro);

// Caso o determinaVez se mantenha com seu valor inicial '0' por qualquer razão quando o jogo é iniciado, a função é chamada para [...]
//Sortear quem fica com a vez atualmente. Da maneira implementada, ele sempre será chamado quando determinaVez = 0.
int determinaVez();

// Preenche tabuleiro inicialmente serve apenas para determinar que espaços as fichas vão ocupar tanto no tabuleiro abstrato quanto no físico.
void preencheTabuliero(Sprite tabuleiro[7][6], SDL_Renderer renderer);

//Tabuleiro abstrato.
Sprite tabuleiro[7][6];

void main()
{
	//Inicialização de tudo, criação da Janela e determinação das texturas.
	SDL_Init(SDL_INIT_EVERYTHING);  
	SDL_Window* janela = SDL_CreateWindow("Teste Connect 4 - G.D.M.", 100, 100, 1500, 1024, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);
	//Esperar a criação do sprite específico
	//SDL_Texture *tabuleiro = IMG_LoadTexture(renderer, "./img/jogo_tabuleiro.png");
	SDL_Texture *ficha_vermelha = IMG_LoadTexture(renderer, "./img/FichaVermelha.png");
	SDL_Texture *ficha_azul = IMG_LoadTexture(renderer, "./img/FichaAzul.png");
  
  	//Ignorar por enquanto, não é tão essencial o quanto eu achava.
 	//SDL_Rect tabuleiroRect;
 	//tabuleiroRect.x = 200; tabuleiroRect.y = 150; tabuleiroRect.w = 1108; tabuleiroRect.h = 887;
 	
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
 	
 	//Posições para 'preview' dos discos.
 	int discoPosInit_X = 275;
 	int discoPosInit_Y = 50;
 	
 	// 0 - Sem time determinado.
 	// 1 - Vermelho
 	// 2 - Azul
 	int determinaVez = 0;
 	
 	while(true)
 	{
 		//EventListener.
 		SDL_Event evento;
 		while(SDL_PollEvent(&evento))
 		{
 			//Clicado o 'X' vermelho gigante para matar o processo.
 			if(evento.type == SDL_QUIT)
 			{
 				SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_FreeWAV(wavBuffer);
                SDL_Quit();
 			}else if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT)
 			{
 				//Registra a posição clicada.
 				int mouse_x = event.button.x;
                int mouse_y = event.button.y;
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);
                
                //Modulzarizar isso muito, muito mais. Queremos estes comportamentos funcionando para qualuqer outra situação.
                //Focar principalmente no contexto de um jogo PlayerxCpu.
                if(foraDosLimites(mouse_x, mouse_y, tabuleiro) > 1)
                {
                	printf("Jogada inválida! Os turnos continuam iguais.\n");
                } else 
                {
                	if(determinaVez = 0)
                	{
                		printf("Vez ainda não determinada...\n");
                		determinaVez = sorteiaVez();
                	}
                	else if(determinaVez == 1)
                	{
                		printf("Vez atual : VERMELHO");
                		determinaVez = determinaVez + 1;
                	} else 
                	{
                		printf("Vez atual : AZUL");
                		determinaVez = determinaVez - 1;
                	}
                
                	//Toca o áudio ao clicar.
                	SDL_QueueAudio(deviceId, wavBuffer, wavLength); 
                	SDL_PauseAudioDevice(deviceId, 0);
                }                
			} 			
		}
		// Clear da página.
		SDL_RenderClear(renderer);
	}
}

//Bem direto o que essa função faz, seguir o nome.
int foraDosLimites(int posMouseX, int posMouseY, SDL_Rect tabuleiro)
{
	if(posMouseX > tabuleiro.x && posMouseX < (tabuleiro.x + tabuleiro.w) && posMouseY > tabuleiro.y && posMouseY < (tabuleiro.y + tabuleiro.h)){return 0;}
	return 1;
}

//Bem direto o que essa função faz, seguir o nome.
void preencheTabuleiro(Sprite tabuleiro[7][6], SDL_Renderer renderer)
{
	for(int colunas; colunas < 7; colunas++)
	{
		for(int linhas; linhas < 6; linhas++)
		{
			//Criando Sprite
			Sprite *sprite;
			
			//Determinando a textura inicial do sprite. Sendo uma ficha invisível de 192x192 em resolução para futura conveniência.
			//Logo, o ID_Time inicial é determinado = 0 (NEUTRO)
			SDL_Texture *ficha = IMG_LoadTexture(renderer, "./img/FichaInvisivel.png");
			sprite->id_time = 1;
			sprite->box.w = 192; sprite->box.h = 192;
			
			//Determinação das posições 'bonitinhas' dentro do tabuleiro.
			posInit_x = colunas * sprite->box.w;
			posInit_y = linhas * sprite->box.h;
			
			// A linha SDL_RenderCopy propriamente imagino que deve apeans ocorrer no main() em si, por tudo que o renderizador deve fazer
			//para realmente mostrar na tela a imagem da ficha renderizada. 
			
			tabuleiro[colunas][linhas] = sprite;
		}
	}
	//PLACEHOLDER, NÃO LEVE A SÉRIO ESTA LINHA DE CÓDIGO! NÃO VAI FUNCIONAR!
	return tabuleiro;
}

// :|
int determinaVez()
{
	srand(time(NULL));
	int rnd = rand();
	int resultado = (rnd > RAND_MAX / 2) ? 1 : 2;
	
	return resultado;
}
