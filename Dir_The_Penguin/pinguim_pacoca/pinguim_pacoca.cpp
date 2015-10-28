#include "Graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Audio.h"

using namespace PlayLib;

struct Pos2D{
	float x;
	float y;
	bool ativo;
	Image imagem;
};

Graphics graphics;

int f = 0;
int level = 1;
int placar = 0;
int frangos_perdidos = 0;
float tempo = 0;
float speed = 200;
float ptime = 0;
float rtime = 0;
bool you_lose = false;
bool winner = false;
bool tinicial = true;
bool pl = false;

Pos2D vetor_frango[7];
Pos2D pinguim;

Image pinguim_dir;
Image background;
Image game_over;
Image play;
Image you_win;
Image botao_jogar;

Audio pop;
Audio irritante;
Audio vencedor;
Audio doublepop;

void gera_pinguim ()
{
	int shuffle = rand ()%700;
	pinguim.ativo=true;
	pinguim.x = shuffle;
	pinguim.y = 0;					
}

void gera_frangos ()
{
	for(int z=0;z<level;z++)
	{
		for (f=0; f<7; f++)
		{
			if (vetor_frango[f].ativo==false)
			{
				int shuffle = rand() % 700; 
				int shuffle_y = rand() % 400; 

				vetor_frango[f].ativo = true;
				vetor_frango[f].x = shuffle;
				vetor_frango[f].y = - shuffle_y;
				break;
			}	
		}
	}	
}

void Loop()
{
	if (tinicial == true)
	{
		graphics.DrawImage2D(0,0,1024,1024, play);
		graphics.DrawImage2D(botao_jogar);
	}

	else if (you_lose == true)
	{
		rtime = rtime + graphics.GetElapsedTime();
		graphics.DrawImage2D(0,0,1024,1024, game_over);
		if (irritante.IsPlaying()==false)
		{
			if (pl == false)
			{
				irritante.Stop();
				irritante.Play();
			}
		}
	}

	else if (winner == true)
	{
		rtime = rtime + graphics.GetElapsedTime();
		graphics.DrawImage2D(0,0,1024,1024, you_win);
	}

	else 
	{
		graphics.DrawImage2D(0,0,800,600, background);
		tempo = tempo + graphics.GetElapsedTime();
		if (tempo > 2)
		{
			gera_frangos ();
			tempo = 0;
		}

		ptime = ptime + graphics.GetElapsedTime();
		if (ptime > 10)
		{
			gera_pinguim ();
			ptime = 0;
		}

		for (f = 0; f < 7; f ++)
		{
			if(vetor_frango[f].ativo == true)
			{
				vetor_frango[f].y = vetor_frango[f].y + graphics.GetElapsedTime() * speed;	
				vetor_frango[f].imagem.SetPosition(vetor_frango[f].x,vetor_frango[f].y,128,128);
				graphics.DrawImage2D(vetor_frango[f].imagem);

				if (vetor_frango[f].y > 700)
				{
					vetor_frango[f].ativo = false;
					frangos_perdidos = frangos_perdidos + 1;
					if (frangos_perdidos >  2)
					{
						you_lose = true;
						irritante.Stop();
						irritante.Play();
					}
				}
			}
		}

		if(pinguim.ativo == true)
		{
			pinguim.y = pinguim.y + graphics.GetElapsedTime() * speed;	
			pinguim.imagem.SetPosition(pinguim.x,pinguim.y,128,128);
			graphics.DrawImage2D(pinguim.imagem);

			if (pinguim.y > 700)
			{
				pinguim.ativo = false;
			}
		}

		graphics.DrawText2D(650, 580, "Frangos Mortos");
		graphics.DrawText2D(705, 555, "%d", placar);
	}
}

void MouseClickIniciar(int button, int state, int x, int y)
{
	if ((state == MOUSE_STATE_DOWN)&&((tinicial == true)))
	{
		tinicial=false;
		botao_jogar.SetPosition(0,0,0,0);
	}
}

