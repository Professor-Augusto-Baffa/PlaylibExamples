#include "Graphics.h"
#include <stdlib.h>
#include <stdio.h>


/*
Indexes:
0 = right
1 = left
2 = up
3 = down
*/


#define SPRITE_SIZE 32 // tamanho de cada imagem da tira de imagens
Graphics graphics;
Image sprite_sheet_RIGHT;
Image sprite_sheet_LEFT;
Image sprite_sheet_UP;
Image sprite_sheet_DOWN;

Image attack_left;
Image attack_right;
Image attack_up;
Image attack_down;

Image attack_;

Image stage0;
Image stage1;
Image stage2;
Image stage3;

Image mummy_right;
Image mummy_up;
Image mummy_down;
Image mummy_left;

Image togezo_right;
Image togezo_left;
Image togezo_up;
Image togezo_down;

Image heart;
Image heart_01;
Image heart_02;
Image heart_03;
Image heart_;

Image life;

Image gameover;

Image triforce;

Image win;


struct Sprite{ // estrutura para armazenar a posição do recorte de cada frame da animação
 int x;
 int y;
 int width;
 int height;
};

Sprite sprite[8][1];
Sprite sprite2[8][1];
Sprite sprite3[8][1];
Sprite sprite4[4][1];

Sprite Mummy[7][1];

Sprite togezo[4][1];

struct Player{
int x;
int y;
int width;
int height;
int collided;
//int health;
};

struct Mumia{
int x;
int y;
int width;
int height;
int collided;
int index;

void moveMumia(int *speed, float *m_x, float *m_y, float *pos_x, float *pos_y){
		if(*m_y > *pos_y && *m_x > *pos_x){
		index = 3;
		*m_y -= *speed * graphics.GetElapsedTime();
		} 
		if(*m_y < *pos_y && *m_x < *pos_x){
		index = 2;
		*m_y += *speed * graphics.GetElapsedTime();

		} 
		if(*m_x > *pos_x && *m_y < *pos_y){
		index = 1;		
		*m_x-= *speed * graphics.GetElapsedTime();

		} 
		if(*m_x < *pos_x && *m_y > *pos_y ){
		index = 0;
		*m_x += *speed * graphics.GetElapsedTime();
		}
}
};

struct Box{
int x;
int y;
int width;
int height;
int hit;
};


int myScore = 0;
int stage = 0; //Stage 0 = Main Menu
int index_player = 2;
float player_x = 241;
float player_y = 55;

float mummy_x = 100;
//float mummy_y = 100;
float mummy_start = 130;


float player_speed = 80;
int animation_clip = 0; // índice para controlar a animação
int animation_index = 0; // índice para controlar o frame da animação
float time_next_frame = 0;
float anime_attack;
float anime_mummy = 0;
float anime_mummy2 = 0;
bool key_states[256]; // vetor para armazenar as teclas pressionadas
bool canWalk = true;
int index_buff = 0;

int player_health = 3;


int animation_index_attack = 0;

Player player1;

int CheckBoxCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
return (x1 < x2+w2 && x2 < x1+w1 && y1 < y2+h2 && y2 < y1+h1);
}

void KeyboardInput(int key, int state, int x, int y)
{

if (state == KEY_STATE_DOWN)
key_states[key] = true;
else if (state == KEY_STATE_UP)
key_states[key] = false;

}


//Mummy Stuff

int mummy_speed = 50;
int mummy_index;
int animation_index_mummy = 0;
Image mummy_;
Player mummy;


void SpawnMummy(int x, int y, int index){

	//m.height = SPRITE_SIZE;
	//m.width = SPRITE_SIZE;
	//m.collided = 0;

	bool next_frame_mummy = false;
	float anime_time = 0;
	anime_time = anime_time + graphics.GetElapsedTime();
	mummy_index = index;
	

	if(mummy_index == 0){
		mummy_ = mummy_right;
		
	}else if(mummy_index == 1){
		mummy_ = mummy_left;
	
	}else if(mummy_index == 2){
	mummy_ = mummy_up;
	
	}else if(mummy_index == 3){
		mummy_ = mummy_down;
	}

	next_frame_mummy = true;

	graphics.DrawImage2D(x, y, SPRITE_SIZE, SPRITE_SIZE,
		 Mummy[animation_clip][animation_index_mummy].x,
		Mummy[animation_clip][animation_index_mummy].y,
		Mummy[animation_clip][animation_index_mummy].width,
		Mummy[animation_clip][animation_index_mummy].height,
		mummy_);

	}


