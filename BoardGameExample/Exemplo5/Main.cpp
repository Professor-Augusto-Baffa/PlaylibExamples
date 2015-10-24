//Author: Edirlei Soares de Lima
//This example shows how to draw a checkers/chess board game on screen. Player uses the mouse to put the pieces over board by clicking on the desired place.

#include "Graphics.h"

Graphics graphics;

int board[8][8];

void MainLoop()
{
	int x, y;
	bool intercala = false;
	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{	
			if (intercala == true)
				graphics.SetColor(0,0,0);
			else
				graphics.SetColor(255,255,255);

			graphics.FillRectangle2D(200 + (x * 50), 100 + (y * 50), 200 + (x * 50) + 50, 100 + (y * 50) + 50);

			if (board[x][y] == 1)
			{
				graphics.SetColor(255,0,0);
				graphics.FillCircle2D((200 + (x * 50) + 25), (100 + (y * 50) + 25), 20, 20);
			}
			else if (board[x][y] == 2)
			{
				graphics.SetColor(0,255,0);
				graphics.FillCircle2D((200 + (x * 50) + 25), (100 + (y * 50) + 25), 20, 20);
			}

			intercala = !intercala;
		}
		intercala = !intercala;
	}
}

void MouseClickInput(int button, int state, int x, int y)
{
  if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
  {
     int selecionado_x = ((x / 50) - (200 / 50));
	 int selecionado_y = ((y / 50) - (100 / 50)); 
	 if ((selecionado_x >= 0)&&(selecionado_x < 8)&&(selecionado_y >= 0)&&(selecionado_y < 8))
	 {
		 board[selecionado_x][selecionado_y] = 1;
	 }
  }
  else if ((button == MOUSE_RIGHT_BUTTON)&&(state == MOUSE_STATE_DOWN))
  {
     int selecionado_x = ((x / 50) - (200 / 50));
	 int selecionado_y = ((y / 50) - (100 / 50)); 
	 if ((selecionado_x >= 0)&&(selecionado_x < 8)&&(selecionado_y >= 0)&&(selecionado_y < 8))
	 {
		 board[selecionado_x][selecionado_y] = 2;
	 }
  }
}

int main(void)
{
	graphics.CreateMainWindow(800, 600, "Board Game Example - Mouse Control");
	graphics.SetBackgroundColor(100,100,100);
	
	int x, y; 
	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{	
			board[x][y] = 0; //Initialize board
		}
	}

	graphics.SetMouseClickInput(MouseClickInput);

	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}
