#include "Graphics.h"
#include "Audio.h"
#include <stdio.h>

#define SPRITE_SIZE 33

Graphics graphics;

/*FUNÇÕES*/
void load ();// carregar todas as variaveis
void update (float dt); // tudo que depende de dt
void draw(); // desenhar tudo do jogo

void MouseClickMinhaImagem(); // clica imagem
//bool Collision(int xmouse, int ymouse, int x1, int y1, int x2, int y2); // colisao mouse/imagem
void MouseMotionInput (int x, int y);// diz a posicao do mouse na tela
void MouseClickInput(int button, int state, int x, int y);// diz a posicao do click
void MouseAction (int x, int y); //identifica onde o mouse está
void MouseClick (int x, int y); //interpreta a posiçao do click

void KeyboardInput(int key, int state, int x, int y); //le entrada do teclado




/*ESTRUTURAS*/
struct heroi{

	float x; //posicao x do trem
	float y; //posicao y do trem
	int animation_clip; //clipa da animacao do trem
	int animation_index; //indice atual da animacao do trem
	float time_next_frame; //contador de tempo para avancar na animacao
	
}; typedef struct heroi Heroi;

struct inimigo{

	float x; //posicao x do trem
	float y; //posicao y do trem
	int animation_clip; //clipa da animacao do trem
	int animation_index; //indice atual da animacao do trem
	float time_next_frame; //contador de tempo para avancar na animaca

}; typedef struct inimigo Inimigo;

struct sprite_sheet{ //armazena as informacoes de cada quadro da sprite sheet
	
	int x;
	int y;
	int width;
	int height;

}; typedef struct sprite_sheet Sprite_sheet;

void InitSprites(Sprite_sheet matriz_sprite[][5]);


/*VARIÁVEIS GLOBAIS*/



//mouse------------------------------------------------------
int mouse_x = -1, mouse_y = -1, destino_x= -1, destino_y= -1;;
int contador = 0; //posição recebida do mouse
//--------------------------------------------------------------


//menu------------------------------
Image menu_entrada;
Image menu_opcao;
Image jogar;
Image carregar;
Image opcao;
Image sair;
Image pegadinha;

Audio musica_menu_entrada;
//---------------------------------------

//personagens----------------------------------------
Sprite_sheet matriz_setsuna[6][5];
Image sheet_setsuna;

Image sheet_asuna
Sprite_sheet matriz_asuna[6][5];

Inimigo asuna;
Inimigo xfd;
Heroi setsuna;
//--------------------------------------------------

float player_x = 300;   
float player_y = 100;   
float player_speed = 80;
int animation_clip = 0;
int animation_index = 0;
float time_next_frame = 0;

 
float player_y = 100;   
float player_speed = 80;
int animation_clip = 0;
int animation_index = 0;
float time_next_frame = 0;
 
bool key_states[256];
 


void MainLoop(void){
	
	float dt = graphics.GetElapsedTime();
	graphics.SetMouseMotionInput(MouseMotionInput);
	graphics.SetMouseClickInput(MouseClickInput);
	graphics.SetKeyboardInput(KeyboardInput);

	
	
	//musica_menu_entrada.Play();

	if (contador == 0)
		graphics.DrawImage2D(0, 0, 800, 600, menu_entrada);

	if (contador ==1)
	{

		MouseAction (mouse_x, mouse_y);
		MouseClick (destino_x, destino_y);

	}


	printf ("contador = %d\n", contador);

	
	update (dt);
	draw();
	

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
	//imagem
	menu_entrada.LoadPNGImage("menu_entrada.png");
	menu_opcao.LoadPNGImage("menu_opcao.png");
	jogar.LoadPNGImage("jogar.png");
	carregar.LoadPNGImage("carregar.png");
	opcao.LoadPNGImage("opcao.png");
	sair.LoadPNGImage("sair.png");
	pegadinha.LoadPNGImage("mapa.png");

	sheet_setsuna.LoadPNGImage("spritesheet_setsuna.png");
	sheet_asuna.LoadPNGImage("spritesheet_asuna.png");

	InitSprites(matriz_setsuna);
	InitSprites(matriz_asuna);



	//audio
	musica_menu_entrada.LoadAudio("pandora.mp3");

	
	

}

