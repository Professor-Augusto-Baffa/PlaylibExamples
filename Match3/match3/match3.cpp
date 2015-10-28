#include <stdio.h>
#include "Graphics.h"
#include <time.h>
#include <stdlib.h>
#include "Audio.h"

Graphics graphics;

int board[8][8];
int xx, yy, b1, b2;
int w = 1, x3, x4, y3, y4;
int mov = 25, cmov = 0;
int pontos = 0, cpontos;
int b, k = 2;
int cc = 0;
int dd;
bool t_inicial = true;
bool gmove = false;
bool g_over = false;
bool semcredito = false;

Image img;
Image tinicial, tcredit, button_play, button_menu, gover, iPonto, iMoves;
Image alien1, alien2, alien3, alien4, alien5, alien6;
Image grid, back;
Audio Musica, boom;

void MainLoop()
{		
	int Xw = ((graphics.GetScreenWidth() / 2) - (292));
	int Yw = ((graphics.GetScreenHeight() / 2) - (200));
	int xb = ((graphics.GetScreenWidth() / 2) - (90));	
	int yb = ((graphics.GetScreenHeight() / 2) - (16));
			
	mov = mov - cmov;
	cmov = 0;
	if (mov == 0)
	{
		g_over = true;
	}
	
	for(b1 = 0; b1 < 3000 ; b1 += 256)
	{
		for ( b2 = 0; b2 < 1000 ; b2 += 256)
		{
			graphics.DrawImage2D(b1, b2, 256, 256, back);
		}
	}

	if (t_inicial == true)
	{
		mov = 25;
		pontos = 0;
		cpontos=0;
		graphics.DrawImage2D(Xw, Yw, 584, 400, tcredit);
		graphics.DrawImage2D(button_play);
		button_play.SetPosition(xb,yb + 20,200,52);
		g_over = false;
	}
	else if (g_over == true)
	{
		graphics.DrawImage2D(Xw, Yw, 584, 400, gover);
		graphics.DrawImage2D(xb, yb, 180, 32, button_menu);
		button_menu.SetPosition(xb,yb,180,32);		
		graphics.DrawImage2D((xb-10),(yb-50),200,52, iPonto);
		graphics.DrawText2D((xb+50),(yb-70),"%d",pontos);	
	} 
	else
	{
		int xw = ((graphics.GetScreenWidth() / 2) - (256));	
		int yw = ((graphics.GetScreenHeight() / 2) - (256));

		graphics.DrawImage2D(xw,yw,512,512, grid);
		
		graphics.DrawImage2D((xw+490),(yw+450),150,32, iPonto);
		graphics.DrawText2D((xw+520),(yw+425),"%d",pontos);
		graphics.DrawImage2D((xw+490),(yw+350),150,32, iMoves);
		graphics.DrawText2D((xw+520),(yw+325),"%d",mov);
		graphics.DrawImage2D(button_menu);
		button_menu.SetPosition((xw+490),(yw+250),180,52);	

		for (xx = 0 ; xx < 8 ; xx ++)
		{
			for(yy = 0; yy < 8; yy ++)
			{

				if (board[xx][yy] == 0)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien1);
				}
				if (board[xx][yy] == 1)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien2);
				}
				if (board[xx][yy] == 2)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien3);
				}
				if (board[xx][yy] == 3)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien4);
				}
				if (board[xx][yy] == 4)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien5);
				}
				if (board[xx][yy] == 5)
				{
					graphics.DrawImage2D(xw + xx*64, yw + yy*64, 64, 64, alien6);
				}
			}
		}
		for (xx = 0 ; xx < 8 ; xx ++)
		{
			
			for (yy = 0 ; yy < 8 ; yy ++)
			{
				cc = 0;

				//Destroi pecas iguais na horizontal
				if (board[xx][yy] != -1)
				{
					while (board[xx + cc][yy] == board[xx + cc + 1][yy])
					{
						cc++;
						if (xx + cc + 1 > 7)
							break;
					}
				}

				if (cc >= 2)
				{
					for (int aa = 0; aa <= cc; aa++)
					{
						if (xx + aa > 7)
							break;
						board[xx + aa][yy] = -1;
					}
					boom.Stop();
					boom.Play();
				}
				k=cc+1;
				cc = 0;

				//Destroi pecas iguais na vertical
				if (board[xx][yy] != -1)
				{
					while (board[xx][yy + cc] == board[xx][yy + cc + 1])
					{
						cc++;
						if (yy + cc + 1 > 7)
							break;
					}
				}

				if (cc >= 2)
				{
					for (int aa = 0; aa <= cc; aa++)
					{
						if (yy + aa > 7)
							break;
						board[xx][yy + aa] = -1;
					}
					boom.Stop();
					boom.Play();
					k = cc + 1;				
				}

				// Pontuação	
				if (k>2)
				{
					cpontos = 20*k;
					pontos = pontos + cpontos;
					k = 0;
				}			 
			}
		}		

		//Abaixa as pecas que estiverem em cima
		for (yy = 0 ; yy < 8 ; yy ++)
		{
			for (xx = 0 ; xx < 8 ; xx ++)
			{
				if (board[xx][yy] == -1)
				{
					for (int aa = yy; aa < 7; aa++)
					{						
						board[xx][aa] = board[xx][aa+1];	

					}
					board[xx][7] = -1;
				
				}
			}
		} 

		//Gera novas pecas nos lugares vazios
		for (yy = 0 ; yy < 8 ; yy ++)
		{
			for (xx = 0 ; xx < 8 ; xx ++)
			{
				if (board[xx][yy] == -1)
				{
					int rnd = rand()%6;
					board[xx][yy] = rnd;
				}
			}
		}
	}
	
}
void ClickButtonPlay(int button, int state, int x, int y)
{
	t_inicial = false;
}

