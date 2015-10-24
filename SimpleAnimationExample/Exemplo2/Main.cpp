//Author: Edirlei Soares de Lima
//This Example shows how create a simple animation.

#include "Graphics.h"
#include <stdio.h>

Graphics graphics;
 
float px = 100;    //Circle X Position.
float py = 100;    //Circle Y Position.
float speed = 150; //Ball Speed.
int dir = 1;       //Ball Direction.
bool pause = false;//Pauses animation.
 
//Controls the Animation
void Animate()
{
    if (!pause)
    {
        if (dir == 1)
        {
            //Calculates the X and Y movements using frames per second.
            px += speed * graphics.GetElapsedTime();
            py += speed * graphics.GetElapsedTime();
        }
        else if (dir == 2)
        {
            px += speed * graphics.GetElapsedTime();
            py -= speed * graphics.GetElapsedTime();
        }
        else if (dir == 3)
        {
            px -= speed * graphics.GetElapsedTime();
            py -= speed * graphics.GetElapsedTime();
        }
        else if (dir == 4)
        {
            px -= speed * graphics.GetElapsedTime();
            py += speed * graphics.GetElapsedTime();
        }
 
        //Control the circle moves
        if (px+50 > graphics.GetScreenWidth())
        {
            if (dir == 1)
                dir = 4;
            else if (dir == 2)
                dir = 3;
        }
        else if (px < 50)
        {
            if (dir == 3)
                dir = 2;
            else if (dir == 4)
                dir = 1;
        }
        else if (py+50 > graphics.GetScreenHeight())
        {
            if (dir == 1)
                dir = 2;
            else if (dir == 4)
                dir = 3;
        }
        else if (py < 50)
        {
            if (dir == 3)
                dir = 4;
            else if (dir == 2)
                dir = 1;
        }
    }
}
 
void KeyboardInput(int key, int state, int x, int y)
{
    if (((key == 'p')||(key == 'P'))&&(state == KEY_STATE_DOWN))
    {
        pause = !pause;
    }
}
 
void MainLoop()
{  
    Animate();
    graphics.SetColor(35, 191, 27);
    graphics.FillCircle2D(px, py, 50, 32); //Draw a circle on screen.
}
 
int main(void)
{
    graphics.CreateMainWindow(800, 600, "Simple Animation Example");
    graphics.SetBackgroundColor(0, 0, 0);
    graphics.SetKeyboardInput(KeyboardInput);
    graphics.SetMainLoop(MainLoop);
    graphics.StartMainLoop();
    return 0;
}