void SpawnTogezo(int x, int y, int index){

	//m.height = SPRITE_SIZE;
	//m.width = SPRITE_SIZE;
	//m.collided = 0;

	bool next_frame_mummy = false;
	float anime_time = 0;
	anime_time = anime_time + graphics.GetElapsedTime();
	mummy_index = index;

	if(mummy_index == 0){
		mummy_ = togezo_right;
		
	}else if(mummy_index == 1){
		mummy_ = togezo_left;
	
	}else if(mummy_index == 2){
	mummy_ = togezo_up;
	
	}else if(mummy_index == 3){
		mummy_ = togezo_down;
	}

	next_frame_mummy = true;

	graphics.DrawImage2D(x, y, 26, 27,
		 togezo[animation_clip][animation_index_mummy].x,
		togezo[animation_clip][animation_index_mummy].y,
		togezo[animation_clip][animation_index_mummy].width,
		togezo[animation_clip][animation_index_mummy].height,
		mummy_);

	}


void drawHitbox(){graphics.DrawRectangle2D(player1.x,player1.y,player1.x+player1.width,player1.y+player1.height);}
void drawHitboxMummy(Mumia m){graphics.DrawRectangle2D(m.x,m.y,m.x+m.width,m.y+m.height);}

bool mummyHit(Mumia m, Player p){
	if((CheckBoxCollision(p.x, p.y, p.width, p.height, m.x, m.y, m.width, m.height))){
	m.collided = 1;
	return true;
	}else{
		return false;
	}
}


bool mumiaDead = false;
bool mumia2Dead = false;
bool mumia3Dead = false;

bool playerHit = false;

float mummy_y = 189;

float mummy_y2 = 189;
float mummy_x2 = 366;

float mum4_x = rand() % 500;
float mum4_y = rand() % 300;
bool mum4_dead = false;

float mum5_x = rand() % 500;
float mum5_y = rand() % 300;
bool mum5_dead = false;

float mum6_x = rand() % 500;
float mum6_y = rand() % 300;
bool mum6_dead = false;

float mum7_x = rand() % 500;
float mum7_y = rand() % 300;
bool mum7_dead = false;

float mum8_x = rand() % 500;
float mum8_y = rand() % 300;
bool mum8_dead = false;

float mum9_x = 45;
float mum9_y = 290;
bool mum9_dead = false;

float mum10_x = 200;
float mum10_y = 290;
bool mum10_dead = false;

float mum11_x = 45;
float mum11_y = 90;
bool mum11_dead = false;

float mum12_x = 200;
float mum12_y = 90;
bool mum12_dead = false;

float mum13_x = 200;
float mum13_y = 290;
bool mum13_dead = false;

float mum14_x = 200;
float mum14_y = 290;
bool mum14_dead = false;

float mum15_x = 200;
float mum15_y = 90;
bool mum15_dead = false;

float mum16_x = 200;
float mum16_y = 90;
bool mum16_dead = false;

bool vida_taken = false;
bool tri_taken = false;

void playerDamage(Mumia m, Player p, int s, bool t){

	if(mummyHit(m,p) && stage == s && t == false ){
	player_health = player_health - 1;
	if(m.x < player_x){
		player_x += 15;
	}else if(m.x > player_x){
		player_x -= 15;
	}else if(m.y > player_y){
		player_y -= 15;
	}else if(m.y < player_y){
		player_y += 15;
		}
	}

} 

void mummyDamage(Mumia m, Player p, int s, bool *dead){
	if(mummyHit(m,p) && stage == s){
			if(player_health < 3 ){
				//player_health+=1;
			}
				*dead = true;
				myScore += 100;
		}
}

