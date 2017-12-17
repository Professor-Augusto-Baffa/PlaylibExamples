#include "Graphics.h" 
#include "Audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#include "Mouse.h"
#include "Keyboard.h"
#include "Test.h"

#define SIZE_THUMBNAIL 100
#define SPRITE_SIZE_X 32
#define SPRITE_SIZE_Y 48
#define MAP_WIDTH 26
#define MAP_HEIGHT 20

Graphics graphics; //definido em teste

int k, j;

float dt;
float timer_monstro;
 
char Mapa[MAP_WIDTH][MAP_HEIGHT];
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

}; typedef struct projetil Projetil;

struct cartucho
{
	int resistencia;
	int resistencia_max;
	bool municao_disponivel;

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
	bool status; //determina se est� sendo ou nao mostrado na tela. � para quando ter a colisao, fazer desaparecer da tela, como se tivesse sido recolhido para o inventario

}; typedef struct equipamento Equipamento;

struct heroi
{

	float x; //posicao x do heroi
	float y; //posicao y do heroi
	int animation_clip; //clip da animacao do heroi
	int animation_index; //indice atual da animacao do heroi
	float time_next_frame; //contador de tempo para avancar na animacao
    

	int vida;
	float velocidade;
	//bool tag; //deterermina se a colisao foi com personagem jogavel
	Equipamento inventario_arma[1];
	Equipamento inventario_item[3];
	int pontuacao;

}; typedef struct heroi Heroi;


/*--------Cron�mentro---------*/
void timer();
float time_next_number1;
float time_next_number2;
float time_next_number3;
float time_next_number4;


int line = 0;
bool contador = true;

int timer1 = 9;
int timer2 = 5;
int timer3 = 9;
int timer4 = 3;
/*-----------------------------*/


/*FUN��ES*/
void load ();// carregar todas as variaveis
void update (float dt); // tudo que depende de dt (tempo)
void draw(); // desenhar tudo do jogo

void movimentacao(Heroi *personagem, Sprite_sheet matriz_personagem[][10], Image sheet_personagem);
void gera_equipamento(Heroi personagem, Equipamento *equipamento, Sprite_sheet matriz_equipamento[][10], Image sheet_equipamento);

void monta_inventario(Heroi personagem, Equipamento *equipamento, Sprite_sheet matriz_equipamento[][10], Image sheet_equipamento);
void controla_arma(Heroi personagem, Equipamento *equipamento, Projetil *projetil, Sprite_sheet matriz_projetil[][10], Image sheet_projetil);

void LoadMapa(char *filename);

void MoveEnemy1();

//void gera_projetil(Heroi personagem, Equipamento *equipamento, Projetil *projetil);//passa um struct do vetor);

//bool key_states[256]; //ver como funciona essa funcao, n�o entendi direito

int scene = 0; //determina a tela exibida
int dificuldade = 0; //variavel auxiliar no menu de dificuldade
int personagem = 0;
bool music = true;

Image menu_entrada[2];

Image menu_principal;
Image menu_principal_selecionado[3];
Image menu_principal_enemy[4];

Image menu_opcoes[3]; //0 - musica on, 1 - musica off, 3- voltar selecionado;

Image menu_dificuldade[8]; //0-dia, 1-tarde, 2-noite, 3-diaminiatura, 4-tardeminiatura, 5-noiteminiatura, 6-voltar, 7-selecionar

Image menu_personagem[8]; //0-amanda, 1-luis, 2-marcelo, 3-amandaminiatura, 4-luisminiatura, 5-marcelominiatura, 6-voltar, 7-selecionar

Image HUD;
Image Cidade_Cerdin;


Audio menu_entrada_timer;
Audio menu_principal_musica;


int animation_index = 0;
int animation_index2 = 0;

float time_next_frame = 0;
float time_next_frame2 = 0;
int number;

Image sheet_numeros;
Sprite_sheet matriz_numeros[4][10];

Image sheet_reaper;
Sprite_sheet reapermenu[4][10];

Image sheet_monstro;
Sprite_sheet matriz_monstro[4][10];
Heroi monstro;

Image sheet_amanda;
Image sheet_luis;
Image sheet_marcelo;

