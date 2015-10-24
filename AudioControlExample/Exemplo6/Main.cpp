// Author: Edirlei Soares de Lima
// This example shows how to play and control music track reproduction


#include "Graphics.h"
#include "Audio.h"

Graphics graphics;

Image botao_play;
Image botao_pause;
Image botao_stop;

Audio Musica;

void MainLoop()
{
	graphics.DrawImage2D(botao_play);
	graphics.DrawImage2D(botao_pause);
	graphics.DrawImage2D(botao_stop);
}

void BotaoPlayClick(int button, int state, int x, int y)
{
	if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
	{
		Musica.Play();
	}
}

void BotaoPauseClick(int button, int state, int x, int y)
{
	if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
	{
		if (Musica.IsPlaying() == true)
			Musica.Pause();
		else
			Musica.Play();
	}
}

void BotaoStopClick(int button, int state, int x, int y)
{
	if ((button == MOUSE_LEFT_BUTTON)&&(state == MOUSE_STATE_DOWN))
	{
		Musica.Stop();
	}
}

int main(void)
{
	graphics.CreateMainWindow(800, 600, "AudioControl Example");
	graphics.SetBackgroundColor(152,209,250);

	botao_play.LoadPNGImage("Button-Play-icon.png");
	botao_play.SetPosition(80, 200, 128, 128);
	botao_play.SetOnClick(BotaoPlayClick);

	botao_pause.LoadPNGImage("Button-Pause-icon.png");
	botao_pause.SetPosition(336, 200, 128, 128);
	botao_pause.SetOnClick(BotaoPauseClick);

	botao_stop.LoadPNGImage("Button-Stop-icon.png");
	botao_stop.SetPosition(592, 200, 128, 128);
	botao_stop.SetOnClick(BotaoStopClick);

	Musica.LoadAudio("Music.mp3");

	graphics.SetMainLoop(MainLoop);
	graphics.StartMainLoop();
	return 0;
}
