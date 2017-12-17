typedef struct sprite_sheet Sprite_sheet;
struct sprite_sheet;

typedef struct projetil Projetil;
struct projetil;

typedef struct cartucho Cartucho;
struct cartucho;

typedef struct equipamento Equipamento;
struct equipamento;


bool Collision(int position_x, int position_y, int x1, int y1, int x2, int y2); // Teste de colisão. Os parametros position_x e position_y indicam a posicao do objeto. Os outros parametros são os limites

void Distance (int position_x, int position_y, int x1, int y1); //mede a distancia entre os pontos desejados

void InitSprites(Sprite_sheet matriz_sprite[][10], int sprite_size_x, int sprite_size_y, int linha, int coluna); //inicia a spritesheet na matriz

void InitSprites(Equipamento *equip, int sprite_size_x, int sprite_size_y, int linha, int coluna); //inicializa os sprites, gravando as posicoes de corte da spritesheet

void Delay (float tempo_atraso, int tipo); // atrasa/pausa a execução do código. ideal para menus de pause e telas de loading. tipo define se vai ser pause ou atraso