void ClickButtonMenu(int button, int state, int x, int y)
{
	t_inicial = true;
}

void MouseClickInput ( int button, int state, int x, int y)
{
	int xw = ((graphics.GetScreenWidth() / 2) - (256));	
	int yw = ((graphics.GetScreenHeight() / 2) - (256));

	if ((!t_inicial)&&(!g_over))
	{
		if (w == 1)
		{
			if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
			{
				x3 = (int)((x-xw)/64); 
				y3 = (int)((y-yw)/64);
				w = 2;
			}
		}
		else if (w == 2)
		{
			if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
			{
				x4 = (int)((x-xw)/64); 
				y4 = (int)((y-yw)/64);
				w = 1;

				if ((x3 == (x4 + 1) || x3 == (x4 -1)) && y3 == y4 )
				{
					int memoria = board[x3][y3];
					board[x3][y3] = board[x4][y3];
					board[x4][y3] = memoria; 
					cmov=1;
				}
				if ((y3 == (y4 +1) || y3 == (y4 - 1)) && x3 == x4 )			
				{
					int memoria = board[x3][y3];
					board[x3][y3] = board[x3][y4];
					board[x3][y4] = memoria;
					cmov=1;
				}
			}
		}	
	}
}

int main(void)
{
	int x, y;
	graphics.CreateMainWindow(784,572,"Match3");
	srand(time(NULL));
	for(y = 0; y < 8; y++)
	{
		for(x = 0; x < 8; x++)
		{
			int rnd = rand()%6;
			board[x][y] = rnd;
		}
	}
	for (int t = 0; t < 5; t++)
	{
		for (xx = 0 ; xx < 8 ; xx ++)
		{
			for (yy = 0 ; yy < 8 ; yy ++)
			{
				int cc = 0;	

				//Destroi pecas iguais na horizontal
				if (board[xx][yy] != -1)
				{
					while (board[xx + cc][yy] == board[xx + cc + 1][yy])
					{
						cc++;
						if (xx + cc + 1 > 7)
							break;
					}
				}

				if (cc >= 2)
				{
					for (int aa = 0; aa <= cc; aa++)
					{
						if (xx + aa > 7)
							break;
						board[xx + aa][yy] = -1;
					}
				}
				cc = 0;

				//Destroi pecas iguais na vertical
				if (board[xx][yy] != -1)
				{
					while (board[xx][yy + cc] == board[xx][yy + cc + 1])
					{
						cc++;
						if (yy + cc + 1 > 7)
							break;
					}
				}

				if (cc >= 2)
				{
					for (int aa = 0; aa <= cc; aa++)
					{
						if (yy + aa > 7)
							break;
						board[xx][yy + aa] = -1;
					}
				}	
			}
		}
		for(y = 0; y < 8; y++)
		{
			for(x = 0; x < 8; x++)
			{
				if (board[x][y] == -1)
				{
				int rnd = rand()%6;
				board[x][y] = rnd;
				}
			}
		}	
	}
	
	for(y = 0; y < 8; y++)
	{
		for(x = 0; x < 8; x++)
		{
			if (board[x][y] == -1)
			{
			int rnd = rand()%6;
			board[x][y] = rnd;
			}
		}
	}
	
	tcredit.LoadPNGImage("Data\\tcredit.png");	
	tinicial.LoadPNGImage("Data\\tinicial.png");
	iPonto.LoadPNGImage("Data\\button_score.png");
	iMoves.LoadPNGImage("Data\\button_move.png");
	button_play.LoadPNGImage("Data\\button_play.png");
	button_play.SetOnClick(ClickButtonPlay);
	gover.LoadPNGImage("Data\\g_over.png");
	button_menu.LoadPNGImage("Data\\button_menu.png");
	button_menu.SetOnClick(ClickButtonMenu);
	Musica.LoadAudio("Data\\Background.mp3");
	Musica.Play();
	boom.LoadAudio("Data\\boom.mp3");
	back.LoadPNGImage("Data\\background.png");
	grid.LoadPNGImage("Data\\grid.png");
	alien1.LoadPNGImage("Data\\alien1.png");
	alien2.LoadPNGImage("Data\\alien2.png");
	alien3.LoadPNGImage("Data\\alien3.png");
	alien4.LoadPNGImage("Data\\alien4.png");
	alien5.LoadPNGImage("Data\\alien5.png");
	alien6.LoadPNGImage("Data\\alien6.png");
	graphics.SetTextFont("Courier Regular", 36, FONT_WEIGHT_NORMAL, false, false);
	graphics.SetMouseClickInput(MouseClickInput);
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();

	return 0;
}
