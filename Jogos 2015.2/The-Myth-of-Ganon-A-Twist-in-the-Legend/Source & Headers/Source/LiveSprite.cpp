#include "LiveSprite.h"

LiveSprite::LiveSprite()
{

};

LiveSprite::LiveSprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y)
{
	frame_w = w;
	frame_h = h;
	sheet_x = sheet_X;
	sheet_y = sheet_Y;
	screen_x = screen_X;
	screen_y = screen_Y;

	walk_speed = 6;
	dash_speed = 12;
	dashing = false;

	for (int i=0; i<4; i++)
		move_restriction[i] = -1;

	layer = l;
	to_delete = true;
};

LiveSprite* LiveSprite::create_node (Sprite* s)
{
	LiveSprite* p = new LiveSprite;
	copy_base_data (p);
	copy_subclass_data (p);
	p->set_ptr (s);
	return p;
};

void LiveSprite::copy_subclass_data (Sprite* s)
{
	((LiveSprite*) s)->set_dashing (dashing);
	for (int i=0; i<4; i++)
		((LiveSprite*) s)->set_move_restriction ((direction) i, move_restriction[i]);
};

void LiveSprite::move_d (direction d)
{
	int s = get_current_speed();

	switch (d)
	{
		case up:
			if (move_restriction[up] == -1)
			{
				move (0, -s);
			}
			else
			{
				move (0, -move_restriction[up]);
				select_frame (0, 2);
				move_restriction[up] = 0;
			};
				break;

		case right:
			if (move_restriction[right] == -1)
			{
				move (s, 0);
			}
			else
			{
				move (move_restriction[right], 0);
				select_frame (0, 3);
				move_restriction[right] = 0;
			};
			break;

		case down:
			if (move_restriction[down] == -1)
			{
				move (0, s);
			}
			else
			{
				move (0, move_restriction[down]);
				select_frame (0, 0);
				move_restriction[down] = 0;
			};
			break;

		case left:
			if (move_restriction[left] == -1)
			{
				move (-s, 0);
			}
			else
			{
				move (-move_restriction[left], 0);
				select_frame (0, 1);
				move_restriction[left] = 0;
			};
			break;
	};
};

void LiveSprite::set_dashing (bool d)
{
	dashing = d;
};

void LiveSprite::toggle_dashing()
{
	dashing = !dashing;
};

bool LiveSprite::is_dashing()
{
	return dashing;
};

int LiveSprite::get_current_speed()
{
	return (dashing ? dash_speed : walk_speed);
};

//void LiveSprite::set_allowed_to_move (direction d, bool b)
//{
//	allowed_to_move[d] = b;
//};

void LiveSprite::set_move_restriction (direction d, int r)
{
	move_restriction[d] = r;
};