void moveMummy(Mumia m, int m_x, int m_y){

		if(m_y > player_y && m_x){
		m.index = 3;
		m_y -= mummy_speed * graphics.GetElapsedTime();

		} if(m_y < player_y){
		m.index = 2;
		m_y += mummy_speed * graphics.GetElapsedTime();

		} if(m_x > player_x){
		m.index = 1;		
		m_x-= mummy_speed * graphics.GetElapsedTime();

		} if(m_x < player_x){
		m.index = 0;
		m_x += mummy_speed * graphics.GetElapsedTime();
		}
}

void MainLoop()
{

	//if(player_health == 0){stage = 1;}

	bool next_frame = false;
	bool next_frame_attack = false;
	bool mummy_frame = 0;
	bool mummy2_frame = 0;
	bool attacking = false;
	bool endAttack;

	bool test = false;

	if(animation_index_attack > 3){	
		animation_index_attack = 0;
		next_frame = true;
		index_player = index_buff;
	
	}

	player1.x = player_x;
	player1.y = player_y;
	player1.width = 32;
	player1.height = 32;
	player1.collided = false;		
	/*
	struct Mumia{
int x;
int y;
int width;
int height;
int collided;
int index;
};
	*/
		
	Mumia mum = {mummy_start, mummy_y, SPRITE_SIZE, SPRITE_SIZE, 0, 0};

	if(mumiaDead != true){
	mum.collided = 0;
	}else{
	mum.collided = 1;
	}

	
	Mumia mum2;

	if(mumia2Dead != true){
	mum2.collided = 0;
	}else{
	mum2.collided = 1;
	}
	
	mum2.height = SPRITE_SIZE;
	mum2.width = SPRITE_SIZE;
	if(stage == 0){
	mum2.x = mummy_x;
	mum2.y = mummy_y;
	}
	mum2.index = 0;

	

	Mumia mum3 = {mummy_x2, mummy_y2, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mumia3Dead != true){
	mum3.collided = 0;
	}else{
	mum3.collided = 1;
	}

	Mumia mum4 = {mum4_x, mum4_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum4_dead != true){
	mum4.collided = 0;
	}else{
	mum4.collided = 1;
	}


	Mumia mum5 = {mum5_x, mum5_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum5_dead != true){
	mum5.collided = 0;
	}else{
	mum5.collided = 1;
	}

	Mumia mum6 = {mum6_x, mum6_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum6_dead != true){
	mum6.collided = 0;
	}else{
	mum6.collided = 1;
	}

	Mumia mum7 = {mum7_x, mum7_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum7_dead != true){
	mum7.collided = 0;
	}else{
	mum7.collided = 1;
	}

	Mumia mum8 = {mum8_x, mum8_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum8_dead != true){
	mum8.collided = 0;
	}else{
	mum5.collided = 1;
	}

		Mumia mum9 = {mum9_x, mum9_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};
			   
	if(mum9_dead != true){
	mum9.collided = 0;
	}else{
	mum9.collided = 1;
	}

		Mumia mum10 = {mum10_x, mum10_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum10_dead != true){
	mum10.collided = 0;
	}else{
	mum10.collided = 1;
	}
	
	   	Mumia mum11 = {mum11_x, mum11_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum11_dead != true){
	mum11.collided = 0;
	}else{
	mum11.collided = 1;
	}


		Mumia mum12 = {mum12_x, mum12_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum12_dead != true){
	mum12.collided = 0;
	}else{
	mum12.collided = 1;
	}


				Mumia mum13 = {mum13_x, mum13_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum13_dead != true){
	mum13.collided = 0;
	}else{
	mum13.collided = 1;
	}

				Mumia mum14 = {mum14_x, mum14_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum14_dead != true){
	mum14.collided = 0;
	}else{
	mum14.collided = 1;
	}

				Mumia mum15 = {mum15_x, mum15_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum15_dead != true){
	mum15.collided = 0;
	}else{
	mum15.collided = 1;
	}

				Mumia mum16 = {mum16_x, mum16_y, SPRITE_SIZE, SPRITE_SIZE, 0, 1};

	if(mum16_dead != true){
	mum16.collided = 0;
	}else{
	mum16.collided = 1;
	}


			Player vida;
		vida.x = 111;
		vida.y = 196;
		vida.width = 26;
		vida.height = 25;
		vida.collided = false;

		
		
		
		if(tri_taken){
			graphics.DrawImage2D(0,0,512,383,win);
		
		
		}else if(player_health == 0){

			graphics.DrawImage2D(0,0,512,383,gameover);

		
		} else if(stage == 0){ //Stage inicial do jogo----------------------------------------------------------------------



		graphics.DrawImage2D(0,0,512,383,stage0);
		//Mumia mum;
		//SpawnMummy(mummy_start, 100,0, mum);
			if(mumiaDead){
				mum.x = -1000;
				mum.y = -1000;
			}
		
			

		if(mum.collided != 1){
		mum.moveMumia(&mummy_speed, &mummy_start, &mummy_y, &player_x, &player_y);
		SpawnTogezo(mum.x,mum.y, mum.index);
		mummy_frame = 1;
		}


			if(mumia3Dead){
				mum3.x = -1000;
				mum3.y = -1000;
			}



		if(mum3.collided != 1){
		
		mum3.moveMumia(&mummy_speed, &mummy_x2, &mummy_y2, &player_x, &player_y);

		SpawnTogezo(mum3.x,mum3.y, mum3.index);
		mummy_frame = 1;
		}


		//drawHitboxMummy(mum);
		
		
		/*		if(mumia2Dead){
				mum2.x = -100;
				mum2.y = -100;
			}

			if(mummyHit(mum2,player1)){
			
			player_health = player_health - 1;
			player_x = player_x + 10;
		}
			
		if(mum2.collided != 1){
		SpawnMummy(mum2.x,mum2.y, mum2.index);
		mummy_x += mummy_speed * graphics.GetElapsedTime();
		mummy_frame = 1;
		}
		*/
	
		if(animation_index_mummy > 3){animation_index_mummy = 0;}
	

		if(player_y > 210){
			player_y = 209;
		}

		if(player_x > 490){
			player_x = 489;
		}

		if(player_x < 0){
			player_x = 1;
		}
		if(player_y < 0){
			player_y = 1;
		}
	
	Player door1;
	Box box2;

	door1.x = 230;
	door1.y = 235;
	door1.width = 50;
	door1.height = 50;
	door1.collided = false;

	if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, door1.x, door1.y, door1.width, door1.height))){
	player1.collided = true;
	door1.collided = true;
		}
	//graphics.DrawRectangle2D(door1.x,door1.y,door1.x+door1.width,door1.y+door1.height);
	

	if(door1.collided){
	player_x = 256;
	player_y = 0;
	stage = 1;
	}

	}else if(stage == 1){
		graphics.DrawImage2D(0,0,512,383,stage1);
		Player door2;
		door2.x = 200;
		door2.y = -80;
		door2.width = 100;
		door2.height = 40;
		door2.collided = false;

		Player door3;
		door3.x = -17;
		door3.y = 171;
		door3.width = 19;
		door3.height = 41;
		door3.collided = false;
		graphics.DrawRectangle2D(door3.x,door3.y,door3.x+door3.width,door3.y+door3.height);

		Player door5;
		door5.x = 512;
		door5.y = 171;
		door5.width = 19;
		door5.height = 41;
		door5.collided = false;
		graphics.DrawRectangle2D(door5.x,door5.y,door5.x+door5.width,door5.y+door5.height);


		
				
		
		if(mum4_dead){
				mum4.x = -1000;
				mum4.y = -1000;
			}



		if(mum4.collided != 1){

		mum4.moveMumia(&mummy_speed, &mum4_x, &mum4_y, &player_x, &player_y);

		SpawnMummy(mum4.x, mum4.y, mum4.index);
		mummy_frame = 1;
		
		}

		if(mum5_dead){
				mum5.x = -1000;
				mum5.y = -1000;
			}

		if(mum5.collided != 1){
		mum5.moveMumia(&mummy_speed, &mum5_x, &mum5_y, &player_x, &player_y);
		SpawnMummy(mum5.x, mum5.y, mum5.index);
		mummy_frame = 1;
		}


		if(mum6_dead){
				mum6.x = -1000;
				mum6.y = -1000;
		}



		if(mum6.collided != 1){

		
	mum6.moveMumia(&mummy_speed, &mum6_x, &mum6_y, &player_x, &player_y);


		SpawnMummy(mum6.x, mum6.y, mum6.index);
		mummy_frame = 1;
		
		}

		if(mum7_dead){
				mum7.x = -1000;
				mum7.y = -1000;
			}



		if(mum7.collided != 1){

		
		mum7.moveMumia(&mummy_speed, &mum7_x, &mum7_y, &player_x, &player_y);


		SpawnMummy(mum7.x, mum7.y, mum7.index);
		mummy_frame = 1;
		
		}

		if(mum8_dead){
				mum8.x = -1000;
				mum8.y = -1000;
			}



		if(mum8.collided != 1){

		
	mum8.moveMumia(&mummy_speed, &mum8_x, &mum8_y, &player_x, &player_y);


		SpawnMummy(mum8.x, mum8.y, mum8.index);
		mummy_frame = 1;
		
		}

	if(player_x > 490){
			player_x = 489;
		}

	if(player_x < 0){
			player_x = 1;
		}
		if(player_y > 340){
			player_y = 339;
		}




		//graphics.DrawRectangle2D(300,30,512,30);
		
		
		//graphics.DrawImage2D(0,0,231,33, mummy_right);

	if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, door2.x, door2.y, door2.width, door2.height))){
	player1.collided = true;
	door2.collided = true;
		}

		graphics.DrawRectangle2D(door2.x,door2.y,door2.x+door2.width,door2.y+door2.height);
	
		if(door2.collided){
		player_x = 230;
		player_y = 190;
		stage = 0;
		}

			if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, door3.x, door3.y, door3.width, door3.height))){
	player1.collided = true;
	door3.collided = true;
		}
	
		if(door3.collided){
		player_x = 511;
		player_y = 183;
		stage = 2;
		}

		
			if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, door5.x, door5.y, door5.width, door5.height))){
	player1.collided = true;
	door5.collided = true;
		}
	
		if(door5.collided){
			player_x = 30;
		player_y = 171;
		stage = 3;
		}

		if(mumiaDead){
				mum.x = -1000;
				mum.y = -1000;
			}

		if(mummyHit(mum,player1)){
			
			player_health = player_health - 1;
			player_x = mum.x + 50;
		}

				if(mumia2Dead){
				mum2.x = -1000;
				mum2.y = -1000;
			}

		if(mummyHit(mum2,player1)){
			
			player_health = player_health - 1;
			player_x = mum2.x + 50;
		}

		if(mum2.collided != 1){
		SpawnMummy(mum2.x,mum2.y, mum2.index);
		
			
		mummy_x += mummy_speed * graphics.GetElapsedTime();
		mummy_frame = 1;
		}

		if(animation_index_mummy > 3){ animation_index_mummy = 0;}
	

}else if(stage == 2){
		graphics.DrawImage2D(0,0,512,383,stage2);

		Player door4;
		door4.x = 550;
		door4.y = 183;
		door4.width = 100;
		door4.height = 40;
		door4.collided = false;

	if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, door4.x, door4.y, door4.width, door4.height))){
	player1.collided = true;
	door4.collided = true;
		}															    
	
		if(door4.collided){
		player_x = 30;
		player_y = 171;
		stage = 1;
		}

		if(mum9_dead){
				mum9.x = -1000;
				mum9.y = -1000;
		}
		if(mum9.collided != 1){
		
		if(player_x < 237){
		mum9.moveMumia(&mummy_speed, &mum9_x, &mum9_y, &player_x, &player_y);
		mummy_frame = 1;
		}
		
		SpawnMummy(mum9.x, mum9.y, mum9.index);
		
		}


		if(mum10_dead){
				mum10.x = -1000;
				mum10.y = -1000;
		}

		if(mum10.collided != 1){
		
		if(player_x < 237){
		mum10.moveMumia(&mummy_speed, &mum10_x, &mum10_y, &player_x, &player_y);
		mummy_frame = 1;
		}
		SpawnMummy(mum10.x, mum10.y, mum10.index);
		}



	if(mum11_dead){
				mum11.x = -1000;
				mum11.y = -1000;
		}

		if(mum11.collided != 1){
		
		if(player_x < 237){
		mum11.moveMumia(&mummy_speed, &mum11_x, &mum11_y, &player_x, &player_y);
		mummy_frame = 1;
		}
		SpawnMummy(mum11.x, mum11.y, mum11.index);
		}
	
	

	if(mum12_dead){
				mum12.x = -1000;
				mum12.y = -1000;
		}

		if(mum12.collided != 1){
		
		if(player_x < 237){
		mum12.moveMumia(&mummy_speed, &mum12_x, &mum12_y, &player_x, &player_y);
		mummy_frame = 1;
		}
		SpawnMummy(mum12.x, mum12.y, mum12.index);
		}

		if(animation_index_mummy > 3){ animation_index_mummy = 0;}



		if(vida_taken == false){
		graphics.DrawImage2D(111,196,26,25,life);
			    if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, vida.x, vida.y, vida.width, vida.height))){
			vida_taken = true;
			player_health = 3;
			myScore += 1000;
		}
		}
	


}else if(stage == 3){
	
	graphics.DrawImage2D(0,0,512,383,stage3);

		if(mum13_dead){
				mum13.x = -1000;
				mum13.y = -1000;
		}

		if(mum13.collided != 1){
		
		mum13.moveMumia(&mummy_speed, &mum13_x, &mum13_y, &player_x, &player_y);
		mummy_frame = 1;
		
		SpawnTogezo(mum13.x, mum13.y, mum13.index);
		}


				if(mum16_dead){
				mum16.x = -1000;
				mum16.y = -1000;
		}

		if(mum16.collided != 1){
		
		mum16.moveMumia(&mummy_speed, &mum16_x, &mum16_y, &player_x, &player_y);
		mummy_frame = 1;
		
		SpawnTogezo(mum16.x, mum16.y, mum16.index);
		}

	   		if(animation_index_mummy > 3){ animation_index_mummy = 0;}

			 
			Player tri;
		tri.x = 360;
		tri.y = 183;
		tri.width = 30;
		tri.height = 27;
		tri.collided = false;

			if(tri_taken == false){
				graphics.DrawImage2D(360,183,30,27,triforce);
			    if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, tri.x, tri.y, tri.width, tri.height))){
			tri_taken = true;
			player_health = 3;
			myScore += 10000;
		}
		}

			

}





	
time_next_frame = time_next_frame + graphics.GetElapsedTime();
anime_mummy = anime_mummy + graphics.GetElapsedTime();
anime_mummy2 = anime_mummy2 + graphics.GetElapsedTime();
anime_attack = anime_attack + graphics.GetElapsedTime();

