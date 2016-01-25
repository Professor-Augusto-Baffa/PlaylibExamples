#include <stdio.h>
#include "Graphics.h"
#include "Audio.h"

#include <locale.h>

Graphics graphics;
Graphics menu;
Audio bounce, tada;
Image j, um, dois, tres, play, sprites;

int btn;

bool keys[256], mouse[3], colado_bool=true, mouse_over_play_btn=false, stop_sys_menu, game = false, countdown = false, pare = false, mudo = false;
int dX=0, dY=0, p1=0, p2=0, colado_who=0, x_MAX, y_MAX, pt_dir = -1, msX, msY, gradDir = 1, OK = 0, stop_game, firstclick, dif2, pointMax = 3, dificuldade = 1;
int detected = 0, ajtX = 260, ajtY, previous_ajtX = 260;
float  bx=400, by=300, y=300, y2=300, spd=200, spdB=300, grad = 0, dt, past = 0, dif = 0, wts = 70, win_text_dir = -1, wta = 0, delay_win = 0;
float pts=70, pta = 0, size=40, cooldown = 0;
float hF = 0, hM = 0, hD = 0;
bool ClicouJogar = false, ClicouDificuldade = false, waitAfterClick = false;

float waitColadoAI = 0;

void Reset(){
	waitColadoAI = 0;
	p1 = 0;
	p2 = 0;
	dX = 0;
	dY = 0;
	dificuldade = 1;
	colado_who = 0;
	colado_bool = true;
	bx = 400;
	by = 300;
	y = 300;
	y2 = 300;
	pare = false;
	countdown = true;
	past = 0;
	size = 40;
	delay_win = 0;
	wta = 0;
	win_text_dir = 0;
	wts = 70;
	dif = 0;
	past = 0;
	grad = 0;
	pts = 50;
	pta = 0;
	pt_dir = 1;
	size = 40;
	ClicouJogar = false;
	ClicouDificuldade = false;
	hF = 0;
	hM = 0;
	hD = 0;
	waitAfterClick = false;
}

void Start(){
	countdown = false;
	colado_who = (int)rand()%2+1;
	colado_bool = true;
}

void S_bounce(){
	if(!mudo){
		bounce.Stop();
		bounce.Play();
	}
}

void S_tada(){
	if(!mudo){
		tada.Stop();
		tada.Play();
	}
}