Sprite_sheet matriz_amanda[4][10];
Sprite_sheet matriz_luis[4][10];
Sprite_sheet matriz_marcelo[4][10];

Heroi amanda;


Image sheet_arma;
Sprite_sheet matriz_arma[4][10];

Equipamento arma;

Projetil bala[10];

int i = 0; // conta o numero de balas atiradas

Projetil projetil_magico;
Image sheet_projetil_magico;
Sprite_sheet matriz_projetil_magico[4][10];


//-------------------------------------------------------------------------------------------- come�o do programa em si------------------------------------------///////////////////////////////////////////

void MainLoop(void)
{
	
	dt = graphics.GetElapsedTime();
	graphics.SetMouseMotionInput(MouseMotionInput); //trata a funcao entre parenteses, inserindo os paramentros obtidos do sistema na funcao
	graphics.SetMouseClickInput(MouseClickInput);
	graphics.SetKeyboardInput(KeyboardInput);

	
	draw();
	update(dt);
	/*LoadMapa("Mapa01.txt");*/
	KeyboardExecute(&scene, graphics); //processa o teclado. Tudo relativo a teclado � processado aqui
	//printf ("x = %d, y = %d\n", mouse_x, mouse_y);
	//printf ("dificuldade = %d\n", dificuldade);
	//printf ("click_x = %d, click_y = %d\n", click_x, click_y);
	
	
	//printf ("balas restantes = %d\n", arma.pente.resistencia);
	//printf ("clip = %d\n", amanda.animation_clip);
	

	click_x = 0;
	click_y = 0;
	
}

int main (void)
{
	graphics.CreateMainWindow(800, 600, "Run Out");

	load();

		for (k=0; k<26; k++)
	{
		for (j=0; j<20; j++)
		{
			printf("%c ", Mapa[k][j]);

			if (j==19)
				printf("\n");
		}
	}

	
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();

	
	return 0;
}

