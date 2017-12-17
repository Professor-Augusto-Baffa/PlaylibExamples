#include "Sprite.h"

Sprite::Sprite()
{
	stop_box_x1 = -1;
	stop_box_y1 = -1;
	stop_box_x2 = -1;
	stop_box_y2 = -1;
	layer = -1;
	to_delete = false;
	is_generated = false;
};

Sprite::Sprite (int w, int h, int l)
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
	to_delete = false;
	is_generated = false;
};

Sprite::Sprite (int w, int h, int l, const char* p)
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
	to_delete = false;
	is_generated = false;
};

Sprite::Sprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y)
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
	to_delete = false;
	is_generated = false;
};

void Sprite::set_frame_w (int w)
{
	frame_w = w;
};

void Sprite::set_frame_h (int h)
{
	frame_h = h;
};

int Sprite::get_frame_w()
{
	return frame_w;
};

int Sprite::get_frame_h()
{
	return frame_h;
};

int Sprite::get_crop_x()
{
	return sheet_x * frame_w;
};

int Sprite::get_crop_y()
{
	return sheet_y * frame_h;
};

int Sprite::get_screen_x()
{
	return screen_x;
};

int Sprite::get_screen_y()
{
	return screen_y;
};

int Sprite::get_sheet_x()
{
	return sheet_x;
};

int Sprite::get_sheet_y()
{
	return sheet_y;
};

int Sprite::get_width()
{
	if (w_mod == 0)
		return frame_w;

	return w_mod;
};

int Sprite::get_height()
{
	if (h_mod == 0)
		return frame_h;

	return h_mod;
};

int Sprite::get_stop_box_x1()
{
	return stop_box_x1;
};

int Sprite::get_stop_box_y1()
{
	return stop_box_y1;
};

int Sprite::get_stop_box_x2()
{
	return stop_box_x2;
};

int Sprite::get_stop_box_y2()
{
	return stop_box_y2;
};

void Sprite::set_stop_box (int x1, int y1, int x2, int y2)
{
	stop_box_x1 = x1;
	stop_box_y1 = y1;
	stop_box_x2 = x2;
	stop_box_y2 = y2;
};

bool Sprite::stop_box_is_set()
{
	if (stop_box_x1 == -1 || stop_box_y2 == -1 || stop_box_x2 == -1 || stop_box_y2 == -1)
		return false;
	return true;
};

void Sprite::select_frame (int x, int y)
{
	sheet_x = x;
	sheet_y = y;
};

void Sprite::load (const char* sub_folder_path, char* file_name)
{
	cat_path (path, sub_folder_path, file_name);
	LoadPNGImage (path);
};

void Sprite::load_direct (char* file_path)
{
	LoadPNGImage (file_path);
};

void Sprite::print_path()
{
	printf("\nSprite path:\n\t%s\n", path);
};

void Sprite::print_pos ()
{
	printf ("(sheet_x = %d , sheet_y = %d , screen_x = %d , screen_y = %d , w = %d , h = %d)\n", sheet_x, sheet_y, screen_x, screen_y, frame_w, frame_h);
};

void Sprite::set_position (int x, int y)
{
	screen_x = x;
	screen_y = y;
};

void Sprite::draw (Graphics* g)
{
	int cx = sheet_x * frame_w;
	int cy = sheet_y * frame_h;

	g->DrawImage2D (screen_x, screen_y, frame_w, frame_h, cx, cy, frame_w, frame_h, *this);
};

void Sprite::draw (iGraphics* i)
{
	int cx = sheet_x * frame_w;
	int cy = sheet_y * frame_h;

	int dw = (w_mod == 0) ? frame_w : w_mod;
	int dh = (h_mod == 0) ? frame_h : h_mod;

	i->draw_image (screen_x, screen_y, dw, dh, cx, cy, frame_w, frame_h, *this);
};

