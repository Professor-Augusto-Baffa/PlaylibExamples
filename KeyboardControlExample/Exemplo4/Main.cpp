//Author: Edirlei Soares de Lima
//This example shows how allow the player to control a car using the directional keys.

#include "Graphics.h"

Graphics graphics;

Image block_floor;
Image car[4];

//Direction to turn the car
int car_direction = 2;

//Car X Position
double car_x = 336;

//Car Y Position
double car_y = 236;

void MainLoop()
{
	int x, y;
	for (x = 0; x < 800; x+=256)
	{
		for (y = 0; y < 600; y+=256)
		{
			graphics.DrawImage2D(x, y, 256, 256, block_floor); //Draw floor blocks
		}
	}

	graphics.DrawImage2D(car_x, car_y, 128, 128, car[car_direction]); //Draw car
}

void KeyboardInput(int key, int state, int x, int y)
{

	if ((key == KEY_LEFT)&&(state == KEY_STATE_DOWN))
	{
		car_direction = 1;
		car_x -= 4;
	}
	if ((key == KEY_RIGHT)&&(state == KEY_STATE_DOWN))
	{
		car_direction = 3;
		car_x += 4;
 	}
	if ((key == KEY_UP)&&(state == KEY_STATE_DOWN))
	{
		car_direction = 2;
		car_y += 4;
 	}
	if ((key == KEY_DOWN)&&(state == KEY_STATE_DOWN))
	{
		car_direction = 0;
		car_y -= 4;
 	}
}

int main(void)
{
	graphics.CreateMainWindow(800, 600, "Keyboard Control Example");
	graphics.SetBackgroundColor(152,209,250);

	//Carrega as imagens
	block_floor.LoadPNGImage("car_ground.png");
	car[0].LoadPNGImage("car_down.png");
	car[1].LoadPNGImage("car_left.png");
	car[2].LoadPNGImage("car_up.png");
	car[3].LoadPNGImage("car_right.png");	

	graphics.SetKeyboardInput(KeyboardInput);
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}