void load ()
{
	menu_entrada[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(sem pressione enter).png");
	//menu_entrada[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(pressione enter 80porcento).png");
	menu_entrada[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(pressione enter 100porcento).png");
	menu_entrada_timer.LoadAudio("Data\\Audios\\timer(menu_entrada).mp3");

	menu_principal.LoadPNGImage("Data\\Imagens\\Menus\\menu_principal.png");
	menu_principal_selecionado[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_principal(jogar selecionado).png");
	menu_principal_selecionado[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_principal(opcoes selecionado).png");
	menu_principal_selecionado[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_principal(sair selecionado).png");
	menu_principal_enemy[0].LoadPNGImage("Data\\Imagens\\Menus\\reaper.png");
	menu_principal_enemy[1].LoadPNGImage("Data\\Imagens\\Menus\\splitter.png");
	menu_principal_musica.LoadAudio("Data\\Audios\\Black Vortex.mp3");

	menu_opcoes[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_opcoes(ligada).png");
	menu_opcoes[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_opcoes(desligada).png");
	menu_opcoes[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_opcoes(voltar selecionado).png");

	menu_dificuldade[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(facil).png");
	menu_dificuldade[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(medio).png");
	menu_dificuldade[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(dificil).png");
	menu_dificuldade[3].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(facil_miniatura).png");
	menu_dificuldade[4].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(medio_miniatura).png");
	menu_dificuldade[5].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(dificil_miniatura).png");
	menu_dificuldade[6].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(voltar).png");
	menu_dificuldade[7].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(selecionar).png");

	menu_personagem[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(amanda-lar).png");
	menu_personagem[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(luis-lar).png");
	menu_personagem[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(marcelo-lar).png");
	menu_personagem[3].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(amandaminiatura).png");
	menu_personagem[4].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(luisminiatura).png");
	menu_personagem[5].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(marcelominiatura).png");
	menu_personagem[6].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(voltar).png");
	menu_personagem[7].LoadPNGImage("Data\\Imagens\\Menus\\menu_personagem(selecionar).png");

	HUD.LoadPNGImage("Data\\Imagens\\Menus\\HUD.png");
	Cidade_Cerdin.LoadPNGImage("Data\\Mapas\\mapa_sala.png");

	sheet_amanda.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_amanda.png");
	InitSprites(matriz_amanda, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);
	sheet_luis.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_luis.png");
	InitSprites(matriz_luis, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);
	sheet_marcelo.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_marcelo.png");
	InitSprites(matriz_marcelo, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	sheet_reaper.LoadPNGImage("Data\\reaper.png");
	InitSprites(reapermenu, 60, 60, 4, 4);

	sheet_monstro.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_monstro.png");
	InitSprites(matriz_monstro, 48, 64, 4, 3);

	sheet_numeros.LoadPNGImage("Data\\Imagens\\numbers2.png");
	InitSprites(matriz_numeros, 32, 32, 1, 10);




	/*----------------LOADstructs-------------------*/

	//AMANDA
	amanda.x = 186; 
	amanda.y = 400; 
	amanda.animation_clip = 0; 
	amanda.animation_index = 0; 
	amanda.time_next_frame = 0;
	amanda.vida = 3;
	amanda.velocidade = 80;
	/*amanda.inventario_arma[1];
	amanda.inventario_item[3];*/

	/*--------------------ARMA---------------------*/

	arma.animation_clip = 0;
	arma.animation_index = 0;
	arma.pente.resistencia_max = 10;
	arma.pente.resistencia = 10;
	arma.x = 350;
	arma.y = 350;
	arma.status=true;
	sheet_arma.LoadPNGImage("Data\\Imagens\\arma.png");
	InitSprites(matriz_arma, 64, 64, 1, 1);

	//------------------BALA1------------//
	
//	for (int g= 0; g < arma.pente.resistencia_max; g++)  //carrega todos os possiveis tiros na tela. Repare que � um vetor de 7 espa�os, 
//	{													//ou seja, o maximo possivel de frangos na tela � 7 
//        bala[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\tiro.png");// carrega a imagem da bala e a matriz do sprite no estruct da bala em si
//		InitSprites(&bala[g], 13, 10, 1, 2);
//		bala[g].disparo = false;
//		bala[g].velocidade = 40;
//		bala[g].timer_projetil = 0;
//		bala[g].tempo_projetil = 0;
//    }

	sheet_projetil_magico.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_magico.png");
	InitSprites(matriz_projetil_magico, 32, 32, 1, 4);
	projetil_magico.velocidade = 140;
	projetil_magico.disparo=false;
	projetil_magico.tempo_projetil = 0;
	projetil_magico.timer_projetil = false;


	LoadMapa("Data\\Mapas\\mapa_sala.txt");

	monstro.x = 700;
	monstro.y = 400;
	monstro.velocidade = 100;
	monstro.vida = 3;



}

void update (float dt)
{

	if (scene == 4)
	{
       
		timer();

		movimentacao(&amanda, matriz_amanda, sheet_amanda);

		gera_equipamento(amanda, &arma, matriz_arma, sheet_arma);

		monta_inventario(amanda, &arma, matriz_arma, sheet_arma);

		controla_arma(amanda, &arma, &projetil_magico, matriz_projetil_magico, sheet_projetil_magico); //<-----------------ERRO AQUI, CERTEZA
		//gera_projetil(amanda, &arma, &bala[i]);//passa um struct do vetor

		MoveEnemy1();
	}
}

void draw()
{
/*
	printf("cena = %d\n", scene);*/

	if (scene ==1 ||scene ==-1 ||scene ==2 || scene ==3)
	{
		menu_entrada_timer.Stop();

		if (music)
		{
			menu_principal_musica.Play();

			if (!menu_principal_musica.IsPlaying())
			{
				menu_principal_musica.Stop();
				menu_principal_musica.Play();
			}
		}

		else
			menu_principal_musica.Stop();
	}


	if (scene == 0)
	{
		graphics.DrawImage2D(menu_entrada[0]);
		menu_entrada_timer.Play();
		number = (rand() % 2); 

		if (!menu_entrada_timer.IsPlaying())
		{
			menu_entrada_timer.Stop();
			menu_entrada_timer.Play();
		}
		
	time_next_frame = time_next_frame + graphics.GetElapsedTime();

	graphics.DrawImage2D(menu_entrada[animation_index]);
 
        if (time_next_frame > 1) //tempo maior que 1 segundo
        {
             animation_index++; //aumenta indice
             time_next_frame = 0; //zera tempo
        }

        if (animation_index > 1)
             animation_index = 0;

		
		/*printf("%d", animation_index);*/

	}

	else if (scene == 1)
	{
		graphics.DrawImage2D(menu_principal);

		time_next_frame2 = time_next_frame2 + graphics.GetElapsedTime();
 
        if (time_next_frame2 > 0.1) //tempo maior que 1 segundo
        {
             animation_index2++; //aumenta indice
             time_next_frame2 = 0; //zera tempo
        }

        if (animation_index2 >3)
             animation_index2 = 0;

		graphics.DrawImage2D(350, 300, 32, 50, matriz_amanda[3][animation_index2].x,  matriz_amanda[3][animation_index2].y,  matriz_amanda[3][animation_index2].width, matriz_amanda[3][animation_index2].height, sheet_amanda);

		
		if (Collision(mouse_x, mouse_y, 45, 222, 272, 280))
			graphics.DrawImage2D(menu_principal_selecionado[0]);

		if (Collision(click_x, click_y, 45, 222, 272, 280))
			scene =2;

		if (Collision(mouse_x, mouse_y, 18, 136, 295, 200))
			graphics.DrawImage2D(menu_principal_selecionado[1]);

		if (Collision(click_x, click_y, 18, 136, 295, 200))
			scene = -1;

		if (Collision(mouse_x, mouse_y, 68, 56, 236, 112))
		{
			graphics.DrawImage2D(menu_principal_selecionado[2]);

			if (Collision(click_x, click_y, 68, 56, 236, 112))
				exit(0);
		}
	}

		else if (scene == -1)
		{
			if (music)
			{
				graphics.DrawImage2D(menu_opcoes[0]);

				if (Collision(click_x, click_y, 480, 390, 661, 415))
				{
					music = false;
					menu_principal_musica.Stop();
				}
			}

			else
			{
				graphics.DrawImage2D(menu_opcoes[1]);
				if (Collision(click_x, click_y, 315, 391, 441, 415))
					music = true;
			}

			if  (Collision(mouse_x, mouse_y, 605, 54, 731, 75))
				graphics.DrawImage2D(menu_opcoes[2]);

			if (Collision(click_x, click_y, 605, 54, 731, 75))
				scene = 1;
		}

		else if (scene == 2)
		{

			if(dificuldade==0)
			{
				graphics.DrawImage2D(menu_dificuldade[0]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					dificuldade = 2;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					dificuldade = 1;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[5]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[4]);
			}

			else if(dificuldade==1)
			{
				graphics.DrawImage2D(menu_dificuldade[1]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					dificuldade = 0;

				if (Collision(click_x, click_y, 571, 273, 672, 373))
					dificuldade = 2;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[3]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[5]);
			}

			else if(dificuldade==2)
			{
				graphics.DrawImage2D(menu_dificuldade[2]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					dificuldade = 1;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					dificuldade = 0;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[4]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[3]);
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_dificuldade[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_dificuldade[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 1;
			if (Collision(click_x, click_y, 504, 39, 705, 66))
			{
				Delay(5, 0);
				scene = 3;
			}

		}

		else if (scene == 3)
		{

			if(personagem==0)
			{
				graphics.DrawImage2D(menu_personagem[0]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					personagem = 2;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					personagem = 1;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, 90, 120, menu_personagem[5]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(574, 274, 90, 120, menu_personagem[4]);
			}

			else if(personagem==1)
			{
				graphics.DrawImage2D(menu_personagem[1]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					personagem = 0;

				if (Collision(click_x, click_y, 571, 273, 672, 373))
					personagem = 2;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, 90, 120, menu_personagem[3]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(574, 274, 90, 120, menu_personagem[5]);
			}

			else if(personagem==2)
			{
				graphics.DrawImage2D(menu_personagem[2]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					personagem = 1;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					personagem = 0;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, 90, 120, menu_personagem[4]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(574, 274, 90, 120, menu_personagem[3]);
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_personagem[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_personagem[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 2;
			if (Collision(click_x, click_y, 504, 39, 705, 66))
				scene = 4;
		}

		else if (scene ==4)
		{
			menu_principal_musica.Stop();
			graphics.DrawImage2D(Cidade_Cerdin);
			graphics.DrawImage2D(0, 0, 800, 600, HUD);

		//	     
  //      int x, y;
  //      for (x = 0; x < 30; x++)//N�O ENTENDI PORQUE COLOCAR 30 AQUI
  //      {
  //          for (y = 0; y < 26; y++)
  //          {
  //              if (Mapa[x][y] == 'x')
  //              {
  //                  graphics.DrawImage2D(x*32, y*32-38, 32, 32, sheet_amanda);
  //              }
  //              if (Mapa[x][y] == 'o')
  //              {
  //                  graphics.DrawImage2D(x*32, y*32-38, 32, 32, sheet_projetil_magico);
  //              }
  //          }
		//}
  }
		}
		
void timer()//teste da contagem regressiva
{

	time_next_number1 = time_next_number1 + dt;
	graphics.DrawImage2D(355, 545, 26, 26, matriz_numeros[line][timer4].x,  matriz_numeros[line][timer4].y,  matriz_numeros[line][timer4].width, matriz_numeros[line][timer4].height, sheet_numeros);
	graphics.DrawImage2D(375, 545, 26, 26, matriz_numeros[line][timer3].x,  matriz_numeros[line][timer3].y,  matriz_numeros[line][timer3].width, matriz_numeros[line][timer3].height, sheet_numeros);
	graphics.DrawImage2D(404, 545, 26, 26, matriz_numeros[line][timer2].x,  matriz_numeros[line][timer2].y,  matriz_numeros[line][timer2].width, matriz_numeros[line][timer2].height, sheet_numeros);
	graphics.DrawImage2D(424, 545, 26, 26, matriz_numeros[line][timer1].x,  matriz_numeros[line][timer1].y,  matriz_numeros[line][timer1].width, matriz_numeros[line][timer1].height, sheet_numeros);


	if (timer1==0 && timer2==0 && timer3==0 && timer4==0)
	{
		contador = false;
	}

	
		if (time_next_number1 >= 1.0)
		{

			if(timer1==0)
			{
				
				if (timer1==0 && timer2==0 && timer3==0 && timer4==0)
					contador = false;

				if(contador)
				timer1 = 10;
			}
			

			if (contador)
			{
			timer1--;
			time_next_number1=0;
			time_next_number2++;
			time_next_number3++;
			time_next_number4++;

			}
		}
	
		if (time_next_number2 == 10.0)
		{

			if(timer2==0)
				timer2 = 6;
			
			timer2--;
			time_next_number2=0;
		}

		if (time_next_number3 == 60.0)
		{

			if(timer3==0)
				timer3 = 10;
			
			timer3--;
			time_next_number3=0;
		}

		if (time_next_number4 == 600.0)
		{

			if(timer4==0)
				timer4 = 6;
			
			timer4--;
			time_next_number4=0;
		}
}

void movimentacao(Heroi *personagem, Sprite_sheet matriz_personagem[][10], Image sheet_personagem)
{

	if (KeyState[KEY_LEFTSHIFT] || KeyState[KEY_RIGHTSHIFT])
		personagem->velocidade=160;
	else
		personagem->velocidade=80;

	bool next_frame = false;
    personagem->time_next_frame = personagem->time_next_frame + dt;
 
    if (KeyState[KEY_LEFT])
    {
		if(Mapa[(int)(((personagem->x)/32)+2)][(int)(((personagem->y)/32)+2)] == 'o')
		{
		   personagem->animation_clip = 1;
		   next_frame = true;      
		   personagem->x -=  personagem->velocidade * dt;
		}
    }

    else if (KeyState[KEY_RIGHT]&& Mapa[(int)(((personagem->x)/32)+2)][(int)(((personagem->y)/32)+2)] == 'o')
    {
      personagem->animation_clip = 2;
      next_frame = true;
      personagem->x += personagem->velocidade * dt;
	
    }

    else if (KeyState[KEY_UP])
    {
		if(Mapa[(int)(((personagem->x)/32)+2)][(int)(((personagem->y)/32)+2)] == 'o')
		{
        personagem->animation_clip = 3;
        next_frame = true;
        personagem->y += personagem->velocidade * dt;
		}
    }
    else if (KeyState[KEY_DOWN])
    {
		if(Mapa[(int)(((personagem->x)/32)+2)][(int)(((personagem->y)/32)+2)] == 'o')
		{
        personagem->animation_clip = 0;
        next_frame = true;
        personagem->y -= personagem->velocidade * dt;
		}
    }
 
    if (next_frame)
    {
        if (personagem->time_next_frame > 0.1)
        {
            personagem->animation_index++;
            personagem->time_next_frame = 0;
        }
        if (personagem->animation_index > 3)
            personagem->animation_index = 0;
    }
 
    graphics.DrawImage2D(personagem->x, personagem->y, SPRITE_SIZE_X+24, SPRITE_SIZE_Y+32, matriz_personagem[personagem->animation_clip][personagem->animation_index].x, matriz_personagem[personagem->animation_clip][personagem->animation_index].y, matriz_personagem[personagem->animation_clip][personagem->animation_index].width, matriz_personagem[personagem->animation_clip][personagem->animation_index].height, sheet_personagem);
}

void gera_equipamento(Heroi personagem, Equipamento *equipamento, Sprite_sheet matriz_equipamento[][10], Image sheet_equipamento)
{

	if (Collision(personagem.x+(SPRITE_SIZE_X)/2, personagem.y-(SPRITE_SIZE_Y)/2, equipamento->x, equipamento->y, equipamento->x+64, equipamento->y-64))
		equipamento->status=false;

	if(equipamento->status)
		graphics.DrawImage2D(equipamento->x, equipamento->y, 32, 32, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].x, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].y, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].width, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].height, sheet_equipamento);

}

void monta_inventario(Heroi personagem, Equipamento *equipamento, Sprite_sheet matriz_equipamento[][10], Image sheet_equipamento)
{

	if (!equipamento->status)
	{
		graphics.DrawImage2D(623, 504, 64, 64, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].x, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].y, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].width, matriz_equipamento[equipamento->animation_clip][equipamento->animation_index].height, sheet_equipamento);
	}

	if (KeyState['t'] && equipamento->status == false)
	{

		if (personagem.animation_clip==0)
				equipamento->y = personagem.x - 50;
			else if (personagem.animation_clip==1)
				equipamento->x = personagem.x - 50;
			else if (personagem.animation_clip==2)
				equipamento->x = personagem.x + 50;
			else if (personagem.animation_clip==3)
				equipamento->y = personagem.y + 50;
		
		equipamento->status=true;  
	}
}

//void controla_arma(Heroi personagem, Equipamento *equipamento, Projetil *projetil)
//{
//	int pos;
//
//	if ((equipamento->pente.resistencia_max - equipamento->pente.resistencia)<equipamento->pente.resistencia_max)
//		equipamento->pente.municao_disponivel = true;
//	else
//		equipamento->pente.municao_disponivel = false;
//
//	if (equipamento->pente.municao_disponivel)
//	{
//
//		if (KeyState['h'] && equipamento->status == false)
//		{
//			projetil->disparo = true;
//			projetil->timer_projetil = true;
//			projetil->y = personagem.y;
//			projetil->x = personagem.x;
//			equipamento->pente.resistencia= equipamento->pente.resistencia-1;
//
//			if (personagem.animation_clip==0)
//				projetil->direcao = 0;
//			else if (personagem.animation_clip==1)
//				projetil->direcao = 1;
//			else if (personagem.animation_clip==2)
//				projetil->direcao = 2;
//			else if (personagem.animation_clip==3)
//				projetil->direcao = 3;
//
//			KeyState['h'] = false;
//
//		}
//			
//	}
//
//		if (projetil->timer_projetil)
//		{
//			projetil->tempo_projetil = projetil->tempo_projetil + dt;
//
//			if (projetil->direcao == 0)
//			{
//				projetil->y = projetil->y - dt * projetil->velocidade;
//				pos = 0;
//			}
//		
//			else if (projetil->direcao == 1)
//			{
//				projetil->x = projetil->x - dt * projetil->velocidade;
//				pos = 1;
//			}
//		
//			else if (projetil->direcao == 2)
//			{
//				projetil->x = projetil->x + dt * projetil->velocidade; 
//				pos = 1;
//			}
//		
//			else if (projetil->direcao == 3)
//			{
//				projetil->y = projetil->y + dt * projetil->velocidade; 
//				pos = 0;
//			}
//		}
//	
//
//		if (projetil->tempo_projetil >= 3.0) //tempo de desaparecimento do projetil
//		{
//			projetil->disparo = false;
//			projetil->tempo_projetil = 0;
//		}
//
//		if (projetil->disparo)
//			graphics.DrawImage2D(projetil->x, projetil->y, 32, 32, projetil->matriz_projetil[0][pos].x, projetil->matriz_projetil[0][pos].y, projetil->matriz_projetil[0][pos].width, projetil->matriz_projetil[0][pos].height, projetil->sheet_projetil);
//	}



void controla_arma(Heroi personagem, Equipamento *equipamento, Projetil *projetil, Sprite_sheet matriz_projetil[][10], Image sheet_projetil)
{
	int pos;
	int ajuste_y = 0;
	static int cont_seg = 0;

	if ((equipamento->pente.resistencia_max - equipamento->pente.resistencia)<equipamento->pente.resistencia_max)
		equipamento->pente.municao_disponivel = true;
	else
		equipamento->pente.municao_disponivel = false;

	if (equipamento->pente.municao_disponivel)
	{

		if (KeyState['h'] && equipamento->status == false && projetil->disparo==false)
		{
			projetil->disparo = true;
			projetil->timer_projetil = true;
			projetil->x = personagem.x;
			projetil->y = personagem.y;
			equipamento->pente.resistencia= equipamento->pente.resistencia-1;

			if (personagem.animation_clip==0)
				projetil->direcao = 0;
			else if (personagem.animation_clip==1)
				projetil->direcao = 1;
			else if (personagem.animation_clip==2)
				projetil->direcao = 2;
			else if (personagem.animation_clip==3)
				projetil->direcao = 3;

			KeyState['h'] = false;

		}
			
	}

		if (projetil->timer_projetil)
		{
			projetil->tempo_projetil = projetil->tempo_projetil + dt;

				if (projetil->tempo_projetil >=1.0)
				{
					cont_seg++;
					projetil->tempo_projetil=0;
				}


			if (projetil->direcao == 0)
			{
				projetil->y -= dt * projetil->velocidade;
				pos = 1;
			}
		
			else if (projetil->direcao == 1)
			{
				projetil->x -= dt * projetil->velocidade;
				ajuste_y = 20;
				pos = 2;
			}
		
			else if (projetil->direcao == 2)
			{
				projetil->x += dt * projetil->velocidade; 
				ajuste_y = 20;
				pos = 0;
			}
		
			else if (projetil->direcao == 3)
			{
				projetil->y += dt * projetil->velocidade; 
				pos = 3;
			}
		}
	

		if (cont_seg == 4 || Collision(projetil_magico.x+10, projetil_magico.y+10, monstro.x, monstro.y, monstro.x+56, monstro.y+80)) //tempo de desaparecimento do projetil
		{
			projetil->disparo = false;
			cont_seg = 0;

			if (Collision(projetil_magico.x+10, projetil_magico.y+10, monstro.x, monstro.y, monstro.x+56, monstro.y+80))
			{
				projetil->disparo = false;
				monstro.vida--; //<---------- bala tirando mais vida do que deveria
				
			}
		}
		

		if (projetil->disparo)
			graphics.DrawImage2D(projetil->x, projetil->y+ajuste_y, 32, 32, matriz_projetil[0][pos].x, matriz_projetil[0][pos].y, matriz_projetil[0][pos].width, matriz_projetil[0][pos].height, sheet_projetil);
	
		/*printf ("x = %f, y = %f\n", personagem.x, personagem.y);
		printf ("x = %f, y = %f\n", projetil->x, projetil->y);
		printf ("numero da casa: x= %d, y=%d", (int)((personagem.x/32)+2), (int)((personagem.y/32)+2));*/
}

//void LoadMapa(char *filename) //LoadMapa (char *filename, int count_line, matriz mapa)
//{
//    int c;
//    int count_line = 19;
//    int count_col = 0;
//    FILE *file;
//    file = fopen(filename, "r");
//    if (file) 
//    {
//        while ((c = getc(file)) != EOF)
//        {
//            if (count_col == MAP_WIDTH-1)
//            {
//                count_line--;
//                count_col = 0;
//            }
//            else
//            {
//                Mapa[count_col][count_line] = (char)c;
//                count_col++;
//            }
//        }
//        fclose(file);
//    }
//}

void MoveEnemy1()
{
	monstro.animation_clip = rand() % 4;
	bool next_frame = false;
    monstro.time_next_frame = monstro.time_next_frame + dt;

    if (monstro.animation_clip == 3) //left
    {
        if (Mapa[(int)((((monstro.x + 24))/24))][((int)(monstro.y + 12)/24)] == 'x')
        {
            monstro.animation_clip = rand() % 4;
        }
        else
        {
            monstro.x -= monstro.velocidade * graphics.GetElapsedTime();
			monstro.animation_clip = 3;
			next_frame = true;
        
        }
    }
    else if (monstro.animation_clip == 1) //right
    {
        if (Mapa[(int)(((monstro.x)/24))][(int)((monstro.y + 12)/24)] == 'x')
        {
            monstro.animation_clip = rand() % 4;
        }
        else
        {
            monstro.x += monstro.velocidade * graphics.GetElapsedTime();
			monstro.animation_clip = 1;
			next_frame = true;
        }
    }
    if (monstro.animation_clip == 2) //down
    {
        if (Mapa[(int)((((monstro.x + 12))/24))][(int)(monstro.y)/24] == 'x')
        {
            monstro.animation_clip = rand() % 4;
        }
        else
        {
            monstro.y -= monstro.velocidade * graphics.GetElapsedTime();
			monstro.animation_clip = 2;
			next_frame = true;
        }
    }
    if (monstro.animation_clip == 0)//up
    {
        if (Mapa[(int)((((monstro.x + 12))/24))][(int)(monstro.y + 24)/24] == 'x')
        {
            monstro.animation_clip = rand() % 4;
        }
        else
        {
            monstro.y += monstro.velocidade * graphics.GetElapsedTime();
			monstro.animation_clip = 0;
			next_frame = true;
        }
    }

	  if (next_frame)
    {
        if (monstro.time_next_frame > 0.1)
        {
            monstro.animation_index++;
            monstro.time_next_frame = 0;
        }
        if (monstro.animation_index > 3)
           monstro.animation_index = 0;
    }

 
	  if (monstro.vida>0)
			graphics.DrawImage2D(monstro.x, monstro.y, 56, 80, matriz_monstro[monstro.animation_clip][monstro.animation_index].x, matriz_monstro[monstro.animation_clip][monstro.animation_index].y, matriz_monstro[monstro.animation_clip][monstro.animation_index].width, matriz_monstro[monstro.animation_clip][monstro.animation_index].height, sheet_monstro);

	  if (monstro.vida == 0)
	  {
		  timer_monstro = timer_monstro +dt;
		  if (timer_monstro >= 3)
		  {
		  monstro.x=500;
		  monstro.y=400;
		  monstro.vida=3;
		  timer_monstro = 0;
		  }
	  }
	  /*printf("vida monstro: %d", monstro.vida);*/
}

void LoadMapa(char *filename) //LoadMapa (char *filename, int count_line, matriz mapa)
{
    int c;
    /*int count_line_max = 19;
	int count_line = 0;
    int count_col = 0;*/
	int x, y;
    FILE *file;
    file = fopen(filename, "r");
    if (file) 
    {
		for (x = 0; x < 30; x++)//N�O ENTENDI PORQUE COLOCAR 30 AQUI
        {
            for (y = 0; y < 26; y++)
			{
				c = fgetc(file);

				if (c != EOF)
					Mapa[x][y] = (char)c;
				else
					fclose(file);
			}
		}


                
               
      /*  while ((c = fgetc(file)) != EOF)
        {
            if (count_col == MAP_WIDTH-1)
            {
                count_line++;
                count_col = 0;
            }
            else
            {
                Mapa[count_line][count_col] = (char)c;
                count_col++;
            }
        }*/
        /*fclose(file);*/
	}
}