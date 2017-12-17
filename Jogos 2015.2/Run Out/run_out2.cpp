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
int gameover_tipo;
bool animacao;
bool vitoria_elevador;
bool vitoria_escada;

bool regua, chave, manutencao, cartao, painel, chave_saida;

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
struct ponto
{
	float x;
	float y;

}; typedef struct ponto Ponto;

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

	char tipo; // R para ranged e M para melee
	float reuso; //tempo para poder utilizar o item normalmente
	int alcance; //diferencia as o alcance entre as armas melee: 0 para curto e 1 para grande

	int tag; //identifica que arma é; 1-arco, 2-pistola, 

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
	bool equipado;
	Equipamento weapon;
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
	bool pode_atacar; // uma vez que o inimigo acerta o player, ele fica um tempo sem poder atacar
	float tempo_bloqueado; //basicamente o tempo do cooldown do ataque

}; typedef struct inimigo Inimigo;

struct sala
{

	bool item_presente;
	bool visitada; //variavel de controle do random que gera as armar e itens da sala. Se a sala ja tiver sido visitada, o random de gerar item não é executado

	Equipamento equipamento[25];//------------>botar com vetor pois algum retardado vai querer dropar todos os itens na mesma sala; acrescentar um contador de itens da sala

	Inimigo inimigo[25];
	int max_inimigo;

	char matriz_mapa[20][26];
	Image mapa_base;
	Image mapa_parallax;
	float x; //posicão de entrada do personagem
	float y;

}; typedef struct sala Sala;


/*--------Cronômentro---------*/
void timer();
/*-----------------------------*/


/*FUNÇÕES*/
void load ();// carregar todas as variaveis
void update (float dt); // tudo que depende de dt (tempo)
void draw(); // desenhar tudo do jogo

void movimentacao(Heroi *personagem);

void gera_equipamento(/*Sala *sala*/); //recebe a id da sala na qual o personagem se encontra
void controle_equipamento(Heroi *personagem);
void inventario(Heroi *personagem);
void controla_arma(Heroi *personagem);
void gera_inimigo();
void controle_inimigo(Inimigo *inimigo, Heroi *personagem);
void imprimeDialogos();

void LoadMapa(char *filename, int linhas, int colunas, char matriz_mapa[][26]);

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
Image modificacao_tela[4]; //0 = luz apagada(noite), 1- hp 1 hit, 2- hp 2 hits

Image sala_sem[4]; //0- sem regua, 1 - sem placa, 2 - sem chave;
Image item[6]; //1-regua, 2-chave almoxarifado, 3-cartao, 4-painel, 5-chavesaida

Image menu_gameover[9];
Image menu_vitoria[9];
Image elevador_animacao[5];


Audio menu_entrada_timer;
Audio menu_principal_musica;


int animation_index = 0;
int animation_index2 = 0;

float time_next_frame = 0;
float time_next_frame2 = 0;
int number;

Image sheet_numeros;
Sprite_sheet matriz_numeros[4][10];

Heroi amanda;
Heroi luis;
Heroi marcelo;

Inimigo hound;
Inimigo greyhound;
Inimigo monstro;

Equipamento equipamentos_jogo[20];
Equipamento pistola;
Equipamento arco;
Equipamento katana;
Equipamento faca;
Equipamento arma;


/*---------------------MAPAS-----------------------------*/
Sala banheiroM; //MODELOS
Sala banheiroF;
Sala blocoA1;
Sala blocoA2;
Sala blocoA3;
Sala blocoB1;
Sala blocoB2;
Sala blocoB3;
Sala aulaP;
Sala almoxarifado;
Sala almoxarifado2;
Sala escritorio;
Sala escritorio2;
Sala laboratorio;
Sala oficina;
Sala escadas;
Sala escadas_primeiroandar;
Sala elevador;
Sala saida;

float tempo_spawn;

Audio passos;

Heroi jogador;
void Mapas(Heroi *personagem);


Sala mapas[20][8]; //matriz com todas as salas do jogo
char acesso;

Image texto[16]; //0-obteve regua, 1 chave, 2 cartao, 3painel, 4chavesaida, 

Image sheet_fogo;
Sprite_sheet matriz_fogo[4][10];

Image elevador_vitoria[6];
Image escada_vitoria[21];
Image escada_animacao[7];
Image historia[11];

