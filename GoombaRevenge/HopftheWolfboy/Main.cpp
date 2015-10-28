#include "Graphics.h"
#include <stdio.h>
#include "Audio.h"

#define MAP_WIDTH 300
#define MAP_HEIGHT 25

Graphics graphics;

char Mapa[MAP_WIDTH][MAP_HEIGHT];

Image fundo;
Image ceu;
Image play;
Image vida3;
Image vida2;
Image vida1;
Image checkpoint;
Image gameover;
Image winner;
Image monstro2;
Image monstro1;
Image vetor[2];
Image bloco_grama;
Image bloco_terra;

Audio Tema;
Audio GameOverEnd;
Audio Win;
Audio Dead;
Audio Titulo;
Audio JumpAudio;

int personagem = 1;
float tempo_reset = 0;
float camera_position = 18;
bool jump = false;
float jump_time = 0;
bool onground = false;
float character_x = 100;
float character_y = 300;
int vida = 3;
bool move_left = false;
bool move_right = false;
bool momento = false;
float move_speed = 10;
float jump_speed = 200;

void LoadMapa(char *filename)
{
	int c;
	int count_line = 25;
	int count_col = 0;
	FILE *file;
	file = fopen(filename, "r");
	if (file) 
	{
		while ((c = getc(file)) != EOF)
		{
			if (count_col == MAP_WIDTH-1)
			{
				count_line--;
				count_col = 0;
			}
			else
			{
				Mapa[count_col][count_line] = (char)c;
				count_col++;
			}
		}
		fclose(file);
	}
}

void ApllyMove()
{
	if (move_left == true)
	{
		if (camera_position > 18)
		{
			if (Mapa[(int)((((character_x))/24) + (camera_position - 18.5))][(int)(character_y + 3)/24] == 'X')
			{
				camera_position-=move_speed * graphics.GetElapsedTime();
			}
		}
	}
	if (move_right == true)
	{	
		if (camera_position < MAP_WIDTH - 18)
		{

			if (Mapa[(int)((((character_x + 35))/24) + (camera_position - 18.5))][(int)(character_y + 3)/24] == 'X')
			{
				camera_position+=move_speed * graphics.GetElapsedTime();			
			}
		}
	}  
}

void ApllyJump()
{

	if (jump == true)
	{
		jump_time = jump_time + graphics.GetElapsedTime();
		character_y += jump_speed * graphics.GetElapsedTime();

		if (jump_time > 0.4)
		{
			jump = false;
		}
	}
}

void ApplyGravity()
{
	if (((Mapa[(int)((((character_x))/24) + (camera_position - 18))][(int)(character_y - 6)/24] == 'X')&&(Mapa[(int)((((character_x))/24) + (camera_position - 17))][(int)(character_y - 6)/24] == 'X'))&&(jump == false))
	{
		character_y -= (jump_speed * 2) * graphics.GetElapsedTime();
		onground = false;
	}
	else
	{
		onground = true;
	}
}

void GameOver()
{
	graphics.DrawImage2D(0,0,800,600,gameover);
}

void Morte()
{
	if (character_y < 24)
	{
		if (Dead.IsPlaying() == false)
		{
			Tema.Stop();
			Dead.Stop();
			Dead.Play();
			tempo_reset = 0;
		}
	}

	if (((Mapa[(int)((((character_x -20))/24) + (camera_position - 18))][(int)(character_y + 1)/24] == 'M'))||((Mapa[(int)((((character_x +35))/24) + (camera_position - 18.5))][(int)(character_y + 1)/24] == 'M'))||(((Mapa[(int)((((character_x -20))/24) + (camera_position - 18))][(int)(character_y + 1)/24] == 'N'))||((Mapa[(int)((((character_x +35))/24) + (camera_position - 18))][(int)(character_y + 1)/24] == 'N'))))
	{
		if (Dead.IsPlaying() == false)
		{
			Tema.Stop();
			Dead.Stop();
			Dead.Play();
			tempo_reset = 0;
		}

	}	


	if (Dead.IsPlaying() == true)
	{
		tempo_reset = tempo_reset + graphics.GetElapsedTime();
		character_y -= (jump_speed * 2) * graphics.GetElapsedTime();

		if (tempo_reset > 3.5)
		{
			LoadMapa("Data\\Mapa01.txt");
			camera_position = 18;
			character_x = 100;
			character_y = 200;
			vida = vida-1;
			if (vida < 0)
			{
				Dead.Stop();
				GameOverEnd.Stop();
				GameOverEnd.Play();	
			}
			else
			{
				Dead.Stop();
				Tema.Stop();
				Tema.Play();			
			}
		}
	}

	if (Mapa[(int)((((character_x))/24) + (camera_position -17))][(int)(character_y - 6)/24] == 'M')
	{
		Mapa[(int)((((character_x))/24) + (camera_position -17))][(int)(character_y - 6)/24] = 'X';

	}
	if (Mapa[(int)((((character_x))/24) + (camera_position -18))][(int)(character_y - 6)/24] == 'M')
	{
		Mapa[(int)((((character_x))/24) + (camera_position -18))][(int)(character_y - 6)/24] = 'X';

	}

	if (Mapa[(int)((((character_x))/24) + (camera_position -17))][(int)(character_y - 6)/24] == 'N')
	{
		Mapa[(int)((((character_x))/24) + (camera_position -17))][(int)(character_y - 6)/24] = 'X';

	}
	if (Mapa[(int)((((character_x))/24) + (camera_position -18))][(int)(character_y - 6)/24] == 'N')
	{
		Mapa[(int)((((character_x))/24) + (camera_position -18))][(int)(character_y - 6)/24] = 'X';

	}
}

