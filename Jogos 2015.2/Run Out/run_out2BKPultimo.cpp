#include "Graphics.h" 
#include "Audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#include "Mouse.h"
#include "Keyboard.h"
#include "Auxiliar.h"

#define SIZE_THUMBNAIL 100
#define SPRITE_SIZE_X 32
#define SPRITE_SIZE_Y 48
#define MAP_WIDTH 25
#define MAP_HEIGHT 19
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5

Graphics graphics; //definido em teste
int k, j;
float camerax, cameray;
bool pause;
int corredor; //comeco do jogo
int aula;


int aux = -1; //auxilia a mudanca de menus pelo teclado
bool delay_on = false; //auxilia a funcao delay;
float dt;
float timer_monstro;

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
	
	Image sheet_equipamento;
	Sprite_sheet matriz_equipamento[4][10];

	bool existe; //só para poder criar um equipamento nulo

	char tag; // R para ranged e M para melee
	float reuso; //tempo para poder utilizar o item normalmente

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
	bool equipado;
	Equipamento weapon;
	Equipamento inventario_item[3];
	int pontuacao;

	Image sheet_heroi;
	Sprite_sheet matriz_heroi[4][10];

}; typedef struct heroi Heroi;

struct inimigo
{

	float x; //posicao x do heroi
	float y; //posicao y do heroi
	int animation_clip; //clip da animacao do heroi
	int animation_index; //indice atual da animacao do heroi
	float time_next_frame; //contador de tempo para avancar na animacao
	bool next_frame;
    
	int vida;
	float velocidade;
	int pontos; //quantos pontos o inimigo dá quando morre
	int dano;

	Image sheet_inimigo;
	Sprite_sheet matriz_inimigo[4][10];

	bool existe;

}; typedef struct inimigo Inimigo;

struct sala
{

	bool item_presente;
	bool visitada; //variavel de controle do random que gera as armar e itens da sala. Se a sala ja tiver sido visitada, o random de gerar item não é executado

	Equipamento equipamento[20];//------------>botar com vetor pois algum retardado vai querer dropar todos os itens na mesma sala; acrescentar um contador de itens da sala

	Inimigo inimigo[10];
	int max_inimigo;

	char matriz_mapa[20][26];
	Image mapa_base;
	Image mapa_parallax;

}; typedef struct sala Sala;


/*--------Cronômentro---------*/
void timer();
/*-----------------------------*/


/*FUNÇÕES*/
void load ();// carregar todas as variaveis
void update (float dt); // tudo que depende de dt (tempo)
void draw(); // desenhar tudo do jogo

void movimentacao(Heroi *personagem, Sala *sala);

void gera_equipamento(/*Sala *sala*/); //recebe a id da sala na qual o personagem se encontra
void controle_equipamento(Heroi *personagem, Sala *sala);
void inventario(Heroi *personagem, Sala *sala);
void controla_arma(Heroi *personagem);
void gera_inimigo(Sala *sala);
void controle_inimigo(Inimigo *inimigo, Sala *sala);

void LoadMapa(char *filename, int linhas, int colunas, char matriz_mapa[][26]);

//void MoveEnemy1();

int scene = 0; //determina a tela exibida
int dificuldade = 0; //variavel auxiliar no menu de dificuldade
int personagem = 0;
int modo = 0;
bool music = true;

Image menu_entrada[2];

Image menu_principal;
Image menu_principal_selecionado[3];
Image menu_principal_enemy[4];

Image menu_opcoes[3]; //0 - musica on, 1 - musica off, 2- voltar selecionado;

Image menu_modo[8]; //0 - contagem, 1 - pesadelo, 2 - cacador, 6-voltar, 7-selecionar

Image menu_dificuldade[8]; //0-dia, 1-tarde, 2-noite, 3-diaminiatura, 4-tardeminiatura, 5-noiteminiatura, 6-voltar, 7-selecionar

Image menu_personagem[8]; //0-amanda, 1-luis, 2-marcelo, 3-amandaminiatura, 4-luisminiatura, 5-marcelominiatura, 6-voltar, 7-selecionar

Image menu_pause[8]; //0-menu, 1-retornarselecionado, 2-menuselecionado
Image HUD;



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


Heroi amanda;
Heroi luis;
Heroi marcelo;

Inimigo hound;
Inimigo greyhound;

Equipamento equipamentos_jogo[20];
Equipamento pistola;
Equipamento arco;
Equipamento katana;
Equipamento faca;
Equipamento arma;


Equipamento pacote_equipamento[100]; //vetor que guarda todas as armas geradas pelo jogo

/*---------------------MAPAS-----------------------------*/
Sala banheiroM; //MODELOS
Sala blocoA1;
Sala blocoA2;
Sala blocoA3;
Sala blocoB1;
Sala blocoB2;
Sala blocoB3;
Sala aulaP;
Sala almoxarifado;
Sala escritorio;
Sala laboratorio;
Sala oficina;
Sala escadas;
Sala elevador;
Sala saida;

Audio passos;

Sala atual;
Heroi jogador;
void Mapas(Heroi *personagem, Sala *sala);


Sala mapas[20][8];
char acesso;

Image amanda_tiro;

//-------------------------------------------------------------------------------------------- começo do programa em si------------------------------------------///////////////////////////////////////////

void MainLoop(void)
{
	
	dt = graphics.GetElapsedTime();
	graphics.SetMouseMotionInput(MouseMotionInput); //trata a funcao entre parenteses, inserindo os paramentros obtidos do sistema na funcao
	graphics.SetMouseClickInput(MouseClickInput);
	graphics.SetKeyboardInput(KeyboardInput);

	
	draw();
	update(dt);
	KeyboardExecute(); //processa o teclado. Tudo relativo a teclado é processado aqui
	//printf ("x = %d, y = %d\n", click_x, click_y);
	/*printf ("dificuldade = %d\n", dificuldade);*/
	/*printf ("mouse_x = %d, mouse_y = %d\n", mouse_x, mouse_y);*/
	//printf ("clip = %d\n", amanda.animation_clip);
	
	/*printf ("balas restantes = %d\n", arma.pente.resistencia);*/
	//printf ("Equipado? = %d\n", jogador.equipado);	

	click_x = 0;
	click_y = 0;
	
}

int main (void)
{
	graphics.CreateMainWindow(800, 600, "Run Out");

	load();

	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();

	
	return 0;
}