bool imprime_texto[16];
bool hist;

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
	printf ("mouse_x = %d, mouse_y = %d\n", mouse_x, mouse_y);
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

	menu_modo[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(contagem).png");
	menu_modo[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(pesadelo).png");
	menu_modo[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(cacador).png");
	menu_modo[3].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(miniatura cronometro).png");
	menu_modo[4].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(miniatura infinito).png");
	menu_modo[5].LoadPNGImage("Data\\Imagens\\Menus\\menu_modo(miniatura lobo).png");
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

	menu_gameover[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo(causaorgao).png");
	menu_gameover[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo_venceu(jogar novamente).png");
	menu_gameover[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo_venceu(menu).png");
	menu_gameover[3].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo(causabomba).png");

	menu_vitoria[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo_venceu.png");
	menu_vitoria[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo_venceu(jogar novamente).png");
	menu_vitoria[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_fimdejogo_venceu(menu).png");

	modificacao_tela[0].LoadPNGImage("Data\\Mapas\\luz_apagada.png");
	modificacao_tela[1].LoadPNGImage("Data\\Mapas\\primeiro_dano.png");
	modificacao_tela[2].LoadPNGImage("Data\\Mapas\\segundo_dano.png");

	elevador_animacao[0].LoadPNGImage("Data\\Mapas\\elevador_animacao1.png");
	elevador_animacao[1].LoadPNGImage("Data\\Mapas\\elevador_animacao2.png");
	elevador_animacao[2].LoadPNGImage("Data\\Mapas\\elevador_animacao3.png");
	elevador_animacao[3].LoadPNGImage("Data\\Mapas\\elevador_animacao4.png");
	
	elevador_vitoria[0].LoadPNGImage("Data\\Mapas\\elevador5.png");
	elevador_vitoria[1].LoadPNGImage("Data\\Mapas\\elevador4.png");
	elevador_vitoria[2].LoadPNGImage("Data\\Mapas\\elevador3.png");
	elevador_vitoria[3].LoadPNGImage("Data\\Mapas\\elevador2.png");
	elevador_vitoria[4].LoadPNGImage("Data\\Mapas\\elevador1.png");

	
	escada_vitoria[0].LoadPNGImage("Data\\Mapas\\1.png");
	escada_vitoria[1].LoadPNGImage("Data\\Mapas\\2.png");
	escada_vitoria[2].LoadPNGImage("Data\\Mapas\\3.png");
	escada_vitoria[3].LoadPNGImage("Data\\Mapas\\4.png");
	escada_vitoria[4].LoadPNGImage("Data\\Mapas\\5.png");
	escada_vitoria[5].LoadPNGImage("Data\\Mapas\\6.png");
	escada_vitoria[6].LoadPNGImage("Data\\Mapas\\7.png");
	escada_vitoria[7].LoadPNGImage("Data\\Mapas\\8.png");
	escada_vitoria[8].LoadPNGImage("Data\\Mapas\\9.png");
	escada_vitoria[9].LoadPNGImage("Data\\Mapas\\10.png");
	escada_vitoria[10].LoadPNGImage("Data\\Mapas\\11.png");
	escada_vitoria[11].LoadPNGImage("Data\\Mapas\\12.png");
	escada_vitoria[12].LoadPNGImage("Data\\Mapas\\13.png");
	escada_vitoria[13].LoadPNGImage("Data\\Mapas\\14.png");
	escada_vitoria[14].LoadPNGImage("Data\\Mapas\\15.png");
	escada_vitoria[15].LoadPNGImage("Data\\Mapas\\16.png");
	escada_vitoria[16].LoadPNGImage("Data\\Mapas\\17.png");
	escada_vitoria[17].LoadPNGImage("Data\\Mapas\\18.png");
	escada_vitoria[18].LoadPNGImage("Data\\Mapas\\19.png");

	

	sala_sem[0].LoadPNGImage("Data\\Mapas\\escritorio2.png");
	sala_sem[1].LoadPNGImage("Data\\Mapas\\oficina(sem painel).png");
	sala_sem[2].LoadPNGImage("Data\\Mapas\\escritorio.png");

	item[0].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(regua).png");
	item[1].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(almoxarifado).png");
	item[2].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(cartao).png");
	item[3].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(painel).png");
	item[4].LoadPNGImage("Data\\Imagens\\Menus\\menu_pause(saida).png");

	texto[0].LoadPNGImage("Data\\Imagens\\Texto\\texto0.png");
	texto[1].LoadPNGImage("Data\\Imagens\\Texto\\texto1.png");
	texto[2].LoadPNGImage("Data\\Imagens\\Texto\\texto2.png");
	texto[3].LoadPNGImage("Data\\Imagens\\Texto\\texto3.png");
	texto[4].LoadPNGImage("Data\\Imagens\\Texto\\texto4.png");
	texto[5].LoadPNGImage("Data\\Imagens\\Texto\\texto5.png");
	texto[6].LoadPNGImage("Data\\Imagens\\Texto\\texto6.png");
	texto[7].LoadPNGImage("Data\\Imagens\\Texto\\texto7.png");
	texto[8].LoadPNGImage("Data\\Imagens\\Texto\\texto8.png");
	texto[9].LoadPNGImage("Data\\Imagens\\Texto\\texto9.png");
	texto[10].LoadPNGImage("Data\\Imagens\\Texto\\texto10.png");
	texto[11].LoadPNGImage("Data\\Imagens\\Texto\\texto11.png");
	texto[12].LoadPNGImage("Data\\Imagens\\Texto\\texto12.png");
	texto[13].LoadPNGImage("Data\\Imagens\\Texto\\texto13.png");
	texto[14].LoadPNGImage("Data\\Imagens\\Texto\\texto14.png");

	historia[0].LoadPNGImage("Data\\Imagens\\Historia\\h1.png");
	historia[1].LoadPNGImage("Data\\Imagens\\Historia\\h2.png");
	historia[2].LoadPNGImage("Data\\Imagens\\Historia\\h3.png");
	historia[3].LoadPNGImage("Data\\Imagens\\Historia\\h4.png");
	historia[4].LoadPNGImage("Data\\Imagens\\Historia\\h5.png");

	regua = false;
	chave = false;
	cartao = false;
	painel = false;
	chave_saida = false;
	vitoria_elevador = false;
	vitoria_escada = false;
	hist = true;

	HUD.LoadPNGImage("Data\\Imagens\\Menus\\HUD.png");

	sheet_numeros.LoadPNGImage("Data\\Imagens\\numbers2.png");
	InitSprites(matriz_numeros, 32, 32, 1, 10);

	sheet_fogo.LoadPNGImage("Data\\Mapas\\fogo.png");
	InitSprites(matriz_fogo, 96, 96, 1, 4);


	/*----------------------------MAPAS---------------------------*/

	corredor = 5; //comeco do jogo
	aula = 2;
	acesso = 'N';
	animacao = false;
	
	//SALA
	aulaP.mapa_base.LoadPNGImage("Data\\Mapas\\mapa_salaP_base.png");
	aulaP.mapa_parallax.LoadPNGImage("Data\\Mapas\\mapa_salaP_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_salaP.txt", 19, 25, aulaP.matriz_mapa);
	aulaP.equipamento[0].x = 254; //posicao de spawn do item na sala
	aulaP.equipamento[0].y = 147;
	aulaP.x = 740;
	aulaP.y = 385;
	aulaP.visitada=false;
	aulaP.max_inimigo = 6;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		aulaP.equipamento[g].existe = false;
		aulaP.inimigo[g].existe = false;
		aulaP.inimigo[g].next_frame = false; 
	}

	//ESCADA PRIMEIRO ANDAR
	escadas_primeiroandar.mapa_base.LoadPNGImage("Data\\Mapas\\escadas_primeiroandar.png");
	escadas_primeiroandar.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_escadas_primeiroandar.txt", 19, 25,escadas_primeiroandar.matriz_mapa);
	escadas_primeiroandar.equipamento[0].x = 400; //posicao de spawn do item na sala
	escadas_primeiroandar.equipamento[0].y = 300;
	escadas_primeiroandar.x = 400;
	escadas_primeiroandar.y = 170;
	escadas_primeiroandar.visitada=true;
	escadas_primeiroandar.max_inimigo = 2;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		escadas_primeiroandar.equipamento[g].existe = false;
		escadas_primeiroandar.inimigo[g].existe = false;
		escadas_primeiroandar.inimigo[g].next_frame = false; 
	}

	//ESCADAS
	escadas.mapa_base.LoadPNGImage("Data\\Mapas\\escadas.png");
	escadas.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_escadas.txt", 19, 25,escadas.matriz_mapa);
	escadas.equipamento[0].x = 400; //posicao de spawn do item na sala
	escadas.equipamento[0].y = 300;
	escadas.x = 400;
	escadas.y = 170;
	escadas.visitada=true;
	escadas.max_inimigo = 2;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		escadas.equipamento[g].existe = false;
		escadas.inimigo[g].existe = false;
		escadas.inimigo[g].next_frame = false; 
	}

	//ESCADA SAÍDA
	saida.mapa_base.LoadPNGImage("Data\\Mapas\\escada_saida.png");
	saida.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_escada_saida.txt", 19, 25,saida.matriz_mapa);
	saida.equipamento[0].x = 400; //posicao de spawn do item na sala
	saida.equipamento[0].y = 300;
	saida.x = 400;
	saida.y = 170;
	saida.visitada=true;
	saida.max_inimigo = 3;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		saida.equipamento[g].existe = false;
		saida.inimigo[g].existe = false;
		saida.inimigo[g].next_frame = false; 
	}

	//ELEVADOR
	elevador.mapa_base.LoadPNGImage("Data\\Mapas\\Elevador_parado.png");
	elevador.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_elevador.txt", 19, 25,elevador.matriz_mapa);
	elevador.equipamento[0].x = 350; //posicao de spawn do item na sala
	elevador.equipamento[0].y = 350;
	elevador.x = 400;
	elevador.y = 360;
	elevador.visitada=true;
	elevador.max_inimigo = 3;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		elevador.equipamento[g].existe = false;
		elevador.inimigo[g].existe = false;
		elevador.inimigo[g].next_frame = false; 
	}

	
	//ALMOXARIFADO
	almoxarifado.mapa_base.LoadPNGImage("Data\\Mapas\\almoxarifado.png");
	almoxarifado.mapa_parallax.LoadPNGImage("Data\\Mapas\\almoxarifado_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_almoxarifado.txt", 19, 25,almoxarifado.matriz_mapa);
	almoxarifado.equipamento[0].x = 350; //posicao de spawn do item na sala
	almoxarifado.equipamento[0].y = 350;
	almoxarifado.x = 220;
	almoxarifado.y = 243;
	almoxarifado.visitada=false;
	almoxarifado.max_inimigo = 4;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		almoxarifado.equipamento[g].existe = false;
		almoxarifado.inimigo[g].existe = false;
		almoxarifado.inimigo[g].next_frame = false; 
	}
	//ALMOXARIFADO 2
	almoxarifado2.mapa_base.LoadPNGImage("Data\\Mapas\\almoxarifado2.png");
	almoxarifado2.mapa_parallax.LoadPNGImage("Data\\Mapas\\almoxarifado2_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_almoxarifado2.txt", 19, 25,almoxarifado2.matriz_mapa);
	almoxarifado2.equipamento[0].x = 350; //posicao de spawn do item na sala
	almoxarifado2.equipamento[0].y = 350;
	almoxarifado2.x = 220;
	almoxarifado2.y = 243;
	almoxarifado2.visitada=false;
	almoxarifado2.max_inimigo = 4;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		almoxarifado2.equipamento[g].existe = false;
		almoxarifado2.inimigo[g].existe = false;
		almoxarifado2.inimigo[g].next_frame = false; 
	}

	//ESCRITORIO
	escritorio.mapa_base.LoadPNGImage("Data\\Mapas\\escritorio(chave).png");
	escritorio.mapa_parallax.LoadPNGImage("Data\\Mapas\\escritorio_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_escritorio.txt", 19, 25, escritorio.matriz_mapa);
	escritorio.equipamento[0].x = 350; //posicao de spawn do item na sala
	escritorio.equipamento[0].y = 350;
	escritorio.x = 220;
	escritorio.y = 243;
	escritorio.visitada=false;
	escritorio.max_inimigo = 4;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		escritorio.equipamento[g].existe = false;
		escritorio.inimigo[g].existe = false;
		escritorio.inimigo[g].next_frame = false; 
	}

	//ESCRITORIO Lado esquerdo
	escritorio2.mapa_base.LoadPNGImage("Data\\Mapas\\escritorio2(regua).png");
	escritorio2.mapa_parallax.LoadPNGImage("Data\\Mapas\\escritorio2_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_escritorio2.txt", 19, 25, escritorio2.matriz_mapa);
	escritorio2.equipamento[0].x = 350; //posicao de spawn do item na sala
	escritorio2.equipamento[0].y = 350;
	escritorio2.x = 556;
	escritorio2.y = 250;
	escritorio2.visitada=true;
	escritorio2.max_inimigo = 2;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		escritorio2.equipamento[g].existe = false;
		escritorio2.inimigo[g].existe = false;
		escritorio2.inimigo[g].next_frame = false; 
	}

	//LABORATORIO
	laboratorio.mapa_base.LoadPNGImage("Data\\Mapas\\laboratorio.png");
	laboratorio.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_laboratorio.txt", 19, 25,laboratorio.matriz_mapa);
	laboratorio.equipamento[0].x = 350; //posicao de spawn do item na sala
	laboratorio.equipamento[0].y = 350;
	laboratorio.x = 700;
	laboratorio.y = 370;
	laboratorio.visitada=false;
	laboratorio.max_inimigo = 5;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		laboratorio.equipamento[g].existe = false;
		laboratorio.inimigo[g].existe = false;
		laboratorio.inimigo[g].next_frame = false; 
	}

	//OFICINA
	oficina.mapa_base.LoadPNGImage("Data\\Mapas\\oficina.png");
	oficina.mapa_parallax.LoadPNGImage("Data\\Mapas\\oficina_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_oficina.txt", 19, 25,oficina.matriz_mapa);
	oficina.equipamento[0].x = 350; //posicao de spawn do item na sala
	oficina.equipamento[0].y = 350;
	oficina.x = 220;
	oficina.y = 243;
	oficina.visitada=false;
	oficina.max_inimigo = 2;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		oficina.equipamento[g].existe = false;
		oficina.inimigo[g].existe = false;
		oficina.inimigo[g].next_frame = false; 
	}

	//BANHEIRO_F
	banheiroF.mapa_base.LoadPNGImage("Data\\Mapas\\BanheiroF.png");
	banheiroF.mapa_parallax.LoadPNGImage("Data\\Mapas\\BanheiroF_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_banheiroF.txt", 19, 25,banheiroF.matriz_mapa);
	banheiroF.equipamento[0].x = 310;//posicao de spawn do item na sala
	banheiroF.equipamento[0].y = 170;
	banheiroF.x = 250;
	banheiroF.y = 170;
	banheiroF.visitada=false;
	banheiroF.max_inimigo = 3;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
	{
		banheiroF.equipamento[g].existe = false;
		banheiroF.inimigo[g].existe = false;
		banheiroF.inimigo[g].next_frame = false; 
	}

	//BANHEIRO_M
	banheiroM.mapa_base.LoadPNGImage("Data\\Mapas\\BanheiroM.png");
	banheiroM.mapa_parallax.LoadPNGImage("Data\\Mapas\\neutro.png");
	LoadMapa("Data\\Mapas\\mapa_banheiroM.txt", 19, 25,banheiroM.matriz_mapa);
	banheiroM.equipamento[0].x = 459;//posicao de spawn do item na sala
	banheiroM.equipamento[0].y = 223;
	banheiroM.x = 259;
	banheiroM.y = 215;
	banheiroM.visitada=false;
	banheiroM.max_inimigo = 2;
	for (int g= 0; g < 25; g++) //inicializo o vetor dizendo que esta vazio
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
	blocoA1.x = 400;
	blocoA1.y = 300;
	blocoA1.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
	{
		blocoA1.equipamento[g].existe = false;
		blocoA1.inimigo[g].existe = false;
		blocoA1.inimigo[g].next_frame = false; 
		blocoA1.inimigo[g].pode_atacar = true;
		blocoA1.inimigo[g].tempo_bloqueado = 0;
	}

	//CORREDOR A2
	blocoA2.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoA2.png");
	blocoA2.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoA2_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoA2.txt", 19, 25, blocoA2.matriz_mapa);
	blocoA2.visitada = false;
	blocoA2.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
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
	blocoA3.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
	{
		blocoA3.equipamento[g].existe = false;
		blocoA3.inimigo[g].existe = false;
		blocoA3.inimigo[g].next_frame = false; 
	}

	//CORREDOR B1
	blocoB1.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoB1.png");
	blocoB1.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoB1_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoB1.txt", 19, 25, blocoB1.matriz_mapa);
	blocoB1.visitada = false;
	blocoB1.x = 400;
	blocoB1.y = 300;
	blocoB1.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
	{
		blocoB1.equipamento[g].existe = false;
		blocoB1.inimigo[g].existe = false;
		blocoB1.inimigo[g].next_frame = false; 
	}

	//CORREDOR B2
	blocoB2.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoB2.png");
	blocoB2.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoB2_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoB2.txt", 19, 25, blocoB2.matriz_mapa);
	blocoB2.visitada = false;
	blocoB2.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
	{
		blocoB2.equipamento[g].existe = false;
		blocoB2.inimigo[g].existe = false;
		blocoB2.inimigo[g].next_frame = false; 
	}
	//CORREDOR B3
	blocoB3.mapa_base.LoadPNGImage("Data\\Mapas\\BlocoB3.png");
	blocoB3.mapa_parallax.LoadPNGImage("Data\\Mapas\\BlocoB3_parallax.png");
	LoadMapa("Data\\Mapas\\mapa_blocoB3.txt", 19, 25, blocoB3.matriz_mapa);
	blocoB3.visitada = false;
	blocoB3.max_inimigo = 9;
	for (int g= 0; g < 25; g++)
	{
		blocoB3.equipamento[g].existe = false;
		blocoB3.inimigo[g].existe = false;
		blocoB3.inimigo[g].next_frame = false; 
	}
	
	passos.LoadAudio("Data\\Audios\\footstep.wav");


	//Salas replicadas (as acima são apenas os modelos)

	mapas[0][0] = blocoA1;
	mapas[0][1] = aulaP;
	mapas[0][3] = almoxarifado2;
	mapas[0][4] = escritorio;

	mapas[1][0] = blocoA2;
	mapas[1][1] = aulaP;
	mapas[1][2] = aulaP;
	mapas[1][3] = elevador;
	mapas[1][4] = escadas_primeiroandar;

	mapas[2][0] = blocoA3;
	mapas[2][1] = aulaP;
	mapas[2][2] = laboratorio;
	mapas[2][3] = banheiroF;
	mapas[2][4] = saida;

	mapas[3][0] = blocoB1;
	mapas[3][1] = aulaP;
	mapas[3][3] = almoxarifado;
	mapas[3][4] = banheiroM;

	mapas[4][0] = blocoB2;
	mapas[4][1] = aulaP;
	mapas[4][2] = aulaP;
	mapas[4][3] = elevador;
	mapas[4][4] = escadas;

	mapas[5][0] = blocoB3;
	mapas[5][1] = aulaP;
	mapas[5][2] = escritorio2;
	mapas[5][3] = banheiroF;
	mapas[5][4] = oficina;


	/*----------------------------LOADstructsHEROI---------------------------*/

	//AMANDA
	amanda.x = 500; 
	amanda.y = 250; 
	amanda.animation_clip = 0; 
	amanda.animation_index = 0; 
	amanda.time_next_frame = 0;
	amanda.vida = 3;
	amanda.pontuacao = 0;
	amanda.velocidade = 80;
	amanda.equipado = false;
	amanda.sheet_heroi.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_amanda.png");
	InitSprites(amanda.matriz_heroi, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	//LUIS
	luis.x = 500; 
	luis.y = 250; 
	luis.animation_clip = 0; 
	luis.animation_index = 0; 
	luis.time_next_frame = 0;
	luis.vida = 3;
	luis.pontuacao = 0;
	luis.velocidade = 80;
	luis.equipado = false;
	luis.sheet_heroi.LoadPNGImage("Data\\Imagens\\Sprites\\sheet_luis.png");
	InitSprites(luis.matriz_heroi, SPRITE_SIZE_X, SPRITE_SIZE_Y, 4, 4);

	//MARCELLO
	marcelo.x = 500; 
	marcelo.y = 250; 
	marcelo.animation_clip = 0; 
	marcelo.animation_index = 0; 
	marcelo.time_next_frame = 0;
	marcelo.vida = 3;
	marcelo.pontuacao = 0;
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
	greyhound.velocidade = 100;
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
	pistola.tipo = 'R';
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
	arco.tipo = 'R';
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
	katana.tipo = 'M';
	katana.reuso = 0.7;
		for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		katana.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_slash.png");
		InitSprites(katana.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		katana.pente.projetil[g].disparo = false;
		katana.pente.projetil[g].timer_projetil = false;
		katana.pente.projetil[g].tempo_projetil = 0;
    }

	//FACA
	faca.animation_clip = 0;
	faca.animation_index = 0;
	faca.pente.resistencia_max = 10;
	faca.pente.resistencia = 10;
	faca.x = 400;
	faca.y = 300;
	faca.sheet_equipamento.LoadPNGImage("Data\\Imagens\\Armas\\faca.png");
	InitSprites(faca.matriz_equipamento, 64, 64, 1, 1);
	faca.tipo = 'M';
	faca.reuso = 0.3;
		for (int g= 0; g < 10; g++)  //carrega todos os possiveis tiros na tela. Repare que é um vetor de 7 espaços, 
	{													//ou seja, o maximo possivel de frangos na tela é 7 
		faca.pente.projetil[g].sheet_projetil.LoadPNGImage("Data\\Imagens\\Projeteis\\tiro_slashcurto.png");
		InitSprites(faca.pente.projetil[g].matriz_projetil, 32, 32, 1, 4);
		faca.pente.projetil[g].disparo = false;
		faca.pente.projetil[g].timer_projetil = false;
		faca.pente.projetil[g].tempo_projetil = 0;
    }

	/*----------------xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx-------------------*/

}

