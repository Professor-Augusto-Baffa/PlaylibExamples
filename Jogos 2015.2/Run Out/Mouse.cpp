#include "Mouse.h"
#include "Auxiliar.h"
#include "Graphics.h"


int mouse_x, mouse_y, click_x, click_y;

bool MouseState[3];

void MouseMotionInput (int x, int y)
{
	
	 mouse_x = x;
	 mouse_y = y;
}

void MouseClickInput(int button, int state, int x, int y)
{

	if (state == KEY_STATE_DOWN)
		MouseState[button] = true;

    else if (state == KEY_STATE_UP)
        MouseState[button] = false;

	if (MouseState[MOUSE_LEFT_BUTTON]) //diz
		{

			click_x = x;
			click_y = y;
		}


}

//void MouseExecute(int *scene, int x1, int y1, int x2, int y2)
//{
//	if (*scene == 1)
//		if (Collision(mouse_x, mouse_y, x1, y1, x2, y2))
//			graphics.DrawImage2D