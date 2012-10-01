#include "../fb.h"
#include "fb_private_console.h"

FBCALL void fb_ConsoleGetXY( int *col, int *row )
{
	int x = __fb_con.cur_x, y = __fb_con.cur_y;

	if (__fb_con.inited) {
		/* Note we read reply from stdin, NOT from __fb_con.f_in */
		BG_LOCK();

#ifdef HOST_LINUX
		fflush(stdin);
		fb_hTermOut(SEQ_QUERY_CURSOR, 0, 0);
		if (fscanf(stdin, "\e[%d;%dR", &y, &x) != 2)
#endif
		{
			x = __fb_con.cur_x;
			y = __fb_con.cur_y;
		}

		BG_UNLOCK();
	}
	if (col)
		*col = x;
	if (row)
		*row = y;
}
