/*
 *  Copyright (C) 2011 Leonel Hernández Sandoval.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef FTPNGWRITER_H
#define FTPNGWRITER_H

#include <pngwriter.h>

class FTpngwriter : public pngwriter
{

public:

    using pngwriter::get_text_width;
    using pngwriter::get_text_width_utf8;
    using pngwriter::plot_text;
    using pngwriter::plot_text_utf8;

    FTpngwriter(int width, int height, int backgroundcolour, char * filename);

    int static get_text_width(unsigned char *buffer, long buffersize, int fontsize, char *text);

    int static get_text_width_utf8(unsigned char *buffer, long buffersize, int fontsize, char *text);

    void plot_text(unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, double red, double green, double blue);
    void plot_text(unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, int red, int green, int blue);

    void plot_text_utf8(unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, double red, double green, double blue);
    void plot_text_utf8(unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, int red, int green, int blue);

    ~FTpngwriter();
};

#endif // FTPNGWRITER_H
