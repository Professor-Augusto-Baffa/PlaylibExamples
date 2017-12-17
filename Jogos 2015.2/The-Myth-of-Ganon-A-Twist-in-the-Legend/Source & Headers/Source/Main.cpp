#ifndef __Main__
#define __Main__

#include "SaveState.h"
#include "LiveSprite.h"
#include "iGraphics.h"
#include "MapParser.h"
// #include "Sprite.h"
#include "StillSprite.h"
#include "Door.h"
#include "Button.h"
#include <math.h>

SaveState save_state;
iGraphics iGraph;
StillSprite Ganondorfs_castle (736, 448, 0);
LiveSprite Ganondorf (40, 40, 5, 1, 0, 80, BIG_SCREEN ? 700 : 190);
Sprite* G_ptr = NULL;

StillSprite portal (337, 270, 1);
StillSprite medallion (50, 50, 1);
StillSprite easter_egg (83, 58, 5); //49, 51
StillSprite Epona (48, 31, 5);
StillSprite heart (22, 19, 10);
StillSprite rupee (21, 24, 10);
StillSprite mp (1, 12, 10);
StillSprite keyboard (56, 49, 10);
StillSprite keyboard_sp (168, 49, 10);
StillSprite box (260, 260, 5);
//StillSprite door (58, 63);
StillSprite key (144, 297, 10);
StillSprite statue (48, 96, 5);

bool keys_visible = true;
bool bounding_box_visible = false;
bool stop_box_visible = false;

StillSprite spell (260, 260, 6);

StillSprite dins_fire (260, 260, 6);
StillSprite nayrus_love (260, 260, 6);
StillSprite farores_wind (260, 260, 6);
StillSprite f_wind_warpball (260, 260, 6);
StillSprite warp (260, 260, 6);

Door door (58, 63, 0, 0, up, locked);
Button button (toggle, -1);

void scan_virtual_keyboard();
void cast_Dins_Fire();
void cast_Nayrus_Love();
void cast_Farores_Wind();
bool is_casting();
int FW_x = -1;
int FW_y = -1;
int FW_stretch = 0;
int warping_animation = 0;
bool Ganondorf_visible = true;
bool keyboard_key[256];
int current_map = 0;
int keys_held = 0;
void display_bounding_boxes();
void display_stop_boxes();
void check_stop_boxes();
void check_locked_doors();
bool Ganondorf_allowed_to_move[4] = { true, true, true, true };
bool print_list = false;
bool fullscreen = false;

int portal_x (int i);
int easter_egg_x (int i);

StillSprite sprite_list_head;
StillSprite forest_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite fire_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite water_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite spirit_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite shadow_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite light_map (TILE_SIZE, TILE_SIZE, 3);
StillSprite home_map (TILE_SIZE, TILE_SIZE, 3);
MapParser map_parser (&sprite_list_head, &forest_map, &fire_map, &water_map, &spirit_map, &shadow_map, &light_map, &home_map, &door, &button);

bool has_generated_map = false;
bool see_generated_map = true;

void load_images()
{
//	char Ganondorfs_castle_path[FILE_PATH_SIZE];
//	//cat_path (Ganondorfs_castle_path, "Level Design\\Casa do Ganondorf\\", "Casa do Ganondorf.png");
//	cat_path (Ganondorfs_castle_path, CO, "Castle with organ and statues.png");
//	Ganondorfs_castle.LoadPNGImage (Ganondorfs_castle_path);
//

	Ganondorfs_castle.load (TR, "tile_mockup.png");
	Ganondorfs_castle.set_position (0, 0);
	Ganondorfs_castle.select_frame (0, 0);
	

	Ganondorf.load	(CO, "Ganondorf.png");
	Ganondorf.select_frame (0, 2);
	Ganondorf.set_position (SCREEN_WIDTH/2 - Ganondorf.get_width(), BIG_SCREEN ? 716 : 345);
	portal.load		(CO, "Portal.png");
	medallion.load	(CO, "Medallion.png");
	easter_egg.load (CO, "Easter Egg.png");
	forest_map.load	(TR, "Ganondorf\'s Castle Tileset.png");
	heart.load		(CO, "Heart.png");
	rupee.load		(CO, "Rupee.png");
	mp.load			(CO, "Mana bar.png");
	spell.load		(CO, "Spell.png");
	dins_fire.load	(CO, "Spell.png");
	nayrus_love.load(CO, "Spell.png");
	farores_wind.load(CO, "Spell.png");
	f_wind_warpball.load (CO, "Spell.png");
	warp.load		(CO, "Spell.png");
	keyboard.load	(CO, "Keys with spells.png");
	keyboard_sp.load(CO, "Keys with spells.png");
	box.load		(CO, "White Square.png");
	door.load		(CO, "Door.png");
	key.load		(CO, "Key.png");

	statue.load		(CO, "Statue.png");
	statue.set_stop_box (6, 32, 40, 5);

	Epona.load		(CO, "Epona.png");
	Epona.select_frame (1, 0);
	Epona.set_position (360, BIG_SCREEN ? 700 : 230);

	door.load (CO, "Door.png");
	button.load (TR, "Button.png");

	water_map.load (TR, "Água3.png");


	//map_parser.parse();

	iGraph.SetFullscreen (false);
};

