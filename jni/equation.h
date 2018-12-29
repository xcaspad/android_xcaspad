#ifndef EQUATION_H
#define EQUATION_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef IN_GIAC
#include <giac/first.h>
#else
#include "first.h"
#endif

#ifndef IN_GIAC
#include <giac/giac.h>
#else
#include "giac.h"
#endif

using namespace std;
using namespace giac;

namespace xcas{

typedef int Fl_Font;
typedef int Fl_Fontsize;
typedef unsigned int Fl_Color;

const Fl_Font FL_HELVETICA = 0;
const Fl_Font FL_TIMES_BOLD = 9;
const Fl_Font FL_TIMES_ITALIC = 10;
const Fl_Font FL_TIMES_BOLD_ITALIC = 11;
const Fl_Font FL_SYMBOL = 12;

class Equation
{
public:

    Equation();

    int x1;
    int y1;
    int w1;
    int h1;

    int x(){return x1;}
    int y(){return y1;}
    int w(){return w1;}
    int h(){return h1;}

    giac::gen data;

    int begin_sel,end_sel; // -1 do not take care, >=0 position of sel
    int clip_x,clip_y,clip_w,clip_h;

    int active_pos;
    bool need_active_parse;

    virtual void fl_color(Fl_Color c)=0;
    virtual double fl_width(const char * c)=0;
    virtual void fl_font(Fl_Font face, Fl_Fontsize size)=0;

    virtual void check_fl_draw(const char * ch,int i0,int j0,int imin,int jmin,int di,int dj,int delta_i,int delta_j)=0;
    //virtual void check_fl_point(int i0,int j0,int imin,int jmin,int di,int dj,int delta_i,int delta_j)=0;
    //virtual void check_fl_rect(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j)=0;
    virtual void check_fl_rectf(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j)=0;
    virtual void check_fl_line(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j)=0;
    virtual void fl_line(int x, int y, int x1, int y1)=0;
    virtual void fl_arc(int x, int y, int w, int h, double a1, double a2)=0;

private:
    Fl_Font cst_greek_translate(string & s0);
    void Equation_vertical_adjust(int hp,int yp,int & h,int & y);
    vecteur Equation_subsizes(const gen & arg,const attributs & a,int windowhsize,GIAC_CONTEXT);
    gen Equation_compute_symb_size(const gen & g,const attributs & a,int windowhsize,GIAC_CONTEXT);
    void Equation_translate(gen & g,int deltax,int deltay);
    void Equation_draw(const eqwdata & e,int x,int y,int rightx,int lowery,Equation * eq,int begin_sel,int end_sel);

protected:
    eqwdata Equation_total_size(const gen & g);
    gen Equation_compute_size(const gen & g,const attributs & a,int windowhsize,GIAC_CONTEXT);
    void Equation_draw(const gen & g,int x,int y,int rightx,int lowery,Equation * equat);
};

class Fl{
    public:
    static Equation *focus(){
        Equation *equ;
        return equ;
    }
};

}//namespace xcas

#endif // EQUATION_H
