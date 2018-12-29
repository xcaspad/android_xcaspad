
#ifndef NO_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#include <cairo-ft.h>

#include "prettyprint.h"
#include "fonts/helvetica_regular.h"

//Horrible hack to center the image
int HORRIBLE_FIX;

typedef struct{
    unsigned char *current_position;
    unsigned char *end_of_array;
} closure_t;

static cairo_status_t
write_png_stream_to_byte_array (void *in_closure, const unsigned char *data, unsigned int length)
{
    closure_t *closure = (closure_t *) in_closure;

    if ((closure->current_position + length) > (closure->end_of_array))
        return CAIRO_STATUS_WRITE_ERROR;

    memcpy (closure->current_position, data, length);
    closure->current_position += length;

    return CAIRO_STATUS_SUCCESS;
}

namespace xcas{

PrettyPrint::PrettyPrint(int windowsize, int fontsize, giac::gen gen):Equation(){

    const context * contextptr=0;

    attr = attributs(fontsize, 255, 0);

    FT_Library library;
    FT_Face face_helvetica_regular;

    FT_Error status = FT_Init_FreeType (& library);

    if (status != 0) {
        fprintf (stderr, "Error %d opening library.\n", status);
    }

    status = FT_New_Memory_Face(library, Helvetica_Regular_ttf, Helvetica_Regular_ttf_len, 0, &face_helvetica_regular);

    if (status != 0) {
        fprintf (stderr, "Error %d opening Helvetica Regular .\n", status);
    }

    helvetica_regular = cairo_ft_font_face_create_for_ft_face (face_helvetica_regular, 0);

    //fl_width needs cairo surface has been created before to call Equation_compute_size
    cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, 0, 0);
    cairo = cairo_create (cs);

    data = Equation_compute_size(gen, attr, windowsize, contextptr);
    eqwdata bounds = Equation_total_size(data);

    HORRIBLE_FIX = fontsize;

    width = bounds.dx + HORRIBLE_FIX;
    height = bounds.dy + HORRIBLE_FIX;
    rightx = bounds.x;
    lowery = bounds.y;

    cairo_destroy(cairo);
    cairo_surface_destroy(cs);
}

PrettyPrint::~PrettyPrint(){

}

void PrettyPrint::draw(cairo_surface_t *cs, double r, double g, double b){

    //now we got cairo surface with correct sizes

    cairo = cairo_create(cs);

    cairo_set_source_rgb(cairo, r, g, b);

    Equation_draw(data, rightx, lowery + height - HORRIBLE_FIX/2, width, -height-HORRIBLE_FIX, this);

    cairo_destroy(cairo);
    cairo_surface_destroy(cs);
}

void PrettyPrint::draw(cairo_surface_t *cs, unsigned char** png_array,int *lenght, double r, double g, double b){

    //now we got cairo surface with correct sizes

    cairo = cairo_create(cs);

    cairo_set_source_rgb (cairo, 1.0, 1.0, 1.0);
    cairo_rectangle (cairo, 0, 0, this->getWidth(), this->getHeight());
    cairo_fill (cairo);

    cairo_set_source_rgb(cairo, r, g, b);

    Equation_draw(data, rightx, lowery + height - HORRIBLE_FIX/2, width, -height-HORRIBLE_FIX, this);

    int image_size = this->getWidth() * this->getWidth();
    *png_array = (unsigned char *)malloc(image_size);

    closure_t closure;
    closure.current_position = *png_array;
    closure.end_of_array = *png_array + image_size;

    cairo_surface_write_to_png_stream (cs, write_png_stream_to_byte_array, &closure);

    *lenght = (closure.current_position - *png_array);

    cairo_destroy(cairo);
    cairo_surface_destroy(cs);
}

void PrettyPrint::fl_font(Fl_Font face, Fl_Fontsize size) {

    //multiple fonts faces make pretty print not so pretty
    cairo_set_font_size(cairo, size);
    cairo_set_font_face(cairo, helvetica_regular);
}

void PrettyPrint::fl_color(Fl_Color c) {}

double PrettyPrint::fl_width(const char * c){
    cairo_text_extents_t extents;
    cairo_text_extents (cairo, c,  &extents);
    return extents.x_advance;
}

void PrettyPrint::check_fl_draw(const char * ch,int i0,int j0,int imin,int jmin,int di,int dj,int delta_i,int delta_j){
    cairo_move_to(cairo, i0, j0);
    cairo_show_text(cairo, ch);
}

void PrettyPrint::check_fl_rectf(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j){
    cairo_rectangle (cairo, i0, j0, i1, j1);
    cairo_stroke(cairo);
}

void PrettyPrint::check_fl_line(int i0,int j0,int i1,int j1,int imin,int jmin,int di,int dj,int delta_i,int delta_j){
    cairo_move_to (cairo, i0, j0);
    cairo_line_to (cairo, i1, j1);
    cairo_set_line_width (cairo, 1.0);
    cairo_stroke(cairo);
}

void PrettyPrint::fl_line(int x, int y, int x1, int y1){
    check_fl_line(x, y, x1, y1, 0, 0, 0, 0, 0, 0);
}

void PrettyPrint::fl_arc(int x, int y, int w, int h, double a1, double a2){

    //FIXME to draw an arc
    /*cairo_set_line_width (c, 1);
    cairo_move_to (c, x, y);
    cairo_arc(c, x, y, w, a1, a2);
    cairo_stroke (c);*/

    cairo_move_to (cairo, x, y);
    cairo_line_to(cairo, x+w, y-h);
    cairo_set_line_width (cairo, 1.5);
    cairo_stroke (cairo);
}

}//namespace xcas
