/*
 *  libgfx2 - FreeBASIC's alternative gfx library
 *	Copyright (C) 2005 Angelo Mottola (a.mottola@libero.it)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
 * blitter.c -- blitter functions for different color depths
 *
 * chng: jan/2005 written [lillo]
 *
 */

#include "fb_gfx.h"


/* MMX functions declarations */
extern void fb_hBlit8to15RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to15BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to16RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to16BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to24RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to24BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to32RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit8to32BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit16to15RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit16to15BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit16to16RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit16to24MMX(unsigned char *dest, int pitch);
extern void fb_hBlit16to32MMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to15RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to15BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to16RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to16BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to24RGBMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to24BGRMMX(unsigned char *dest, int pitch);
extern void fb_hBlit32to32RGBMMX(unsigned char *dest, int pitch);

void fb_hBlit_code_start(void) { }

/*:::::*/
static void fb_hBlitCopy(unsigned char *dest, int pitch)
{
	unsigned char *src = fb_mode->framebuffer;
	char *dirty = fb_mode->dirty;
	int y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty)
			fb_hMemCpy(dest, src, fb_mode->pitch);
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to15RGB(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c1, c2, *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = pal[*s];
				c2 = pal[*(s + 1)];
				*d = ((c1 >> 3) & 0x001F) | ((c1 >> 6) & 0x03E0) | ((c1 >> 6) & 0x7C00) |
				     (((c2 >> 3) & 0x001F) | ((c2 >> 6) & 0x03E0) | ((c2 >> 6) & 0x7C00) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to15BGR(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c1, c2, *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = pal[*s];
				c2 = pal[*(s + 1)];
				*d = ((c1 << 7) & 0x7C00) | ((c1 >> 6) & 0x03E0) | ((c1 >> 19) & 0x001F) |
				     ((((c2 << 7) & 0x7C00) | ((c2 >> 6) & 0x03E0) | ((c2 >> 19) & 0x001F)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to16RGB(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c1, c2, *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = pal[*s];
				c2 = pal[*(s + 1)];
				*d = ((c1 >> 3) & 0x001F) | ((c1 >> 5) & 0x07E0) | ((c1 >> 5) & 0xF800) |
				     (((c2 >> 3) & 0x001F) | ((c2 >> 5) & 0x07E0) | ((c2 >> 5) & 0xF800) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to16BGR(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c1, c2, *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = pal[*s];
				c2 = pal[*(s + 1)];
				*d = ((c1 << 8) & 0xF800) | ((c1 >> 5) & 0x07E0) | ((c1 >> 19) & 0x001F) |
				     ((((c2 << 8) & 0xF800) | ((c2 >> 5) & 0x07E0) | ((c2 >> 19) & 0x001F)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to24RGB(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c;
	unsigned char *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned char *)dest;
			for (x = fb_mode->w; x; x--) {
				c = pal[*s];
				d[0] = (c >> 16) & 0xFF;
				d[1] = (c >> 8) & 0xFF;
				d[2] = c & 0xFF;
				s++;
				d += 3;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to24BGR(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c;
	unsigned char *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned char *)dest;
			for (x = fb_mode->w; x; x--) {
				c = pal[*s];
				d[0] = c & 0xFF;
				d[1] = (c >> 8) & 0xFF;
				d[2] = (c >> 16) & 0xFF;
				s++;
				d += 3;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to32RGB(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned int *d;
	unsigned char *s, *src = fb_mode->framebuffer;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w; x; x--) {
				*d = pal[*s];
				s++;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit8to32BGR(unsigned char *dest, int pitch)
{
	unsigned int *pal = fb_mode->device_palette;
	unsigned int *d;
	unsigned char *s, *src = fb_mode->framebuffer;
	unsigned int c;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;

	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w; x; x--) {
				c = pal[*s];
				*d = ((c << 16) & 0xFF0000) | (c & 0xFF00) | (c >> 16);
				s++;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit16to15RGB(unsigned char *dest, int pitch)
{
	unsigned int *d;
	unsigned short *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned short *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = ((c1 << 10) & 0x7C00) | ((c1 >> 1) & 0x03E0) | (c1 >> 11) |
				     ((((c2 << 10) & 0x7C00) | ((c2 >> 1) & 0x03E0) | (c2 >> 11)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit16to15BGR(unsigned char *dest, int pitch)
{
	unsigned int *d;
	unsigned short *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned short *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = (c1 & 0x001F) | ((c1 >> 1) & 0x7FE0) |
				     (((c2 & 0x001F) | ((c2 >> 1) & 0x7FE0)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit16to16RGB(unsigned char *dest, int pitch)
{
	unsigned int *d;
	unsigned short *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned short *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = ((c1 << 11) & 0xF800) | (c1 & 0x07E0) | (c1 >> 11) |
				     ((((c2 << 11) & 0xF800) | (c2 & 0x07E0) | (c2 >> 11)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit16to24(unsigned char *dest, int pitch)
{
	unsigned char *src = fb_mode->framebuffer;
	unsigned int *s, *d, c1, c2, c3, temp;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 2; x; x--) {
				c1 = fb_color_conv_16to32[*s & 0xFF] | fb_color_conv_16to32[512 + ((*s >> 8) & 0xFF)];
				temp = fb_color_conv_16to32[(*s >> 16) & 0xFF] | fb_color_conv_16to32[512 + ((*s >> 24) & 0xFF)];
				*d++ = c1 | (temp << 24);
				c2 = temp >> 8;
				s++;
				temp = fb_color_conv_16to32[*s & 0xFF] | fb_color_conv_16to32[512 + ((*s >> 8) & 0xFF)];
				*d++ = c2 | (temp << 16);
				c3 = temp >> 16;
				*d++ = c3 | ((fb_color_conv_16to32[(*s >> 16) & 0xFF] | fb_color_conv_16to32[512 + ((*s >> 24) & 0xFF)]) << 8);
				s++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit16to32(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c = *s++;
				*d++ = fb_color_conv_16to32[c & 0xFF] | fb_color_conv_16to32[256 + ((c >> 8) & 0xFF)];
				*d++ = fb_color_conv_16to32[(c >> 16) & 0xFF] | fb_color_conv_16to32[256 + ((c >> 24) & 0xFF)];
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to15RGB(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = (c1 >> 19) | ((c1 >> 6) & 0x03E0) | ((c1 << 7) & 0x7C00) |
				     (((c2 >> 19) | ((c2 >> 6) & 0x03E0) | ((c2 << 7) & 0x7C00)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to15BGR(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = ((c1 >> 3) & 0x001F) | ((c1 >> 6) & 0x03E0) | ((c1 >> 9) & 0x7C00) |
				     ((((c2 >> 3) & 0x001F) | ((c2 >> 6) & 0x03E0) | ((c2 >> 9) & 0x7C00)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to16RGB(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = (c1 >> 19) | ((c1 >> 5) & 0x07E0) | ((c1 << 8) & 0xF800) |
				     (((c2 >> 19) | ((c2 >> 5) & 0x07E0) | ((c2 << 8) & 0xF800)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to16BGR(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c1, c2;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w >> 1; x; x--) {
				c1 = *s;
				c2 = *(s + 1);
				*d = ((c1 >> 3) & 0x001F) | ((c1 >> 5) & 0x07E0) | ((c1 >> 8) & 0xF800) |
				     ((((c2 >> 3) & 0x001F) | ((c2 >> 5) & 0x07E0) | ((c2 >> 8) & 0xF800)) << 16);
				s += 2;
				d++;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to24RGB(unsigned char *dest, int pitch)
{
	unsigned int *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c;
	unsigned char *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned char *)dest;
			for (x = fb_mode->w; x; x--) {
				c = *s;
				d[0] = c >> 16;
				d[1] = (c >> 8) & 0xFF;
				d[2] = c & 0xFF;
				s++;
				d += 3;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to24BGR(unsigned char *dest, int pitch)
{
	unsigned int *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c;
	unsigned char *d;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned char *)dest;
			for (x = fb_mode->w; x; x--) {
				c = *s;
				d[0] = c & 0xFF;
				d[1] = (c >> 8) & 0xFF;
				d[2] = c >> 16;
				s++;
				d += 3;
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
static void fb_hBlit32to32RGB(unsigned char *dest, int pitch)
{
	unsigned int *d, *s;
	unsigned char *src = fb_mode->framebuffer;
	unsigned int c;
	char *dirty = fb_mode->dirty;
	int x, y, z = 0;
	
	for (y = fb_mode->h * fb_mode->scanline_size; y; y--) {
		if (*dirty) {
			s = (unsigned int *)src;
			d = (unsigned int *)dest;
			for (x = fb_mode->w; x; x--) {
				c = *s++;
				*d++ = (c >> 16) | (c & 0xFF00) | (c << 16);
			}
		}
		z++;
		if (z >= fb_mode->scanline_size) {
			z = 0;
			dirty++;
			src += fb_mode->pitch;
		}
		dest += pitch;
	}
}


/*:::::*/
BLITTER *fb_hGetBlitter(int device_depth, int is_rgb)
{
	BLITTER *all_blitters[] = {
		/* RGB (C) */
		fb_hBlit8to15RGB, fb_hBlit8to16RGB, fb_hBlit8to24RGB, fb_hBlit8to32RGB,
		fb_hBlit16to15RGB, fb_hBlit16to16RGB, fb_hBlit16to24, fb_hBlit16to32,
		fb_hBlit32to15RGB, fb_hBlit32to16RGB, fb_hBlit32to24RGB, fb_hBlit32to32RGB,
		/* BGR (C) */
		fb_hBlit8to15BGR, fb_hBlit8to16BGR, fb_hBlit8to24BGR, fb_hBlit8to32BGR,
		fb_hBlit16to15BGR, fb_hBlitCopy, fb_hBlit16to24, fb_hBlit16to32,
		fb_hBlit32to15BGR, fb_hBlit32to16BGR, fb_hBlit32to24BGR, fb_hBlitCopy,
		/* RGB (MMX) */
		fb_hBlit8to15RGBMMX, fb_hBlit8to16RGBMMX, fb_hBlit8to24RGBMMX, fb_hBlit8to32RGBMMX,
		fb_hBlit16to15RGBMMX, fb_hBlit16to16RGBMMX, fb_hBlit16to24MMX, fb_hBlit16to32MMX,
		fb_hBlit32to15RGBMMX, fb_hBlit32to16RGBMMX, fb_hBlit32to24RGBMMX, fb_hBlit32to32RGBMMX,
		/* BGR (MMX) */
		fb_hBlit8to15BGRMMX, fb_hBlit8to16BGRMMX, fb_hBlit8to24BGRMMX, fb_hBlit8to32BGRMMX,
		fb_hBlit16to15BGRMMX, fb_hBlitCopy, fb_hBlit16to24MMX, fb_hBlit16to32MMX,
		fb_hBlit32to15BGRMMX, fb_hBlit32to16BGRMMX, fb_hBlit32to24BGRMMX, fb_hBlitCopy,
	}, **blitter = &all_blitters[0];
	int i;
	
	for (i = 0; i < 256; i++) {
		if (is_rgb) {
			fb_color_conv_16to32[i] = ((i & 0x1F) << 19) | ((i & 0xE0) << 5);
			fb_color_conv_16to32[256+i] = ((i & 0x07) << 13) | (i & 0xF8);
		}
		else {
			fb_color_conv_16to32[i] = ((i & 0x1F) << 3) | ((i & 0xE0) << 5);
			fb_color_conv_16to32[256+i] = ((i & 0x07) << 13) | ((i & 0xF8) << 16);
		}
	}
	
	if ((fb_mode->flags & HAS_MMX) && (fb_mode->scanline_size == 1))
		blitter = &blitter[24];
	if (!is_rgb)
		blitter = &blitter[12];
	
	switch (fb_mode->depth) {
		case 1:
		case 2:
		case 4:
		case 8:
			switch (device_depth) {
				case 8:		return fb_hBlitCopy;
				case 15:	return blitter[0];
				case 16:	return blitter[1];
				case 24:	return blitter[2];
				case 32:	return blitter[3];
			}
			break;
		
		case 15:
		case 16:
			switch (device_depth) {
				case 15:	return blitter[4];
				case 16:	return blitter[5];
				case 24:	return blitter[6];
				case 32:	return blitter[7];
			}
			break;
		
		case 24:
		case 32:
			switch (device_depth) {
				case 15:	return blitter[8];
				case 16:	return blitter[9];
				case 24:	return blitter[10];
				case 32:	return blitter[11];
			}
			break;
	}
	return NULL;
}

void fb_hBlit_code_end(void) { }