void main_loop()
{
	//if (current_map == 0)
		//iGraph.DrawImage2D (0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,Ganondorfs_castle);


	








	//Ganondorf.draw (&iGraph);
	//Ganondorf.print_pos();
	if (Ganondorf_visible)
	{
		Ganondorf.set_stop_box (10, 3, 30, 13);
		Ganondorf.set_layer (5);
		G_ptr = sprite_list_head.insert_node (&Ganondorf);
		G_ptr->set_to_delete (true);
	};







	int portal_y = BIG_SCREEN ? 400 : 280;
	int medallion_y = portal_y - 60;

	Sprite* current_portal;
	
	for (int i=0; i<5; i++)
	{
		portal.select_frame (i,0);
		portal.set_position (portal_x(i), portal_y);
		portal.set_layer (portal.get_sheet_x() == 2 ? 4 : 5); // Water is an exception.
		portal.set_to_delete (true);

		switch (i)
		{
			case 0:	portal.set_stop_box (140, 110, 200, 120); break;
			case 1:	portal.set_stop_box (140, 120, 200, 150); break;
			case 2: portal.set_stop_box (-1, -1, -1, -1); break;
			case 3: portal.set_stop_box (160, 115, 187, 140); break;
			case 4: portal.set_stop_box (140, 115, 205, 140); break;
		};
		
		

		sprite_list_head.insert_node (&portal);
	};

	
	portal.set_position (portal_x(5), portal_y);
	switch (save_state.get_phase())
	{
		case 0: 
			portal.select_frame (5, 0);
			portal.set_layer (5);
			portal.set_to_delete (true);
			current_portal = sprite_list_head.insert_node (&portal);
		break;

		case 1: 
			portal.select_frame (7, 0);
			portal.set_layer (5);
			portal.set_to_delete (true);
			sprite_list_head.insert_node (&portal);
			portal.select_frame (8, 0);
			portal.set_layer (6);
			portal.set_to_delete (true);
			sprite_list_head.insert_node (&portal);
			break;

		case 2:
			portal.select_frame (6, 0);
			portal.set_layer (5);
			portal.set_to_delete (true);
			sprite_list_head.insert_node (&portal);
			break;
	};
		

	for (int i=0; i<6; i++)
	{
		if (save_state.get_temple(i))
		{
			medallion.select_frame (i, 0);
			medallion.set_position (portal_x(i) + 146, medallion_y);
			medallion.set_layer (1);
			medallion.set_to_delete (true);
			sprite_list_head.insert_node (&medallion);
		};
	};








	for (int i=0; i<6; i++)
		if (save_state.get_easter_egg(i))
		{
			easter_egg.select_frame (i, 0);
			//easter_egg.set_position (90 + (SCREEN_WIDTH / 2) - ((3-i) * 109) - (30 * (3-i>0)), 414);
			easter_egg.set_position (easter_egg_x (i), 700);
			easter_egg.set_layer (5);
			easter_egg.set_to_delete (true);
			sprite_list_head.insert_node (&easter_egg);
		};
		

	if (save_state.light())
	{
		int i = 5;
		easter_egg.select_frame (i+1, 0);
		//easter_egg.set_position (605, 352);
		easter_egg.set_position (easter_egg_x (5), 700-62);
		easter_egg.set_layer (5);
		easter_egg.set_to_delete (true);
		sprite_list_head.insert_node (&easter_egg);
	};








	int statue_padding = 50;
	int statue_y =  BIG_SCREEN ? 700 : 350;
	statue.select_frame (0, 0);
	statue.set_position (SCREEN_WIDTH/2 - statue.get_width() - statue_padding, statue_y);
	statue.set_layer (5);
	statue.set_to_delete (true);
	sprite_list_head.insert_node (&statue);
	statue.set_position (SCREEN_WIDTH/2 - statue.get_width() + statue_padding, statue_y);
	statue.set_layer (5);
	statue.set_to_delete (true);
	sprite_list_head.insert_node (&statue);
	





	Epona.set_position ((SCREEN_WIDTH - Epona.get_frame_w()) / 2 - 12, 345);
	if (Ganondorf.get_screen_x() + Ganondorf.get_frame_w()/2 > Epona.get_screen_x() + Epona.get_frame_w()/2)
		Epona.select_frame (1, 0);
	if (Ganondorf.get_screen_x() + Ganondorf.get_frame_w()/2 < Epona.get_screen_x() + Epona.get_frame_w()/2)
		Epona.select_frame (0, 0);
	Epona.set_layer (5);
	Epona.set_to_delete (true);
	sprite_list_head.insert_node (&Epona);





	float hearts = save_state.get_hearts();
	int whole_hearts = floor (hearts);
	float fraction = hearts - whole_hearts;
	int heart_pad = 21;
	int heart_y_1 = BIG_SCREEN ? -60 : 20;
	int heart_y_2 = heart_y_1 + 20;

	for (int i=0; i<20; i++)
	{
		if (i < 10)				heart.set_position (heart_pad*i + 20, heart_y_1);
		else					heart.set_position (heart_pad*(i-10) + 20, heart_y_2);
		

		if (i < whole_hearts)				heart.select_frame (0, 0);
		else if (i == whole_hearts)
		{
			if (fraction == 0.0f)			heart.select_frame (4, 0);
			else if (fraction == 0.25f)		heart.select_frame (3, 0);
			else if (fraction == 0.50f)		heart.select_frame (2, 0);
			else if (fraction == 0.75f)		heart.select_frame (1, 0);
		}
		else if (i > whole_hearts)			heart.select_frame (4, 0);
	
		if (i+1 <= save_state.get_heart_containers())
		{
			heart.set_layer (10);
			heart.set_to_delete (true);
			sprite_list_head.insert_node (&heart);
		};
	};




	int mp_y_1 = heart_y_1 + 19;
	int mp_y_2 = mp_y_1 + 19;

	for (int i=0; i<save_state.get_max_mp(); i++)
	{
		if (i==0)
			mp.select_frame (0, 0);
		else if (i<save_state.get_max_mp()-1)
		{
			if (save_state.get_mp() >= i)
				mp.select_frame (1,0);
			else
				mp.select_frame (2,0);
		}
		else
			mp.select_frame (3,0);

		mp.set_position (24 + i, 
			save_state.get_heart_containers() > 10 ? mp_y_2 : mp_y_1);

		mp.set_layer (10);
		mp.set_to_delete (true);
		sprite_list_head.insert_node (&mp);
	};







	


	





	int casting_Dins_Fire = save_state.get_DF();
	int casting_Nayrus_Love = save_state.get_NL();
	int casting_Farores_Wind = save_state.get_FW();

	if (casting_Dins_Fire > 0)
	{
		int growth_rate = 2;
		if (casting_Dins_Fire > 240/2)
			growth_rate = 4;
		if (casting_Dins_Fire > 240 * 3/4)
			growth_rate = 6;
		dins_fire.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-dins_fire.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+dins_fire.get_height()/2);
		dins_fire.select_frame (0, 0);
		dins_fire.set_layer (6);
		dins_fire.set_to_delete (true);
		dins_fire.set_modifiers (casting_Dins_Fire, casting_Dins_Fire);
		sprite_list_head.insert_node (&dins_fire);
		save_state.alter_DF (growth_rate);
		if (casting_Dins_Fire >= 240)
			save_state.set_DF (0);
	};


	
	if (casting_Nayrus_Love > 0)
	{
		static int angle = 0;
		nayrus_love.set_modifiers (70, 70);
		nayrus_love.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-nayrus_love.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+nayrus_love.get_height()/2);
		nayrus_love.select_frame (1, 0);
		nayrus_love.set_layer (6);
		nayrus_love.set_to_delete (true);
		sprite_list_head.insert_node (&nayrus_love);
		Sprite* p;
		for (p=&sprite_list_head; p->get_sprite()!=(&nayrus_love); p=p->get_ptr());
		p->set_modifiers (70, 70);
		save_state.alter_NL (-1);
	};


	if (casting_Farores_Wind > 0)
	{
		if (casting_Farores_Wind < 260)
			FW_stretch = casting_Farores_Wind;
		else
			FW_stretch = 260;

		int growth_rate = 10;
		int FW_limit = 380;
		if (casting_Farores_Wind > FW_limit/2)
			growth_rate = 1;
		farores_wind.set_position (Ganondorf.get_screen_x()+Ganondorf.get_width()/2-farores_wind.get_width()/2, Ganondorf.get_screen_y()-Ganondorf.get_height()/2+farores_wind.get_height()/2 + 15);
		farores_wind.select_frame (2, 0);
		farores_wind.set_layer (6);
		farores_wind.set_to_delete (true);
		farores_wind.set_to_delete (true);		
		nayrus_love.set_modifiers (FW_stretch, FW_stretch);
		save_state.alter_FW (growth_rate);
		sprite_list_head.insert_node (&farores_wind);
		if (casting_Farores_Wind >= FW_limit)
			save_state.set_FW (0);
	};


	if (FW_x != -1)
	{
		warp.select_frame (2, 1);
		warp.set_position (
			FW_x + Ganondorf.get_frame_w()/2 - warp.get_frame_w()/2,
			FW_y - Ganondorf.get_frame_h() + warp.get_frame_h()/2 + 20
			);
		if (casting_Farores_Wind > 50 || casting_Farores_Wind == 0)
		{
			warp.set_layer (7);
			warp.set_to_delete (true);
			sprite_list_head.insert_node (&warp);
		};
	};
	


	



	int spell_keys_y = heart_y_1;

	if (keys_visible)
	{

		//Farore's Wind
		keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*3 - 10, spell_keys_y + keyboard.get_height());
		keyboard.select_frame (0, 0);
		keyboard.set_layer (10);
		keyboard.set_to_delete (true);
		sprite_list_head.insert_node (&keyboard);

		// Nayru's Love
		keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*2 - 10, spell_keys_y + keyboard.get_height());
		keyboard.select_frame (1, 0);
		sprite_list_head.insert_node (&keyboard);

		// Din's Fire
		keyboard.set_position (SCREEN_WIDTH - keyboard.get_width()*1 - 10, spell_keys_y + keyboard.get_height());
		keyboard.select_frame (2, 0);
		sprite_list_head.insert_node (&keyboard);

		// Shift
		/*keyboard_sp.set_position (SCREEN_WIDTH - keyboard.get_width()*3 - 10, 10 + keyboard_sp.get_height()*2 + 5);
		keyboard_sp.select_frame (0, 3);
		sprite_list_head.insert_node (&keyboard_sp, 10);*/

		// Tab
		/*keyboard_sp.set_position (SCREEN_WIDTH - keyboard.get_width()*6 - 10, 10 + keyboard_sp.get_height());
		keyboard_sp.select_frame (0, 4);
		sprite_list_head.insert_node (&keyboard_sp, 10);*/
	};


	
	/*Sprite* door_ptr = NULL;
	door.set_position (SCREEN_WIDTH/2 - door.get_width()/2, 48);
	door.set_door_direction (up);
	door.set_state (open);
	door.update_stop_box();
	door.check_lock (0, 0, 0, 0, &keys_held, keys_visible, &iGraph);
	door.set_layer (2);
	door.set_to_delete (true);
	door_ptr = sprite_list_head.insert_node (&door);
	door.copy_subclass_data (door_ptr);
	
	door.set_position (40, 300);
	door.check_lock (0, 0, 0, 0, &keys_held, keys_visible, &iGraph);
	door.set_door_direction (left);
	door.set_state (locked);
	door.update_stop_box();
	door.set_to_delete (true);
	door_ptr = sprite_list_head.insert_node (&door);
	door_ptr -> set_door_direction (door.get_direction());
	door.copy_subclass_data (door_ptr);*/

	
	button.set_position (500, 200);
	button.set_ptr (NULL);
	button.set_frame_w (16);
	button.set_frame_h (16);
	sprite_list_head.insert_node (&button);





	




	if (see_generated_map)
	{
		if (!has_generated_map)
		{
			map_parser.parse();
			has_generated_map = true;
		};
	}
	else if (!see_generated_map)
	{
		sprite_list_head.clear (true);
		has_generated_map = false;
	};



	int total_rupees = save_state.get_rupees();
	int single_digit_rupees = 0;
	char rupee_str[4];
	for (int i=0; i<3; i++)
	{
		int divisor = pow ((double) 10, (int) 2-i);
		single_digit_rupees = total_rupees / divisor;
		rupee_str[i] = '0' + single_digit_rupees;
		total_rupees -= single_digit_rupees * divisor;
	};
	rupee_str[3] = '\0';
	rupee.set_position (20, SCREEN_HEIGHT - 10);
	rupee.set_layer (10);
	rupee.set_to_delete (true);
	sprite_list_head.insert_node (&rupee);
	


	char keys_held_text[2];
	keys_held_text[0] = keys_held+'0';
	keys_held_text[1] = '\0';
	key.select_frame (1, 0);
	key.set_modifiers (16, 32);
	key.set_position (SCREEN_WIDTH - 40 - key.get_width(), SCREEN_HEIGHT - 10);
	key.set_layer (10);
	key.set_to_delete (true);
	sprite_list_head.insert_node (&key);
	






	if (print_list)
	{
		//sprite_list_head.print_node_line();
		sprite_list_head.print_whole_list();
		print_list = false;
	};

	//do_something

	//sprite_list_head.draw_list (&iGraph);
	Sprite* sprite_to_draw = sprite_list_head.get_ptr();
	while (sprite_to_draw != NULL)
	{
		sprite_to_draw->do_something();
		sprite_to_draw->draw_node(&iGraph);
		sprite_to_draw = sprite_to_draw->get_ptr();
	};

	display_bounding_boxes();
	display_stop_boxes();
	check_stop_boxes();
	scan_virtual_keyboard();
	sprite_list_head.clear (false);
	sprite_list_head.sort_list();


	iGraph.SetColor (255,255,255);
	iGraph.SetTextFont ("Helvetica", 25, 10, 0, 0);
	iGraph.draw_text (45, SCREEN_HEIGHT - 13, rupee_str);


	iGraph.SetColor (255,255,255);
	iGraph.SetTextFont ("Helvetica", 25, 10, 0, 0);
	iGraph.draw_text (SCREEN_WIDTH - 34, SCREEN_HEIGHT - 13, keys_held_text);

	/*iGraph.SetColor (0, 255, 255);
	iGraph.draw_rectangle (
		Ganondorf.get_screen_x(), 
		Ganondorf.get_screen_y(),
		Ganondorf.get_screen_x() + Ganondorf.get_width(),
		Ganondorf.get_screen_y() - Ganondorf.get_height()
		);*/
};