void update (float dt)
{

	if (scene == -2)
	{
			static float tempo = 0;
			static int i = 0;
			tempo += dt;
			graphics.DrawImage2D(historia[i]);

			if (tempo >= 4)
			{
				i++;
				tempo = 0;
			}
			
			if (i>4)
			{
				i =0;
				scene = 5;
			}
	}

	if (scene == 5)
	{
		menu_principal_musica.Stop();


		graphics.DrawImage2D(0, 0, 800, 600, mapas[corredor][aula].mapa_base); //MAPA BASE

	/*	if(dificuldade == 2)
			graphics.DrawImage2D(0, 0, 1600, 1200, modificacao_tela[0]);*/

       	graphics.DrawImage2D(0, 0, 800, 600, HUD); //HUD

		timer(); //funcao do cronometro

		Mapas(&jogador);
		
		controle_equipamento(&jogador);
		
		if (vitoria_elevador)
		{ 
			static float tempo = 0;
			static int i = 0;
			tempo += dt;
			graphics.DrawImage2D(elevador_vitoria[i]);

			if (tempo >= 0.2)
			{
				i++;
				tempo = 0;
			}
			
			if (i>4)
			{
				i = 0;
				scene = 7;
			}
		}

			if (vitoria_escada)
		{ 
			static float tempo = 0;
			static int i = 0;
			tempo += dt;
			graphics.DrawImage2D(escada_vitoria[i]);

			if (tempo >= 0.2)
			{
				i++;
				tempo = 0;
			}
			
			if (i>18)
			{
				i=0;
				scene = 7;
			}
		}
		
	
		movimentacao(&jogador); //nem precisavar passar endereco de jogador, por jogador ser global, mas é bom que facilita um reset
		inventario(&jogador);
		controle_inimigo(&monstro, &jogador);
		controla_arma(&jogador); 
		
		graphics.DrawImage2D(0, 0, 800, 600, mapas[corredor][aula].mapa_parallax); //MAPA PARALLAX
		imprimeDialogos();
		
		static float regeneracao = 0;
		if(jogador.vida == 2)
		{
			
			regeneracao += dt;

			graphics.DrawImage2D(0, 0, 800, 600, modificacao_tela[1]);

			if (regeneracao >= 5.0)
			{
				jogador.vida = 3;
				regeneracao = 0;
			}

		}

		if(jogador.vida == 1)
		{
			static float regeneracao_low = 0;
			regeneracao_low += dt;

			graphics.DrawImage2D(0, 0, 800, 600, modificacao_tela[1]);

			if (regeneracao_low >= 5.0)
			{
				jogador.vida = 2;
				regeneracao_low = 0;
				regeneracao = 0;
			}

			graphics.DrawImage2D(0, 0, 800, 600, modificacao_tela[2]);
		}

		if(jogador.vida == 0)
		{
			scene = 6;
			gameover_tipo = 0;
		}
		
		if (animacao == true)
		{ 
			static float tempo = 0;
			static int i = 0;

			tempo += dt;

			graphics.DrawImage2D(elevador_animacao[i]);

			if (tempo >= 0.2)
			{
				i++;
				tempo = 0;
			}
			
			if (i>3)
			{
				animacao = false;
				i = 0;
			}
		}
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
				scene = 1;
		}

		if (regua == true)
			graphics.DrawImage2D(item[0]);
		if (chave == true)
			graphics.DrawImage2D(item[1]);
		if (cartao == true)
			graphics.DrawImage2D(item[2]);
		if (painel == true)
			graphics.DrawImage2D(item[3]);
		if (chave_saida == true)
			graphics.DrawImage2D(item[4]);
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
					graphics.DrawImage2D(69, 217, 224, 224, menu_modo[5]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(511, 217, 224, 224, menu_modo[4]);
			}

			else if(modo==1)
			{
				graphics.DrawImage2D(menu_modo[1]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					modo = 0;

				if (Collision(click_x, click_y, 571, 273, 672, 373))
					modo = 2;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(69, 217, 224, 224, menu_modo[3]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(511, 217, 224, 224, menu_modo[5]);
			}

			else if(modo==2)
			{
				graphics.DrawImage2D(menu_modo[2]);
				if (Collision(click_x, click_y, 137, 272, 237, 375))
					modo = 1;
				else if (Collision(click_x, click_y, 571, 273, 672, 373))
					modo = 0;

				if (Collision(mouse_x, mouse_y, 137, 272, 237, 375))
					graphics.DrawImage2D(66, 217, 224, 224, menu_modo[4]);
				else if (Collision(mouse_x, mouse_y, 571, 273, 672, 373))
					graphics.DrawImage2D(511, 217, 224, 224, menu_modo[3]);
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_modo[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_modo[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 1;
			if (Collision(click_x, click_y, 504, 39, 705, 66) || delay_on)
					scene = 3;

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

				tempo_spawn = 40;
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

				tempo_spawn = 25;
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

				tempo_spawn = 10;
			}

			if (Collision(mouse_x, mouse_y, 117, 39, 241, 66))
				graphics.DrawImage2D(menu_dificuldade[6]);
			if (Collision(mouse_x, mouse_y, 504, 39, 705, 66))
				graphics.DrawImage2D(menu_dificuldade[7]);
			if (Collision(click_x, click_y, 117, 39, 241, 66))
				scene = 2;
			if (Collision(click_x, click_y, 504, 39, 705, 66))
				scene = 4;

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
				scene = -2;
		}

		else if (scene == 6) //Game over
		{
			
			if (gameover_tipo == 0)//determina se o game over foi por tempo ou colisao
				graphics.DrawImage2D(menu_gameover[0]);
	
			else if (gameover_tipo == 1)
				graphics.DrawImage2D(menu_gameover[4]);

			if (Collision(mouse_x, mouse_y, 268, 155, 535, 176))
			{
				graphics.DrawImage2D(menu_gameover[1]);

				if (Collision(click_x, click_y, 268, 155, 535, 176))
				{
					load();
					scene = 2;
				}

			}

			else if (Collision(mouse_x, mouse_y, 286, 112, 511, 134))
			{
				graphics.DrawImage2D(menu_gameover[2]);

				if (Collision(click_x, click_y, 286, 112, 511, 134))
				{
					load();
					scene = 1;
				}
			}

		}

		else if (scene == 7) //vitoria
		{
			
			graphics.DrawImage2D(menu_vitoria[0]);
				
			if (Collision(mouse_x, mouse_y, 268, 155, 535, 176))
			{
				graphics.DrawImage2D(menu_vitoria[1]);

				if (Collision(click_x, click_y, 268, 155, 535, 176))
				{  
					load();
					scene = 2;
				}
			}

			else if (Collision(mouse_x, mouse_y, 286, 112, 511, 134))
			{
				graphics.DrawImage2D(menu_vitoria[2]);

				if (Collision(click_x, click_y, 286, 112, 511, 134))
				{
					load();
					scene = 1;
				}
			}
	
		}

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
		scene = 6;
		gameover_tipo = 1;
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

void movimentacao(Heroi *personagem)
{

	if (KeyState[KEY_LEFTSHIFT] || KeyState[KEY_RIGHTSHIFT])
		personagem->velocidade=160;
	else
		personagem->velocidade=80;

	bool next_frame = false;
    personagem->time_next_frame = personagem->time_next_frame + dt;
 
    if (KeyState[KEY_LEFT])
    {
		personagem->animation_clip = 1;
		 if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+24-8))/32))][(int)(personagem->x+5)/32] != 'x' && mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-1)/32] != 'x')
		 {
			 	passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
		   next_frame = true;      
		   personagem->x -=  personagem->velocidade * dt;

		   camerax = personagem->x-400;
		 }
    }

    else if (KeyState[KEY_RIGHT])
	{           
		 personagem->animation_clip = 2;
		if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+24-8))/32))][(int)(personagem->x+45)/32] != 'x' && mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+50)/32] != 'x')
		{
				passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
		  next_frame = true;
		  personagem->x += personagem->velocidade * dt;
		  
		  if (abs(personagem->x-400)>18) 
		  camerax = personagem->x-400-18;
		}
	
    }

    else if (KeyState[KEY_UP]) 
    {
		personagem->animation_clip = 3;
		if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+24))/32))][(int)(personagem->x)/32] != 'x' && mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+10))/32))][(int)(personagem->x+40)/32] != 'x' )//limita altura maxima que o personagem alcança (no caso, bloqueia a 24 pixels de x)
		{
			passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
			next_frame = true;
			personagem->y += personagem->velocidade * dt;
			cameray = personagem->y-300;
		}
		
    }
    else if (KeyState[KEY_DOWN])
    {
		personagem->animation_clip = 0;
		if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y-2))/32))][(int)(personagem->x+30)/32] != 'x' && mapas[corredor][aula].matriz_mapa[(int)((((personagem->y-2))/32))][(int)(personagem->x+8)/32] != 'x')
		{
			passos.Play();

			if (!passos.IsPlaying())
			{
				passos.Stop();
				passos.Play();
			}
			next_frame = true;
			personagem->y -= personagem->velocidade * dt;
			cameray = personagem->y-300;
		}
	}

	else 
		personagem->animation_index = 0;

	acesso = mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32];
 
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


