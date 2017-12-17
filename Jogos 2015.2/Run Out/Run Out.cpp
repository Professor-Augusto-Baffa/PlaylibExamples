#include "Graphics.h"
#include <stdio.h>

Graphics graphics;

//funções
void load ();// carregar todas as variaveis
void update (float dt); // tudo que depende de dt
void draw(); // desenhar tudo do jogo
void MouseClickMinhaImagem(); // clica imagem
//bool Collision(int xmouse, int ymouse, int x1, int y1, int x2, int y2); // colisao mouse/imagem
void MouseMotionInput (int x, int y);// diz a posicao do mouse na tela
void MouseClickInput(int button, int state, int x, int y);// diz a posicao do click
void KeyboardInput(int key, int state, int x, int y); //le entrada do teclado

void MouseAction (int x, int y); //identifica onde o mouse está
void MouseClick (int x, int y); //interpreta a posiçao do click

//variáveis globais
int mouse_x = -1, mouse_y = -1, destino_x= -1, destino_y= -1;;
int contador = 0; //posição recebida do mouse


Image menu_entrada;
Image menu_opcao;
Image jogar;
Image carregar;
Image opcao;
Image sair;
Image pegadinha;


void MainLoop(void){
	
	float dt = graphics.GetElapsedTime();
	graphics.SetMouseMotionInput(MouseMotionInput);
	graphics.SetMouseClickInput(MouseClickInput);
	graphics.SetKeyboardInput(KeyboardInput);

	
	

	if (contador == 0)
		graphics.DrawImage2D(0, 0, 800, 600, menu_entrada);

	if (contador ==1)
	{

		MouseAction (mouse_x, mouse_y);
		MouseClick (destino_x, destino_y);

	}


	printf ("contador = %d", contador);

	
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

	menu_entrada.LoadPNGImage("menu_entrada.png");
	menu_opcao.LoadPNGImage("menu_opcao.png");
	jogar.LoadPNGImage("jogar.png");
	carregar.LoadPNGImage("carregar.png");
	opcao.LoadPNGImage("opcao.png");
	sair.LoadPNGImage("sair.png");
	pegadinha.LoadPNGImage("pegadinha.png");

}

void update (float dt){
}

void draw(){


	//menu_entrada.LoadPNGImage("menu_entrada.png");
	//menu_opcao.LoadPNGImage("menu_opcao.png");
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
}