int main (void)
{
	save_state.print_table();
	
	if(1)
	{
		iGraph.CreateMainWindow (SCREEN_WIDTH, SCREEN_HEIGHT,
			"The Myth of Ganon: A Twist in the Legend");

		iGraph.SetKeyboardInput(KeyboardInput);
		iGraph.SetBackgroundColor (26,32,40);
		load_images();
		iGraph.SetMainLoop(main_loop);
		iGraph.StartMainLoop();
	};

	halt();

	return 0;
};

void cat_path (char* ptr, const char* sub_folder_path, const char* file_name)
{
	strcpy(ptr,PROJECT_PATH);
	strcat(ptr,sub_folder_path);
	strcat(ptr,file_name);
};

/*
void OldKeyboardInput(int key, int state, int x, int y)
{
	if (state == KEY_STATE_DOWN)
	{
		switch (key)
		{
			case (KEY_RIGHT): Ganondorf.move (Ganondorf.get_current_speed(), horizontal); break;
			case (KEY_DOWN): Ganondorf.move (Ganondorf.get_current_speed(), vertical); break;
			case (KEY_LEFT): Ganondorf.move (-Ganondorf.get_current_speed(), horizontal); break;
			case (KEY_UP): Ganondorf.move (-Ganondorf.get_current_speed(), vertical); break;
			//case (' '): Ganondorf.toggle_dashing(); break;
			case ('m'): see_generated_map = !see_generated_map; break;
			case ('v'): exit(0); break;

			case ('t'): save_state.tweak_temple(0); break;
			case ('y'): save_state.tweak_temple(1); break;
			case ('u'): save_state.tweak_temple(2); break;
			case ('i'): save_state.tweak_temple(3); break;
			case ('o'): save_state.tweak_temple(4); break;
			case ('p'): save_state.tweak_temple(5); break;

			case ('f'): save_state.alter_rupees(-100); break;
			case ('g'): save_state.alter_rupees(-10); break;
			case ('h'): save_state.alter_rupees(-1); break;
			case ('j'): save_state.alter_rupees(1); break;
			case ('k'): save_state.alter_rupees(10); break;
			case ('l'): save_state.alter_rupees(100); break;


			case ('b'): save_state.alter_hearts (-0.25f); break;
			case ('n'): save_state.alter_hearts (0.25f); break;
			
			case (','): save_state.alter_mp (-5); break;
			case ('.'): save_state.alter_mp (5); break;

			case ('e') : cast_Dins_Fire(); break;
			case ('w') : cast_Nayrus_Love(); break;
			case ('q') : cast_Farores_Wind(); break;

			case ('a'): save_state.gain_heart_container (16); break;
			case ('s'): save_state.lose_heart_container (16); break;
		};

		if (key>='5' && key<='9')
			save_state.tweak_easter_egg (key-'5');
		if (key=='0')
			save_state.tweak_easter_egg (5);
	};
};
*/


