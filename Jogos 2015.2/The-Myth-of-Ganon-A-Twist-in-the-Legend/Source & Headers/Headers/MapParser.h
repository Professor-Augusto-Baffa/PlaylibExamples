#ifndef __MapParser__
#define __MapParser__

#include "MapNode.h"
#include "StillSprite.h"
#include "Door.h"
#include "Button.h"

class MapParser
{
	private:

		Sprite* list_head;
		Sprite* map[7]; /*[6] = Ganondorf's Castle*/
		Sprite* working_map;
		Sprite* last_added_tile;
		Door* door;
		Button* button;
		bool keep_last_added_tile;
		int current_layer;
		StillSprite buffer_sprite;
		

	public:
	
		MapParser (Sprite* head, Sprite* forest, Sprite* fire, Sprite* water, Sprite* spirit, Sprite* shadow, Sprite* light, Sprite* home, Door* door_ptr, Button* button_ptr);

		void parse();

		void Tile		(int tx, int ty, int x, int y);
		void Rect		(int tx, int ty, int x, int y, int w, int h);
		void Row		(int tx, int ty, int x, int y, int w);
		void Col		(int tx, int ty, int x, int y, int h);
		void As_is		(int tx, int ty, int x, int y, int w, int h);
		void Stretch3	(int tx, int ty, int x, int y, int w, int h);
		void Stretch4	(int tx, int ty, int x, int y, int w, int h);
		void set_working_map (int n);
		void Assign_file (int n, char* c);
		void Stop (int x, int y, int w, int h);
		void Layer (int l);
		void PutDoor (door_state s, direction d, int x, int y, int id);
		void PutButton (button_type t, int x, int y, int id);
		int p_atoi (char* c);

		Sprite* get_working_map();
};

#endif /* defined(__MapParser__) */


