#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

int vezes = 0;
int flag = 0;

int isInside(int mouseX, int mouseY, SDL_Rect quad);
void changeColor(int *currentColor);

int main(int argc, char** argv)
{
    // Inicialização de TUDO e criação da janela
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* janela = SDL_CreateWindow("Teste matriz 2x2", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    
    // Inicialização do renderizador
    SDL_Renderer * renderer = SDL_CreateRenderer(janela, -1, 0);
    SDL_Rect quad1, quad2, quad3, quad4;
    
    // Determinações para o WAVLoader
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("./wavs/secret.wav", &wavSpec, &wavBuffer, &wavLength);
    
    // 'Caminho' para o dispositivo de áudio disponível
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    
    // Posicionamento e dimensionamento dos quadrados
    quad1.x = 50;  quad1.y = 50;  quad1.w = 100; quad1.h = 100;
    quad2.x = 150; quad2.y = 50;  quad2.w = 100; quad2.h = 100;
    quad3.x = 50;  quad3.y = 150; quad3.w = 100; quad3.h = 100;
    quad4.x = 150; quad4.y = 150; quad4.w = 100; quad4.h = 100;
    
    // Estado atual das cores (1 = azul, 2 = branco, 3 = vermelho, 4 = verde)
    int currentColor1 = 1;
    int currentColor2 = 2;
    int currentColor3 = 3;
    int currentColor4 = 4;
    
    while(true)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                exit(0);
            }
            else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);
                
                if(isInside(mouse_x, mouse_y, quad1))
                {
                    changeColor(&currentColor1);
                }
                else if(isInside(mouse_x, mouse_y, quad2))
                {
                    changeColor(&currentColor2);
                }
                else if(isInside(mouse_x, mouse_y, quad3))
                {
                    changeColor(&currentColor3);
                }
                else if(isInside(mouse_x, mouse_y, quad4))
                {
                    changeColor(&currentColor4);
                }

                // Toca o áudio ao clicar
                SDL_QueueAudio(deviceId, wavBuffer, wavLength); 
                SDL_PauseAudioDevice(deviceId, 0);
            }            
        }

        SDL_RenderClear(renderer);
        
        // Definir cores baseadas no estado atual
        switch(currentColor1)
        {
            case 1: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
            case 5: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break; // cor alternativa
        }
        SDL_RenderFillRect(renderer, &quad1);
        
        switch(currentColor2)
        {
            case 2: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break;
            case 6: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); break; // cor alternativa
        }
        SDL_RenderFillRect(renderer, &quad2);
        
        switch(currentColor3)
        {
            case 3: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break;
            case 7: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break; // cor alternativa
        }
        SDL_RenderFillRect(renderer, &quad3);
        
        switch(currentColor4)
        {
            case 4: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break;
            case 8: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break; // cor alternativa
        }
        SDL_RenderFillRect(renderer, &quad4);
        
        // Cor de fundo
        SDL_SetRenderDrawColor(renderer, 9, 20, 33, 255);
        SDL_RenderPresent(renderer);        
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);    
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();
    return 0;
}

// Função para verificar se o clique está dentro de um quadrado
int isInside(int mouseX, int mouseY, SDL_Rect quad)
{
    if(mouseX > quad.x && mouseX < (quad.x + quad.w) && mouseY > quad.y && mouseY < (quad.y + quad.h))
    {
        return 1;
    }
    return 0;
}

// Função para alternar a cor de um quadrado
void changeColor(int *currentColor)
{
    if (*currentColor <= 4)
        *currentColor += 4; // Alterna para a cor alternativa
    else
        *currentColor -= 4; // Volta para a cor original
}