void MouseClickRejogar(int button, int state, int x, int y)
{
	if ((state == MOUSE_STATE_DOWN)&&((tinicial == false)&&((you_lose == true)||(winner == true)))&&(rtime > 2))
	{
		you_lose=false;
		winner=false;
		f = 0;
		level = 1;
		placar = 0;
		frangos_perdidos = 0;
		tempo = 0;
		speed = 200;
		ptime = 0;
		tinicial = true;
		rtime = 0;
		botao_jogar.SetPosition(290,145,256,256);

		for (f = 0; f < 7; f++)
		{
			vetor_frango[f].imagem.SetPosition(0,0,0,0);		
			vetor_frango[f].ativo = false;
		}

		pinguim.imagem.SetPosition(0,0,0,0);
		pinguim.ativo = false;

	}
}

void MouseClickPinguim(int button, int state, int x, int y)
{
	if (state == MOUSE_STATE_DOWN)
	{
		if (pinguim.ativo==true)
		{ 
			if ((x > pinguim.x)&&(x < pinguim.x + 83)&&(y > pinguim.y)&&(y < pinguim.y + 128))
			{
				you_lose = true;
				doublepop.Stop();
				doublepop.Play();
				pl = true;
			}
		}
	}
}

void MouseClickInput(int button, int state, int x, int y)
{
	if (state == MOUSE_STATE_DOWN)
	{
		for (f=0; f<7; f++)
		{
			if (vetor_frango[f].ativo==true)
			{
				if ((x > vetor_frango[f].x)&&(x < vetor_frango[f].x + 108)&&(y > vetor_frango[f].y)&&(y<vetor_frango[f].y + 128))
				{
					vetor_frango[f].ativo = false;
					pop.Stop();
					pop.Play();
					placar = placar + 1;
					if(placar==15)
					{
						level=2;
					}
					else if (placar==40)
					{
						level=3;
					}
					else if (placar==60)
					{
						level=4;
					}
					else if (placar==180)
					{
						level=5;
					}
					else if (placar==300)
					{
						level=6;
					}
					else if (placar==600)
					{
						level=7;
					}
					else if (placar==1000)
					{
						level=8;
					}
					if(placar > 1499)
					{
						winner = true;
					}
				}
				
			}		
		}
	}
}

int main (void)
{
	srand(time(NULL));

	graphics.CreateMainWindow(800, 600, "Dir, The Penguin");

	for (f = 0; f < 7; f++)
	{
		vetor_frango[f].imagem.LoadPNGImage("data\\chicken.png");
		vetor_frango[f].imagem.SetPosition(0,0,0,0);
		vetor_frango[f].imagem.SetOnClick(MouseClickInput);
	}

	graphics.SetTextFont("Arial", 22, FW_NORMAL, false, false);

	pinguim.imagem.LoadPNGImage("data\\pinguim_dir.png");
	pinguim.imagem.SetPosition(0,0,0,0);
	pinguim.imagem.SetOnClick(MouseClickPinguim);
	
	background.LoadPNGImage("data\\background2.png");
	play.LoadPNGImage("data\\play.png");
	
	botao_jogar.LoadPNGImage("data\\botao_jogar.png");
	botao_jogar.SetPosition(290,145,256,256);
	botao_jogar.SetOnClick(MouseClickIniciar);

	you_win.LoadPNGImage("data\\you_win.png");
	you_win.SetPosition(0,0,1024,1024);
	you_win.SetOnClick(MouseClickRejogar);	

	game_over.LoadPNGImage("data\\game_over.png");
	game_over.SetPosition(0,0,1024,1024);
	game_over.SetOnClick(MouseClickRejogar);

	pop.LoadAudio("data\\pop.wav");
	irritante.LoadAudio("data\\irritante.wav");
	doublepop.LoadAudio("data\\doublepop.wav");

	graphics.SetMainLoop(Loop);
	graphics.StartMainLoop();
	
	return 0;
}