void Menu(){

	dt=menu.GetElapsedTime();
	if(cooldown > 0)
		cooldown -= dt;
	if(game){
	int x_max=menu.GetScreenWidth();
	int y_max=menu.GetScreenHeight();

	if(wts >= 55){
		win_text_dir = -1;
	}else if(wts < 55){
		win_text_dir = 1;
	}

		for (float i=0; i <= y_max+30; i += 50){
		menu.DrawLine2D(x_max/2,i,x_max/2,i+30);
	}

	if (delay_win > 7){
		Reset();
		game = false;
		pare = false;
	}

	int midX = graphics.GetScreenWidth()/2;
	int midY = graphics.GetScreenHeight()/2;

	 if(p1 >= pointMax){
		pare = true;
		wta += dt * 0.1 * win_text_dir;
		wts +=  dt * 0.1 * win_text_dir + wta;
		graphics.DrawImage2D(midX -  7.08*wts/2, midY - .92*wts/2, 7.08*wts, .92*wts, 378, 224, 354, 46, sprites);
		delay_win += dt;
	}else if(p2 >= pointMax){
		pare = true;
		wta += dt * 0.1 * win_text_dir;
		wts +=  dt * 0.1 * win_text_dir + wta;
		graphics.DrawImage2D(midX -  7.08*wts/2, midY - .92*wts/2, 7.08*wts, .92*wts, 0, 224, 354, 46, sprites);
		delay_win += dt;
	}
	if(countdown || pare || p1 >= pointMax || p2 >= pointMax){
		menu.SetColor(70,70,70);
	}else{
		menu.SetColor(255,255,255);
	}
	menu.FillRectangle2D(30,(int)y-50,60,(int)y+50);
	menu.FillRectangle2D(x_max-30,(int)y2-50,x_max-60,(int)y2+50);
	menu.DrawText2D(90,40,"%d", p1);
	menu.DrawText2D(x_max-100,40,"%d", p2);
	if(!pare){
		menu.DrawImage2D(x_max/2-30,y_max-80,60,60,447,114,60,60,sprites); //pausar o jogo
		stop_game=1;
	}
	menu.FillCircle2D(bx,by,10,30);

	if(msX >= x_max/2-30 && msX <= x_max/2+30 && msY >= y_max-80 && msY <= y_max-20 && mouse[MOUSE_LEFT_BUTTON] && !countdown && !pare && p1 < pointMax && p2 < pointMax){
		pare = true;
	}
	if(msX >= x_max/2-145 && msX <= x_max/2-85 && msY >= y_max/4+30 && msY <= y_max/4+90 && mouse[MOUSE_LEFT_BUTTON] && !countdown && pare && p1 < pointMax && p2 < pointMax){
		pare = false;
	}
	if(msX >= x_max/2-30 && msX <= x_max/2+30 && msY >= y_max/2-30 && msY <= y_max/2+30 && mouse[MOUSE_LEFT_BUTTON] && cooldown < 0.01 && pare && !countdown){
		if(!mudo){
			mudo = true;
		}else{
			mudo = false;
		}
		cooldown = 1;
	}
	if(msX >= x_max/2-55 && msX <= x_max/2+145 && msY >= y_max/4+30 && msY <= y_max/4+90 && mouse[MOUSE_LEFT_BUTTON] && !countdown && pare){
		game = false;
		Reset();
	}
	if (pare && p1 < pointMax && p2 < pointMax){
		menu.SetColor(100,100,100);
		menu.FillRectangle2D(x_max/4,y_max/4,x_max*3/4,y_max*3/4);
		menu.DrawImage2D(x_max/2-100,y_max*3/4-70,200,60,569,49,200,60,sprites); // titulo de pausar
		menu.DrawImage2D(x_max/2-145,y_max/4+30,60,60,play);
		if(mudo){
			menu.DrawImage2D(x_max/2-30,y_max/2-30,60,60,677,122,109,101,sprites); //sem som
		}else{
			menu.DrawImage2D(x_max/2-30,y_max/2-30,60,60,567,122,109,102,sprites); //com som
		}
		menu.DrawImage2D(x_max/2-55,y_max/4+30,200,60,0,164,200,60,sprites); //botao de voltar ao menu
		stop_game = 0;
	}
	if(past >= 6 && countdown){
		Start();
	}else if(countdown){
		if(past >= 2 && past < 3){
			size += dt*500;
			menu.DrawImage2D((x_max-size)/2,(y_max-size)/2,size,size,tres);
			if(past >=2.99)
				size = 40;
		}else if(past >= 3 && past < 4){
			size += dt*500;
			menu.DrawImage2D((x_max-size)/2,(y_max-size)/2,size,size,dois);
			if(past >=3.99)
				size = 40;
		}else if(past >= 4 && past < 5){
			size += dt*500;
			menu.DrawImage2D((x_max-size)/2,(y_max-size)/2,size,size,um);
			if(past >=4.99)
				size = 40;
		}else if(past >= 5 && past < 6){
			size += dt*500;
			menu.DrawImage2D((x_max-size*1.11)/2,(y_max-size*0.6)/2,size*1.11,size*0.6,447,45,111,54,sprites);
		}
		past += dt;
	}
	if (!countdown){



//Game AI
		float precisionMargin = 30;
		float speedLevel = 1.1;
		float watchLimit = (x_max / 2);

		if(dificuldade == 3){
			precisionMargin = 30;
			speedLevel = 1.1;
			watchLimit = (x_max / 2);
		}
		else if(dificuldade == 2){
			precisionMargin = 40;
			speedLevel = 1.05;
			watchLimit = (x_max / 2) + 30;
		}
		else{
			precisionMargin = 40;
			speedLevel = 1.01;
			watchLimit = (x_max / 2) + 30;
		}

		if(!colado_bool){
			if((by + precisionMargin) > y && watchLimit > bx)
				y+=dt*(spd * speedLevel);
			else if((by - precisionMargin) < y && watchLimit > bx)
				y-=dt*(spd * speedLevel);
		} else if(colado_who==1 && !pare){
			waitColadoAI += graphics.GetElapsedTime();

			if(waitColadoAI > 2){
				colado_bool = false;
				colado_who = 0;
				waitColadoAI = 0;
				dX = 1;
				if((int)(y*y2)%2==0){
					dY=-1;
				}else{
					dY=1;
				}
			}
		}
//Player 1
		if(keys['w'] && y+50 < y_max && !pare){
			y+=dt*spd;
		}
		if(keys['s'] && y-50 > 0 && !pare){
			y-=dt*spd;
		}

		if(keys['d'] && colado_bool && colado_who==1 && !pare){
			colado_bool = false;
			colado_who = 0;
			dX = 1;
			if((int)(y*y2)%2==0){
				dY=-1;
			}else{
				dY=1;
			}
		}

//Player2
		if(keys[KEY_UP] && y2+50 < y_max && !pare){
			y2+=dt*spd;
		}
		if(keys[KEY_DOWN] && y2-50 > 0 && !pare){
			y2-=dt*spd;
		}

		if(keys[KEY_LEFT] && colado_bool && colado_who==2 && !pare){
			colado_bool = false;
			colado_who = 0;
			dX = -1;
			if((int)(y*y2)%2==0){
				dY=-1;
			}else{
				dY=1;
			}
		}
///////////


		if (y-50 < 0){
			y = 50;
		}else if (y+50 > y_max){
			y = y_max-50;
		}
		if (y2-50 < 0){
			y2 = 50;
		}else if (y2+50 > y_max){
			y2 = y_max-50;
		}
		

		if(bx >= x_max-60 && bx <= x_max-30 && by >= y2+50 && by <= y2+60){
			dY=1;
			S_bounce();
		}
		if(bx <= 60 && bx >= 30 && by >= y+50 && by <= y+60){
			dY=1;
			S_bounce();
		}
		if(bx >= x_max-60 && bx <= x_max-30 && by <= y2-50 && by >= y2-60){
			dY=-1;
			S_bounce();
		}
		if(bx <= 60 && bx >= 30 && by <= y-50 && by >= y-60){
			dY=-1;
			S_bounce();
		}
		if(bx >= x_max-70 && bx <= x_max-60 && by >= y2-50 && by <= y2+50 && !colado_bool){
			dX=-1;
			S_bounce();
		}else if(bx <= 70 && bx >= 60 && by >= y-50 && by <= y+50 && !colado_bool){
			dX=1;
			S_bounce();
		}
		if(by > y_max-10 && !colado_bool){
			dY=-1;
			S_bounce();
		}else if(by < 10 && !colado_bool){
			dY=1;
			S_bounce();
		}

		if(bx >= x_max-10){
			colado_bool = true;
			colado_who = 1;
			dX=0;
			dY=0;
			p1++;
			S_tada();
		}
		if(bx <= 10){
			colado_bool = true;
			colado_who = 2;
			dX=0;
			dY=0;
			p2++;
			S_tada();
		}

		if(colado_bool && colado_who==2){
			bx=x_max-70;
			by=y2;
		}else if(colado_bool && colado_who==1){
			bx=70;
			by=y;
		}

		bx+=dX*spdB*dt*stop_game;
		by+=dY*spdB*dt*stop_game;
	}
	
	}else{													//antes do jogo começar (tela inicial)

		dt = menu.GetElapsedTime();
		int x_MAX=menu.GetScreenWidth();
		int y_MAX=menu.GetScreenHeight();

		if(pts>160 || pts<0){
			pts = 50;
			pt_dir = 1;
			pta = 0;
		}

		if(pts > 100){
			pt_dir = -1;
		}else{
			pt_dir = 1;
		}
		float i = dt * 0.1 * pt_dir;
		pta += i;
		pts += i + pta;

		menu.SetLineWidth(4);
		if(msX >= x_MAX/2-100 && msX <= x_MAX/2+100 && msY >= y_MAX/5-40 && msY <= y_MAX/5+40 && !ClicouJogar){
			if (grad <= 70){
				grad += dt * 80;	
			}
			if (mouse[MOUSE_LEFT_BUTTON]){
				ClicouJogar = true;
				waitAfterClick = true;
			}
		}else if (grad >= 0){
			grad -= dt * 10;
		}

		if(waitAfterClick && !mouse[MOUSE_LEFT_BUTTON]){
			waitAfterClick = false;
		}

		if(ClicouJogar){
			if(msX >= x_MAX/16 && msX <= x_MAX/16 + 200*graphics.GetScreenWidth()/800 && msY >= y_MAX/5-40 + hF && msY <= y_MAX/5 + 80*graphics.GetScreenWidth()/800 + hF){
				if(hF < 10){
					hF += 30*dt;
				}
				if(mouse[MOUSE_LEFT_BUTTON]){
					dificuldade = 1;
					ClicouDificuldade = true;
				}
			}else{
				hF = 0;
			}

			if(msX >= x_MAX*6/16 && msX <= x_MAX*6/16 + 200*graphics.GetScreenWidth()/800 && msY >= y_MAX/5-40 + hM && msY <= y_MAX/5 + 80*graphics.GetScreenWidth()/800 + hM && waitAfterClick <= 0){
				if(hM < 10){
					hM += 30*dt;
				}
				if(mouse[MOUSE_LEFT_BUTTON]){
					dificuldade = 2;
					ClicouDificuldade = true;
				}
			}else{
				hM = 0;
			}

			if(msX >= x_MAX*11/16 && msX <= x_MAX*11/16 + 200*graphics.GetScreenWidth()/800 && msY >= y_MAX/5-40 + hD && msY <= y_MAX/5 + 80*graphics.GetScreenWidth()/800 + hD){
				if(hD < 10){
					hD += 30*dt;
				}
				if(mouse[MOUSE_LEFT_BUTTON]){
					dificuldade = 3;
					ClicouDificuldade = true;
				}
			}else{
				hD = 0;
			}
		}

		if(!ClicouJogar){
			menu.SetColor(grad,grad,grad);
			menu.FillRectangle2D(x_MAX/2-100,y_MAX/5-40,x_MAX/2+100,y_MAX/5+40);
			menu.SetColor(255,255,255);
			menu.DrawRectangle2D(x_MAX/2-100,y_MAX/5-40,x_MAX/2+100,y_MAX/5+40);
			menu.DrawImage2D(x_MAX/2-100,y_MAX/5-40,200,80,j);
		}else{
			menu.DrawImage2D(x_MAX/16,y_MAX/5-40+hF,200*graphics.GetScreenWidth()/800,80*graphics.GetScreenWidth()/800,731,101,100,40,sprites); //facil
			menu.DrawImage2D(x_MAX*6/16,y_MAX/5-40+hM,200*graphics.GetScreenWidth()/800,80*graphics.GetScreenWidth()/800,731,142,100,40,sprites); //medio
			menu.DrawImage2D(x_MAX*11/16,y_MAX/5-40+hD,200*graphics.GetScreenWidth()/800,80*graphics.GetScreenWidth()/800,731,183,100,40,sprites); //dificil
		}
		menu.DrawImage2D(x_MAX/2-4.3*pts/2, 4*y_MAX/5-pts*1.31/2, pts*4.3, pts*1.31, 0, 33, 430, 131, sprites); //titulo do pong
		menu.DrawImage2D(x_MAX/2-119.5,y_MAX*2/3-50,239,15,0,0,476,28,sprites); //creditos

		if(ClicouJogar && ClicouDificuldade){
			game = true;
			countdown = true;
			spdB = 300 + (dificuldade-1)*75;
			spd = 200 + (dificuldade-1)*50;
		}
	
		}
	}