if (key_states[KEY_RIGHT])
 {
 next_frame = true;
 index_player = 0;
 if(canWalk){player_x += player_speed * graphics.GetElapsedTime();

 }
 }

if(key_states['a']){

	if(!attacking){
	if(index_buff == 0){
	attack_ = attack_right;
	}else if(index_buff == 1){ 
		attack_ = attack_left;
	}else if(index_buff == 2){
		attack_ = attack_up;
	}else if(index_buff == 3){
		attack_ = attack_down;
	}

	animation_index_attack = 0;
	index_player = 5;
	next_frame_attack = true;

}
}

if (key_states[KEY_LEFT])
{
index_player = 1;
next_frame = true;
if(canWalk){player_x -= player_speed * graphics.GetElapsedTime();}
}

if (key_states[KEY_DOWN])
 {
index_player = 3;
 next_frame = true;
 if(canWalk){ player_y -= player_speed * graphics.GetElapsedTime();}
}

if (key_states[KEY_UP])
{
index_player = 2;
 next_frame = true;
 if(canWalk){player_y += player_speed * graphics.GetElapsedTime();}
}

if(index_player == 5 && animation_index_attack < 4){   //Player atacando
	attacking = true;
	next_frame_attack = true;
}

if (next_frame)
{
if (time_next_frame > 0.1) // controle da velocidade do link
{
animation_index++;




//animation_index_attack++;

//if(animation_index_attack == 2){
	//	animation_index_attack == 0;
		//animation_index = 1;
//}

time_next_frame = 0;
}
}