//printf ("CASA = %c\n", mapas[corredor][aula].matriz_mapa[(int)personagem->y/32][(int)personagem->x/32]);
//printf ("x = %d  y = %d\n", (int)personagem->x/32, (int)personagem->y/32);
//printf ("x = %.2f  y = %.2f", personagem->x, personagem->y);
//printf ("Teste+ = %d  teste- = %d", (int)1.8/2, (int)0.8/2);
}

void gera_equipamento()
{
	int aux_random;
	
	if (mapas[corredor][aula].visitada == false)
	{
		aux_random = rand() % 4;

		if (aux_random == 0)
		{
			mapas[corredor][aula].equipamento[0] = pistola;
			mapas[corredor][aula].equipamento[0].existe = true;
		}
		else if (aux_random == 1)
		{
			mapas[corredor][aula].equipamento[0] = arco;
			mapas[corredor][aula].equipamento[0].existe = true;
		}
		else if (aux_random == 2)
		{
			mapas[corredor][aula].equipamento[0] = katana;
			mapas[corredor][aula].equipamento[0].existe = true;
		}

		else
		{
			mapas[corredor][aula].equipamento[0] = faca;
			mapas[corredor][aula].equipamento[0].existe = true;
		}

		mapas[corredor][aula].visitada = true;
	}
}

