#include "Graphics.h"

void KeyboardInput(int key, int state, int x, int y); //le entrada do teclado, determinando apenas se a tecla esta pressionada

void KeyboardExecute();//essa funcao executa uma ação, de acordo com a tecla pressionada



extern bool KeyState[256];
extern float camera_position;