void load ()
{
	menu_entrada[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(sem pressione enter).png");
	//menu_entrada[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(pressione enter 80porcento).png");
	menu_entrada[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_entrada(pressione enter 100porcento).png");
	menu_entrada_timer.LoadAudio("Data\\Audios\\timer(TIRAR ISSOmenu_entrada).mp3");

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

	menu_modo[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(contagem).png");
	menu_modo[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(pesadelo).png");
	menu_modo[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(cacador).png");
	menu_modo[6].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(voltar).png");
	menu_modo[7].LoadPNGImage("Data\\Imagens\\Menus\\menu_dificuldade(selecionar).png");


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

	menu_pause[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause.png");
	menu_pause[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(retornar).png");
	menu_pause[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(menu).png");

	HUD.LoadPNGImage("Data\\Imagens\\Menus\\HUD.png");

	sheet_reaper.LoadPNGImage("Data\\reaper.png");
	InitSprites(reapermenu, 60, 60, 4, 4);

	sheet_numeros.LoadPNGImage("Data\\Imagens\\numbers2.png");
	InitSprites(matriz_numeros, 32, 32, 1, 10);


	/*----------------------------MAPAS---------------------------*/

	corredor = 0; //comeco do jogo
	aula = 0;

	//SALA
	aulaP.mapa_base.LoadPNGImage("Data\\Mapas\\mapa_salaP_base.png");
	aulaP.mapa_parallax.LoadPNGImage("Data\\Mapas\\mapa_salaP_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_salaP.txt", 19, 25, aulaP.matriz_mapa);
	aulaP.equipamento[0].x = 350; //posicao de spawn do item na sala
	aulaP.equipamento[0].y = 350;
	aulaP.visitada=false;
	aulaP.max_inimigo = 4;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		aulaP.equipamento[g].existe = false;
		aulaP.inimigo[g].existe = false;
		aulaP.inimigo[g].next_frame = false; 
	}

	//ESCADAS
	escadas.mapa_base.LoadPNGImage("Data\\Mapas\\escadas.png");
	escadas.mapa_parallax.LoadPNGImage("Data\\Mapas\\Escadaria_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_escadaria.txt", 19, 25,escadas.matriz_mapa);
	escadas.equipamento[0].x = 400; //posicao de spawn do item na sala
	escadas.equipamento[0].y = 300;
	escadas.visitada=false;
	escadas.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		escadas.equipamento[g].existe = false;
		escadas.inimigo[g].existe = false;
		escadas.inimigo[g].next_frame = false; 
	}

		//SAIDA
	saida.mapa_base.LoadPNGImage("Data\\Mapas\\escadas.png");
	saida.mapa_parallax.LoadPNGImage("Data\\Mapas\\Escadaria_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_saida.txt", 19, 25,saida.matriz_mapa);
	saida.equipamento[0].x = 400; //posicao de spawn do item na sala
	saida.equipamento[0].y = 300;
	saida.visitada=false;
	saida.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		saida.equipamento[g].existe = false;
		saida.inimigo[g].existe = false;
		saida.inimigo[g].next_frame = false; 
	}

	//ELEVADOR
	elevador.mapa_base.LoadPNGImage("Data\\Mapas\\Elevador(consertado).png");
	elevador.mapa_parallax.LoadPNGImage("Data\\Mapas\\Elevador_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_elevador.txt", 19, 25,elevador.matriz_mapa);
	elevador.equipamento[0].x = 350; //posicao de spawn do item na sala
	elevador.equipamento[0].y = 350;
	elevador.visitada=false;
	elevador.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		elevador.equipamento[g].existe = false;
		elevador.inimigo[g].existe = false;
		elevador.inimigo[g].next_frame = false; 
	}

		//ALMOXARIFADO
	almoxarifado.mapa_base.LoadPNGImage("Data\\Mapas\\almoxarifado.png");
	almoxarifado.mapa_parallax.LoadPNGImage("Data\\Mapas\\Escadaria_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_almoxarifado.txt", 19, 25,almoxarifado.matriz_mapa);
	almoxarifado.equipamento[0].x = 350; //posicao de spawn do item na sala
	almoxarifado.equipamento[0].y = 350;
	almoxarifado.visitada=false;
	almoxarifado.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		almoxarifado.equipamento[g].existe = false;
		almoxarifado.inimigo[g].existe = false;
		almoxarifado.inimigo[g].next_frame = false; 
	}

	//LABORATORIO
	laboratorio.mapa_base.LoadPNGImage("Data\\Mapas\\laboratorio.png");
	laboratorio.mapa_parallax.LoadPNGImage("Data\\Mapas\\Escadaria_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_laboratorio.txt", 19, 25,laboratorio.matriz_mapa);
	laboratorio.equipamento[0].x = 350; //posicao de spawn do item na sala
	laboratorio.equipamento[0].y = 350;
	laboratorio.visitada=false;
	laboratorio.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		laboratorio.equipamento[g].existe = false;
		laboratorio.inimigo[g].existe = false;
		laboratorio.inimigo[g].next_frame = false; 
	}

	//OFICINA
	oficina.mapa_base.LoadPNGImage("Data\\Mapas\\oficina.png");
	oficina.mapa_parallax.LoadPNGImage("Data\\Mapas\\Escadaria_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_oficina.txt", 19, 25,oficina.matriz_mapa);
	oficina.equipamento[0].x = 350; //posicao de spawn do item na sala
	oficina.equipamento[0].y = 350;
	oficina.visitada=false;
	oficina.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		oficina.equipamento[g].existe = false;
		oficina.inimigo[g].existe = false;
		oficina.inimigo[g].next_frame = false; 
	}

	//BANHEIRO_M
	banheiroM.mapa_base.LoadPNGImage("Data\\Mapas\\BanheiroM.png");
	banheiroM.mapa_parallax.LoadPNGImage("Data\\Mapas\\Elevador_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_banheiroM.txt", 19, 25,banheiroM.matriz_mapa);
	banheiroM.equipamento[0].x = 280;//posicao de spawn do item na sala
	banheiroM.equipamento[0].y = 190;
	banheiroM.visitada=false;
	banheiroM.max_inimigo = 2;
	for (int g= 0; g < 10; g++) //inicializo o vetor dizendo que esta vazio
	{
		banheiroM.equipamento[g].existe = false;
		banheiroM.inimigo[g].existe = false;
		banheiroM.inimigo[g].next_frame = false; 
	}


	//CORREDOR A1
	blocoA1.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoA1.png");
	blocoA1.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoA1_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoA1.txt", 19, 25, blocoA1.matriz_mapa);
	blocoA1.visitada = false;
	blocoA1.max_inimigo = 7;
	for (int g= 0; g < 10; g++)
	{
		blocoA1.equipamento[g].existe = false;
		blocoA1.inimigo[g].existe = false;
		blocoA1.inimigo[g].next_frame = false; 
	}

	//CORREDOR A2
	blocoA2.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoA2.png");
	blocoA2.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoA2_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoA2.txt", 19, 25, blocoA2.matriz_mapa);
	blocoA2.visitada = false;
	blocoA2.max_inimigo = 7;
	for (int g= 0; g < 10; g++)
	{
		blocoA2.equipamento[g].existe = false;
		blocoA2.inimigo[g].existe = false;
		blocoA2.inimigo[g].next_frame = false; 
	}
	//CORREDOR A3
	blocoA3.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoA3.png");
	blocoA3.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoA3_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoA3.txt", 19, 25, blocoA3.matriz_mapa);
	blocoA3.visitada = false;
	blocoA3.max_inimigo = 7;
	for (int g= 0; g < 10; g++)
	{
		blocoA3.equipamento[g].existe = false;
		blocoA3.inimigo[g].existe = false;
		blocoA3.inimigo[g].next_frame = false; 
	}
	
	passos.LoadAudio("Data\\Audios\\footstep.wav");


	//Salas replicadas (as acima são apenas os modelos)

	mapas[0][0] = blocoA1;
	mapas[0][1] = aulaP;
	mapas[0][2] = escadas;
	mapas[0][3] = oficina;

	mapas[1][0] = blocoA2;
	mapas[1][1] = aulaP;
	mapas[1][2] = aulaP;
	mapas[1][3] = elevador;
	mapas[1][4] = escadas;

	mapas[2][0] = blocoA3;
	mapas[2][1] = escritorio;
	mapas[2][2] = laboratorio;
	mapas[2][3] = banheiroM;
	mapas[2][4] = saida;

	mapas[3][0] = blocoB1;
	mapas[3][1] = aulaP;
	mapas[3][2] = oficina;
	mapas[3][3] = oficina;

	mapas[4][0] = blocoB2;
	mapas[4][1] = aulaP;
	mapas[4][2] = aulaP;
	mapas[4][3] = elevador;
	mapas[4][4] = elevador;

	mapas[5][0] = blocoB3;
	mapas[5][1] = escritorio;
	mapas[5][2] = laboratorio;
	mapas[5][3] = banheiroM;
	mapas[5][4] = saida;

	atual = mapas[corredor][aula]; //onde o personagem comeca no jogo


	/*----------------------------LOADstructsHEROI---------------------------*/

	//AMANDA
	amanda.x = 186; 
	amanda.y = 350; 
	amanda.animation_clip = 0; 
	amanda.animation_index = 0; 
	amanda.time_next_frame = 0;
	amanda.vida = 3;
	amanda.velocidade = 80;
	amanda.equipado = false;
	amanda.sheet_heroi.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_amanda.png");
	InitSprites(amanda.matriz_heroi, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	amanda_tiro.LoadPNGImage("Data\\Imagens\\Sprites\\amanda_tiro.png");

	//LUIS
	luis.x = 186; 
	luis.y = 400; 
	luis.animation_clip = 0; 
	luis.animation_index = 0; 
	luis.time_next_frame = 0;
	luis.vida = 3;
	luis.velocidade = 80;
	luis.equipado = false;
	luis.sheet_heroi.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_luis.png");
	InitSprites(luis.matriz_heroi, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	//MARCELLO
	marcelo.x = 186; 
	marcelo.y = 400; 
	marcelo.animation_clip = 0; 
	marcelo.animation_index = 0; 
	marcelo.time_next_frame = 0;
	marcelo.vida = 3;
	marcelo.velocidade = 80;
	marcelo.equipado = false;
	marcelo.sheet_heroi.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_marcelo.png");
	InitSprites(marcelo.matriz_heroi, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	/*----------------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-------------------*/



	/*----------------------------LOADstructsINIMIGOS---------------------------*/

	//HOUND
	hound.x = 240; 
	hound.y = 400; 
	hound.animation_clip = 0; 
	hound.animation_index = 0; 
	hound.time_next_frame = 0;
	hound.vida = 2;
	hound.velocidade = 60;
	hound.sheet_inimigo.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_hound.png");
	InitSprites(hound.matriz_inimigo, 48, 64, 4, 3);

	//GREYHOUND
	greyhound.x = 240; 
	greyhound.y = 400; 
	greyhound.animation_clip = 0; 
	greyhound.animation_index = 0; 
	greyhound.time_next_frame = 0;
	greyhound.vida = 2;
	greyhound.velocidade = 90;
	greyhound.sheet_inimigo.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_greyhound.png");
	InitSprites(greyhound.matriz_inimigo, 48, 64, 4, 3);


	/*----------------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-------------------*/


	/*----------------------------EQUIPAMENTOS----------------------------*/

	//ARMA
	arma.animation_clip = 0;
	arma.animation_index = 0;
	arma.pente.resistencia_max = 10;
	arma.pente.resistencia = 10;
	arma.x = 350;
	arma.y = 350;
	arma.sheet_equipamento.LoadPNGImage("Data\\Imagens\\arma.png");
	InitSprites(arma.matriz_equipamento, 64, 64, 1, 1);
	
	for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		arma.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_magico.png");
		InitSprites(arma.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		arma.pente.projetil[g].disparo = false;
		arma.pente.projetil[g].velocidade = 200;
		arma.pente.projetil[g].timer_projetil = false;
		arma.pente.projetil[g].tempo_projetil = 0;
    }

	//PISTOLA
	pistola.animation_clip = 0;
	pistola.animation_index = 0;
	pistola.pente.resistencia_max = 14;
	pistola.pente.resistencia = 14;
	pistola.x = 400;
	pistola.y = 300;
	pistola.sheet_equipamento.LoadPNGImage("Data\\Imagens\\Armas\\pistola.png");
	InitSprites(pistola.matriz_equipamento, 64, 64, 1, 1);
	pistola.tag = 'R';
	pistola.reuso = 1.0;
	
	for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		pistola.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_pistola.png");
		InitSprites(pistola.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		pistola.pente.projetil[g].disparo = false;
		pistola.pente.projetil[g].velocidade = 300;
		pistola.pente.projetil[g].timer_projetil = false;
		pistola.pente.projetil[g].tempo_projetil = 0;
    }

	//ARCO
	arco.animation_clip = 0;
	arco.animation_index = 0;
	arco.pente.resistencia_max = 7;
	arco.pente.resistencia = 7;
	arco.x = 400;
	arco.y = 300;
	arco.sheet_equipamento.LoadPNGImage("Data\\Imagens\\Armas\\arco.png");
	InitSprites(arco.matriz_equipamento, 64, 64, 1, 1);
	arco.tag = 'R';
	arco.reuso = 2.5;
	
	for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		arco.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_arco.png");
		InitSprites(arco.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		arco.pente.projetil[g].disparo = false;
		arco.pente.projetil[g].velocidade = 200;
		arco.pente.projetil[g].timer_projetil = false;
		arco.pente.projetil[g].tempo_projetil = 0;
    }

	//KATANA
	katana.animation_clip = 0;
	katana.animation_index = 0;
	katana.pente.resistencia_max = 10;
	katana.pente.resistencia = 10;
	katana.x = 400;
	katana.y = 300;
	katana.sheet_equipamento.LoadPNGImage("Data\\Imagens\\Armas\\katana.png");
	InitSprites(katana.matriz_equipamento, 64, 64, 1, 1);
	katana.tag = 'M';
	katana.reuso = 0.7;
		for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		katana.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_slash.png");
		InitSprites(katana.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		katana.pente.projetil[g].disparo = false;
		katana.pente.projetil[g].timer_projetil = false;
		katana.pente.projetil[g].tempo_projetil = 0;
    }

	/*----------------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-------------------*/

}

void update (float dt)
{

	if (scene == 5)
	{
		menu_principal_musica.Stop();
		
		graphics.DrawImage2D(0, 0, 800, 600, atual.mapa_base); //MAPA BASE
       	graphics.DrawImage2D(0, 0, 800, 600, HUD); //HUD

		timer(); //funcao do cronometro

		Mapas(&jogador,&atual);
		movimentacao(&jogador, &atual); //nem precisavar passar endereco de jogador, por jogador ser global, mas é bom que facilita um reset
		controle_equipamento(&jogador, &atual);
		inventario(&jogador, &atual);
		controle_inimigo(&hound, &atual);
		controla_arma(&jogador); 
		
		//graphics.DrawImage2D(0, 0, 800, 600, atual.mapa_parallax); //MAPA PARALLAX

		/*MoveEnemy1();*/
	}

	if (pause == true)
	{
		graphics.DrawImage2D(0, 0, 800, 600, menu_pause[0]);

		if (Collision(mouse_x, mouse_y, 266, 170, 533, 192))
		{
			graphics.DrawImage2D(menu_pause[1]);

			if (Collision(click_x, click_y, 266, 170, 533, 192))
				pause = false;
		}

		else if (Collision(mouse_x, mouse_y, 287, 123, 510, 144))
		{
			graphics.DrawImage2D(menu_pause[2]);

			if (Collision(click_x, click_y, 287, 123, 510, 144))
				pause = false;
		}
	}
}

void draw()
{

	/*printf("cena = %d\n", scene);*/

	if (scene ==1 ||scene ==-1 ||scene ==2 || scene ==3 || scene ==4)
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
		static float time_aux = 0;
		time_aux += dt;
		//--------------------------------------RESPONSAVEL PELA ANIMACAO DO MENU--------------------------------------------------//
		graphics.DrawImage2D(menu_principal);

		time_next_frame2 = time_next_frame2 + graphics.GetElapsedTime();
 
        if (time_next_frame2 > 0.1) //tempo maior que 1 segundo
        {
             animation_index2++; //aumenta indice
             time_next_frame2 = 0; //zera tempo
        }

        if (animation_index2 >3)
             animation_index2 = 0;

		/*graphics.DrawImage2D(350, 300, 32, 50, matriz_amanda[3][animation_index2].x,  matriz_amanda[3][animation_index2].y,  matriz_amanda[3][animation_index2].width, matriz_amanda[3][animation_index2].height, sheet_amanda);*/
		
		//------------------------------------------------------------------------------------------------------------------------------//
		
		if (Collision(mouse_x, mouse_y, 45, 222, 272, 280)||aux==0)
		{
			aux = 0;//evitar conflito do mouse e teclado
			graphics.DrawImage2D(menu_principal_selecionado[0]);

			if (Collision(click_x, click_y, 45, 222, 272, 280))
				scene =2;
		}

		if (Collision(mouse_x, mouse_y, 18, 136, 295, 200)||aux==1)
		{
			aux = 1;
			graphics.DrawImage2D(menu_principal_selecionado[1]);

			if (Collision(click_x, click_y, 18, 136, 295, 200))
				scene = -1;
		}

		if (Collision(mouse_x, mouse_y, 68, 56, 236, 112)||aux==2)
		{
			aux = 2;
			graphics.DrawImage2D(menu_principal_selecionado[2]);

			if (Collision(click_x, click_y, 68, 56, 236, 112))
				exit(0);
		}

		
		if ((aux == 0 || aux == 1 || aux == 2) && time_aux >= 25) //faz sumir a marcacao pelo teclado
		{
			aux=-1;
			time_aux=0;
		}//OBS: corrigir o bugzinho: se só ficar mechendo com o teclado, depois de um tempo ele apaga de qualquer jeito
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

			if(modo==0)
			{
				graphics.DrawImage2D(menu_modo[0]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					modo = 2;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					modo = 1;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[5]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[4]);
			}

			else if(modo==1)
			{
				graphics.DrawImage2D(menu_modo[1]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					modo = 0;

				if (Collision(click_x, click_y, 571, 273, 672, 373))
					modo = 2;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[3]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[5]);
			}

			else if(modo==2)
			{
				graphics.DrawImage2D(menu_modo[2]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					modo = 1;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					modo = 0;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(138, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[4]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(572, 273, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_modo[3]);
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_modo[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_modo[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 1;
			if (Collision(click_x, click_y, 504, 39, 705, 66) || delay_on)
			{
				delay_on = true;

				if (Delay(2, 0) == 0)
				{
					scene = 3;
					delay_on = false;
				}
			}

		}

		else if (scene == 3)
		{

			if(dificuldade==0)
			{
				graphics.DrawImage2D(menu_dificuldade[0]);
				if (Collision(click_x, click_y, 128, 274, 237, 375))
					dificuldade = 2;
				else if (Collision(click_x, click_y, 571, 274, 672, 373))
					dificuldade = 1;

				if (Collision(mouse_x, mouse_y, 128, 274, 237, 375))
					graphics.DrawImage2D(128, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[5]);
				else if (Collision(mouse_x, mouse_y, 571, 274, 672, 373))
					graphics.DrawImage2D(572, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[4]);
			}

			else if(dificuldade==1)
			{
				graphics.DrawImage2D(menu_dificuldade[1]);
				if (Collision(click_x, click_y, 128, 274, 237, 375))
					dificuldade = 0;

				if (Collision(click_x, click_y, 571, 274, 672, 373))
					dificuldade = 2;

				if (Collision(mouse_x, mouse_y, 128, 274, 237, 375))
					graphics.DrawImage2D(128, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[3]);
				else if (Collision(mouse_x, mouse_y, 571, 274, 672, 373))
					graphics.DrawImage2D(572, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[5]);
			}

			else if(dificuldade==2)
			{
				graphics.DrawImage2D(menu_dificuldade[2]);
				if (Collision(click_x, click_y, 128, 274, 237, 375))
					dificuldade = 1;
				else if (Collision(click_x, click_y, 571, 274, 672, 373))
					dificuldade = 0;

				if (Collision(mouse_x, mouse_y, 128, 274, 237, 375))
					graphics.DrawImage2D(128, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[4]);
				else if (Collision(mouse_x, mouse_y, 571, 274, 672, 373))
					graphics.DrawImage2D(572, 274, SIZE_THUMBNAIL, SIZE_THUMBNAIL, menu_dificuldade[3]);
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_dificuldade[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_dificuldade[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 2;
			if (Collision(click_x, click_y, 504, 39, 705, 66))
			{
				Delay(5, 0);
				scene = 4;
			}

		}

		else if (scene == 4)
		{

			if(personagem==0)
			{
				graphics.DrawImage2D(menu_personagem[0]);
				if (Collision(click_x, click_y, 151, 272, 208, 375))
					personagem = 2;
				else if (Collision(click_x, click_y, 589, 273, 655, 373))
					personagem = 1;

				if (Collision(mouse_x, mouse_y, 151, 273, 208, 375))
					graphics.DrawImage2D(130, 275, 90, 120, menu_personagem[5]);
				else if (Collision(mouse_x, mouse_y, 589, 273, 655, 373))
					graphics.DrawImage2D(574, 275, 90, 120, menu_personagem[4]);

				jogador = amanda;
			}

			else if(personagem==1)
			{
				graphics.DrawImage2D(menu_personagem[1]);
				if (Collision(click_x, click_y, 151, 272, 208, 375))
					personagem = 0;

				if (Collision(click_x, click_y, 589, 273, 655, 373))
					personagem = 2;

				if (Collision(mouse_x, mouse_y, 151, 273, 208, 375))
					graphics.DrawImage2D(130, 276, 90, 120, menu_personagem[3]);
				else if (Collision(mouse_x, mouse_y, 589, 273, 655, 373))
					graphics.DrawImage2D(574, 275, 90, 120, menu_personagem[5]);

				jogador = luis;
			}

			else if(personagem==2)
			{
				graphics.DrawImage2D(menu_personagem[2]);
				if (Collision(click_x, click_y, 151, 273, 208, 375))
					personagem = 1;
				else if (Collision(click_x, click_y, 589, 273, 655, 373))
					personagem = 0;

				if (Collision(mouse_x, mouse_y, 151, 274, 208, 375))
					graphics.DrawImage2D(129, 275, 90, 120, menu_personagem[4]);
				else if (Collision(mouse_x, mouse_y, 589, 273, 655, 373))
					graphics.DrawImage2D(574, 276, 90, 120, menu_personagem[3]);
				
				jogador = marcelo;
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_personagem[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_personagem[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 3;
			if (Collision(click_x, click_y, 504, 39, 705, 66))
				scene = 5;
		}

		//else if (scene == 6) //Game over
		//{
		//	if (tipo == 0)//determina se o game over foi por tempo ou colisao
		//	menu_principal_musica.Stop();
	
		//	graphics.DrawImage2D(0, 0, 800, 600, atual->mapa);
		//	graphics.DrawImage2D(0, 0, 800, 600, HUD);
		//	else

		//}

}

void timer()//teste da contagem regressiva
{
	static float time_next_number1;
	static float time_next_number2;
	static float time_next_number3;
	static float time_next_number4;

	int line = 0;
	static bool contador = true;

	static int timer1 = 9;
	static int timer2 = 5;
	static int timer3 = 9;
	static int timer4 = 0;

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

void movimentacao(Heroi *personagem, Sala *sala)
{

	if (KeyState[KEY_LEFTSHIFT] || KeyState[KEY_RIGHTSHIFT])
		personagem->velocidade=160;
	else
		personagem->velocidade=80;

	bool next_frame = false;
    personagem->time_next_frame = personagem->time_next_frame + dt;
 
    if (KeyState[KEY_LEFT])
    {
		 if (sala->matriz_mapa[(int)((((personagem->y+24-8))/32))][(int)(personagem->x+5)/32] != 'x' && sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-1)/32] != 'x')
		 {
			 	passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
		   personagem->animation_clip = 1;
		   next_frame = true;      
		   personagem->x -=  personagem->velocidade * dt;

		   camerax = personagem->x-400;
		 }
    }

    else if (KeyState[KEY_RIGHT])
	{           
    
		if (sala->matriz_mapa[(int)((((personagem->y+24-8))/32))][(int)(personagem->x+45)/32] != 'x' && sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+40)/32] != 'x')
		{
				passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}

		  personagem->animation_clip = 2;
		  next_frame = true;
		  personagem->x += personagem->velocidade * dt;
		  
		  if (abs(personagem->x-400)>18) 
		  camerax = personagem->x-400-18;
		}
	
    }

    else if (KeyState[KEY_UP]) 
    {
		if (sala->matriz_mapa[(int)((((personagem->y+24))/32))][(int)(personagem->x)/32] != 'x')//limita altura maxima que o personagem alcança (no caso, bloqueia a 24 pixels de x)
		{
			passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
			personagem->animation_clip = 3;
			next_frame = true;
			personagem->y += personagem->velocidade * dt;
			cameray = personagem->y-300;
		}
		
    }
    else if (KeyState[KEY_DOWN])
    {
		if (sala->matriz_mapa[(int)((((personagem->y-2))/32))][(int)(personagem->x+30)/32] != 'x' && sala->matriz_mapa[(int)((((personagem->y-2))/32))][(int)(personagem->x+8)/32] != 'x')
		{
			passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
			personagem->animation_clip = 0;
			next_frame = true;
			personagem->y -= personagem->velocidade * dt;
			cameray = personagem->y-300;
		}
	}

	acesso = sala->matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32];
 
    if (next_frame)
    {
        if (personagem->time_next_frame > 0.15)
        {
            personagem->animation_index++;
            personagem->time_next_frame = 0;
        }
        if (personagem->animation_index > 3)
            personagem->animation_index = 0;
    }
 
    graphics.DrawImage2D(personagem->x, personagem->y, SPRITE_SIZE_X+24, SPRITE_SIZE_Y+34, personagem->matriz_heroi[personagem->animation_clip][personagem->animation_index].x, personagem->matriz_heroi[personagem->animation_clip][personagem->animation_index].y, personagem->matriz_heroi[personagem->animation_clip][personagem->animation_index].width, personagem->matriz_heroi[personagem->animation_clip][personagem->animation_index].height, personagem->sheet_heroi);


//printf ("CASA = %c\n", sala->matriz_mapa[(int)personagem->y/32][(int)personagem->x/32]);
//printf ("x = %d  y = %d\n", (int)personagem->x/32, (int)personagem->y/32);
//printf ("x = %.2f  y = %.2f", personagem->x, personagem->y);
//printf ("Teste+ = %d  teste- = %d", (int)1.8/2, (int)0.8/2);
}

void gera_equipamento(Sala *sala)
{
	int aux_random;
	
	if (sala->visitada == false)
	{
		aux_random = rand() % 3;

		if (aux_random == 0)
		{
			sala->equipamento[0] = pistola;
			sala->equipamento[0].existe = true;
		}
		else if (aux_random == 1)
		{
			sala->equipamento[0] = arco;
			sala->equipamento[0].existe = true;
		}
		else
		{
			sala->equipamento[0] = katana;
			sala->equipamento[0].existe = true;
		}
		printf("Sorteio = %d\n", aux_random);
		sala->visitada = true;
	}
	mapas[corredor][aula] = *sala;
}

void controle_equipamento(Heroi *personagem, Sala *sala)
{
	int i;

	for(i = 0; i<20; i++)
	{
		if(sala->equipamento[i].existe == true)//desenha o equipamento na sala
			{

				graphics.DrawImage2D(sala->equipamento[i].x, sala->equipamento[i].y, 32, 32,/* sala->equipamento[i].matriz_equipamento[sala->equipamento[i].animation_clip][sala->equipamento[i].animation_index].x, sala->equipamento[i].matriz_equipamento[sala->equipamento[i].animation_clip][sala->equipamento[i].animation_index].y, sala->equipamento[i].matriz_equipamento[sala->equipamento[i].animation_clip][sala->equipamento[i].animation_index].width, sala->equipamento[i].matriz_equipamento[sala->equipamento[i].animation_clip][sala->equipamento[i].animation_index].height*/sala->equipamento[i].sheet_equipamento);
			
				if (personagem->equipado == false)
				{
			
					if (Collision(personagem->x+(SPRITE_SIZE_X)/2, personagem->y-(SPRITE_SIZE_Y)/2, sala->equipamento[i].x, sala->equipamento[i].y, sala->equipamento[i].x+64, sala->equipamento[i].y-64)) //checa colisao com equipamento, a posicao ja esta predefinida de acordo com o mapa
					{
						personagem->weapon = sala->equipamento[i];
						personagem->equipado = true;
						sala->equipamento[i].existe = false;
					}
				}
			}
		}
	mapas[corredor][aula] = *sala;
}

void inventario(Heroi *personagem, Sala *sala)
{
	int i;

	if (personagem->equipado)
	{
		graphics.DrawImage2D(623, 500, 64, 64, personagem->weapon.matriz_equipamento[personagem->weapon.animation_clip][personagem->weapon.animation_index].x, personagem->weapon.matriz_equipamento[personagem->weapon.animation_clip][personagem->weapon.animation_index].y, personagem->weapon.matriz_equipamento[personagem->weapon.animation_clip][personagem->weapon.animation_index].width, personagem->weapon.matriz_equipamento[personagem->weapon.animation_clip][personagem->weapon.animation_index].height, personagem->weapon.sheet_equipamento);
	}

	
	if (KeyState['t'] && personagem->equipado)
	{
		
		for (i=0; i<20; i++)
		{
			if (sala->equipamento[i].existe == false) //procura uma posicao vazia no vetor
			{
				if (personagem->animation_clip==0)
					{ 
						if (sala->matriz_mapa[(int)((((personagem->y-50))/32))][(int)(personagem->x)/32] == 'o') //verificação se é possivel dropar na posicao requerida
						{
							sala->equipamento[i] = personagem->weapon;
							sala->equipamento[i].x = personagem->x;
							sala->equipamento[i].y = personagem->y - 50;
							personagem->equipado = false; 
							sala->equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}

					else if (personagem->animation_clip==1)
					{ 
						if (sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-50)/32] == 'o')
						{
							sala->equipamento[i]=personagem->weapon;
							sala->equipamento[i].x = personagem->x-50;
							sala->equipamento[i].y = personagem->y;
							personagem->equipado = false; 
							sala->equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
					else if (personagem->animation_clip==2)
					{ 
						if (sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+50)/32] == 'o')
						{
							sala->equipamento[i]=personagem->weapon;
							sala->equipamento[i].x = personagem->x+50;
							sala->equipamento[i].y = personagem->y;
							personagem->equipado = false; 
							sala->equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
					else if (personagem->animation_clip==3)
					{
						if (sala->matriz_mapa[(int)((((personagem->y+50))/32))][(int)(personagem->x)/32] == 'o')
						{
							sala->equipamento[i] = personagem->weapon;
							sala->equipamento[i].x = personagem->x;
							sala->equipamento[i].y = personagem->y + 50;
							personagem->equipado = false;
							sala->equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
			break; //interrompo o for; não faz sentido continuar rodando o vetor
		}
	}
}
	mapas[corredor][aula] = *sala;
/*
	printf ("CASA SUL = %c\n", sala->matriz_mapa[(int)((((personagem->y-50))/32))][(int)(personagem->x)/32]);
	printf ("CASA LESTE = %c\n", sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-50)/32]);
	printf ("CASA OESTE = %c\n", sala->matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+50)/32]);
	printf ("CASA NORTE = %c\n", sala->matriz_mapa[(int)((((personagem->y+50))/32))][(int)(personagem->x)/32]);*/
}

void controla_arma(Heroi *personagem)
{
	static int i = 0; //auxilia a geracao de projeteis
	int z = 0;
	int pos;
	static int pos_corte;
	int ajuste_y = 0;
	int ajuste_x = 0;
	static float cooldown = 0;
	cooldown += dt;
	static float tempo_corte = 0;

	if (personagem->weapon.pente.resistencia > 0) //verificacao desnecessaria?
	{
		if (cooldown >= personagem->weapon.reuso)
		{
			if (KeyState['h'] && personagem->equipado == true)
			{
				cooldown = 0;
				i++;
				personagem->weapon.pente.resistencia--;

				if (personagem->weapon.tag == 'R')
				{
					for(z=0;z<i ;z++)
					{

							if (personagem->weapon.pente.projetil[z].disparo==false)
							{
								personagem->weapon.pente.projetil[z].disparo = true;
								personagem->weapon.pente.projetil[z].timer_projetil = true;
								personagem->weapon.pente.projetil[z].x = personagem->x;
								personagem->weapon.pente.projetil[z].y = personagem->y;
								personagem->weapon.pente.projetil[z].direcao = personagem->animation_clip;
								break;
							}
					}
				}

				else if (personagem->weapon.tag == 'M')
				{
					personagem->weapon.pente.projetil[z].direcao = personagem->animation_clip;
					personagem->weapon.pente.projetil[z].disparo = true;
					

						if (personagem->animation_clip == 0)
					{
						personagem->weapon.pente.projetil[z].x = personagem->x;
						personagem->weapon.pente.projetil[z].y = personagem->y - 50;
						pos_corte = 1;
					}
		
					else if (personagem->animation_clip == 1)
					{
						personagem->weapon.pente.projetil[z].x = personagem->x - 50;
						personagem->weapon.pente.projetil[z].y = personagem->y;
						pos_corte = 2;
					}
		
					else if (personagem->animation_clip == 2)
					{
						personagem->weapon.pente.projetil[z].x = personagem->x + 50;
						personagem->weapon.pente.projetil[z].y = personagem->y;
						pos_corte = 0;
					}
		
					else if (personagem->animation_clip == 3)
					{
						personagem->weapon.pente.projetil[z].x = personagem->x;
						personagem->weapon.pente.projetil[z].y = personagem->y + 50; 
						pos_corte = 3;
					}
				}


				KeyState['h'] = false;
			}
		}
	}

	if (personagem->weapon.tag == 'R')
	{
	for (z = 0; z < i; z++)
		{
			if(personagem->weapon.pente.projetil[z].disparo)
			{


				if (personagem->weapon.pente.projetil[z].direcao == 0)
				{
					personagem->weapon.pente.projetil[z].y -= dt * personagem->weapon.pente.projetil[z].velocidade;
					pos = 1;
				}
		
				else if (personagem->weapon.pente.projetil[z].direcao == 1)
				{
					personagem->weapon.pente.projetil[z].x -= dt * personagem->weapon.pente.projetil[z].velocidade;
					ajuste_y = 20;
					pos = 2;
				}
		
				else if (personagem->weapon.pente.projetil[z].direcao == 2)
				{
					personagem->weapon.pente.projetil[z].x += dt * personagem->weapon.pente.projetil[z].velocidade; 
					ajuste_y = 20;
					pos = 0;
				}
		
				else if (personagem->weapon.pente.projetil[z].direcao == 3)
				{
					personagem->weapon.pente.projetil[z].y += dt * personagem->weapon.pente.projetil[z].velocidade; 
					pos = 3;
				}

				graphics.DrawImage2D(personagem->weapon.pente.projetil[z].x, personagem->weapon.pente.projetil[z].y+ajuste_y, 32, 32, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].x, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].y, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].width, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].height, personagem->weapon.pente.projetil[z].sheet_projetil);
			}
			
		}	
		/*printf("personagem = %d, modelo = %d\n", personagem->weapon.pente.resistencia, arma.pente.resistencia);*/
	}

	else if (personagem->weapon.tag == 'M')
	{
		if(personagem->weapon.pente.projetil[z].disparo == true)
		{
			tempo_corte += dt;
			graphics.DrawImage2D(personagem->weapon.pente.projetil[z].x, personagem->weapon.pente.projetil[z].y, 64, 64, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos_corte].x, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos_corte].y, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos_corte].width, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos_corte].height, personagem->weapon.pente.projetil[z].sheet_projetil);

			if (tempo_corte >= 0.3)
			{
				personagem->weapon.pente.projetil[z].disparo = false;
				tempo_corte = 0;
			}
			
		}
	
	}
}

void gera_inimigo()
{
	int aux_random;
	static float tempo = 0;
	int i, g, h;

	tempo += dt;

	if (tempo > 10.0)
	{
		for(g = 0; g<20; g++) //roda os corredores
		{
			for(h = 0; h<8; h++)// roda as salas
			{
				for (i=0; i < mapas[g][h].max_inimigo; i++)
				{
					if (mapas[g][h].inimigo[i].existe == false) //procura uma posicao vazia no vetor
					{
						aux_random = rand() % 3;

						if (aux_random == 0)
							mapas[g][h].inimigo[i] = greyhound;
			
						else 
							mapas[g][h].inimigo[i] = hound;
						
						mapas[g][h].inimigo[i].existe = true;
						mapas[g][h].inimigo[i].x += i*90;
				
						break;
					}
				}
			}
		}

		tempo = 0;
	}

	/*printf ("Tempo: %.2f\n", tempo);*/
}

void controle_inimigo(Inimigo *inimigo, Sala *sala)
{
	gera_inimigo();

	int i;

	for (i=0; i<3; i++)
		mapas[corredor][aula].inimigo[i].time_next_frame += dt;

	for(i = 0; i < 2/*sala->max_inimigo*/; i++)
	{
		printf("TEMPO: %f", atual.inimigo[i].time_next_frame);
			mapas[corredor][aula].inimigo[i].next_frame = false;

			if (mapas[corredor][aula].inimigo[i].animation_clip == 0) //up
			{
				if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y+24))/32))][(int)(mapas[corredor][aula].inimigo[i].x)/32] == 'o')
				{
					mapas[corredor][aula].inimigo[i].y += mapas[corredor][aula].inimigo[i].velocidade * dt;
					mapas[corredor][aula].inimigo[i].animation_clip = 0;
					mapas[corredor][aula].inimigo[i].next_frame = true;
				}	

				else
					mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;
			 }

			else if (mapas[corredor][aula].inimigo[i].animation_clip == 3) //left
			{
				if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y+24-8))/32))][(int)(mapas[corredor][aula].inimigo[i].x+5)/32] == 'o' && mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y))/32))][(int)(mapas[corredor][aula].inimigo[i].x-1)/32] == 'o')
				{
					mapas[corredor][aula].inimigo[i].x -= mapas[corredor][aula].inimigo[i].velocidade * dt;
					mapas[corredor][aula].inimigo[i].animation_clip = 3;
					mapas[corredor][aula].inimigo[i].next_frame = true;
				}	
 
				else
					mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;
			}

		if (mapas[corredor][aula].inimigo[i].animation_clip == 1) //right
		{
		   if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y+24-8))/32))][(int)(mapas[corredor][aula].inimigo[i].x+45)/32] == 'o' && mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y))/32))][(int)(mapas[corredor][aula].inimigo[i].x+40)/32] == 'o')
			{	mapas[corredor][aula].inimigo[i].next_frame = true;
				mapas[corredor][aula].inimigo[i].x += mapas[corredor][aula].inimigo[i].velocidade * dt;
				mapas[corredor][aula].inimigo[i].animation_clip = 1;
				
			}

			else
			mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;
		}

		if (mapas[corredor][aula].inimigo[i].animation_clip == 2)//down
		{
			if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y-2))/32))][(int)(mapas[corredor][aula].inimigo[i].x+30)/32] == 'o' && mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y-2))/32))][(int)(mapas[corredor][aula].inimigo[i].x+8)/32] == 'x')
			{
				mapas[corredor][aula].inimigo[i].y -= mapas[corredor][aula].inimigo[i].velocidade * dt;
				mapas[corredor][aula].inimigo[i].animation_clip = 2;
				mapas[corredor][aula].inimigo[i].next_frame = true;
			}

			else
				mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;
		}

		  if (mapas[corredor][aula].inimigo[i].next_frame == true)
			{
				if (mapas[corredor][aula].inimigo[i].time_next_frame > 0.1)
				{
					mapas[corredor][aula].inimigo[i].animation_index++;
					mapas[corredor][aula].inimigo[i].time_next_frame = 0;
				}
				if (mapas[corredor][aula].inimigo[i].animation_index > 2)
				   mapas[corredor][aula].inimigo[i].animation_index = 0;
			}

			if(mapas[corredor][aula].inimigo[i].existe == true)//desenha o equipamento na sala
				{
				/*	printf("Imprimindo, tudo certo\n");*/
					graphics.DrawImage2D(mapas[corredor][aula].inimigo[i].x, mapas[corredor][aula].inimigo[i].y, 70, 85,  mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].x, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].y, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].width, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].height, mapas[corredor][aula].inimigo[i].sheet_inimigo);
			
				}

			printf ("Letra: %c\n ", mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y-2))/32))][(int)(mapas[corredor][aula].inimigo[i].x+30)/32]);
	}
	
}

