void MouseMotionInput (int x, int y);// diz a posicao do mouse na tela

void MouseClickInput(int button, int state, int x, int y);// diz a posicao do click e se se o botao esta pressionado
void MouseExecute(int *scene, int x1, int y1, int x2, int y2);

extern int mouse_x, mouse_y;//variaveis globais da posicao (em tempo real) do mouse na tela
extern int click_x, click_y;//variaveis globais da posicao do click do mouse

extern bool MouseState[3]; //vetor global que indica qual botao do mouse foi pressionado