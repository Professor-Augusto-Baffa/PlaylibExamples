#pragma once

#include "Door.h"

enum button_type
{
	toggle,
	hold
};

enum button_state
{
	pressed,
	idle
};

class Button : public Sprite
{
	private:

		button_type type;
		button_state state;
		int ID;

	public:

		Button();
		Button (button_type t, int id);

		void set_type (button_type t);
		void set_state (button_state s);
		void set_ID (int id);

		button_type get_type();
		button_state get_state();
		int get_ID();
		void do_something();


		void set_door_direction (direction d) { };
		void copy_subclass_data (Sprite* s);

		Button* create_node (Sprite* s);
};