void Sprite::draw_node (iGraphics* i)
{
	/*sprite->select_frame (sheet_x, sheet_y);
	sprite->set_position (screen_x, screen_y);
	sprite->set_modifiers (w_mod, h_mod);*/

	copy_base_data (sprite);
	copy_subclass_data (sprite);

	char* buffer = NULL;
	buffer = sprite->get_name (buffer);
	/*if (strcmp (buffer, "Spell") == 0 && sheet_x == 1) // Nayru's Love gira.
	{
		i->RotateBegin (0 % 360);
		sprite->draw (i);
		i->RotateEnd();
	}
	else*/
		sprite->draw (i);
};

void Sprite::print_sprite_file_name()
{
	printf ("%s", sprite->get_path());
};

Sprite* Sprite::insert_node_RECURSIVELY (Sprite* s)
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
				return (ptr->insert_node(s));
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

Sprite* Sprite::insert_node (Sprite* s)
{
	int sprite_L = s->get_layer();
	int sprite_y = s->get_screen_y() - s->get_frame_h()/2;
	int current_y;
	int current_L;
	Sprite* current = this;
			
	if (s == NULL)
	{
		printf ("Tried to insert a null sprite pointer to the list.\n");
		return NULL;
	};

	if (ptr == NULL)
	{
		ptr = s->create_node (NULL);
		return ptr;
	};

	while (1)
	{
		current_y = current->get_screen_y() - current->get_frame_h()/2;
		current_L = current->get_layer();

		if ((sprite_L > current_L) || ((sprite_L == current_L) && (sprite_y >= current_y)))
		{
			if (current->get_ptr() == NULL)
			{
				//ptr = new Sprite (s, NULL);
				current->set_ptr (s->create_node (NULL));
				return current->get_ptr();
			}
			else
			{
				int next_L = current->get_ptr()->get_layer();
				int next_y = current->get_ptr()->get_screen_y() - current->get_ptr()->get_frame_h()/2;
				if ((sprite_L > next_L) || ((sprite_L == next_L) && (sprite_y >= next_y)))
				{
					current = current->get_ptr();
				}
				else
				{
					current->set_ptr (s->create_node(current->get_ptr()));
					return current->get_ptr();
				};
			};
		}
		else
		{
			printf ("Tried to insert a sprite to the list in the wrong order.\n");
			return NULL;
		};
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

void Sprite::copy_base_data (Sprite* s)
{
	s->set_position (screen_x, screen_y);
	s->select_frame (sheet_x, sheet_y);
	s->set_frame_w (frame_w);
	s->set_frame_h (frame_h);
	s->set_modifiers (w_mod, h_mod);
	s->set_layer (layer);
	s->set_stop_box (stop_box_x1, stop_box_y1, stop_box_x2, stop_box_y2);
	s->set_to_delete (to_delete);
	s->set_generated (is_generated);
	//s->set_ptr (NULL);
	
	s->set_sprite (this);
};

void Sprite::move (int x, int y)
{
	static int last_step_right_leg = false;

	screen_x += x;
	screen_y += y;

	if (x > 0)
	{
		if (sheet_y != 3)
		{
			sheet_x = 0;
			sheet_y = 3;
		};

		if (sheet_y == 3)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	}
	else if (x < 0)
	{
		if (sheet_y != 1)
		{
			sheet_x = 0;
			sheet_y = 1;
		};

		if (sheet_y == 1)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	}
	else if (y > 0)
	{
		if (sheet_y != 0)
		{
			sheet_x = 0;
			sheet_y = 0;
		};

		if (sheet_y == 0)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		}
	}
	else if (y < 0)
	{
		if (sheet_y != 2)
		{
			sheet_x = 0;
			sheet_y = 2;
		};

		if (sheet_y == 2)
		{
			if (sheet_x == 0)
			{
				if (last_step_right_leg)
					sheet_x = 1;
				else
					sheet_x = 2;
				last_step_right_leg = !last_step_right_leg;
			}
			else
				sheet_x = 0;
		};
	};
};
		
void Sprite::move (int n, axis a)
{
	a == horizontal ? move(n,0) : move(0,n);
};

char* Sprite::get_path()
{
	return path;
};

char* Sprite::get_name (char* buffer)
{
	int last_backslash = 0;
	int explorer = 0;
	int buffer_i = 0;
	
	//delete buffer;
	buffer = new char[40];

	for (;;)
	{
		explorer++;
		if (path[explorer] == '\\')
		{
			last_backslash = explorer;
			continue;
		}
		else if (path[explorer] == '\0')
		{
			break;
		};
	};

	for (int path_i = last_backslash+1; path[path_i] != '.'; path_i++, buffer_i++)
	{
		buffer[buffer_i] = path[path_i];
	};
	buffer[buffer_i] = '\0';

	return buffer;
};

void Sprite::set_modifiers (int wm, int hm)
{
	w_mod = wm;
	h_mod = hm;
};

void Sprite::set_layer (int l)
{
	layer = l;
};

int Sprite::get_layer()
{
	return layer;
};

void Sprite::set_ptr (Sprite* p)
{
	ptr = p;
};

void Sprite::clear (bool only_generated)
{
	/*if (ptr != NULL)
	{
		ptr->clear();
		delete ptr;
		ptr = NULL;
	};*/


	Sprite* previous = this;
	Sprite* current = ptr;
	Sprite* next;
	
	while (current != NULL)
	{
		next = current->get_ptr();
			
		if ((!only_generated && current->get_to_delete()) || (only_generated && current->get_generated()))
		{
			delete current;
			current = next;
			previous->set_ptr (next);
		}
		else
		{
			previous = current;
			current = next;
		};
	};
};

void Sprite::print_node_line()
{
	if (layer == -1)
	{
		printf ("[head]\n");
	}
	else
	{
		char* buffer = NULL;
		buffer = sprite->get_name (buffer);

		printf ("[%s] @ %d : [%d, %d] [%d, %d] [%d x %d] [generated: %s] [delete: %s]\n", buffer, layer, sheet_x, sheet_y, screen_x, screen_y-frame_h/2, frame_w, frame_h, is_generated ? "yes" : "no", to_delete ? "yes" : "no");
	};

	if (ptr != NULL)
	{
		//ptr->print_node_line();
	}
	else
		printf ("[tail]\n\n");
};

void Sprite::print_whole_list()
{
	Sprite* current = ptr;

	while (current != NULL)
	{
		current->print_node_line();
		current = current->get_ptr();
	};
};

void Sprite::draw_list (iGraphics* i)
{
	/*if (ptr != NULL)
	{
		ptr->draw_node (i);
		ptr->draw_list (i);
	};*/

	Sprite* p = ptr;

	while (p != NULL)
	{
		p->draw_node (i);
		p = p->get_ptr();
	};
};


Sprite* Sprite::get_ptr()
{
	return ptr;
};

Sprite* Sprite::get_sprite()
{
	return sprite;
};

void Sprite::set_sprite (Sprite* s)
{
	sprite = s;
};

sort_condition Sprite::sort_next (Sprite* previous)
{
	while (previous->get_ptr() != this)
	{
		previous = previous->get_ptr();
	};

	if (ptr != NULL)
	{
		int next_layer = ptr->get_layer();
		int next_y = ptr->get_screen_y() - ptr->get_height()/2;
		int this_y = get_screen_y() - get_height() / 2;

		if (next_layer < layer || (next_layer == layer && next_y < this_y))
		{
			previous->set_ptr (ptr);
			ptr = ptr->get_ptr();
			previous->get_ptr()->set_ptr (this);
			return must_restart;
		}
		else
			return move_on;
	};

	return move_on;
};

void Sprite::sort_list()
{
	Sprite* current = ptr;
	Sprite* previous = this;

	sort_condition sc = move_on;
	while (current != NULL)
	{
		sc = current->sort_next (previous);
		
		if (sc == must_restart)
		{
			current = ptr;
			previous = this;
			sc = move_on;
		}
		else if (sc == move_on)
		{
			previous = current;
			current = current->get_ptr();
		};
	};
};

bool Sprite::get_to_delete()
{
	return to_delete;
};

void Sprite::set_to_delete (bool b)
{
	to_delete = b;
};

void Sprite::set_generated (bool b)
{
	is_generated = b;
};

bool Sprite::get_generated()
{
	return is_generated;
};