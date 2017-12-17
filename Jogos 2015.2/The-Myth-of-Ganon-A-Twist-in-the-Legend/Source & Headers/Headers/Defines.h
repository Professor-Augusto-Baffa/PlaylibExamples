#ifndef __Defines__
#define __Defines__

#include <string.h>
#include <stdio.h>

//#define PROJECT_PATH "\\\\Mac\\Home\\Projects\\The-Myth-of-Ganon-A-Twist-in-the-Legend\\"
#define PROJECT_PATH "Z:\\Projects\\The-Myth-of-Ganon-A-Twist-in-the-Legend\\"

#define BIG_SCREEN 1
#define SCREEN_WIDTH (BIG_SCREEN ? 1440 : 736)	//736
#define SCREEN_HEIGHT (BIG_SCREEN ? 800 : 448)	//448

#define TILE_SIZE (16)
#define MAX_RUPEES (999)
#define FILE_PATH_SIZE (300)

#define SH "Source & Headers\\"
#define CR "Resources\\Sprites\\Chars\\Raw\\"
#define CO "Resources\\Sprites\\Chars\\Ongoing\\"
#define CS "Resources\\Sprites\\Chars\\Sheet\\"
#define TR "Resources\\Sprites\\Terrenos\\"


void cat_path (char* ptr, const char* sub_folder_path, const char* file_name);
//void OldKeyboardInput (int key, int state, int x, int y);
void KeyboardInput (int key, int state, int x, int y);
bool between (int a, int b, int c);
bool between_ordered (int a, int b, int c);
int max (int a, int b);
int min (int a, int b);
void halt();

#endif /* defined(__Defines__) */


/*

Easter Egg sprite size: 49 x 51



*/