void update (float dt){

	 bool next_frame = false;
    time_next_frame = time_next_frame + graphics.GetElapsedTime();
 
    if (key_states[KEY_LEFT])
    {
       animation_clip = 2;
       next_frame = true;      
       player_x -= player_speed * graphics.GetElapsedTime();
	   setsuna.x = player_x;
    }
    else if (key_states[KEY_RIGHT])
    {
      animation_clip = 2;
      next_frame = true;
      player_x += player_speed * graphics.GetElapsedTime();
	  setsuna.x = player_x;
    }
    else if (key_states[KEY_UP])
    {
        animation_clip = 1;
        next_frame = true;
        player_y += player_speed * graphics.GetElapsedTime();
		setsuna.x = player_y;
    }
    else if (key_states[KEY_DOWN])
    {
        animation_clip = 0;
        next_frame = true;
        player_y -= player_speed * graphics.GetElapsedTime();
		setsuna.x = player_y;
    }
 
    if (next_frame)
    {
        if (time_next_frame > 0.1)
        {
            animation_index++;
            time_next_frame = 0;
        }
        if (animation_index > 2)
            animation_index = 0;
    }
 
    graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE, matriz_setsuna[animation_clip][animation_index].x, matriz_setsuna[animation_clip][animation_index].y, matriz_setsuna[animation_clip][animation_index].width, matriz_setsuna[animation_clip][animation_index].height, sheet_setsuna);

	graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE, matriz_asuna[animation_clip][animation_index].x, matriz_asuna[animation_clip][animation_index].y, matriz_asuna[animation_clip][animation_index].width, matriz_asuna[animation_clip][animation_index].height, sheet_asuna);
}

void draw(){
	
	if(musica_menu_entrada.IsPlaying())
		contador=contador;
	else
		musica_menu_entrada.Stop();
		musica_menu_entrada.Play();


}


void MouseMotionInput (int x, int y)
{

	mouse_x = x;
	mouse_y = y;
}

void MouseAction (int x, int y)
{
	printf("x= %d, y= %d\n", x, y);
	
	if (contador ==1)
	{

		if (x >=92 && y>=255 && x<=265 && y<=290)
		graphics.DrawImage2D(0, 0, 800, 600, jogar);
			

		else if (x >=41 && y>=182 && x<=322 && y<=223)
		graphics.DrawImage2D(0, 0, 800, 600, carregar);

		else if (x >=41 && y>=106 && x<=283 && y<=147)
		graphics.DrawImage2D(0, 0, 800, 600, opcao);

		else if (x >=56 && y>=19 && x<=262 && y<=69)
		graphics.DrawImage2D(0, 0, 800, 600, sair);

		else 
		graphics.DrawImage2D(0, 0, 800, 600, menu_opcao);

	}
}

void MouseClickInput(int button, int state, int x, int y)
{

	if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
		{

			destino_x = x;
			destino_y = y;

		}
}

void MouseClick (int x, int y)
{

	if (contador == 1)
	{
		if (x >=92 && y>=255 && x<=265 && y<=290)
		graphics.DrawImage2D(0, 0, 800, 600, pegadinha);
			

		else if (x >=56 && y>=19 && x<=262 && y<=69)
		exit(0);

		
	}
}

void KeyboardInput(int key, int state, int x, int y)
{
	if ((key == 'f')&&(state == KEY_STATE_DOWN))
	{
		if (contador ==0)
			{
				graphics.DrawImage2D(0, 0, 800, 600, menu_opcao);
				contador = 1;
		}
		
	}

	  if (state == KEY_STATE_DOWN)
        key_states[key] = true;
    else if (state == KEY_STATE_UP)
        key_states[key] = false;
	/* if ((key == KEY_LEFT)&&(state == KEY_STATE_DOWN))
    {
        carro_direcao = 1;
        carro_x -= 4;
    }
    if ((key == KEY_RIGHT)&&(state == KEY_STATE_DOWN))
    {
        carro_direcao = 3;
        carro_x += 4;
    }
    if ((key == KEY_UP)&&(state == KEY_STATE_DOWN))
    {
        carro_direcao = 2;
        carro_y += 4;
    }
    if ((key == KEY_DOWN)&&(state == KEY_STATE_DOWN))
    {
        carro_direcao = 0;
        carro_y -= 4;
    }*/
}

void InitSprites(Sprite_sheet matriz_sprite[][5])
{

	int x, y;
	int sprite_x = 1; //posicao x onde o sprite realmente comeca. Ideal para imagens com sprite que nao comeca no (0,0)
	int sprite_y = 1;

	for (y = 0; y < 6; y++)
		for (x = 0; x < 5; x++)
			{
				matriz_sprite[y][x].x = sprite_x + SPRITE_SIZE * x;
				matriz_sprite[y][x].y = sprite_y + SPRITE_SIZE * y;
				matriz_sprite[y][x].width = SPRITE_SIZE;
				matriz_sprite[y][x].height = SPRITE_SIZE;
			}
}

