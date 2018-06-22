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

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <string>
#include <stdexcept>

#include <gen.h>
#include "Equation.h"

#include "ftpngwriter.h"

#define LOG_TAG ("JNI XcasPad")
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG, ))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

static AAssetManager* asset_manager;

giac::context giac_context;
giac::context * contextptr;

struct font_face_buffer{
    unsigned char* buffer;
    long size;
}typedef font_face_buffer;

struct rgb{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;
}typedef rgb;


//some horrible global vars
rgb global_rgbcolor;
int global_fonsize_fix = 5;
int global_delta;

int global_font_face;
int global_font_size;
int global_font_color;

FTpngwriter *global_eqwpng;

font_face_buffer helvetica;
font_face_buffer times_bold;
font_face_buffer times_italic;
font_face_buffer times_bold_italic;

font_face_buffer *current_font_face;


struct Point{
  int x;
  int y;
};


const char* file_png = "/data/local/tmp/result.png";

void fl_font(FT_Font face, FT_Fontsize size) {

    global_font_face = face;
    global_font_size = size - global_fonsize_fix;

    switch (global_font_face) {
        case FL_HELVETICA:
            current_font_face = &helvetica;
            break;
        case FL_TIMES_BOLD:
            current_font_face = &times_bold;
            break;
        case FL_TIMES_ITALIC:
            current_font_face = &times_italic;
            break;
        case FL_TIMES_BOLD_ITALIC:
            current_font_face = &times_bold_italic;
            break;
        default:
            break;
    }
}

void fl_color(FT_Color c) {

    global_font_color = c;
}

double check_fl_width(const char * c) {

    return double(global_eqwpng->get_text_width(current_font_face->buffer, current_font_face->size, global_font_size, c));
}

Point mirror_point(Point p, int x0, int y0, int x1, int y1) {

  double dx, dy, a, b;
  long x2, y2;
  Point p1;

  dx = (double)(x1 - x0);
  dy = (double)(y1 - y0);

  a = (dx * dx - dy * dy) / (dx * dx + dy * dy);
  b = 2 * dx * dy / (dx * dx + dy * dy);

  x2 = round(a * (p.x - x0) + b * (p.y - y0) + x0);
  y2 = round(b * (p.x - x0) - a * (p.y - y0) + y0);

  p1.x = (int) x2;
  p1.y = (int) y2;

  return p1;
}

void check_fl_draw(const char * ch, int i0, int j0) {

  global_eqwpng->plot_text_utf8(current_font_face->buffer, current_font_face->size, global_font_size, i0, global_delta - j0, 0.0, ch, global_rgbcolor.r, global_rgbcolor.g, global_rgbcolor.b);
}

void check_fl_rectf(int x, int y, int w, int h) {

  Point p1 {x,y};
  Point p2 {w,h};

  p1 = mirror_point(p1, 0, 0, 1, 0);
  p2 = mirror_point(p2, 0, 0, 1, 0);

  global_eqwpng->square(p1.x, p1.y + global_delta, p2.x, p2.y + global_delta,  global_rgbcolor.r, global_rgbcolor.g, global_rgbcolor.b);
}

void check_fl_line(int i0, int j0, int i1, int j1) {

  Point p1 {i0,j0};
  Point p2 {i1,j1};

  p1 = mirror_point(p1, 0, 0, 1, 0);
  p2 = mirror_point(p2, 0, 0, 1, 0);

  global_eqwpng->line(p1.x, p1.y + global_delta, p2.x, p2.y + global_delta,  global_rgbcolor.r, global_rgbcolor.g, global_rgbcolor.b);
}

void check_fl_arc(int x, int y, int w, int h, double a1, double a2) {

  Point p1 {x,y};
  Point p2 {w,h};

  p1 = mirror_point(p1, 0, 0, 1, 0);
  p2 = mirror_point(p2, 0, 0, 1, 0);

  global_eqwpng->line(p1.x, p1.y + global_delta, p1.x + w, p1.y + h + global_delta,  global_rgbcolor.r, global_rgbcolor.g, global_rgbcolor.b);

}

font_face_buffer get_font_face_buffer(const char * asset){

    font_face_buffer font_face;

    AAsset* file = AAssetManager_open(asset_manager, asset, AASSET_MODE_BUFFER);

    if(file == NULL){
         LOGD("Error loading asset: %s", asset);
    }

    size_t fileLength = AAsset_getLength(file);
    const char* fileContent = new char[fileLength];
    AAsset_read(file, fileContent, fileLength);

    font_face.buffer = fileContent;
    font_face.size = fileLength;

    return font_face;
}

