typedef struct sprite_sheet Sprite_sheet;
struct sprite_sheet;

typedef struct projetil Projetil;
struct projetil;

typedef struct cartucho Cartucho;
struct cartucho;

typedef struct equipamento Equipamento;
struct equipamento;

bool CheckBoxCollision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

bool Collision(int position_x, int position_y, int x1, int y1, int x2, int y2); // Teste de colisão. Os parametros position_x e position_y indicam a posicao do objeto. Os outros parametros são os limites

void Distance (int position_x, int position_y, int x1, int y1); //mede a distancia entre os pontos desejados

void InitSprites(Sprite_sheet matriz_sprite[][10], int sprite_size_x, int sprite_size_y, int linha, int coluna); //inicia a spritesheet na matriz

void InitSprites(Equipamento *equip, int sprite_size_x, int sprite_size_y, int linha, int coluna); //inicializa os sprites, gravando as posicoes de corte da spritesheet

int Delay (float tempo_atraso, int tipo); // atrasa/pausa a execução do código. ideal para menus de pause e telas de loading. tipo define se vai ser pause ou atraso




