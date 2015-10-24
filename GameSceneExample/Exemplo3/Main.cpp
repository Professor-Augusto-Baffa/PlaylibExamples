//Author: Edirlei Soares de Lima
//This example shows how to use images to create a similar "Super Mario" game scene.

#include "Graphics.h"

Graphics graphics;

Image block_glass;
Image block_pipe;
Image block_hill;

void MainLoop()
{
	int x;

	for (x = 0; x < 800; x+=256)
	{
		graphics.DrawImage2D(x, 128, 256, 256, block_hill); //Draw Hills
	}

	graphics.DrawImage2D(480, 70, 128, 128, block_pipe); //Draw pipe

	for (x = 0; x < 800; x+=128)
	{
		graphics.DrawImage2D(x, 0, 128, 128, block_glass); //Draw grass blocks
	}
}


int main(void)
{
	graphics.CreateMainWindow(800, 600, "2D Game Scene Example");
	graphics.SetBackgroundColor(152,209,250);

	block_glass.LoadPNGImage("mario_ground.png");
	block_pipe.LoadPNGImage("mario_pipe.png");
	block_hill.LoadPNGImage("mario_background.png");

	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}
