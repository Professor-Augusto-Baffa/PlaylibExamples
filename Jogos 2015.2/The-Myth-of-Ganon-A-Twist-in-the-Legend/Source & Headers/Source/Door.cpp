#include "Door.h"

Door::Door ()
{

};

Door::Door (int w, int h, int x, int y, direction d, door_state s)
{
	frame_w = w;
	frame_h = h;
	screen_x = x;
	screen_y = y;
	state = s;
	ID = -1;

	set_door_direction (d);
};

Door* Door::create_node (Sprite* s)
{
	Door* p = new Door;
	copy_base_data (p);
	copy_subclass_data (p);
	p->set_ptr (s);
	return p;
};

void Door::update_stop_box()
{
	if (state==locked || state==barred)
	{
		switch (dir)
		{
			case up: case down:
				set_stop_box (0, 0, get_width(), get_height());
				break;
			case right: case left:
				set_stop_box (0, 0, 39, get_height());
				break;
		};
	}
	else
	{
		set_stop_box (-1, -1, -1, -1);
	};
};

door_state Door::get_state()
{
	return state;
};

void Door::set_state (door_state s)
{
	state = s;
	set_door_direction (dir);
};

void Door::check_lock (int x1, int y1, int x2, int y2, int *k, bool v, iGraphics* iGraph)
{
	//printf ("Door::check_lock\t%(%d, %d) (%d, %d) | %d | %d\n", x1, y1, x2, y2, *k, v);

	int gap = 20;
	int offset = 39;

	switch (dir)
	{
		case up:
			contact_x1 = screen_x + gap;
			contact_y1 = screen_y + 1;
			contact_x2 = screen_x + get_width() - gap;
			contact_y2 = screen_y + 3;
			break;

		case right:
			contact_x1 = screen_x - 1;
			contact_y1 = screen_y - get_height() + gap;
			contact_x2 = screen_x - 3;
			contact_y2 = screen_y - gap;
			break;
		
		case down:
			contact_x1 = screen_x + get_width() - gap;
			contact_y1 = screen_y - get_height() - 1;
			contact_x2 = screen_x + gap;
			contact_y2 = screen_y - get_height() - 3;
			break;

		case left:
			contact_x1 = screen_x + offset + 1;
			contact_y1 = screen_y - gap;
			contact_x2 = screen_x + offset + 3;
			contact_y2 = screen_y - get_height() + gap;
			break;

		default: ;
	};

	if (v && state==locked)
	{
		iGraph->SetColor (0, 255, 120);
		iGraph->draw_rectangle (contact_x1, contact_y1, contact_x2, contact_y2);
	};
};

void Door::set_door_direction (direction d)
{
	dir = d;

	switch (dir)
	{
		case up: sheet_x = 0; break;
		case left: sheet_x = 1; break;
		case right: sheet_x = 2; break;
		default: ;
	};

	switch (state)
	{
		case open: sheet_y = 0; break;
		case barred: sheet_y = 1; break;
		case locked: sheet_y = 2; break;
		default: ;
	};
};

direction Door::get_direction()
{
	return dir;
}
int Door::get_ID()
{
	return ID;
}
void Door::set_ID(int id)
{
	ID = id;
};

void Door::copy_subclass_data (Sprite* s)
{
	((Door*) s)->set_door_direction (dir);
	((Door*) s)->set_state (state);
	((Door*) s)->set_ID (ID);

};