#ifndef StillSprite_H
#define StillSprite_H

#include "Sprite.h"

class StillSprite : public Sprite
{

	public:
		StillSprite();
		StillSprite (int w, int h, int l);
		StillSprite (int w, int h, int l, const char* p);
		StillSprite (int w, int h, int l, int sheet_X, int sheet_Y, int screen_X, int screen_Y);

		StillSprite* create_node (Sprite* s);
		void copy_subclass_data (Sprite* s) { };
		void set_door_direction (direction d) { };

		void StillSprite::do_something() { };
};


#endif