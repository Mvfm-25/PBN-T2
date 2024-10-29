#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct Quad
{
    int id;
    SDL_Rect q;
    int flag;  // 0 para cor inicial, 1 para cor alternada
} Quad;

int isInside(int mouseX, int mouseY, SDL_Rect quad);
void changeColor(SDL_Renderer *renderer, Quad *quad);

int main(int argc, char** argv)
{
    // Inicialização de TUDO e criação da janela
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* janela = SDL_CreateWindow("Teste matriz 2x2", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
    
    // Inicialização do renderizador
    SDL_Renderer *renderer = SDL_CreateRenderer(janela, -1, 0);

    Quad rec1 = {1, {50, 50, 100, 100}, 0};
    Quad rec2 = {2, {150, 50, 100, 100}, 0};
    Quad rec3 = {3, {50, 150, 100, 100}, 0};
    Quad rec4 = {4, {150, 150, 100, 100}, 0};
    
    // Determinações para o WAVLoader
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV("./wavs/secret.wav", &wavSpec, &wavBuffer, &wavLength);
    
    // 'Caminho' para o dispositivo de áudio disponível
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(janela);
                SDL_FreeWAV(wavBuffer);
                SDL_Quit();
                return 0;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;
                printf("Coordenada X : %d\nCoordenada Y : %d\n", mouse_x, mouse_y);
                
                if (isInside(mouse_x, mouse_y, rec1.q))
                {
                    changeColor(renderer, &rec1);
                }
                else if (isInside(mouse_x, mouse_y, rec2.q))
                {
                    changeColor(renderer, &rec2);
                }
                else if (isInside(mouse_x, mouse_y, rec3.q))
                {
                    changeColor(renderer, &rec3);
                }
                else if (isInside(mouse_x, mouse_y, rec4.q))
                {
                    changeColor(renderer, &rec4);
                }

                // Toca o áudio ao clicar
                SDL_QueueAudio(deviceId, wavBuffer, wavLength); 
                SDL_PauseAudioDevice(deviceId, 0);
            }
        }

        SDL_RenderClear(renderer);

        // Definir cor para rec1 com base no valor de flag
        if (rec1.flag == 0) SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Azul
        else SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);  // Rosa
        SDL_RenderFillRect(renderer, &rec1.q);

        // Definir cor para rec2
        if (rec2.flag == 0) SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Branco
        else SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Preto
        SDL_RenderFillRect(renderer, &rec2.q);

        // Definir cor para rec3
        if (rec3.flag == 0) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Vermelho
        else SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Ciano
        SDL_RenderFillRect(renderer, &rec3.q);

        // Definir cor para rec4
        if (rec4.flag == 0) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Verde
        else SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Azul
        SDL_RenderFillRect(renderer, &rec4.q);

        // Cor de fundo
        SDL_SetRenderDrawColor(renderer, 9, 20, 33, 255);
        SDL_RenderPresent(renderer);        
    }

    return 0;
}

int isInside(int mouseX, int mouseY, SDL_Rect quad)
{
    return (mouseX > quad.x && mouseX < (quad.x + quad.w) && mouseY > quad.y && mouseY < (quad.y + quad.h));
}

void changeColor(SDL_Renderer *renderer, Quad *quad)
{
    // Alterna a flag para alternar a cor
    quad->flag = !quad->flag;
}

