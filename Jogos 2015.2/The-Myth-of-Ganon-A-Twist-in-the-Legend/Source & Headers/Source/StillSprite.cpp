#include "StillSprite.h"

StillSprite::StillSprite()
{
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;

	layer = -1;
};

StillSprite::StillSprite (int w, int h, int l)
{
	frame_w = w;
	frame_h = h;
	w_mod = 0;
	h_mod = 0;

	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;

	layer = l;
};

StillSprite::StillSprite (int w, int h, int l, const char* p)
{
	frame_w = w;
	frame_h = h;
	int count = 0;

	for (int i=0; p[i]!='\0'; i++)
		path[i] = p[i];

	w_mod = 0;
	h_mod = 0;
	
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;

	layer = l;
};

StillSprite::StillSprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y)
{
	frame_w = w;
	frame_h = h;
	sheet_x = sheet_X;
	sheet_y = sheet_Y;
	screen_x = screen_X;
	screen_y = screen_Y;
	w_mod = 0;
	h_mod = 0;
	
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;

	layer = l;
};

StillSprite* StillSprite::create_node (Sprite* s)
{
	StillSprite* p = new StillSprite;
	copy_base_data (p);
	copy_subclass_data (p);
	p->set_ptr (s);
	return p;
};