void controle_equipamento(Heroi *personagem)
{
	int i;

	for(i = 0; i<20; i++)
	{
		if(mapas[corredor][aula].equipamento[i].existe == true)//desenha o equipamento na sala
			{

				graphics.DrawImage2D(mapas[corredor][aula].equipamento[i].x, mapas[corredor][aula].equipamento[i].y, 45, 45, mapas[corredor][aula].equipamento[i].sheet_equipamento);
			
				if (personagem->equipado == false)
				{
			
					if (CheckBoxCollision(personagem->x+30, personagem->y+20, 56-30, 82-20, mapas[corredor][aula].equipamento[i].x+16, mapas[corredor][aula].equipamento[i].y+20, 64-16, 64-30)) //checa colisao com equipamento, a posicao ja esta predefinida de acordo com o mapa
					{
						personagem->weapon = mapas[corredor][aula].equipamento[i];
						personagem->equipado = true;
						mapas[corredor][aula].equipamento[i].existe = false;
					}
				}
			}
		}
	
}

void inventario(Heroi *personagem)
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
			if (mapas[corredor][aula].equipamento[i].existe == false) //procura uma posicao vazia no vetor
			{
				if (personagem->animation_clip==0)
					{ 
						if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y-50))/32))][(int)(personagem->x)/32] == 'o') //verificação se é possivel dropar na posicao requerida
						{
							mapas[corredor][aula].equipamento[i] = personagem->weapon;
							mapas[corredor][aula].equipamento[i].x = personagem->x;
							mapas[corredor][aula].equipamento[i].y = personagem->y - 50;
							personagem->equipado = false; 
							mapas[corredor][aula].equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}

					else if (personagem->animation_clip==1)
					{ 
						if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-50)/32] == 'o')
						{
							mapas[corredor][aula].equipamento[i]=personagem->weapon;
							mapas[corredor][aula].equipamento[i].x = personagem->x-50;
							mapas[corredor][aula].equipamento[i].y = personagem->y;
							personagem->equipado = false; 
							mapas[corredor][aula].equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
					else if (personagem->animation_clip==2)
					{ 
						if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+50)/32] == 'o')
						{
							mapas[corredor][aula].equipamento[i]=personagem->weapon;
							mapas[corredor][aula].equipamento[i].x = personagem->x+50;
							mapas[corredor][aula].equipamento[i].y = personagem->y;
							personagem->equipado = false; 
							mapas[corredor][aula].equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
					else if (personagem->animation_clip==3)
					{
						if (mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+50))/32))][(int)(personagem->x)/32] == 'o')
						{
							mapas[corredor][aula].equipamento[i] = personagem->weapon;
							mapas[corredor][aula].equipamento[i].x = personagem->x;
							mapas[corredor][aula].equipamento[i].y = personagem->y + 70;
							personagem->equipado = false;
							mapas[corredor][aula].equipamento[i].existe = true;
						}
						else
							personagem->equipado = true; 
					}
			break; //interrompo o for; não faz sentido continuar rodando o vetor
		}
	}
}
	