void KeyboardInput (int key, int state, int x, int y)
{
	if (state == KEY_STATE_DOWN)
	{
		for (int i=0; i<256; i++)
			if (key == i)
				keyboard_key[i] = true;
	}
	else if (KEY_STATE_UP)
	{
		for (int i=0; i<256; i++)
			if (key == i)
				keyboard_key[i] = false;
	};

	/*if (keyboard_key[KEY_LEFTSHIFT])
		Ganondorf.set_dashing (true);
	else if (!keyboard_key[KEY_LEFTSHIFT])
		Ganondorf.set_dashing (false);*/

	
	if (keyboard_key[KEY_RIGHT])
		Ganondorf.move_d (right);
	if (keyboard_key[KEY_DOWN])
		Ganondorf.move_d (down);
	if (keyboard_key[KEY_LEFT])
		Ganondorf.move_d (left);
	if (keyboard_key[KEY_UP])
		Ganondorf.move_d (up);

	if (keyboard_key[' ']) print_list = true;
	if (keyboard_key['/']) 
	{
		fullscreen = !fullscreen;
		iGraph.SetFullscreen (fullscreen);
	};
	
	if (keyboard_key['e']) cast_Dins_Fire();
	if (keyboard_key['w']) cast_Nayrus_Love();
	if (keyboard_key['q']) cast_Farores_Wind();
	
	if (keyboard_key['m']) see_generated_map = !see_generated_map;
	if (keyboard_key['v']) exit (0);

	if (keyboard_key['t']) save_state.tweak_temple(0);
	if (keyboard_key['y']) save_state.tweak_temple(1);
	if (keyboard_key['u']) save_state.tweak_temple(2);
	if (keyboard_key['i']) save_state.tweak_temple(3);
	if (keyboard_key['o']) save_state.tweak_temple(4);
	if (keyboard_key['p']) save_state.tweak_temple(5);

	if (keyboard_key['f']) save_state.alter_rupees(-100);
	if (keyboard_key['g']) save_state.alter_rupees(-10);
	if (keyboard_key['h']) save_state.alter_rupees(-1);
	if (keyboard_key['j']) save_state.alter_rupees(1);
	if (keyboard_key['k']) save_state.alter_rupees(10);
	if (keyboard_key['l']) save_state.alter_rupees(100);

	if (keyboard_key['b']) save_state.alter_hearts (-0.25f);
	if (keyboard_key['n']) save_state.alter_hearts (0.25f);
			
	if (keyboard_key[',']) save_state.alter_mp (-5);
	if (keyboard_key['.']) save_state.alter_mp (5);
	if (keyboard_key['1']) save_state.alter_max_mp (-5);
	if (keyboard_key['2']) save_state.alter_max_mp (5);


	if (keyboard_key['s']) save_state.gain_heart_container (save_state.first_heart_container(0));
	if (keyboard_key['a']) save_state.lose_heart_container (save_state.first_heart_container(1));

	if (keyboard_key['\t']) keys_visible = !keys_visible;

	if (keyboard_key['z']) bounding_box_visible = !bounding_box_visible;
	if (keyboard_key['x']) stop_box_visible = !stop_box_visible;

	if (keyboard_key['['] && keys_held > 0) keys_held--;
	if (keyboard_key[']'] && keys_held < 9) keys_held++;

	for (int i='5'; i<='9'; i++)
		if (keyboard_key[i])	save_state.tweak_easter_egg (key-'5');
	if (keyboard_key['0'])		save_state.tweak_easter_egg (5);
};


