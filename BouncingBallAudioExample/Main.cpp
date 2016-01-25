#include "Graphics.h"
#include "Audio.h"

Graphics graphics;
Audio boing;

float positionX = 400;
float positionY = 400;
float speed = 400;
int dirX = 1;
int dirY = 1;

int colorR = 0;
int colorG = 0;
int colorB = 0;

void MainLoop(void){

	positionX = positionX + (speed * graphics.GetElapsedTime()) * dirX;
	positionY = positionY + (speed * graphics.GetElapsedTime()) * dirY;

	colorR += (1);
	colorG += (5);
	colorB += (10);

	if (colorR > 255)
		colorR = 0;
	if (colorG > 255)
		colorG = 0;
	if (colorB > 255)
		colorB = 0;

	graphics.SetColor(colorR,colorG,colorB);
	graphics.FillCircle2D(positionX, positionY, 60, 20);

	if(positionX > 800 - 60){
		dirX = -1;

		boing.Stop();
		boing.Play();
	}
	else if(positionX < 0 + 60){
		dirX = 1;
		
		boing.Stop();
		boing.Play();
	}

		if(positionY > 600 - 60){
		dirY = -1;

		boing.Stop();
		boing.Play();
		}
	else if(positionY < 0 + 60){
		dirY = 1;
		
		boing.Stop();
		boing.Play();
	}

}

int main (void)
{
	graphics.CreateMainWindow(800, 600, "Example Project");
	
	boing.LoadAudio("cartoon053.mp3");
	graphics.SetBackgroundColor(200, 200, 200);
	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();

	return 0;
}
