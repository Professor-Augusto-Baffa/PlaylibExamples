#ifndef LiveSprite_H
#define LiveSprite_H

#include "Sprite.h"

class LiveSprite : public Sprite
{
private:
	int walk_speed;
	int dash_speed;
	bool dashing;
	//bool allowed_to_move[4];
	int move_restriction[4];

public:
	LiveSprite();
	LiveSprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y);
	LiveSprite* create_node (Sprite* s);

	void set_dashing (bool dash);
	void move_d (direction d);
	void toggle_dashing();
	bool is_dashing();
	int get_current_speed();
	//void set_allowed_to_move (direction d, bool b);
	void set_move_restriction (direction d, int r);
	void set_door_direction (direction d) { };

	void copy_subclass_data (Sprite* s);
	void LiveSprite::do_something() { };
};

#endif