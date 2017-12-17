// 1.1

#ifndef __Sprite__
#define __Sprite__

#include "Defines.h"
#include "Graphics.h"
#include "iGraphics.h"
using namespace PlayLib;

enum axis
{
	horizontal,
	vertical
};

enum direction
{
	right,
	down,
	left,
	up,
};

enum sort_condition
{
	move_on,
	must_restart
};

class Sprite : public Image
{
	protected:
		int screen_x;
		int screen_y;
		int sheet_x;
		int sheet_y;
		int frame_w;
		int frame_h;
		int w_mod;
		int h_mod;
		int layer;
		int stop_box_x1;
		int stop_box_y1;
		int stop_box_x2;
		int stop_box_y2;
		int interact_x1;
		int interact_y1;
		int interact_x2;
		int interact_y2;
		char path[FILE_PATH_SIZE];
		bool to_delete;
		bool is_generated;
		
		Sprite* ptr;
		Sprite* sprite;


	public:
		Sprite ();
		Sprite (int w, int h, int l, const char* p);
		Sprite (int w, int h, int l);
		Sprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y);
		Sprite (Sprite* s, Sprite* p);


		void load (const char* sub_folder_path, char* file_name);
		void load_direct (char* file_path);
		void set_position (int x, int y);
		void draw (Graphics* g);
		void draw (iGraphics* i);
		void set_frame_w (int w);
		void set_frame_h (int h);
		int get_frame_w();
		int get_frame_h();
		int get_crop_x();
		int get_crop_y();
		int get_screen_x();
		int get_screen_y();
		int get_sheet_x();
		int get_sheet_y();
		int get_width();
		int get_height();
		int get_stop_box_x1();
		int get_stop_box_y1();
		int get_stop_box_x2();
		int get_stop_box_y2();
		bool get_to_delete();
		bool get_generated();

		void set_to_delete (bool b);
		void set_generated (bool b);
		bool stop_box_is_set();
		void set_stop_box (int x1, int y1, int x2, int y2);
		void select_frame (int x, int y);
		void set_modifiers (int wm, int hm);
		void print_path();
		void print_pos ();
		void move (int x, int y);
		void move (int n, axis a);
		char* get_path();
		char* get_name (char* buffer);
		void set_layer (int l);
		int get_layer();

		void copy_base_data (Sprite* s);
		virtual void copy_subclass_data (Sprite* s) = 0;
		virtual void set_door_direction (direction d) = 0;

		virtual void check_lock (int x1, int y1, int x2, int y2, int* k, bool v, iGraphics* iGraph) { };





		Sprite* get_ptr();
		Sprite* get_sprite();

		void set_ptr (Sprite* p);
		void set_sprite (Sprite* s);

		Sprite* insert_node_RECURSIVELY (Sprite* s);
		Sprite* insert_node (Sprite* s);
		virtual Sprite* create_node (Sprite* s) = 0;
		void clear (bool only_generated);
		void draw_list (iGraphics* i);
		void draw_node (iGraphics* i);
		sort_condition sort_next (Sprite* previous);
		void sort_list();

		void print_sprite_file_name();
		void print_node_line();
		void print_whole_list();

		bool check_stop_box_collision (direction d, int n1, int n2);

		virtual void do_something() = 0;
};

#endif