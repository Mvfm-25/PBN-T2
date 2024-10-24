#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

int flag = 0;

int main(int argc, char** argv)
{
	//Inicialização de TUDO e criação da janela
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* janela = SDL_CreateWindow("Teste matriz 2x2", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
	
	//Inicialização do renderizador
	SDL_Renderer * renderer = SDL_CreateRenderer(janela, -1,0);
	SDL_Rect quad1, quad2, quad3, quad4;
	
	//Determinações para o WAVLoader
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;
	SDL_LoadWAV("secret.wav", &wavSpec, &wavBuffer, &wavLength);
	
	//'Caminho' para o dispositivo de áudio disponível.
	SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	
	//Posicionamento e dimensionamento do Quad1
	quad1.x=50; quad1.y=50; quad1.w=100; quad1.h=100;
	
	//Posicionamento e dimensionamento do Quad2
	quad2.x=150; quad2.y=50; quad2.w=100; quad2.h=100;
	
	//Posicionamento e dimensionamento do Quad3
	quad3.x=50; quad3.y=150; quad3.w=100; quad3.h=100;
	
	//Posicionamento e dimensionamento do Quad4
	quad4.x=150; quad4.y=150; quad4.w=100; quad4.h=100;
	
	while(true)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				exit(0);
			} else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				int mouse_x = event.button.x;
				int mouse_y = event.button.y;
				
				if(mouse_x > 50 && mouse_x < 150 && mouse_y > 50 && mouse_y < 150)
				{
					//Toca o áudio determinado lá em cima.
					int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength); SDL_PauseAudioDevice(deviceId, 0);
					if(flag == 0){ flag = 1; }
					else { flag = 0; }
				}
			}			
		}
		SDL_RenderClear(renderer);
		if(flag == 1)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_RenderFillRect(renderer, &quad1);
		} else {
			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderFillRect(renderer, &quad1);
		}
	
		SDL_SetRenderDrawColor(renderer, 255,255,255,255);
		SDL_RenderFillRect(renderer, &quad2);
	
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(renderer, &quad3);
	
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &quad4);
	
		SDL_SetRenderDrawColor(renderer, 9, 20, 33, 255);
		SDL_RenderPresent(renderer);		
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(janela);	
	SDL_Quit();
	return 0;
}
