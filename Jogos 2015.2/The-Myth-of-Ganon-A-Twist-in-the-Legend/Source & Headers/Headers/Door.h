#ifndef Door_H
#define Door_H

#include "Sprite.h"

enum door_state
{
	open,
	locked,
	barred
};

class Door : public Sprite
{
	private:
		door_state state;
		direction dir;
		int contact_x1;
		int contact_y1;
		int contact_x2;
		int contact_y2;
		int ID;

	public:
		Door (int w, int h, int x, int y, direction d, door_state s);
		Door ();
		Door* create_node (Sprite* s);

		void update_stop_box();

		door_state get_state();
		void set_state (door_state s);
		void check_lock (int x1, int y1, int x2, int y2, int *k, bool v, iGraphics* iGraph);
		void set_door_direction (direction d);
		void copy_subclass_data (Sprite* s);
		direction get_direction();
		int get_ID();
		void set_ID (int id);

		void Door::do_something() { };
};

#endif