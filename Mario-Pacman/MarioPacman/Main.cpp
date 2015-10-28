#include "Graphics.h"
#include <stdio.h>
#include <time.h>

#define MAP_WIDTH 34
#define MAP_HEIGHT 26

Graphics graphics;

char Mapa[MAP_WIDTH][MAP_HEIGHT];

Image personagem;
Image bloco_grama;
Image bloco_terra;
Image moeda;
Image gameoverim;
Image sucessoim;
bool ganhou = false;
bool gameover = false;
int pontos = 0;
float move_speed = 200;
float character_x = 22;
float character_y = 336;
bool move_left = false;
bool move_right = false;
bool move_up = false;
bool move_down = false;

//Variaveis para o controle do inimigo 1
float enemy1_x = 300;
float enemy1_y = 375;
Image enemy1_image;
int enemy1_direction;
int enemy1_speed = 100;

//Variaveis para o controle do inimigo 2
float enemy2_x = 300;
float enemy2_y = 120;
Image enemy2_image;
int enemy2_direction;
int enemy2_speed = 100;

//Variaveis para o controle do inimigo 3
float enemy3_x = 500;
float enemy3_y = 505;
Image enemy3_image;
int enemy3_direction;
int enemy3_speed = 100;

void LoadMapa(char *filename)
{
    int c;
    int count_line = 25;
    int count_col = 0;
    FILE *file;
    file = fopen(filename, "r");
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            if (count_col == MAP_WIDTH-1)
            {
                count_line--;
                count_col = 0;
            }
            else
            {
                Mapa[count_col][count_line] = (char)c;
                count_col++;
            }
        }
        fclose(file);
    }
}