void Vencer()
{
	if (Win.IsPlaying() == false)
	{
		Tema.Stop();
		Win.Stop();
		Win.Play();
	}
	graphics.DrawImage2D(0,0,800,600,winner);
}

void MainLoop()
{

	if (momento == false)
	{
		graphics.DrawImage2D (0,0,800,600,fundo);
		graphics.DrawImage2D (94,88,512,512,play);
	}
	if (momento == true) 
	{
		if (vida<0)
		{
			GameOver();
		}
		else if (((Mapa[(int)((((character_x +35))/24) + (camera_position - 18))][(int)(character_y + 1)/24] == 'C'))||(((Mapa[(int)((((character_x))/24) + (camera_position - 18))][(int)(character_y - 6)/24] == 'C')||(Mapa[(int)((((character_x))/24) + (camera_position - 17))][(int)(character_y - 6)/24] == 'C'))))
		{
			Vencer();
		}
		else
		{
			ApplyGravity();
			Morte();
			if (Dead.IsPlaying() == false)
			{
				ApllyJump();
				ApllyMove();
			}
			graphics.DrawImage2D(0,0,800,600, ceu);
			int x, y;

			for (x = camera_position - 18; x < camera_position + 18; x++)
			{
				for (y = 0; y < 25; y++)
				{
					if (Mapa[x][y] == 'G')
					{
						graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 24, 24, bloco_grama);
					}
					if (Mapa[x][y] == 'T')
					{
						graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 24, 24, bloco_terra);
					}
					if (Mapa [x][y] == 'M')
					{
						graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 36,48, monstro1);
					}
					if (Mapa [x][y] == 'N')
					{
						graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 36,48, monstro2);
					}
					if (Mapa [x][y] == 'C')
					{
						graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 36,48, checkpoint);
					}
				}
			} 

			if (vida==3)
			{
				graphics.DrawImage2D (700,520, 64,64, vida3);
			}
			else if (vida ==2)
			{ 
				graphics.DrawImage2D (700,520, 64,64, vida2);
			}
			else if (vida==1)
			{
				graphics.DrawImage2D (700,520, 64,64, vida1);
			}
			graphics.DrawImage2D(character_x - 10, character_y - 3,30,60, vetor[personagem]);


		}
	}
}


void KeyboardInput(int key, int state, int x, int y)
{
	if (state == KEY_STATE_DOWN)
	{
		if (key == ' ')
		{
			if ((jump == false)&&(onground == true))
			{
				jump_time = 0;
				jump = true;
			
				if ((JumpAudio.IsPlaying() == false)&&(vida > 0)&&(Win.IsPlaying() == false))
				{
					JumpAudio.Stop();
					JumpAudio.Play();
				}

			}
		}

		if ((key== 'r')||(key =='R'))
		{
			camera_position = 18;
			character_x = 100;
			character_y = 200;
			LoadMapa("Data\\Mapa01.txt");
			vida = 3;
			GameOverEnd.Stop();
			Tema.Stop();
			Tema.Play();	
		}

		if (key == KEY_LEFT)
		{
			personagem = 0;
		
			move_left = !move_left;
			move_right = false;

		}
		if (key == KEY_RIGHT)
		{	
			personagem = 1;		

			move_right = !move_right;
			move_left = false;
		}
	}
	else if (state = KEY_STATE_UP)
	{
		if ((key == KEY_LEFT)||(key == KEY_RIGHT))
		{
			move_right = false;
			move_left = false;	
		}
	}
}

void MouseClickplay(int button, int state, int x, int y)
{
	if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
	{
		Titulo.Stop();
		Tema.Stop();
		Tema.Play();
		momento = true;
	}
}

int main(void)
{
	graphics.CreateMainWindow(800, 576, "Goomba's Revenge");

	LoadMapa("Data\\Mapa01.txt");

	play.LoadPNGImage("Data\\play.png");
	play.SetPosition(256,52,502,146);
	play.SetOnClick(MouseClickplay);

	vetor[0].LoadPNGImage("Data\\goomba.png");
	vetor[1].LoadPNGImage("Data\\goomba2.png");
	monstro2.LoadPNGImage("Data\\mario2.png");
	monstro1.LoadPNGImage("Data\\monstro.png");

	fundo .LoadPNGImage("Data\\fundo.png");
	ceu.LoadPNGImage("Data\\ceu.png");
	bloco_grama.LoadPNGImage("Data\\ferro.png");
	bloco_terra.LoadPNGImage("Data\\tijolo.png");

	winner.LoadPNGImage("Data\\winner.png");
	checkpoint.LoadPNGImage("Data\\checkpoint.png");
	gameover.LoadPNGImage("Data\\gameover.png");

	vida3.LoadPNGImage("Data\\vida3.png");
	vida2.LoadPNGImage("Data\\vida2.png");
	vida1.LoadPNGImage("Data\\vida1.png");

	Tema.LoadAudio("Data\\MainTheme.mp3");
	Win.LoadAudio("Data\\End.mp3");
	Dead.LoadAudio("Data\\Dead.mp3");
	GameOverEnd.LoadAudio("Data\\GameOver.mp3");
	JumpAudio.LoadAudio("Data\\Jump.mp3");
	Titulo.LoadAudio("Data\\Title.mp3");
	Titulo.Play();

	graphics.SetKeyboardInput(KeyboardInput);


	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}