/*
	printf ("CASA SUL = %c\n", mapas[corredor][aula].matriz_mapa[(int)((((personagem->y-50))/32))][(int)(personagem->x)/32]);
	printf ("CASA LESTE = %c\n", mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x-50)/32]);
	printf ("CASA OESTE = %c\n", mapas[corredor][aula].matriz_mapa[(int)((((personagem->y))/32))][(int)(personagem->x+50)/32]);
	printf ("CASA NORTE = %c\n", mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+50))/32))][(int)(personagem->x)/32]);*/
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

				if (personagem->weapon.tipo == 'R')
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

				else if (personagem->weapon.tipo == 'M')
				{
					personagem->weapon.pente.projetil[z].direcao = personagem->animation_clip;
					personagem->weapon.pente.projetil[z].disparo = true;
					

						if (personagem->animation_clip == 0)
					{	
						pos_corte = 1;
						personagem->weapon.pente.projetil[z].x = personagem->x;

						if (personagem->weapon.alcance == 0)
							personagem->weapon.pente.projetil[z].y = personagem->y - 20;
						else
							personagem->weapon.pente.projetil[z].y = personagem->y - 50;
						
					}
		
					else if (personagem->animation_clip == 1)
					{
						pos_corte = 2;
						personagem->weapon.pente.projetil[z].y = personagem->y;

						if (personagem->weapon.alcance == 0)
							personagem->weapon.pente.projetil[z].x = personagem->x - 20;
						else
							personagem->weapon.pente.projetil[z].x = personagem->x - 50;
						
					}
		
					else if (personagem->animation_clip == 2)
					{
				
						pos_corte = 0;
						personagem->weapon.pente.projetil[z].y = personagem->y;
						
						if (personagem->weapon.alcance == 0)
							personagem->weapon.pente.projetil[z].x = personagem->x + 20;
						else
							personagem->weapon.pente.projetil[z].x = personagem->x + 50;
					}
		
					else if (personagem->animation_clip == 3)
					{
						pos_corte = 3;
						personagem->weapon.pente.projetil[z].x = personagem->x;

						if (personagem->weapon.alcance == 0)
							personagem->weapon.pente.projetil[z].y = personagem->y + 20;
						else
							personagem->weapon.pente.projetil[z].y = personagem->y + 50;
						
					}
				}


				KeyState['h'] = false;
			}
		}
	}

	if (personagem->weapon.tipo == 'R')
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

				if (personagem->weapon.pente.projetil[z].x > 800 || personagem->weapon.pente.projetil[z].x < 0 || personagem->weapon.pente.projetil[z].y > 600 || personagem->weapon.pente.projetil[z].y < 0)
					personagem->weapon.pente.projetil[z].disparo = false;

				graphics.DrawImage2D(personagem->weapon.pente.projetil[z].x, personagem->weapon.pente.projetil[z].y+ajuste_y, 32, 32, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].x, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].y, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].width, personagem->weapon.pente.projetil[z].matriz_projetil[0][pos].height, personagem->weapon.pente.projetil[z].sheet_projetil);
			}
			
		}	
		/*printf("personagem = %d, modelo = %d\n", personagem->weapon.pente.resistencia, arma.pente.resistencia);*/
	}

	else if (personagem->weapon.tipo == 'M')
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

	if (tempo > tempo_spawn)
	{
		for(g = 0; g<20; g++) //roda os corredores
		{
			for(h = 0; h<8; h++)// roda as salas
			{
				for (i=0; i < mapas[g][h].max_inimigo; i++)
				{
					if ((g == 5 && h == 2) || g == C || g == D)
						break;

					if (mapas[g][h].inimigo[i].existe == false) //procura uma posicao vazia no vetor
					{
						aux_random = rand() % 3;

						if (aux_random == 0)
							mapas[g][h].inimigo[i] = greyhound;
			
						else 
							mapas[g][h].inimigo[i] = hound;
						
						mapas[g][h].inimigo[i].existe = true;

						if(mapas[g][h].matriz_mapa[(int)((((mapas[g][h].inimigo[i].y))/32))][(int)(mapas[g][h].inimigo[i].x)/32] == 'o')
						{
							mapas[g][h].inimigo[i].x = 300;
							mapas[g][h].inimigo[i].y = 300;
						}
						else
							mapas[g][h].inimigo[i].existe = false;;
				
						break;
					}
				}
			}
		}

		tempo = 0;
	}

	/*printf ("Tempo: %.2f\n", tempo);*/
}