//Movimenta o inimigo 1 aleatoriamente
void MoveEnemy1()
{
    if (enemy1_direction == 0) //left
    {
        if (Mapa[(int)((((enemy1_x + 24))/24))][((int)(enemy1_y + 12)/24)] == 'T')
        {
            enemy1_direction = rand() % 4;
        }
        else
        {
            enemy1_x += enemy1_speed * graphics.GetElapsedTime();
        }
    }
    else if (enemy1_direction == 1) //right
    {
        if (Mapa[(int)(((enemy1_x)/24))][(int)((enemy1_y + 12)/24)] == 'T')
        {
            enemy1_direction = rand() % 4;
        }
        else
        {
            enemy1_x -= enemy1_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy1_direction == 2) //down
    {
        if (Mapa[(int)((((enemy1_x + 12))/24))][(int)(enemy1_y)/24] == 'T')
        {
            enemy1_direction = rand() % 4;
        }
        else
        {
            enemy1_y -= enemy1_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy1_direction == 3)//up
    {
        if (Mapa[(int)((((enemy1_x + 12))/24))][(int)(enemy1_y + 24)/24] == 'T')
        {
            enemy1_direction = rand() % 4;
        }
        else
        {
            enemy1_y += enemy1_speed * graphics.GetElapsedTime();
        }
    }
}

//Movimenta o inimigo 2 aleatoriamente
void MoveEnemy2()
{
    if (enemy2_direction == 0) //left
    {
        if (Mapa[(int)((((enemy2_x + 24))/24))][((int)(enemy2_y + 12)/24)] == 'T')
        {
            enemy2_direction = rand() % 4;
        }
        else
        {
            enemy2_x += enemy2_speed * graphics.GetElapsedTime();
        }
    }
    else if (enemy2_direction == 1) //right
    {
        if (Mapa[(int)(((enemy2_x)/24))][(int)((enemy2_y + 12)/24)] == 'T')
        {
            enemy2_direction = rand() % 4;
        }
        else
        {
            enemy2_x -= enemy2_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy2_direction == 2) //down
    {
        if (Mapa[(int)((((enemy2_x + 12))/24))][(int)(enemy2_y)/24] == 'T')
        {
            enemy2_direction = rand() % 4;
        }
        else
        {
            enemy2_y -= enemy2_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy2_direction == 3)//up
    {
        if (Mapa[(int)((((enemy2_x + 12))/24))][(int)(enemy2_y + 24)/24] == 'T')
        {
            enemy2_direction = rand() % 4;
        }
        else
        {
            enemy2_y += enemy2_speed * graphics.GetElapsedTime();
        }
    }
}

//Movimenta o inimigo 3 aleatoriamente
void MoveEnemy3()
{
    if (enemy3_direction == 0) //left
    {
        if (Mapa[(int)((((enemy3_x + 24))/24))][((int)(enemy3_y + 12)/24)] == 'T')
        {
            enemy3_direction = rand() % 4;
        }
        else
        {
            enemy3_x += enemy3_speed * graphics.GetElapsedTime();
        }
    }
    else if (enemy3_direction == 1) //right
    {
        if (Mapa[(int)(((enemy3_x)/24))][(int)((enemy3_y + 12)/24)] == 'T')
        {
            enemy3_direction = rand() % 4;
        }
        else
        {
            enemy3_x -= enemy3_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy3_direction == 2) //down
    {
        if (Mapa[(int)((((enemy3_x + 12))/24))][(int)(enemy3_y)/24] == 'T')
        {
            enemy3_direction = rand() % 4;
        }
        else
        {
            enemy3_y -= enemy3_speed * graphics.GetElapsedTime();
        }
    }
    if (enemy3_direction == 3)//up
    {
        if (Mapa[(int)((((enemy3_x + 12))/24))][(int)(enemy3_y + 24)/24] == 'T')
        {
            enemy3_direction = rand() % 4;
        }
        else
        {
            enemy3_y += enemy3_speed * graphics.GetElapsedTime();
        }
    }
}

//Movimenta mario
void ApllyMove()
{
    if (move_left == true)
    {

        if (Mapa[(int)(((character_x)/24))][(int)((character_y + 12)/24)] == 'O')
        {
            Mapa[(int)(((character_x)/24))][(int)((character_y + 12)/24)] = 'X';
			pontos++;
        }

        if (Mapa[(int)(((character_x)/24))][(int)((character_y + 12)/24)] == 'X')
        {
            character_x-=move_speed * graphics.GetElapsedTime();
        }
    }
    if (move_right == true)
    {       
        if (Mapa[(int)((((character_x + 24))/24))][((int)(character_y + 12)/24)] == 'O')
        {
            Mapa[(int)((((character_x + 24))/24))][((int)(character_y + 12)/24)] = 'X';
			pontos++;
        }

        if (Mapa[(int)((((character_x + 24))/24))][((int)(character_y + 12)/24)] == 'X')
        {
            character_x+=move_speed * graphics.GetElapsedTime();           
        }
       
    } 
    if (move_down == true)
    {
        if (Mapa[(int)((((character_x + 12))/24))][(int)(character_y)/24] == 'O')
        {
            Mapa[(int)((((character_x + 12))/24))][(int)(character_y)/24] = 'X';
			pontos++;

        }

        if (Mapa[(int)((((character_x + 12))/24))][(int)(character_y)/24] == 'X')
        {
            character_y-=move_speed * graphics.GetElapsedTime();
        }
    }
    if (move_up == true)
    {
        if (Mapa[(int)((((character_x + 12))/24))][(int)(character_y + 24)/24] == 'O')
        {
            Mapa[(int)((((character_x + 12))/24))][(int)(character_y + 24)/24] = 'X';
			pontos++;
        }

        if (Mapa[(int)((((character_x + 12))/24))][(int)(character_y + 24)/24] == 'X')
        {
            character_y+=move_speed * graphics.GetElapsedTime();
        }
    }
}

int VerificaColisao(int ob1x, int ob1y, int ob2x, int ob2y)
{
    if (ob1y + 24 < ob2y)
        return(0);
    if (ob1y > ob2y + 24)
        return(0);

    if (ob1x + 24 < ob2x)
        return(0);
    if (ob1x > ob2x + 24)
        return(0);

    return(1);
}

void MainLoop()
{   
	if ((gameover == false)&&(ganhou == false))
	{
		ApllyMove();
		MoveEnemy1();
		MoveEnemy2();
		MoveEnemy3();
           
		int x, y;
		for (x = 0; x < MAP_WIDTH - 1; x++)
		{
			for (y = 0; y < MAP_HEIGHT - 1; y++)
			{
				if (Mapa[x][y] == 'T')
				{
					graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + (x*24), ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + (y * 24), 24, 24, bloco_terra);
				}
				if (Mapa[x][y] == 'O')
				{
					graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + (x*24), ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + (y * 24), 24, 24, moeda);
				}
			}
		}   
		//Desenha o mario
		graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + character_x, ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + character_y, 24, 24, personagem);

		//Desenha inimigos
		graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + enemy1_x, ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + enemy1_y, 24, 24, enemy1_image);
		graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + enemy2_x, ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + enemy2_y, 24, 24, enemy2_image);
		graphics.DrawImage2D(((graphics.GetScreenWidth()/2) - ((MAP_WIDTH * 24) / 2) + 12) + enemy3_x, ((graphics.GetScreenHeight()/2) - ((MAP_HEIGHT * 24) / 2) + 12) + enemy3_y, 24, 24, enemy3_image);
    
		if (VerificaColisao(character_x, character_y, enemy1_x, enemy1_y) == 1)
		{
		   gameover = true;
		}
		if (VerificaColisao(character_x, character_y, enemy2_x, enemy2_y) == 1)
		{
		   gameover = true;
		}
		if (VerificaColisao(character_x, character_y, enemy3_x, enemy3_y) == 1)
		{
		   gameover = true;
		}

		if (pontos == 319)
			ganhou = true;
	}

    if(gameover == true)
		graphics.DrawImage2D(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(), gameoverim);
	else if(ganhou == true)
        graphics.DrawImage2D(0, 0, graphics.GetScreenWidth(), graphics.GetScreenHeight(),sucessoim);
	
	graphics.DrawText2D(10,2,"Score: %d", pontos);
}