if (next_frame_attack)
{
if (anime_attack > 0.1) // controle da velocidade de ataque
{
animation_index_attack++;
anime_attack = 0;



if((CheckBoxCollision(player1.x, player1.y, player1.width, player1.height, mum.x, mum.y, mum.width, mum.height))){
			mum.collided = 1;		
		}
}
}

if (mummy_frame)
{
if (anime_mummy > 0.15) // controle da velocidade da mumia
{
animation_index_mummy++;
anime_mummy = 0;
}
}

if (mummy2_frame)
{
if (anime_mummy2 > 0.15) // controle da velocidade da mumia
{
animation_index_mummy++;
anime_mummy2 = 0;
}
}

if (animation_index > 7){
animation_index = 0;
}


// desenha a imagem na tela


	
if(index_player == 0  && player_health != 0 && tri_taken == false){
graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE,
sprite[animation_clip][animation_index].x,
sprite[animation_clip][animation_index].y,
sprite[animation_clip][animation_index].width,
sprite[animation_clip][animation_index].height,
sprite_sheet_RIGHT);
index_buff = index_player;
}
else if(index_player == 1 && player_health != 0 && tri_taken == false){
graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE,
sprite[animation_clip][animation_index].x,
sprite[animation_clip][animation_index].y,
sprite[animation_clip][animation_index].width,
sprite[animation_clip][animation_index].height,
sprite_sheet_LEFT);
index_buff = index_player;
}
else if(index_player == 2 && player_health != 0 && tri_taken == false){
graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE,
sprite2[animation_clip][animation_index].x,
sprite2[animation_clip][animation_index].y,
sprite2[animation_clip][animation_index].width,
sprite2[animation_clip][animation_index].height,
sprite_sheet_UP);
index_buff = index_player;
}
else if(index_player == 3 && player_health != 0 && tri_taken == false){
graphics.DrawImage2D(player_x, player_y, SPRITE_SIZE, SPRITE_SIZE,
sprite3[animation_clip][animation_index].x,
sprite3[animation_clip][animation_index].y,
sprite3[animation_clip][animation_index].width,
sprite3[animation_clip][animation_index].height,
sprite_sheet_DOWN);
index_buff = index_player;
}
else if(index_player == 5 && animation_index_attack < 4 && player_health != 0  && tri_taken == false){

	if(index_buff == 0){
		player1.x += 3;
	}else if(index_buff == 1){
		player1.x -= 3;
	}else if(index_buff == 2){
		player1.y += 3;
	}else if(index_buff == 3){
		player1.y -= 3;
	}

	test = true;
	
graphics.DrawImage2D(player_x, player_y, 38, 38,
sprite4[animation_clip][animation_index_attack].x,
sprite4[animation_clip][animation_index_attack].y,
sprite4[animation_clip][animation_index_attack].width,
sprite4[animation_clip][animation_index_attack].height,
attack_);

mummyDamage(mum,player1,0,&mumiaDead);
mummyDamage(mum3,player1,0,&mumia3Dead);
mummyDamage(mum4,player1,1,&mum4_dead);
mummyDamage(mum5,player1,1,&mum5_dead);
mummyDamage(mum6,player1,1,&mum6_dead);
mummyDamage(mum7,player1,1,&mum7_dead);
mummyDamage(mum8,player1,1,&mum8_dead);
mummyDamage(mum9,player1,2,&mum9_dead);
mummyDamage(mum10,player1,2,&mum10_dead);
mummyDamage(mum11,player1,2,&mum11_dead);
mummyDamage(mum12,player1,2,&mum12_dead);
mummyDamage(mum13,player1,3,&mum13_dead);
mummyDamage(mum16,player1,3,&mum16_dead);
}

		playerDamage(mum, player1, 0, test);
		playerDamage(mum3, player1, 0, test);
		playerDamage(mum4, player1, 1, test);
		playerDamage(mum5, player1, 1, test);
		playerDamage(mum6, player1, 1, test);
		playerDamage(mum7, player1, 1, test);
		playerDamage(mum8, player1, 1, test);
		playerDamage(mum9, player1, 2, test);
		playerDamage(mum10, player1, 2, test);
		playerDamage(mum11, player1, 2, test);
		playerDamage(mum12, player1, 2, test);
		playerDamage(mum13, player1, 3, test);
		playerDamage(mum16, player1, 3, test);
			
			
