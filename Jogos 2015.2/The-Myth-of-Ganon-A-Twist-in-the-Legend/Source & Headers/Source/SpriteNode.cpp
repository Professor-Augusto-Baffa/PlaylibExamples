#include "Sprite.h"

Sprite::Sprite()
{
	ptr = NULL;
	sprite = NULL;
	layer = -1;
	w_mod = 0;
	h_mod = 0;

	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;

	interact_x1 = -1;
	interact_y1 = -1;
	interact_x2 = -1;
	interact_y2 = -1;
};

Sprite::Sprite (Sprite* s, Sprite* p)
{
	if (s == NULL && s->get_layer() != -1)
	{
		printf ("Trying to construct a Sprite with a null sprite pointer.\n\n");
		return;
	};

	sprite = s;
	sheet_x = s->get_crop_x() / s->get_frame_w();
	sheet_y = s->get_crop_y() / s->get_frame_h();
	screen_x = s->get_screen_x();
	screen_y = s->get_screen_y();
	frame_w = s->get_frame_w();
	frame_h = s->get_frame_h();
	layer = s->get_layer();
	ptr = p;
	w_mod = s->get_width();
	h_mod = s->get_height();

	stop_box_x1 = s->get_stop_box_x1();
	stop_box_y1 = s->get_stop_box_y1();
	stop_box_x2 = s->get_stop_box_x2();
	stop_box_y2 = s->get_stop_box_y2();
};


void Sprite::set_ptr (Sprite* p)
{
	ptr = p;
};

void Sprite::set_sprite (Sprite* s)
{
	sprite = s;
};

Sprite* Sprite::insert_node (Sprite* s)
{
	int l = s->get_layer();
	int s_y = s->get_screen_y() - s->get_frame_h()/2;
	int y = screen_y - frame_h/2;
			
	if (s == NULL)
	{
		printf ("Tried to insert a null sprite pointer to the list.\n");
		return NULL;
	};

	if ((l > layer) || (l == layer) && (s_y >= y))
	{
		if (ptr == NULL)
		{
			//ptr = new Sprite (s, NULL);
			ptr = s->create_node (ptr);
			return ptr;
		}
		else
		{
			int p_y = ptr->get_screen_y() - ptr->get_frame_h()/2;

			if ((l > ptr->get_layer()) || (l == ptr->get_layer()) && (s_y >= p_y))
			{
				//ptr->insert_node (s, l);
				ptr = s->create_node (ptr);
				return ptr;
			}
			else
			{
				//Sprite* temp = ptr;
				//ptr = new Sprite (s, temp);
				ptr = s->create_node (ptr);
				return ptr;
			};
		};
	}
	else
	{
		printf ("Tried to insert a sprite to the list in the wrong order.\n");
		return NULL;
	};
};

bool Sprite::check_stop_box_collision (direction d, int n1, int n2)
{
	int gx1 = get_screen_x() + get_stop_box_x1();
	int gy1 = get_screen_y() - get_stop_box_y1() + 2;
	int gx2 = get_screen_x() + get_stop_box_x2() - 1;
	int gy2 = get_screen_y() - get_stop_box_y2() + 1;
	
	int gx_r = max (gx1, gx2);
	int gx_l = min (gx1, gx2);
	int gy_d = max (gy1, gy2);
	int gy_u = min (gy1, gy2);

	int n_min = min (n1, n2);
	int n_max = max (n1, n2);

	switch (d)
	{
		case left: case right:
			if (between (n_min, gy_u, n_max) ||
				between (gy_u, n_min, gy_d) ||
				between (n_min, gy_d, n_max) ||
				between (gy_u, n_max, gy_d))
				return true;
			break;

		case up: case down:
			if (between (n_min, gx_l, n_max) ||
				between (gx_l, n_min, gx_r) ||
				between (n_min, gx_r, n_max) ||
				between (gx_l, n_max, gx_r))
				return true;
			break;

		default: return false;
	};
};

void Sprite::clear()
{
	if (ptr != NULL)
	{
		ptr->clear();
		delete ptr;
		ptr = NULL;
	};
};

void Sprite::draw_list (iGraphics* i)
{
	if (ptr != NULL)
	{
		ptr->draw (i);
		ptr->draw_list (i);
	};
};

void Sprite::draw (iGraphics* i)
{
	/*sprite->select_frame (sheet_x, sheet_y);
	sprite->set_position (screen_x, screen_y);
	sprite->set_modifiers (w_mod, h_mod);*/
	
	copy_base_data (sprite);
	copy_subclass_data (sprite);
	
	char* buffer = NULL;
	buffer = sprite->get_name (buffer);
	if (strcmp (buffer, "Spell") == 0 && sheet_x == 1) // Nayru's Love
	{
		i->RotateBegin (0 % 360);
		sprite->draw (i);
		i->RotateEnd();
	}
	else
		sprite->draw (i);
};

Sprite* Sprite::get_ptr()
{
	return ptr;
};

Sprite* Sprite::get_sprite()
{
	return sprite;
};

void Sprite::print_node()
{
	if (layer == -1)
		printf ("[L | x, y | w, h]\n[head] -> ");
	else
		printf ("[%d | %d, %d | %d, %d] -> ", layer, screen_x, screen_y+frame_h/2, frame_w, frame_h);


	if (ptr != NULL)
		ptr->print_node();
	else
		printf ("[tail]\n\n");
};





void Sprite::set_door_direction (direction d)
{

};