void ClickLoop(int button, int state, int x, int y){
		if(state==MOUSE_STATE_DOWN){
			mouse[button] = true;
		}
		if(state==MOUSE_STATE_UP){
			mouse[button] = false;
		}
}

void MotionLoop(int x, int y){
	msX = x;
	msY = y;
}

void KeyLoop(int key, int state, int x, int y){

	if(state==KEY_STATE_DOWN)
		keys[key] = true;
	else if(state==KEY_STATE_UP)
		keys[key] = false;

}
int main (void)
{
	setlocale(LC_ALL,"Portuguese");
	menu.CreateMainWindow(800, 600, "Menu");
	menu.SetBackgroundColor(0,0,0);
	menu.SetMainLoop(Menu);
	menu.SetMouseClickInput(ClickLoop);
	menu.SetMouseMotionInput(MotionLoop);
	menu.SetKeyboardInput(KeyLoop);
	menu.SetMainLoop(Menu);
	bounce.LoadAudio("bounce.mp3");
	tada.LoadAudio("tada.mp3");
	j.LoadPNGImage("jogar.png");
	sprites.LoadPNGImage("sprites.png");
	um.LoadPNGImage("1.png");
	dois.LoadPNGImage("2.png");
	tres.LoadPNGImage("3.png");
	play.LoadPNGImage("play.png");
	menu.StartMainLoop();

	return 0;
}
