//Author: Edirlei Soares de Lima
//This exmaple shows how to read a tile map from a file and draw the scene. Also, shows how to move the camera over the scene.

#include "Graphics.h"
#include <stdio.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 25

Graphics graphics;

char Mapa[MAP_WIDTH][MAP_HEIGHT];

Image block_glass;
Image block_terrain;

float camera_position = 18;

void LoadMap(char *filename)
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

void MainLoop()
{
	int x, y;
    for (x = camera_position - 18; x < camera_position + 18; x++)
	{
		for (y = 0; y < 25; y++)
		{
			if (Mapa[x][y] == 'G')
			{
				graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 24, 24, block_glass);
			}
			if (Mapa[x][y] == 'T')
			{
				graphics.DrawImage2D(((x - (camera_position - 18))*24),y*24, 24, 24, block_terrain);
			}
		}
	}
}

void KeyboardInput(int key, int state, int x, int y)
{
	if ((key == KEY_LEFT)&&(state == KEY_STATE_DOWN))
	{
		if (camera_position > 18)
			camera_position = camera_position - 0.1; 
    }
	if ((key == KEY_RIGHT)&&(state == KEY_STATE_DOWN))
	{
		if (camera_position < MAP_WIDTH - 18)
			camera_position = camera_position + 0.1; 
	}  
}
 
 
int main(void)
{
    graphics.CreateMainWindow(800, 600, "TileMap Platform Game Example");
    graphics.SetBackgroundColor(152,209,250);

	LoadMap("Map01.txt");
 
    block_glass.LoadPNGImage("grass.png");
    block_terrain.LoadPNGImage("stone.png");
    
	graphics.SetKeyboardInput(KeyboardInput);
 
    graphics.SetMainLoop(MainLoop);
    graphics.StartMainLoop();
    return 0;
}