void KeyboardInput(int key, int state, int x, int y)
{
    if (state == KEY_STATE_DOWN)
    {   

        if (key == KEY_LEFT)
        {
            move_left = !move_left;
            move_right = false;
            move_up = false;
            move_down = false;
        }
        if (key == KEY_RIGHT)
        {   
            move_right = !move_right;
            move_left = false;
            move_up = false;
            move_down = false;
        }
        if (key == KEY_UP)
        {
            move_up = !move_up;
            move_right = false;
            move_left = false;
            move_down = false;
        }
        if (key == KEY_DOWN)
        {   
            move_down = !move_down;
            move_left = false;
            move_right = false;
            move_up = false;           
        }
    }
}

int main(void)
{
    graphics.CreateMainWindow(800, 600, "Mario Pacman");
   
    srand(time(NULL));
    enemy1_direction = rand() % 4;
	enemy2_direction = rand() % 4;
	enemy3_direction = rand() % 4;

    LoadMapa("Data\\Mapa01.txt");
    moeda.LoadPNGImage("Data\\moeda.png");
    personagem.LoadPNGImage("Data\\mario.png");
    bloco_terra.LoadPNGImage("Data\\tijolo.png");
    enemy1_image.LoadPNGImage("Data\\greenshell.png");
    enemy2_image.LoadPNGImage("Data\\redshell.png");
	enemy3_image.LoadPNGImage("Data\\blueshell.png");
    gameoverim.LoadPNGImage("Data\\gameover.png");
	sucessoim.LoadPNGImage("Data\\sucesso.png");

    graphics.SetKeyboardInput(KeyboardInput);
    graphics.SetMainLoop(MainLoop);
    graphics.StartMainLoop();
    return 0;
}