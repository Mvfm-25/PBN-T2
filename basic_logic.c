#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int cor;     // cores: vermelho(1), azul(2) e null(0).    
} casa;

// NOTA SOBRE O CÓDIGO: desde que as cores das fichas sejam mantidas como números, este código deve bastar.
// desde que quando for adicionado uma ficha, seja adicionado nessa matriz interna. Para isso poderia criar
// um método addCasa();

// Dica de análise do código: começe pela main e siga o fluxo, é bem simples.

casa matriz[6][7];
int conect = 0, medX, medY;

// recebe a posição do último colocado e cor
void CheckBoard(int y, int x, int c){
    CheckHorizontal(y, x, c);
    CheckVertical(y, x, c);
    CheckDiagonalEsquerda(y, x, c);
    CheckDiagonalDireita(y, x, c);
    printf("nao teve vencedores");
}

// olha se tem 4 da mesma cor na horizontal
void CheckHorizontal(int y, int x, int c){
    conect = 0;
    //printf("Y: %d; X: %d; Cor: %d;\n", y, x, c);
    //olha esquerda
    for (int i=1; i<4; i++){
        //printf("Cor olhada esquerda: %d; i: %d\n", matriz[y][x-i].cor, i);
        if(x-i < 0 || matriz[y][x-i].cor != c ){
            break;
        } 
        conect++;
    }

    // olha direita
    for (int i=1; i<4; i++){
        //printf("Cor olhada direita: %d; i: %d\n", matriz[y][x+i].cor, i);
        if(x+i > 7 || matriz[y][x+i].cor != c ){
            break;
        }
        conect++;
    }

    if (conect >= 3)Ganhador(c);
    //printf("Nao foi pela horizontal!\n\n");
}

// olha se tem 4 da mesma cor pra baixo
void CheckVertical(int y, int x, int c){
    conect = 0;
    //printf("Y: %d; X: %d; Cor: %d;\n", y, x, c);
    //olha pra baixo
    for (int i=1; i<4; i++){
        //printf("Cor olhada baixo: %d; i: %d\n", matriz[y-i][x].cor, i);
        if(y-i < 0 || matriz[y-i][x].cor != c){
            //printf("Break hori-b\n");
            break;
        }
        conect++;
    }

    if (conect >= 3)Ganhador(c);
    //printf("Nao foi pela vertical!\n\n");
}

// olha se tem 4 da mesma cor na diagonal esquerda "/" <- assim
void CheckDiagonalEsquerda(int y, int x, int c){
    conect = 0;
    //printf("Y: %d; X: %d; Cor: %d;\n", y, x, c);
    //olha diagonal esquerda inferior "/"
    for (int i=1; i<4; i++){
        //printf("Cor olhada esquerda baixo: %d; i: %d\n", matriz[y-i][x-i].cor, i);
        if(x-i < 0 || y-i < 0 || matriz[y-i][x-i].cor != c ){
            break;
        }
        conect++;
    }

    // olha diagonal direita superior "/"
    for (int i=1; i<4; i++){
        //printf("Cor olhada direita cima: %d; i: %d\n", matriz[y+i][x+i].cor, i);
        if(x+i > 7 || y+i > 6 || matriz[y+i][x+i].cor != c ){
            break;
        } 
        conect++;
    }

    if (conect >= 3)Ganhador(c);
    //printf("Nao foi pela diagonal esquerda!\n\n");
}

// olha se tem 4 da mesma cor na diagonal direita "\" <- assim
void CheckDiagonalDireita(int y, int x, int c){
    conect = 0;
    //printf("Y: %d; X: %d; Cor: %d;\n", y, x, c);
    //olha diagonal direita inferior "\"
    for (int i=1; i<4; i++){
        //printf("Cor olhada direita baixo: %d; i: %d\n", matriz[y-i][x+i].cor, i);
        if(x+i > 7 || y-i < 0 || matriz[y-i][x+i].cor != c ){
            break;
        } 
        conect++;
    }

    // olha diagonal esquerda superior "\"
    for (int i=1; i<4; i++){
        //printf("Cor olhada esquerda cima: %d; i: %d\n", matriz[y+i][x+i].cor, i);
        if(x-i < 0 || y+i > 6 || matriz[y+i][x-i].cor != c ){
            break;
        } 
        conect++;
    }

    if (conect >= 3)Ganhador(c);
    //printf("Nao foi pela diagonal direita!\n\n");
}

void Ganhador(int c){
    printf("A cor %d GANHOU", c);
    exit(0);
}

int main(){
    // <- x +>   
    // o 1 o o    ^
    //[1]1 2 1    +
    // 2 1 1 o    y
    // o 1 1 o    -
    // 1 2 o 1  

    //matriz[y][x]
    for(int y= 0; y<6; y++){
        for(int x= 0; x<7; x++){
            matriz[y][x].cor = 0;
        }
    }

    //matriz[0][0].cor = 1;
    //matriz[0][1].cor = 2;
    //matriz[0][3].cor = 1;
    //matriz[1][1].cor = 1;
    //matriz[1][2].cor = 1;
    //matriz[2][0].cor = 2;
    //matriz[2][1].cor = 1;
    //matriz[2][2].cor = 1;
    //matriz[3][0].cor = 1;
    //matriz[3][1].cor = 1;
    //matriz[3][2].cor = 2;
    //matriz[3][3].cor = 1;
    //matriz[4][1].cor = 1;

    CheckBoard(0, 0, matriz[0][0].cor);

    return 0;
}