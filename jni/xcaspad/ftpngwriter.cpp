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

#include "ftpngwriter.h"

FTpngwriter::FTpngwriter(int width, int height, int backgroundcolour, char *filename):pngwriter(width, height, backgroundcolour, filename)
{

}

int FTpngwriter::get_text_width(unsigned char *buffer, long buffersize, int fontsize, char *text){


    FT_Library  library;
    FT_Face     face;
    FT_Matrix   matrix;      // transformation matrix
    FT_Vector   pen;

    FT_UInt glyph_index;
    FT_Error error;

    FT_Bool use_kerning;
    FT_UInt previous = 0;

    /* Set up transformation Matrix */
    matrix.xx = (FT_Fixed)( 1.0*0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
    matrix.xy = (FT_Fixed)( 0.0*0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
    matrix.yx = (FT_Fixed)( 0.0*0x10000);  //   matrix.yx = - matrix.xy;
    matrix.yy = (FT_Fixed)( 1.0*0x10000);  //   matrix.yy = matrix.xx;

    /* Place starting coordinates in adequate form. */
    pen.x = 0;
    pen.y = 0;

    /*Count the length of the string */
    int num_chars = strlen(text);

    /* Initialize FT Library object */
    error = FT_Init_FreeType( &library );
    if (error) { std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Could not init Library."<< std::endl; return 0;}

    /* Initialize FT face object whit FT_New_Memory_Face istead FT_New_Face in order to handle truetype fonts from Android assets*/
    //error = FT_New_Face( library,face_path,0,&face );
    error = FT_New_Memory_Face(library, buffer, buffersize, 0, &face );

    if ( error == FT_Err_Unknown_File_Format ) { std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Font was opened, but type not supported."<< std::endl; return 0; } else if (error){ std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Could not find or load font file." << std::endl; return 0; }

    /* Set the Char size */
    error = FT_Set_Char_Size( face,          /* handle to face object           */
                  0,             /* char_width in 1/64th of points  */
                  fontsize*64,   /* char_height in 1/64th of points */
                  100,           /* horizontal device resolution    */
                  100 );         /* vertical device resolution      */

    /* A way of accesing the glyph directly */
    FT_GlyphSlot  slot = face->glyph;  // a small shortcut

    /* Does the font file support kerning? */
    use_kerning = FT_HAS_KERNING( face );

    int n;
    for ( n = 0; n < num_chars; n++ )
      {
     /* Convert character code to glyph index */
     glyph_index = FT_Get_Char_Index( face, text[n] );

     /* Retrieve kerning distance and move pen position */
     if ( use_kerning && previous&& glyph_index )
       {
          FT_Vector  delta;
          FT_Get_Kerning( face,
                  previous,
                  glyph_index,
                  ft_kerning_default, //FT_KERNING_DEFAULT,
                  &delta );

          /* Transform this kerning distance into rotated space */
          pen.x += (int) ( delta.x);
          pen.y +=  0;
       }

     /* Set transform */
     FT_Set_Transform( face, &matrix, &pen );

 /*set char size*/

     if (error) { std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Set char size error." << std::endl; return 0; }

     /* Retrieve glyph index from character code */
     glyph_index = FT_Get_Char_Index( face, text[n] );

     /* Load glyph image into the slot (erase previous one) */
     error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
     if (error) {
       std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error <<")." << std::endl;
       std::cerr.copyfmt(std::ios(NULL));
       return 0;
     }

     /* Convert to an anti-aliased bitmap */
     //	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
     error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
     if (error) { std::cerr << " PNGwriter::get_text_width - ERROR **: FreeType: Render glyph error." << std::endl; return 0;}

     /* Now, draw to our target surface */
 /*	my_draw_bitmap( &slot->bitmap,
             slot->bitmap_left,
             slot->bitmap_top,
             red,
             green,
             blue );
 */
     /* Advance to the next position */
     pen.x += slot->advance.x;
 //	std::cout << ((double) pen.x)/64.0 << std::endl;
     pen.y += slot->advance.y;

     /* record current glyph index */
     previous = glyph_index;
      }


    /* Free the face and the library objects */
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );

    return (int)( ((double)pen.x)/64.0 );
}

int FTpngwriter::get_text_width_utf8(unsigned char *buffer, long buffersize, int fontsize, char *text){

    FT_Library  library;
    FT_Face     face;
    FT_Matrix   matrix;      // transformation matrix
    FT_Vector   pen;

    FT_UInt glyph_index;
    FT_Error error;

    FT_Bool use_kerning;
    FT_UInt previous = 0;

    /* Set up transformation Matrix */
    matrix.xx = (FT_Fixed)( 0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
    matrix.xy = (FT_Fixed)( 0*0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
    matrix.yx = (FT_Fixed)( 0*0x10000);  //   matrix.yx = - matrix.xy;
    matrix.yy = (FT_Fixed)( 0x10000);  //   matrix.yy = matrix.xx;

    /* Place starting coordinates in adequate form. */
    pen.x = 0 ;
    pen.y = 0;

    /*Count the length of the string */
    int num_bytes=0;
    while(text[num_bytes]!=0)
      {
     num_bytes++;
      }

      /*
    std::cout << "Num bytes is: "<< num_bytes << std::endl;
    */

    //The array of ucs4 glyph indexes, which will by at most the number of bytes in the utf-8 file.
    long * ucs4text;
    ucs4text = new long[num_bytes+1];

    unsigned char u,v,w,x,y;

    int num_chars=0;

    long iii=0;

    while(iii<num_bytes)
      {
     unsigned char const z = text[iii];

     if(z<=127)
       {
          ucs4text[num_chars] = z;
       }

     if((192<=z)&&(z<=223))
       {
          iii++; y = text[iii];
          ucs4text[num_chars] = (z-192)*64 + (y -128);
       }

     if((224<=z)&&(z<=239))
       {
          iii++; y = text[iii];
          iii++; x = text[iii];
          ucs4text[num_chars] = (z-224)*4096 + (y -128)*64 + (x-128);
       }

     if((240<=z)&&(z<=247))
       {
          iii++; y = text[iii];
          iii++; x = text[iii];
          iii++; w = text[iii];
          ucs4text[num_chars] = (z-240)*262144 + (y -128)*4096 + (x-128)*64 + (w-128);
       }

     if((248<=z)&&(z<=251))
       {
          iii++; y = text[iii];
          iii++; x = text[iii];
          iii++; w = text[iii];
          iii++; v = text[iii];
          ucs4text[num_chars] = (z-248)*16777216 + (y -128)*262144 + (x-128)*4096 + (w-128)*64 +(v-128);
       }

     if((252==z)||(z==253))
       {
          iii++; y = text[iii];
          iii++; x = text[iii];
          iii++; w = text[iii];
          iii++; v = text[iii];
          u = text[iii];
          ucs4text[num_chars] = (z-252)*1073741824 + (y -128)*16777216   + (x-128)*262144 + (w-128)*4096 +(v-128)*64 + (u-128);
       }

     if((z==254)||(z==255))
       {
          std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: Problem with character: invalid UTF-8 data."<< std::endl;
       }
     // std::cerr << "\nProblem at " << iii << ".\n";
     //
     iii++;
     num_chars++;
      }

    // num_chars now contains the number of characters in the string.
    /*
    std::cout << "Num chars is: "<< num_chars << std::endl;
    */

    /* Initialize FT Library object */
    error = FT_Init_FreeType( &library );
    if (error) {
      std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: FreeType: Could not init Library." << std::endl;
      delete[] ucs4text;
      return 0;
    }

    /* Initialize FT face object whit FT_New_Memory_Face istead FT_New_Face in order to handle truetype fonts from Android assets*/
    //error = FT_New_Face( library,face_path,0,&face );
    error = FT_New_Memory_Face(library, buffer, buffersize, 0, &face );

    if ( error == FT_Err_Unknown_File_Format ) {
      std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: FreeType: Font was opened, but type not supported." << std::endl;
      delete[] ucs4text;
      return 0;
    } else if (error) {
      std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not find or load font file." << std::endl;
      delete[] ucs4text;
      return 0;
    }

    /* Set the Char size */
    error = FT_Set_Char_Size( face,          /* handle to face object           */
                  0,             /* char_width in 1/64th of points  */
                  fontsize*64,   /* char_height in 1/64th of points */
                  100,           /* horizontal device resolution    */
                  100 );         /* vertical device resolution      */

    /* A way of accesing the glyph directly */
    FT_GlyphSlot  slot = face->glyph;  // a small shortcut

    /* Does the font file support kerning? */
    use_kerning = FT_HAS_KERNING( face );

    int n;
    for ( n = 0; n < num_chars; n++ )
      {
     /* Convert character code to glyph index */
     glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

     /* Retrieve kerning distance and move pen position */
     if ( use_kerning && previous&& glyph_index )
       {
          FT_Vector  delta;
          FT_Get_Kerning( face,
                  previous,
                  glyph_index,
                  ft_kerning_default, //FT_KERNING_DEFAULT,
                  &delta );

          /* Transform this kerning distance into rotated space */
          pen.x += (int) (delta.x);
          pen.y +=  0;
       }

     /* Set transform */
     FT_Set_Transform( face, &matrix, &pen );

 /*set char size*/

     if (error) {
       std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: FreeType: Set char size error." << std::endl;
       delete[] ucs4text;
       return 0;
     }

     /* Retrieve glyph index from character code */
     glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

     /* Load glyph image into the slot (erase previous one) */
     error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
     if (error) {
       std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error <<")." << std::endl;
       std::cerr.copyfmt(std::ios(NULL));
       delete[] ucs4text;
       return 0;
     }

     /* Convert to an anti-aliased bitmap */
     error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
     if (error) {
       std::cerr << " PNGwriter::get_text_width_utf8 - ERROR **: FreeType: Render glyph error." << std::endl;
       delete[] ucs4text;
       return 0;
     }

     /* Now, draw to our target surface */
 /*	my_draw_bitmap( &slot->bitmap,
             slot->bitmap_left,
             y_start + slot->bitmap_top,
             red,
             green,
             blue );
 */
     /* Advance to the next position */
     pen.x += slot->advance.x;
     pen.y += slot->advance.y;

     /* record current glyph index */
     previous = glyph_index;
      }

    /* Free the face and the library objects */
    FT_Done_Face    ( face );
    FT_Done_FreeType( library );

    delete[] ucs4text;

    return (int) (((double) pen.x)/64.0);
}


void FTpngwriter::plot_text( unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, double red, double green, double blue)
{
   FT_Library  library;
   FT_Face     face;
   FT_Matrix   matrix;      // transformation matrix
   FT_Vector   pen;

   FT_UInt glyph_index;
   FT_Error error;

   FT_Bool use_kerning;
   FT_UInt previous = 0;

   /* Set up transformation Matrix */
   matrix.xx = (FT_Fixed)( cos(angle)*0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
   matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
   matrix.yx = (FT_Fixed)( sin(angle)*0x10000);  //   matrix.yx = - matrix.xy;
   matrix.yy = (FT_Fixed)( cos(angle)*0x10000);  //   matrix.yy = matrix.xx;

   /* Place starting coordinates in adequate form. */
   pen.x = x_start*64 ;
   pen.y =   (int)(y_start/64.0);

   /*Count the length of the string */
   int num_chars = strlen(text);

   /* Initialize FT Library object */
   error = FT_Init_FreeType( &library );
   if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not init Library."<< std::endl; return;}

   /* Initialize FT face object whit FT_New_Memory_Face istead FT_New_Face in order to handle truetype fonts from Android assets*/
   //error = FT_New_Face( library,face_path,0,&face );
   error = FT_New_Memory_Face(library, buffer, buffersize, 0, &face );
   if ( error == FT_Err_Unknown_File_Format ) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Font was opened, but type not supported."<< std::endl; return; } else if (error){ std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not find or load font file."<< std::endl; return; }

   /* Set the Char size */
   error = FT_Set_Char_Size( face,          /* handle to face object           */
                 0,             /* char_width in 1/64th of points  */
                 fontsize*64,   /* char_height in 1/64th of points */
                 100,           /* horizontal device resolution    */
                 100 );         /* vertical device resolution      */

   /* A way of accesing the glyph directly */
   FT_GlyphSlot  slot = face->glyph;  // a small shortcut

   /* Does the font file support kerning? */
   use_kerning = FT_HAS_KERNING( face );

   int n;
   for ( n = 0; n < num_chars; n++ )
     {
    /* Convert character code to glyph index */
    glyph_index = FT_Get_Char_Index( face, text[n] );

    /* Retrieve kerning distance and move pen position */
    if ( use_kerning && previous&& glyph_index )
      {
         FT_Vector  delta;
         FT_Get_Kerning( face,
                 previous,
                 glyph_index,
                 ft_kerning_default, //FT_KERNING_DEFAULT,
                 &delta );

         /* Transform this kerning distance into rotated space */
         pen.x += (int) (((double) delta.x)*cos(angle));
         pen.y +=  (int) (((double) delta.x)*( sin(angle)));
      }

    /* Set transform */
    FT_Set_Transform( face, &matrix, &pen );

/*set char size*/

    if (error) {
      std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Set char size error." << std::endl; return;
    }

    /* Retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index( face, text[n] );

    /* Load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
    if (error) {
      std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error <<")." << std::endl;
      std::cerr.copyfmt(std::ios(NULL));
      return;
    }

    /* Convert to an anti-aliased bitmap */
    //	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
    error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
    if (error) { std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Render glyph error." << std::endl; return;}

    /* Now, draw to our target surface */
    my_draw_bitmap( &slot->bitmap,
            slot->bitmap_left,
            y_start + slot->bitmap_top,
            red,
            green,
            blue );

    /* Advance to the next position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;

    /* record current glyph index */
    previous = glyph_index;
     }

   /* Free the face and the library objects */
   FT_Done_Face    ( face );
   FT_Done_FreeType( library );
}

void FTpngwriter::plot_text_utf8( unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle,  char * text, double red, double green, double blue)
{
   FT_Library  library;
   FT_Face     face;
   FT_Matrix   matrix;      // transformation matrix
   FT_Vector   pen;

   FT_UInt glyph_index;
   FT_Error error;

   FT_Bool use_kerning;
   FT_UInt previous = 0;

   /* Set up transformation Matrix */
   matrix.xx = (FT_Fixed)( cos(angle)*0x10000);   /* It would make more sense to do this (below), but, bizzarely, */
   matrix.xy = (FT_Fixed)(-sin(angle)*0x10000);   /* if one does, FT_Load_Glyph fails consistently.               */
   matrix.yx = (FT_Fixed)( sin(angle)*0x10000);  //   matrix.yx = - matrix.xy;
   matrix.yy = (FT_Fixed)( cos(angle)*0x10000);  //   matrix.yy = matrix.xx;

   /* Place starting coordinates in adequate form. */
   pen.x = x_start*64 ;
   pen.y = (int)(y_start/64.0);

   /*Count the length of the string */
   int num_bytes=0;
   while(text[num_bytes]!=0)
     {
    num_bytes++;
     }

     /*
   std::cout << "Num bytes is: "<< num_bytes << std::endl;
   */

   //The array of ucs4 glyph indexes, which will by at most the number of bytes in the utf-8 file.
   long * ucs4text;
   ucs4text = new long[num_bytes+1];

   unsigned char u,v,w,x,y;

   int num_chars=0;

   long iii=0;

   while(iii<num_bytes)
     {
    unsigned char const z = text[iii];

    if(z<=127)
      {
         ucs4text[num_chars] = z;
      }

    if((192<=z)&&(z<=223))
      {
         iii++; y = text[iii];
         ucs4text[num_chars] = (z-192)*64 + (y -128);
      }

    if((224<=z)&&(z<=239))
      {
         iii++; y = text[iii];
         iii++; x = text[iii];
         ucs4text[num_chars] = (z-224)*4096 + (y -128)*64 + (x-128);
      }

    if((240<=z)&&(z<=247))
      {
         iii++; y = text[iii];
         iii++; x = text[iii];
         iii++; w = text[iii];
         ucs4text[num_chars] = (z-240)*262144 + (y -128)*4096 + (x-128)*64 + (w-128);
      }

    if((248<=z)&&(z<=251))
      {
         iii++; y = text[iii];
         iii++; x = text[iii];
         iii++; w = text[iii];
         iii++; v = text[iii];
         ucs4text[num_chars] = (z-248)*16777216 + (y -128)*262144 + (x-128)*4096 + (w-128)*64 +(v-128);
      }

    if((252==z)||(z==253))
      {
         iii++; y = text[iii];
         iii++; x = text[iii];
         iii++; w = text[iii];
         iii++; v = text[iii];
         u = text[iii];
         ucs4text[num_chars] = (z-252)*1073741824 + (y -128)*16777216   + (x-128)*262144 + (w-128)*4096 +(v-128)*64 + (u-128);
      }

    if((z==254)||(z==255))
      {
         std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: Problem with character: invalid UTF-8 data."<< std::endl;
      }
    // std::cerr << "\nProblem at " << iii << ".\n";
    //
    iii++;
    num_chars++;
     }

   // num_chars now contains the number of characters in the string.
   /*
   std::cout << "Num chars is: "<< num_chars << std::endl;
   */

   /* Initialize FT Library object */
   error = FT_Init_FreeType( &library );
   if (error) {
     std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: FreeType: Could not init Library." << std::endl;
     delete[] ucs4text;
     return;
   }

   /* Initialize FT face object whit FT_New_Memory_Face istead FT_New_Face in order to handle truetype fonts from Android assets*/
   //error = FT_New_Face( library,face_path,0,&face );
   error = FT_New_Memory_Face(library, buffer, buffersize, 0, &face );

   if ( error == FT_Err_Unknown_File_Format ) {
     std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: FreeType: Font was opened, but type not supported." << std::endl;
     delete[] ucs4text;
     return;
   } else if (error) {
     std::cerr << " PNGwriter::plot_text - ERROR **: FreeType: Could not find or load font file." << std::endl;
     delete[] ucs4text;
     return;
   }

   /* Set the Char size */
   error = FT_Set_Char_Size( face,          /* handle to face object           */
                 0,             /* char_width in 1/64th of points  */
                 fontsize*64,   /* char_height in 1/64th of points */
                 100,           /* horizontal device resolution    */
                 100 );         /* vertical device resolution      */

   /* A way of accesing the glyph directly */
   FT_GlyphSlot  slot = face->glyph;  // a small shortcut

   /* Does the font file support kerning? */
   use_kerning = FT_HAS_KERNING( face );

   int n;
   for ( n = 0; n < num_chars; n++ )
     {
    /* Convert character code to glyph index */
    glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

    /* Retrieve kerning distance and move pen position */
    if ( use_kerning && previous&& glyph_index )
      {
         FT_Vector  delta;
         FT_Get_Kerning( face,
                 previous,
                 glyph_index,
                 ft_kerning_default, //FT_KERNING_DEFAULT,
                 &delta );

         /* Transform this kerning distance into rotated space */
         pen.x += (int) (((double) delta.x)*cos(angle));
         pen.y +=  (int) (((double) delta.x)*( sin(angle)));
      }

    /* Set transform */
    FT_Set_Transform( face, &matrix, &pen );

/*set char size*/

    if (error) {
      std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: FreeType: Set char size error." << std::endl;
      delete[] ucs4text;
      return;
    }

    /* Retrieve glyph index from character code */
    glyph_index = FT_Get_Char_Index( face, ucs4text[n] );

    /* Load glyph image into the slot (erase previous one) */
    error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
    if (error) {
      std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: FreeType: Could not load glyph (in loop). (FreeType error " << std::hex << error <<")." << std::endl;
      std::cerr.copyfmt(std::ios(NULL));
      delete[] ucs4text;
      return;
    }

    /* Convert to an anti-aliased bitmap */
    error = FT_Render_Glyph( face->glyph, ft_render_mode_normal );
    if (error) {
      std::cerr << " PNGwriter::plot_text_utf8 - ERROR **: FreeType: Render glyph error." << std::endl;
      delete[] ucs4text;
      return;
    }

    /* Now, draw to our target surface */
    my_draw_bitmap( &slot->bitmap,
            slot->bitmap_left,
            y_start + slot->bitmap_top,
            red,
            green,
            blue );

    /* Advance to the next position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;

    /* record current glyph index */
    previous = glyph_index;
     }

   /* Free the face and the library objects */
   FT_Done_Face    ( face );
   FT_Done_FreeType( library );

   delete[] ucs4text;
}

void FTpngwriter::plot_text( unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, int red, int green, int blue)
{
   plot_text( buffer, buffersize, fontsize, x_start, y_start,  angle,  text,  ((double) red)/65535.0,  ((double) green)/65535.0,  ((double) blue)/65535.0   );
}

void FTpngwriter::plot_text_utf8( unsigned char *buffer, long buffersize, int fontsize, int x_start, int y_start, double angle, char * text, int red, int green, int blue)
{
   plot_text_utf8( buffer, buffersize, fontsize, x_start, y_start,  angle,  text,  ((double) red)/65535.0,  ((double) green)/65535.0,  ((double) blue)/65535.0   );
}

FTpngwriter::~FTpngwriter(){

}