void scan_virtual_keyboard()
{
	if (keyboard_key[KEY_LEFTSHIFT])
		Ganondorf.set_dashing (true);
	else
		Ganondorf.set_dashing (false);

	/*if (keyboard_key[KEY_RIGHT])
		Ganondorf.move_d (right);
	if (keyboard_key[KEY_DOWN])
		Ganondorf.move_d (down);
	if (keyboard_key[KEY_LEFT])
		Ganondorf.move_d (left);
	if (keyboard_key[KEY_UP])
		Ganondorf.move_d (up);*/


	//if (keyboard_key[KEY_LEFTSHIFT])
	//Ganondorf.set_dashing (true);
	//else if (!keyboard_key[KEY_LEFTSHIFT])
	//Ganondorf.set_dashing (false);
};


void cast_Dins_Fire()
{
	if (! is_casting())
		if (save_state.get_mp() >= 40.0f)
		{
			save_state.alter_mp (-40.0f);
			save_state.set_DF (1);
		};
};

void cast_Nayrus_Love()
{
	if (! is_casting())
		if (save_state.get_mp() >= 40.0f)
		{
			save_state.alter_mp (-40.0f);
			save_state.set_NL (1200);
		};
};

void cast_Farores_Wind()
{
	if (! is_casting())
	{
		int cost_to_set = 20;
		int cost_to_warp = 50;

		if (FW_x == -1)
		{
			if (save_state.get_mp() < cost_to_set)
				return;
			FW_x = Ganondorf.get_screen_x();
			FW_y = Ganondorf.get_screen_y();
			save_state.alter_mp (-cost_to_set);
			save_state.set_FW (1);
		}
		else
		{
			if (save_state.get_mp() < cost_to_warp)
				return;
			Ganondorf.set_position (FW_x, FW_y);
			save_state.alter_mp (-50);
			FW_x = -1;
			FW_y = -1;
			Ganondorf.select_frame (0, 0);
			save_state.set_FW (0);
		};
	};
};

