#include "Keyboard.h"
#include "Graphics.h"
#include <stdio.h>


#define MAP_WIDTH 100
#define MAP_HEIGHT 25

extern Graphics graphics;
extern int scene, aux, personagem, dificuldade, modo;

bool KeyState[256];
float camera_position = 18;
extern bool pause;

void KeyboardInput(int key, int state, int x, int y)
{

    if (state == KEY_STATE_DOWN)
        KeyState[key] = true;
    else if (state == KEY_STATE_UP)
        KeyState[key] = false;

}

void KeyboardExecute()
{
	
	if (KeyState[KEY_ENTER])
	{
		if (scene == 0 || scene == 1 || scene == 2 || scene == 3 || scene == 4)
		{
			if (scene == 0)
				scene = 1;

			else if (scene == 1)
			{
				if (aux==0)
					scene = 2;
			
				else if (aux==1)
					scene = -1;

				else if (aux==2)
					exit(0);
			}

			else if (scene == 2)
				scene = 3;

			else if (scene == 3)
				scene = 4;

			else if (scene == 4)
				scene = -2;
			
			KeyState[KEY_ENTER] = false; //faz com que somente um clique por vez seja aceito
		}
	}


	if (KeyState['f'])
	{
		graphics.SetFullscreen(true);
	}


	if (KeyState['t'])
	{

	}


	if (KeyState[KEY_ESC] )
	{

		if (scene == 1)
			scene = 0;

		else if (scene == 2)
			scene = 1;

		else if (scene == 3)
			scene = 2;

		else if (scene == 4)
			scene = 3;

		if (scene == 5)
		{
			if (pause == false)
				pause = true;
			else
				pause = false;
		}

		KeyState[KEY_ESC] = false;
	}


	if (KeyState[KEY_LEFT])
	{
		if (scene == 2 || scene == 3 || scene == 4)
		{
		
			if (scene == 2)
			{
				if (modo==0)
					modo=2;

				else if (modo==1)
					modo=0;

				else if (modo==2)
					modo=1;
			}

			if (scene == 3)
			{
				if (dificuldade==0)
					dificuldade=2;

				else if (dificuldade==1)
					dificuldade=0;

				else if (dificuldade==2)
					dificuldade=1;
			}

			if (scene == 4)
			{
				if (personagem==0)
					personagem=2;

				else if (personagem==1)
					personagem=0;

				else if (personagem==2)
					personagem=1;
			}
			KeyState[KEY_LEFT] = false;
		}
    }


	if (KeyState[KEY_RIGHT])
	{
		if (scene == 2 || scene == 3 || scene == 4)
		{
				if (scene == 2)
			{
				if (modo==0)
					modo=1;

				else if (modo==1)
					modo=2;

				else if (modo==2)
					modo=0;
			}
		
			if (scene == 3)
			{
				if (dificuldade==0)
					dificuldade=1;

				else if (dificuldade==1)
					dificuldade=2;

				else if (dificuldade==2)
					dificuldade=0;
			}

			if (scene == 4)
			{
				if (personagem==0)
					personagem=1;

				else if (personagem==1)
					personagem=2;

				else if (personagem==2)
					personagem=0;
			}
			KeyState[KEY_RIGHT] = false; 
		}
	}  


	if (KeyState[KEY_UP])
	{
		if (scene == 1)
		{

			if (aux==2)
				aux=1;

			else if (aux==1)
				aux=0;

			KeyState[KEY_UP] = false;
		}

		/*KeyState[KEY_UP] = false;*/
	}


	if (KeyState[KEY_DOWN])
	{
		if (scene == 1)
		{
			if (aux==-1)
				aux=0;

			else if (aux==0)
				aux=1;

			else if (aux==1)
				aux=2;

			KeyState[KEY_DOWN] = false; 
		}	
	}
	
	//printf("\nDIFICULDADE = %d\n", dificuldade);
}

