#include "iGraphics.h"

iGraphics::iGraphics()
{
	screen_offset_x = 0;
	screen_offset_y = 0;
};

void iGraphics::tweak_offset_x (int x)
{
	screen_offset_x += x;
};

void iGraphics::tweak_offset_y (int y)
{
	screen_offset_y += y;
};

int iGraphics::offset_x()
{
	return screen_offset_x * SCREEN_WIDTH;
};

int iGraphics::offset_y()
{
	return screen_offset_y * SCREEN_HEIGHT;
};

int iGraphics::invert (int y)
{
	return (SCREEN_HEIGHT - y - 1);
};

void iGraphics::draw_image (Image i)
{
	DrawImage2D (i);
};

void iGraphics::draw_image (int x, int y, int w, int h, Image i)
{
	DrawImage2D (offset_x() + x, offset_y() + invert (y), w, h, i);
};

void iGraphics::draw_image (int x, int y, int w, int h, int cx, int cy, int cw, int ch, Image i)
{
	DrawImage2D (offset_x() + x, offset_y() + invert (y), w, h, cx, cy, cw, ch, i);
};

void iGraphics::draw_point (int x, int y)
{
	DrawPoint2D (offset_x() + x, offset_y() + invert (y));
};

void iGraphics::draw_line (int x1, int y1, int x2, int y2)
{
	DrawLine2D (offset_x() + x1, offset_y() + invert (y1), offset_x() + x2, offset_y() + invert (y2));
};

void iGraphics::draw_circle (int x, int y, int r)
{
	DrawCircle2D (offset_x() + x, offset_y() + invert (y), r);
};

void iGraphics::draw_triangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
	DrawTriangle2D (offset_x() + x1, offset_y() + invert (y1), offset_x() + x2, offset_y() + invert (y2), offset_x() + x3, offset_y() + invert (y3));	
};

void iGraphics::draw_rectangle (int x1, int y1, int x2, int y2)
{
	DrawRectangle2D (offset_x() + x1, offset_y() + invert (y1), offset_x() + x2, offset_y() + invert (y2));
};

void iGraphics::draw_text (int x, int y, const char* c)
{
	DrawText2D (offset_x() + x, offset_y() + invert (y), c);
};

void iGraphics::fill_triangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
	FillTriangle2D (offset_x() + x1, offset_y() + invert (y1), offset_x() + x2, offset_y() + invert (y2), offset_x() + x3, offset_y() + invert (y3));	
};

void iGraphics::fill_rectangle (int x1, int y1, int x2, int y2)
{
	FillRectangle2D (offset_x() + x1, invert (y1), offset_x() + x2, offset_y() + offset_y() + invert (y2));
};

