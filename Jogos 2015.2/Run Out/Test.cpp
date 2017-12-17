#include <math.h>
#include <stdio.h>
#include "Graphics.h"

extern Graphics graphics;

struct sprite_sheet //guarda as posicoes de corte da spritesheet
{
	
	int x;
	int y;
	int width;
	int height;

}; typedef struct sprite_sheet Sprite_sheet;

struct projetil //colocar depois projetil no cartucho
{

	float x; //posicao x do item
	float y; //posicao y do item
	int animation_clip; //clip da animacao do item
	int animation_index; //indice atual da animacao do item
	float time_next_frame; //contador de tempo para avancar na animacao
	float velocidade;
	bool timer_projetil;
	float tempo_projetil;
	int direcao;

	bool disparo;
	Image sheet_projetil;
	Sprite_sheet matriz_projetil[4][10];



}; typedef struct projetil Projetil;

struct cartucho
{
	int resistencia;
	int resistencia_max;
	bool municao_disponivel;

	Projetil projetil[20];

}; typedef struct cartucho Cartucho;

struct equipamento
{

	float x; //posicao x do item
	float y; //posicao y do item
	int animation_clip; //clip da animacao do item
	int animation_index; //indice atual da animacao do item
	float time_next_frame; //contador de tempo para avancar na animacao

	Cartucho pente;

	Image nome[2];
	int tag; //determina se eh item ou arma
	bool status; //determina se está sendo ou nao mostrado na tela. é para quando ter a colisao, fazer desaparecer da tela, como se tivesse sido recolhido para o inventario

}; typedef struct equipamento Equipamento;


bool Collision(int position_x, int position_y, int x1, int y1, int x2, int y2)
{

	if (x2<=x1 && y2>=y1)//caso1
		if (position_x >= x2 && position_x <= x1 && position_y <=y2 && position_y >=y1)
			return true;

	if (x2>=x1 && y2>=y1)//caso2
		if (position_x >= x1 && position_x <= x2 && position_y >=y1 && position_y <=y2)
			return true;

	if (x2>=x1 && y2<=y1) //caso4
		if (position_x >= x1 && position_x <= x2 && position_y >=y2 && position_y <=y1)
			return true;

	if (x2<=x1 && y2<=y1) //caso3
		if (position_x <= x1 && position_x >= x2 && position_y >=y2 && position_y <=y1)
			return true;

	return false;
}

void Distance (int x1, int y1, int x2, int y2)
{

	int measure_x, measure_y;

	if (x1>x2)
		measure_x = abs(x1-x2);
	else
		measure_x = abs(x2-x1);

	if (y1>y2)
		measure_x = abs(y1-y2);
	else
		measure_x = abs(y2-y1);
}

void InitSprites(Sprite_sheet matriz_sprite[][10], int sprite_size_x, int sprite_size_y, int linha, int coluna)
{

	int x, y;
	int sprite_x = 1; //posicao x onde o sprite realmente comeca. Ideal para imagens com sprite que nao comeca no (0,0)
	int sprite_y = 1;

	for (y = 0; y < linha; y++)
		for (x = 0; x < coluna; x++)
			{
				matriz_sprite[y][x].x = sprite_x + sprite_size_x * x;
				matriz_sprite[y][x].y = sprite_y + sprite_size_y * y;
				matriz_sprite[y][x].width = sprite_size_x;
				matriz_sprite[y][x].height = sprite_size_y;
			}
}

void InitSprites(Equipamento *equip, int sprite_size_x, int sprite_size_y, int linha, int coluna)
{

	int x, y, z;
	int sprite_x = 1; //posicao x onde o sprite realmente comeca. Ideal para imagens com sprite que nao comeca no (0,0)
	int sprite_y = 1;

	for (z=0; z<20; z++)
		for (y = 0; y < linha; y++)
			for (x = 0; x < coluna; x++)
				{
					equip->pente.projetil[z].matriz_projetil[y][x].x = sprite_x + sprite_size_x * x;
					equip->pente.projetil[z].matriz_projetil[y][x].y = sprite_y + sprite_size_y * y;
					equip->pente.projetil[z].matriz_projetil[y][x].width = sprite_size_x;
					equip->pente.projetil[z].matriz_projetil[y][x].height = sprite_size_y;
				}
}

void Delay(float tempo_atraso, int tipo) //0 - atraso, 1 - pause
{

	static int controle = 1; //pause ativo
	static float tempo_corrido = 0;
	tempo_corrido += graphics.GetElapsedTime();

	if (tipo == 0)
	{

		for  (tempo_corrido = 0; tempo_corrido < tempo_atraso; tempo_corrido = tempo_corrido + graphics.GetElapsedTime())
		{
			 //faz nada :)
		}

		tempo_corrido = 0;
	}

	else
	{
		if (controle == 1)
		{
			//funcao de pause
		}

		controle = 1;
	}

}