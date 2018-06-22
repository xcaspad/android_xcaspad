// -*- mode:C++ ; compile-command: "g++-3.4 -I. -I.. -g -c Equation.cc -DHAVE_CONFIG_H -DIN_GIAC -Wall" -*-
/*
 *  Copyright (C) 2005,2014 B. Parisse, Institut Fourier, 38402 St Martin d'Heres
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
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _EQUATION_H
#define _EQUATION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vector.h"
#include <string>

#ifndef IN_GIAC
#include <giac/giac.h>
#else
#include "giac.h"
#endif

#ifdef HAVE_LC_MESSAGES
#include <locale.h>
#endif

#ifndef IN_GIAC
#include <giac/giacintl.h>
#else
#include "giacintl.h"
#endif

typedef int FT_Font;
typedef int FT_Fontsize;
typedef unsigned int FT_Color;

const FT_Font FL_HELVETICA = 0;
const FT_Font FL_TIMES_BOLD = 9;
const FT_Font FL_TIMES_ITALIC = 10;
const FT_Font FL_TIMES_BOLD_ITALIC = 11;

void fl_font(FT_Font face, FT_Fontsize size);
void fl_color(FT_Color c);
double check_fl_width(const char * c);
void check_fl_draw(const char * ch, int i0, int j0);
void check_fl_rectf(int x, int y, int w, int h);
void check_fl_line(int i0, int j0, int i1, int j1);
void check_fl_arc(int x, int y, int w, int h, double a1, double a2);

namespace xcas {

  // maximum "size" of symbolics displayed in an Equation (pretty print)
  extern unsigned max_prettyprint_equation; 

  class Equation {

  public:

    giac::gen data; // of type eqwdata or undef if empty
    giac::attributs attr;

    bool need_active_parse; // true if the active cell must be parsed

    int active_pos; // position of the cursor in string (active mode)
    int begin_sel,end_sel; // -1 do not take care, >=0 position of sel

    int x1;
    int y1;
    int w1;
    int h1;

    int x(){return x1;}
    int y(){return y1;}
    int w(){return w1;}
    int h(){return h1;}

    Equation(int windowsize, int fontsize, const char* l,const giac::gen & g);
    draw();

    ~Equation();

  };

  giac::gen Equation_compute_size(const giac::gen & g,const giac::attributs & a,int windowhsize,const giac::context * contextptr);

} // namespace xcas


#endif // _EQUATION_H