bool is_casting()
{
	//printf ("%d\n", (save_state.get_FW() || save_state.get_NL() || save_state.get_DF()));
	return (save_state.get_FW() || save_state.get_NL() || save_state.get_DF());
};

void display_bounding_boxes()
{
	if (bounding_box_visible)
	{
		iGraph.SetColor (255, 255, 255);
		for (Sprite* p = sprite_list_head.get_ptr(); p != NULL; p = p->get_ptr())
		{
			iGraph.draw_rectangle 
			(
				p->get_screen_x() + 1,
				p->get_screen_y(),
				p->get_screen_x() + p->get_width(),
				p->get_screen_y() - p->get_height()
			);
		};
	};
};

void display_stop_boxes()
{
	if (stop_box_visible)
	{
		for (Sprite* p = sprite_list_head.get_ptr(); p != NULL; p = p->get_ptr())
		{
			if (p->stop_box_is_set())
			{
				if (p->get_sprite() != &Ganondorf)
					iGraph.SetColor (255, 0, 0);
				else
					iGraph.SetColor (255, 200, 0);

				iGraph.draw_rectangle
				(
					p->get_screen_x() + p->get_stop_box_x1(),
					p->get_screen_y() - p->get_stop_box_y1(),
					p->get_screen_x() + p->get_stop_box_x2(),
					p->get_screen_y() - p->get_stop_box_y2()
				);
			};
		};
	};
};