void controle_inimigo(Inimigo *inimigo, Heroi *personagem)
{
	gera_inimigo();

	int i, a;
	static float muda_random = 0;

	muda_random += dt;

	if (muda_random >= 2.5)
	{
		for (i = 0; i < mapas[corredor][aula].max_inimigo; i++)
			if (mapas[corredor][aula].inimigo[i].existe)
				mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;

		muda_random = 0;
	}


	for(i = 0; i < mapas[corredor][aula].max_inimigo; i++)
	{
		mapas[corredor][aula].inimigo[i].time_next_frame += dt;
		mapas[corredor][aula].inimigo[i].next_frame = false;

		if (mapas[corredor][aula].inimigo[i].animation_clip == 0) //up
		{
			if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y+24))/32))][(int)(mapas[corredor][aula].inimigo[i].x)/32] == 'o' && mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y+24))/32))][(int)(mapas[corredor][aula].inimigo[i].x+40)/32] != 'x')
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
			{	
				mapas[corredor][aula].inimigo[i].next_frame = true;
				mapas[corredor][aula].inimigo[i].x += mapas[corredor][aula].inimigo[i].velocidade * dt;
				mapas[corredor][aula].inimigo[i].animation_clip = 1;
				
			}

			else
				mapas[corredor][aula].inimigo[i].animation_clip = rand() % 4;
		}

		if (mapas[corredor][aula].inimigo[i].animation_clip == 2)//down
		{
			if (mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y-2))/32))][(int)(mapas[corredor][aula].inimigo[i].x+30)/32] == 'o' && mapas[corredor][aula].matriz_mapa[(int)((((mapas[corredor][aula].inimigo[i].y-2))/32))][(int)(mapas[corredor][aula].inimigo[i].x+8)/32] == 'o')
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

		if (personagem->weapon.tipo == 'R')
		{
			for (a = 0; a<personagem->weapon.pente.resistencia; a++)
			{
				if(personagem->weapon.pente.projetil[a].disparo)
				{
				
					if (Collision(personagem->weapon.pente.projetil[a].x+10, personagem->weapon.pente.projetil[a].y+10, mapas[corredor][aula].inimigo[i].x, mapas[corredor][aula].inimigo[i].y, mapas[corredor][aula].inimigo[i].x+56, mapas[corredor][aula].inimigo[i].y+80))
					{
						mapas[corredor][aula].inimigo[i].vida--;
						personagem->weapon.pente.projetil[a].disparo = false;
					}
				}
			}
		}

		else 
		{
			a = 0;
			if (Collision(personagem->weapon.pente.projetil[a].x+16, personagem->weapon.pente.projetil[a].y+16, mapas[corredor][aula].inimigo[i].x, mapas[corredor][aula].inimigo[i].y, mapas[corredor][aula].inimigo[i].x+56, mapas[corredor][aula].inimigo[i].y+80))
			{
				mapas[corredor][aula].inimigo[i].vida--;
				personagem->weapon.pente.projetil[a].disparo = false;
				
			}
		}
		
		if (mapas[corredor][aula].inimigo[i].vida == 0)
		{
			mapas[corredor][aula].inimigo[i].existe = false;
			personagem->pontuacao += 100;
			
		}

		
		if(mapas[corredor][aula].inimigo[i].pode_atacar == true)
		{
			if(mapas[corredor][aula].inimigo[i].existe == true)
			{
			if(CheckBoxCollision(personagem->x+8, personagem->y+8, 56-8, 82-15, mapas[corredor][aula].inimigo[i].x+15, mapas[corredor][aula].inimigo[i].y+10, 56-12, 82-32))
			{
				jogador.vida--;
				mapas[corredor][aula].inimigo[i].pode_atacar = false;

			}
			}
		}

		else
		{
			mapas[corredor][aula].inimigo[i].tempo_bloqueado +=dt;

			if (mapas[corredor][aula].inimigo[i].tempo_bloqueado >= 2.0)
			{
				mapas[corredor][aula].inimigo[i].pode_atacar = true;
				mapas[corredor][aula].inimigo[i].tempo_bloqueado = 0;
			}
		}

		if(mapas[corredor][aula].inimigo[i].existe == true)//desenha o equipamento na sala
			graphics.DrawImage2D(mapas[corredor][aula].inimigo[i].x, mapas[corredor][aula].inimigo[i].y, 70, 85,  mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].x, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].y, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].width, mapas[corredor][aula].inimigo[i].matriz_inimigo[mapas[corredor][aula].inimigo[i].animation_clip][mapas[corredor][aula].inimigo[i].animation_index].height, mapas[corredor][aula].inimigo[i].sheet_inimigo);
	
	/*printf("Vida: %d\n", jogador.vida);
	printf("ATAQUE: %d\n", mapas[corredor][aula].inimigo[i].pode_atacar);*/
	}
	
}

