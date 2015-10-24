//Author: Edirlei Soares de Lima
//This example shows how to draw imagens stored in a sprites sheet

#include "Graphics.h"
 
#define SPRITE_SIZE 128

Graphics graphics;

struct Sprite{
	int x;
	int y;
	int width;
	int height;
};

Sprite sprite[4][4];
Image sprite_sheet;

float player_x = 350;   
float player_y = 250;   
float player_speed = 80;
int animation_clip = 1;
int animation_index = 0;
float time_next_frame = 0;

bool key_states[256];

void KeyboardInput(int key, int state, int x, int y)
{
	if (state == KEY_STATE_DOWN)
		key_states[key] = true;
	else if (state == KEY_STATE_UP)
		key_states[key] = false;
}

void MainLoop()
{  
	bool next_frame = false;
	time_next_frame = time_next_frame + graphics.GetElapsedTime();

	if (key_states[KEY_LEFT])
    {
       animation_clip = 0;
	   next_frame = true;	   
	   player_x -= player_speed * graphics.GetElapsedTime();
    }
    else if (key_states[KEY_RIGHT])
    {
      animation_clip = 1;
	  next_frame = true;
	  player_x += player_speed * graphics.GetElapsedTime();
    }
    else if (key_states[KEY_UP])
    {
        animation_clip = 3;
		next_frame = true;
		player_y += player_speed * graphics.GetElapsedTime();
    }
    else if (key_states[KEY_DOWN])
    {
		animation_clip = 2;
		next_frame = true;
		player_y -= player_speed * graphics.GetElapsedTime();
    }

	if (next_frame)
	{
		if (time_next_frame > 0.1)
		{
			animation_index++;
			time_next_frame = 0;
		}
		if (animation_index > 3)
			animation_index = 0;
	}

	graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE, sprite[animation_clip][animation_index].x, sprite[animation_clip][animation_index].y, sprite[animation_clip][animation_index].width, sprite[animation_clip][animation_index].height, sprite_sheet);
}

void InitSprites()
{
	int x = 0, y = 0;
	int spritex = 0;
	int spritey = 0;
	for (int w = 0; w < 16; w++)
	{
		sprite[x][y].x = spritex;
		sprite[x][y].y = spritey;
		sprite[x][y].width = SPRITE_SIZE;
		sprite[x][y].height = SPRITE_SIZE;
		spritex += SPRITE_SIZE;
		y++;
		if (spritex >= 512)
		{		
			spritex = 0;
			spritey += SPRITE_SIZE;
			x++;
			y = 0;
		}
	}
}
 
int main(void)
{
    graphics.CreateMainWindow(800, 600, "Sprite Sheet Example");
	graphics.SetBackgroundColor(152,209,250);
	graphics.SetKeyboardInput(KeyboardInput);

	sprite_sheet.LoadPNGImage("Char_Sprite.png");
	InitSprites();
        
    graphics.SetMainLoop(MainLoop);
    graphics.StartMainLoop();
    return 0;
}