void Mapas(Heroi *personagem, Sala *sala)
{
	int i;
	
	if (acesso == 'a')
	{
		mapas[corredor][0] = atual;
		/**sala = mapas[corredor][A];*/ //passa o endereço de uma sala especifica para manipulação no jogo
		personagem->x = 500;
		personagem->y = 350;

		/*if (mapas[corredor][A].visitada == false)
		{
			mapas[corredor][A].visitada = true;
			gera_equipamento(sala);
		}*/
		aula = A;
	}

	else if (acesso == 'b')
	{
		mapas[corredor][0] = atual;
		/**sala = mapas[corredor][B]; */
		personagem->x = 500;
		personagem->y = 350;

		//if (mapas[corredor][B].visitada == false)
		//{
		//	mapas[corredor][B].visitada = true;
		//	gera_equipamento(sala);
		//}
		aula = B;
	}

	else if (acesso == 'c')
	{
		mapas[corredor][0] = atual;
		/**sala = mapas[corredor][C];*/
		personagem->x = 300;
		personagem->y = 200;

		/*if (mapas[corredor][C].visitada == false)
		{
			mapas[corredor][C].visitada = true;
			gera_equipamento(sala);
		}*/
		aula = C;
	}

	else if (acesso == 'd')
	{
		mapas[corredor][0] = atual;
		/**sala = mapas[corredor][D];*/
		personagem->x = 400;
		personagem->y = 300;

		/*if (mapas[corredor][D].visitada == false)
		{
			mapas[corredor][D].visitada = true;
			gera_equipamento(sala);
		}
		*/
		aula = D;
	}

	else if (acesso == 'e')
	{
		mapas[corredor][0] = atual;
		/**sala = mapas[corredor][E];*/
		personagem->x = 450;
		personagem->y = 300;

		/*if (mapas[corredor][E].visitada == false)
		{
			mapas[corredor][E].visitada = true;
			gera_equipamento(sala);
		}*/
		aula = E;
	}

	else if (acesso == 'P') //PORTA de sala de aula
	{
		mapas[corredor][aula] = atual;
		/**sala = mapas[corredor][0];*/
		personagem->x = 350;
		personagem->y = 350;
		aula = 0;

	}

	else if (acesso == 'S') //SUBIR corredor
	{
		mapas[corredor][0] = atual;
		corredor--;
		/**sala = mapas[corredor][0]; */
		personagem->x = personagem->x;
		personagem->y = 32;

	}

	else if (acesso == 'D') //DESCER corredor
	{
		mapas[corredor][0] = atual;
		corredor ++;
		/**sala = mapas[corredor][0]; */
		personagem->x = personagem->x;
		personagem->y = 560;
	}

	if (mapas[corredor][aula].visitada == false && aula != 0)
	{
		gera_equipamento(&mapas[corredor][aula]);
		/*printf("AQUI ENTROU");*/
	}

	*sala = mapas[corredor][aula];

	acesso = 'N'; //define char mapa como um caractere que não faz nada, nao roda funcao mapa
	/*printf ("Mapa = %d", corredor);	*/
}

void LoadMapa(char *filename, int linhas, int colunas, char matriz_mapa[][26])
{
    char c;
	int x, y;
    FILE *file;
    file = fopen(filename, "r");
    if (file) 
    {
		for (x = linhas-1; x >-1; x--)	
		{
			for (y = 0; y <colunas; y++)	
			{
				c = fgetc(file);
				while(c == '\r' ||  c == '\n')
					c = fgetc(file);
				
				if (c != EOF)
					matriz_mapa[x][y] = c;
				else
					fclose(file);
			}
		}
	}

	for (x = 0; x < 19 ; x++)
    {
       for (y = 0; y < 25; y++)
  			printf("%c", matriz_mapa[x][y]);
		
		printf("\n");
	}


}