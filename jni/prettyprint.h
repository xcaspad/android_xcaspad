#ifndef PRETTYPRINT_H
#define PRETTYPRINT_H

#include <cairo.h>

#include "equation.h"

namespace xcas{

class PrettyPrint: public Equation {

public:

    PrettyPrint(int windowsize, int fontsize, giac::gen gen);

    void draw(cairo_surface_t *cs, double r, double g, double b);
    void draw(cairo_surface_t *cs, unsigned char** png_array,int *lenght, double r, double g, double b);

    inline int getWidth(){
        return width;
    }

    inline int getHeight(){
        return height;
    }

    ~PrettyPrint();

private:

    cairo_t *cairo;

    int width;
    int height;
    int rightx;
    int lowery;

    giac::attributs attr;

    void fl_color(Fl_Color c);
    double fl_width(const char * c);
    void fl_font(Fl_Font face, Fl_Fontsize size);

    cairo_font_face_t * helvetica_regular;
    cairo_font_face_t * times_bold_italic;
    cairo_font_face_t * times_italic;
    cairo_font_face_t * times_bold;

    void check_fl_draw(const char * ch,int i0,int j0,int imin,int jmin,int di,int dj,int delta_i,int delta_j);
    //void check_fl_point(int i0,int j0,int imin,int jmin,int di,int dj,int delta_i,int delta_j);
    //void check_fl_rect(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j);
    void check_fl_rectf(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j);
    void check_fl_line(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j);
    void fl_line(int x, int y, int x1, int y1);
    void fl_arc(int x, int y, int w, int h, double a1, double a2);
};

}//namespace xcas
#endif // PRETTYPRINT_H