void check_stop_boxes()
{
	for (int i=0; i<4; i++)
		Ganondorf.set_move_restriction ((direction) i, -1);
		//Ganondorf.set_allowed_to_move ((direction) i, true);


	
	int gx1 = G_ptr->get_screen_x() + G_ptr->get_stop_box_x1();
	int gy1 = G_ptr->get_screen_y() - G_ptr->get_stop_box_y1();
	int gx2 = G_ptr->get_screen_x() + G_ptr->get_stop_box_x2();
	int gy2 = G_ptr->get_screen_y() - G_ptr->get_stop_box_y2();
	int dist = Ganondorf.get_current_speed(); //?

	int px1;
	int py1;
	int px2;
	int py2;

	int gx_r = max (gx1, gx2) - 1;
	int gx_l = min (gx1, gx2) - 1;
	int gy_d = max (gy1, gy2) + 1;
	int gy_u = min (gy1, gy2) + 1;

	for (Sprite* p = sprite_list_head.get_ptr(); p != NULL; p = p->get_ptr())
	{
		if (p->stop_box_is_set() && p->get_sprite() != &Ganondorf)
		{
			px1 = p->get_screen_x() + p->get_stop_box_x1();
			py1 = p->get_screen_y() - p->get_stop_box_y1();
			px2 = p->get_screen_x() + p->get_stop_box_x2();
			py2 = p->get_screen_y() - p->get_stop_box_y2();

			int px_r = max (px1, px2) - 1;
			int px_l = min (px1, px2) - 1;
			int py_d = max (py1, py2) + 1;
			int py_u = min (py1, py2) + 1;

			
			if (stop_box_visible)
			{
				iGraph.SetColor (120, 255, 0);
				iGraph.draw_point (gx_l, gy_d);

				iGraph.SetColor (0, 255, 255);
				iGraph.draw_point (gx_r, gy_u);

				iGraph.SetColor (120, 255, 0);
				iGraph.draw_point (px_l, py_d);

				iGraph.SetColor (0, 255, 255);
				iGraph.draw_point (px_r, py_u);
			};

			if (G_ptr->check_stop_box_collision(left, py_u, py_d) && between (0, gx_l-px_r, dist))
			{
				//Ganondorf.set_allowed_to_move (left, false);
				Ganondorf.set_move_restriction (left, gx_l - px_r);
			};

			if (G_ptr->check_stop_box_collision(up, px_l, px_r) && between (-1, gy_u-py_d, dist))
			{
				//Ganondorf.set_allowed_to_move (up, false);
				Ganondorf.set_move_restriction (up, gy_u - py_d);
			};

			if (G_ptr->check_stop_box_collision(right, py_u, py_d) && between (-1, px_l-gx_r, dist))
			{
				//Ganondorf.set_allowed_to_move (right, false);
				Ganondorf.set_move_restriction (right, px_l - gx_r);
			};

			if (G_ptr->check_stop_box_collision(down, px_l, px_r) && between (-1, py_u-gy_d, dist))
			{
				//Ganondorf.set_allowed_to_move (down, false);
				Ganondorf.set_move_restriction (down, py_u - gy_d);
			};


			p->get_sprite()->check_lock (0, 0, 0, 0, &keys_held, stop_box_visible, &iGraph);
		};
	};
};


bool between (int a, int b, int c)
{
	return ((a <= b && b <= c) || (c <= b && b <= a));
};

bool between_ordered (int a, int b, int c)
{
	return (a <= b && b <= c);
};

int max (int a, int b)
{
	return (a >= b ? a : b);
};

int min (int a, int b)
{
	return (a <= b ? a : b);
};

int portal_x (int i)
{
	int width = 337;
	int gap = -130;
	int offset = width;
	int sign = (i-2 >0) ? 1 : -1;

	int big_x = (SCREEN_WIDTH/2) - (3 * (width + gap)) + (i * (width + gap)) - 70;
	int small_x = -107 + i*120;


	if (BIG_SCREEN)
		return big_x;
	else
		return small_x;
};


int easter_egg_x (int i)
{
	int width = 83;
	int gap = 160;
	int offset = 70;
	int sign = (i-2 >0) ? 1 : -1;

	int big_x = (SCREEN_WIDTH/2) - (3 * (width + gap)) + (i * (width + gap)) + offset;
	int small_x = -107 + i*120;


	if (BIG_SCREEN)
		return big_x;
	else
		return small_x;
};



void halt()
{
	getchar();
};

#endif /* defined(__Main__) */