//drawHitbox();

char score[100];
itoa(myScore,score,10);

graphics.DrawText2D(150,355,"Score: ");
graphics.DrawText2D(240,355,score);


if(player_health == 3){
	heart_ = heart;
}else if(player_health == 2){
	heart_ = heart_01;
}else if(player_health == 1){
	heart_ = heart_02;
}else{
	heart_ = heart_03;
}

if(player_health < 0){player_health = 0;}

graphics.DrawImage2D(35,350,80,25,heart_);
}

void loadTogezo(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{
 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 togezo[x][y].x = spritex;
 togezo[x][y].y = spritey;
 togezo[x][y].width = sprite_width;
 togezo[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

void SpriteCut(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{
 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 sprite[x][y].x = spritex;
 sprite[x][y].y = spritey;
 sprite[x][y].width = sprite_width;
 sprite[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

void SpriteCut2(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{
 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 sprite2[x][y].x = spritex;
 sprite2[x][y].y = spritey;
 sprite2[x][y].width = sprite_width;
 sprite2[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

void SpriteCut3(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{
 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 sprite3[x][y].x = spritex;
 sprite3[x][y].y = spritey;
 sprite3[x][y].width = sprite_width;
 sprite3[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

void SpriteCut4(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{

 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 sprite4[x][y].x = spritex;
 sprite4[x][y].y = spritey;
 sprite4[x][y].width = sprite_width;
 sprite4[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

void MummyLoad(int f, int sprite_width, int sprite_height) // função que calcula a posição do recorte e armazena na matriz sprite
{
 int x = 0, y = 0;
 int spritex = 0;
 int spritey = 0;
 for (int w = 0; w < f; w++)
 {
 Mummy[x][y].x = spritex;
 Mummy[x][y].y = spritey;
 Mummy[x][y].width = sprite_width;
 Mummy[x][y].height = sprite_height;
 spritex += sprite_width;
 y++;
 if (spritex >= 512)
 {
 spritex = 0;
 spritey += sprite_height;
 x++;
 y = 0;
 }
 }
}

int main(void)
{

graphics.CreateMainWindow(512, 383, "Link Reborn");
graphics.SetBackgroundColor(0,255,255);
graphics.SetKeyboardInput(KeyboardInput);
sprite_sheet_RIGHT.LoadPNGImage("player_right2.png");
sprite_sheet_LEFT.LoadPNGImage("player_left2.png");
sprite_sheet_UP.LoadPNGImage("player_up2.png");
sprite_sheet_DOWN.LoadPNGImage("player_down2.png");

attack_left.LoadPNGImage("attack_left.png");
attack_right.LoadPNGImage("attack_right.png");
attack_up.LoadPNGImage("attack_up.png");
attack_down.LoadPNGImage("attack_down.png");

stage0.LoadPNGImage("pyramid_entrada.png");
stage1.LoadPNGImage("stage_1.png");
stage2.LoadPNGImage("stage_3.png");
stage3.LoadPNGImage("stage_4.png");

gameover.LoadPNGImage("gameover.png");

win.LoadPNGImage("triforce_ember.png");


mummy_right.LoadPNGImage("mummy_right.png");
mummy_up.LoadPNGImage("mummy_up.png");
mummy_down.LoadPNGImage("mummy_down2.png");
mummy_left.LoadPNGImage("mummy_left.png");

togezo_right.LoadPNGImage("togezo_right.png");
togezo_left.LoadPNGImage("togezo_left.png");
togezo_up.LoadPNGImage("togezo_up.png");
togezo_down.LoadPNGImage("togezo_down.png");

heart.LoadPNGImage("heart.png");
heart_01.LoadPNGImage("heart_01.png");
heart_02.LoadPNGImage("heart_02.png");
heart_03.LoadPNGImage("heart_03.png");
life.LoadPNGImage("life.png");

triforce.LoadPNGImage("triforce.png");

SpriteCut(8,26,33);
SpriteCut2(8,32,35);
SpriteCut3(8, 25, 32);

SpriteCut4(4, 38,35);

MummyLoad(7, 33,33);

loadTogezo(4, 26,27);

 graphics.SetMainLoop(MainLoop);
 graphics.StartMainLoop();
 return 0;
}