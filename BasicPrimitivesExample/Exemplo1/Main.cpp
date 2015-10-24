//Author: Edirlei Soares de Lima
//Create a program to draw a simple house.
//This example shows how draw basic primitives such as dots, lines, rectangles, triangles and circles.

#include "Graphics.h"

Graphics graphics;

void MainLoop() 
{
	graphics.SetColor(41, 156, 0);
	graphics.FillRectangle2D(0, 0, 800, 100);  //Draw terrain

	graphics.SetColor(100, 100, 100); 
	graphics.FillRectangle2D(200, 100, 400, 300);  //Draw wall

	graphics.SetColor(255, 136, 0);
	graphics.FillTriangle2D(200, 300, 400, 300, 300, 450); //Draw roof

	graphics.SetColor(120, 76, 0);
	graphics.FillRectangle2D(300, 100, 370, 240);  //Draw door

	graphics.SetColor(9, 9, 100);
	graphics.FillRectangle2D(220, 150, 270, 220);  //Draw window

	graphics.SetColor(0, 0, 0);
	graphics.FillCircle2D(310, 170, 5, 20);  //Draw doorknob	
}

int main(void)
{
	graphics.CreateMainWindow(800, 600, "Basic Primitives Example");
	graphics.SetBackgroundColor(152, 209, 250);
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}
