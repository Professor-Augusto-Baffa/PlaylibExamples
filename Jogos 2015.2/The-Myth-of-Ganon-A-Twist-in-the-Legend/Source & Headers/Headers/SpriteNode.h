#ifndef SpriteNode_H
#define SpriteNode_H

#include "LiveSprite.h"

class SpriteNode : public Sprite
{
	private:

		
	public:

		SpriteNode();
		SpriteNode (Sprite* s, SpriteNode* p);
	
		void set_door_direction (direction d);
};


#endif