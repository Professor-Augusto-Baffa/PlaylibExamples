#include "..\Headers\Button.h"


Button::Button ()
{

};

Button::Button (button_type t, int id)
{
	type = t;
	state = idle;
	ID = id;
};

void Button::set_type(button_type t)
{
	type = t;
};

void Button::set_state(button_state s)
{
	state = s;
}
void Button::set_ID (int id)
{
	ID = id;
};

button_type Button::get_type()
{
	return type;
};

button_state Button::get_state()
{
	return state;
}
int Button::get_ID()
{
	return ID;
};

void Button::do_something()
{





	if (type == toggle)
		sheet_y = 0;
	else if (type == hold)
		sheet_y = 1;

	if (state == pressed)
		sheet_x = 1;
	else if (state == idle)
		sheet_x = 0;
};


void Button::copy_subclass_data (Sprite* s)
{
	((Button*)s)->set_type (type);
	((Button*)s)->set_state (state);
	((Button*)s)->set_ID (ID);
};

Button* Button::create_node (Sprite* s)
{
	Button* p = new Button;
	copy_base_data (p);
	copy_subclass_data (p);
	p->set_ptr (s);
	return p;
};