void Mapas(Heroi *personagem)
{

	static float entrada_y = 214, entrada_x = 210;

	if (acesso == 'a')
	{
		entrada_y = personagem->y;
		entrada_x = personagem->x+20;
		aula = A;
	}

	else if (acesso == 'b')
	{
		entrada_y = personagem->y;
		entrada_x = personagem->x+20;
		aula = B;
	}

	else if (acesso == 'c')
	{

		if (corredor == 1 || corredor == 4)
			animacao = true;

			entrada_y = personagem->y;
			entrada_x = personagem->x-20;
			aula = C;

	}

	else if (acesso == 'd')
	{	
		entrada_y = personagem->y;
		entrada_x = personagem->x-20;
		aula = D;
	}

	else if (acesso == 'P') //PORTA de sala de aula
	{
		personagem->x = entrada_x+5;
		personagem->y = entrada_y;
		aula = 0;
	}

	else if (acesso == 'C')
	{	
		personagem->x = 312;
		personagem->y = 315;
		corredor = 4;
	}

	else if (acesso == 'B')
	{	
		corredor = 1;
		personagem->x = 443;
		personagem->y = 300;
		personagem->animation_clip = 2;
		personagem->animation_index = 0;
	}

	else if (acesso == 'S') //SUBIR corredor
	{
		corredor--;
		personagem->x = personagem->x;
		personagem->y = 32;

	}

	else if (acesso == 'D') //DESCER corredor
	{
		corredor ++;
		personagem->x = personagem->x;
		personagem->y = 560;
	}

	if (mapas[corredor][aula].visitada == false && aula != 0)
		gera_equipamento();

	if (acesso == 'a' || acesso == 'b' || acesso == 'c'|| acesso == 'd')
	{
		personagem->x = mapas[corredor][aula].x;
		personagem->y = mapas[corredor][aula].y;
	}

	if (acesso == 'i')
	{
		if (corredor == 2 && aula == D)
				imprime_texto[8] = true;


		if (KeyState[' '])
		{
			if (corredor == 0 && aula == D)
			{
				mapas[corredor][aula].mapa_base = sala_sem[2];
				imprime_texto[11] = true;
				chave_saida = true;
			}

			if (corredor == 1 && aula == C)
				if(painel == false)
					imprime_texto[12] = true;
				else
					imprime_texto[13] = true;

			if (corredor == 2 && aula == B)
			{
				chave = true;
				imprime_texto[10] = true;

			}

			if (corredor == 5 && aula == D)
			{
				mapas[corredor][aula].mapa_base = sala_sem[1];
				painel = true;
				imprime_texto[7] = true;

			}

			if (corredor == 5 && aula == B)
			{
				mapas[corredor][aula].mapa_base = sala_sem[0];
				regua = true;
				imprime_texto[0] = true;
			}

			if (corredor == 3 && aula == D)
			{
				cartao = true;
				imprime_texto[9] = true;
			}

		}
	}


	if (acesso == 'T')
	{

		if (KeyState[' '])
			if (corredor == 2 && aula == D)
			{
				if(chave_saida == false)
					imprime_texto[4] = true;
				else
					imprime_texto[13] = true;
			}

		if (corredor == 4 && aula == D)
				imprime_texto[6] = true;
			
		if (corredor == 5 && aula == B)
		{
			if(regua == false)
				imprime_texto[1] = true;
			else
				mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32] = 'P';

		}

	}

	if (acesso == 'e')
	{
		if (corredor == 0 && aula == 0)
		{
			if(chave == false)
				imprime_texto[2] = true;
			else
				mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32] = 'c';
		}

		if (corredor == 4 && aula == 0)
				imprime_texto[5] = true;
		
		if (corredor == 3 && aula == 0)
		{
			if(chave == false)
				imprime_texto[2] = true;
			else
				mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32] = 'P';

		}

	}

	if (acesso == 'f')
	{
		if (corredor == 0 && aula == 0)
		{
			if(cartao == false)
				imprime_texto[3] = true;
			else
				mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32] = 'd';
		}


		if (corredor == 5 && aula == 0)
		{
			if(chave == false)
				imprime_texto[2] = true;
			else
				mapas[corredor][aula].matriz_mapa[(int)((((personagem->y+15))/32))][(int)(personagem->x+20)/32] = 'd';

		}
	}

	if (corredor == 4 && aula == D) //animacao do fogo
		{
			static float tempo = 0;
			static int i = 0;

			tempo += dt;

			graphics.DrawImage2D(395, 400, 96, 96, matriz_fogo[0][i].x, matriz_fogo[0][i].y, matriz_fogo[0][i].width, matriz_fogo[0][i].height, sheet_fogo);
			graphics.DrawImage2D(442, 400, 96, 96, matriz_fogo[0][i].x, matriz_fogo[0][i].y, matriz_fogo[0][i].width, matriz_fogo[0][i].height, sheet_fogo);
			if (tempo >= 0.1)
			{
				i++;
				tempo = 0;
			}
			
			if (i>3)
				i = 0;
		}


	acesso = 'N'; //define char mapa como um caractere que não faz nada, nao roda funcao mapa
	/*printf ("Mapa = %d", corredor);	*/
}

void imprimeDialogos()
{

	if (imprime_texto[0] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[0]);
		else
			imprime_texto[0] = false;
	}


	else if (imprime_texto[1] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[1]);

		else
		{
			imprime_texto[1] = false;
			tempo = 0;
		}

	}

	else if (imprime_texto[2] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[2]);
		else
		{
			imprime_texto[2] = false;
			tempo = 0;
		}
	}

	else if (imprime_texto[3] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[3]);
		else
		{
			imprime_texto[3] = false;
			tempo = 0;
		}
	}

	else if (imprime_texto[4] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[4]);

		else
		{
			imprime_texto[4] = false;
			tempo = 0;
		}

	}

	else if (imprime_texto[5] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[5]);

		else
		{
			imprime_texto[5] = false;
			tempo = 0;
		}

	}

	else if (imprime_texto[6] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[6]);

		else
		{
			imprime_texto[6] = false;
			tempo = 0;
		}

	}

	else if (imprime_texto[7] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[7]);
		else
			imprime_texto[7] = false;
	}

	
	else if (imprime_texto[8] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 4)
			graphics.DrawImage2D(texto[8]);
		else
		{
			imprime_texto[8] = false;
			tempo = 0;
		}
	}

	else if (imprime_texto[9] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[9]);
		else
			imprime_texto[9] = false;
	}

	else if (imprime_texto[10] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[10]);
		else
			imprime_texto[10] = false;
	}

	else if (imprime_texto[11] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[11]);
		else
			imprime_texto[11] = false;
	}

	else if (imprime_texto[12] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[12]);
		else
		{
			imprime_texto[12] = false;
			tempo = 0;
		}
	}

			
	else if (imprime_texto[13] == true)
	{
		static float tempo = 0;
		tempo += dt;

		if (tempo <= 3)
			graphics.DrawImage2D(texto[13]);
		else
		{
			imprime_texto[13] = false;
			tempo = 0;
		}

		if (KeyState[' '])
		{
			if (corredor == 2)
				vitoria_escada = true;
			else
				vitoria_elevador = true;
		}

	}
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