void init_assets_fonts(){

    helvetica = get_font_face_buffer("Helvetica Regular.ttf");
    times_bold = get_font_face_buffer("Times Bold.ttf");
    times_italic = get_font_face_buffer("Times Italic.ttf");
    times_bold_italic = get_font_face_buffer("Times Bold Italic.ttf");
}

extern "C"{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * reserved);

jstring
Java_org_giac_xcaspad_Calculator_executeOperation(JNIEnv* env, jobject thiz, jstring operation);

jbyteArray
Java_org_giac_xcaspad_Calculator_imageBytes(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jint fontsizefix, jint windowfix, jint backgroundcolor, jdouble r, jdouble g, jdouble b, jstring operation);

void JNICALL Java_org_giac_xcaspad_Calculator_initAssetManager( JNIEnv* env, jobject thiz, jobject java_asset_manager);

void JNICALL Java_org_giac_xcaspad_Calculator_setWorkDir( JNIEnv* env, jobject thiz, jstring file_png_path);

static void signal_handler(int signal, siginfo_t *info, void *reserved);

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * reserved){

        LOGD("Init JNI Xcas Pad");

        giac::context * contextptr = &giac_context;

        struct sigaction handler;
        memset(&handler, 0, sizeof(handler));
        handler.sa_sigaction = signal_handler;
        handler.sa_flags = SA_SIGINFO;
        sigaction(SIGILL, &handler, NULL);
        sigaction(SIGABRT, &handler, NULL);
        sigaction(SIGBUS, &handler, NULL);
        sigaction(SIGFPE, &handler, NULL);
        sigaction(SIGSEGV, &handler, NULL);
        //sigaction(SIGSTKFLT, &handler, NULL);

        return JNI_VERSION_1_6;
}

void JNICALL Java_org_giac_xcaspad_Calculator_initAssetManager( JNIEnv* env, jobject thiz, jobject java_asset_manager){

    asset_manager = AAssetManager_fromJava(env, java_asset_manager);

    if(asset_manager == NULL){
         LOGD("Error loading asset manager");
    }

    init_assets_fonts();
}

void JNICALL Java_org_giac_xcaspad_Calculator_setWorkDir( JNIEnv* env, jobject thiz, jstring file_png_path)
{
    file_png = strcat(env->GetStringUTFChars(file_png_path, 0), "/result.png");

    LOGD("File path: %s", file_png);
}

jstring
Java_org_giac_xcaspad_Calculator_executeOperation(JNIEnv* env, jobject thiz, jstring operation)
{
    const char *compute = env->GetStringUTFChars(operation, 0);

    try{

        giac::gen g(compute, contextptr);

        g = eval(g, contextptr);
        g = eval(g, contextptr);

        return env->NewStringUTF(g.print().c_str());
    }
    catch (std::runtime_error & err){
        //std::string error = std::string("Error:") + std::string(err.what());
        //return env->NewStringUTF(error.c_str());
        env->ThrowNew(env->FindClass("java/lang/Exception"), err.what());
    }
}

jbyteArray
Java_org_giac_xcaspad_Calculator_imageBytes(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jint fontsizefix, jint windowfix, jint backgroundcolor, jdouble r, jdouble g, jdouble b, jstring operation){

    const char *compute = env->GetStringUTFChars(operation, 0);

    global_fonsize_fix = fontsizefix;

    global_rgbcolor.r = (double)r;
    global_rgbcolor.g = (double)g;
    global_rgbcolor.b = (double)b;

    try{

        giac::gen g(compute, contextptr);

        xcas::Equation *eqw = new xcas::Equation((int)windowsize, (int)fontsize, file_png, g);

        global_delta = eqw->h1 + (int)windowfix;

        global_eqwpng = new FTpngwriter(eqw->w1, eqw->h1 + (int)windowfix * 3, (int)backgroundcolor, file_png);

        eqw->draw();

        global_eqwpng->close();

        delete global_eqwpng;
        delete eqw;

    }catch (std::runtime_error & err){
        env->ThrowNew(env->FindClass("java/lang/Exception"), err.what());
    }

    FILE *imagefile = fopen(file_png, "rb");

    if(imagefile == NULL){
         LOGD("Error opening file output png: %s", file_png);
    }

    fseek(imagefile, 0, SEEK_END);
    long fsize = ftell(imagefile);
    fseek(imagefile, 0, SEEK_SET);

    unsigned char *buffer = (unsigned char *)malloc(fsize + 1);
    fread(buffer, fsize, 1, imagefile);
    fclose(imagefile);

    jbyteArray array = env->NewByteArray(fsize);
    env->SetByteArrayRegion(array, 0, fsize, reinterpret_cast<jbyte*>(buffer));

    free(buffer);

    return array;
}

static void signal_handler(int signal, siginfo_t *info, void *reserved){

  kill(getpid(),SIGKILL);
}

