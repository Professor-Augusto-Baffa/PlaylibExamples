//Author: Edirlei Soares de Lima
//This example shows how to rotate an object.

#include "Graphics.h"

Graphics graphics;

float rotation_angle = 0;
int rotation_speed = 120;

void MainLoop()
{  	
	rotation_angle += rotation_speed * graphics.GetElapsedTime();
	if (rotation_angle > 360)
		rotation_angle = 0;

	graphics.RotateBegin(rotation_angle);
	graphics.FillRectangle2D(300, 200, 500, 400);
	graphics.RotateEnd();
}

int main(void)
{
    graphics.CreateMainWindow(800, 600, "Simple Rotation Example");
	graphics.SetBackgroundColor(0,0,0);
	graphics.SetColor(35, 191, 27);
        
    graphics.SetMainLoop(MainLoop);
    graphics.StartMainLoop();
    return 0;
}