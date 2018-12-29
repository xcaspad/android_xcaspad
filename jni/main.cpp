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
#include <android/bitmap.h>
#include "cairo/src/cairo.h"

#include <string>
#include <stdexcept>

#include <gen.h>
#include "prettyprint.h"

#include "cpp-base64/base64.h"

#define LOG_TAG ("JNI XcasPad")
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG, ))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

giac::context giac_context;
giac::context * contextptr;

extern "C"{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * reserved);

jstring
Java_org_giac_xcaspad_Calculator_executeOperation(JNIEnv* env, jobject thiz, jstring operation);

jobject
Java_org_giac_xcaspad_Calculator_getBitmap(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation);

jbyteArray
Java_org_giac_xcaspad_Calculator_getImageBytes(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation);

jstring
Java_org_giac_xcaspad_Calculator_getImageBase64(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation);

jobject
createBitmap(JNIEnv* env, int width, int height, const char* format);

}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * reserved){

        LOGD("Init JNI Xcas Pad");

        giac::context * contextptr = &giac_context;

        return JNI_VERSION_1_6;
}

jobject
createBitmap(JNIEnv* env, int width, int height, const char* format){

    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID recycleFunction = env->GetMethodID(bitmapCls, "recycle", "()V");
    env->CallObjectMethod(bitmapCls, recycleFunction);
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF(format);
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass, valueOfBitmapConfigFunction, configName);
    return env->CallStaticObjectMethod(bitmapCls, createBitmapFunction, width, height, bitmapConfig);
}

jstring
Java_org_giac_xcaspad_Calculator_executeOperation(JNIEnv* env, jobject thiz, jstring operation)
{
    const char *compute = env->GetStringUTFChars(operation, 0);

    try{

        giac::gen g(compute, contextptr);

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
Java_org_giac_xcaspad_Calculator_getImageBytes(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation){

    const char *compute = env->GetStringUTFChars(operation, 0);

    giac::gen gen(compute, contextptr);

    xcas::PrettyPrint prettyprint((int)windowsize, (int)fontsize, gen);

    int width = prettyprint.getWidth();
    int height = prettyprint.getHeight();

    cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, width, height);

    try{

        unsigned char* png_array;
        unsigned int lenght;

        prettyprint.draw(cs, &png_array, &lenght, (double)r, (double)g, (double)b);

        jbyteArray array = env->NewByteArray(lenght);
        env->SetByteArrayRegion(array, 0, lenght, reinterpret_cast<jbyte*>(png_array));
        free(png_array);

        return array;

    }catch (std::runtime_error & err){
        env->ThrowNew(env->FindClass("java/lang/Exception"), err.what());
    }
}

jstring
Java_org_giac_xcaspad_Calculator_getImageBase64(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation){

    const char *compute = env->GetStringUTFChars(operation, 0);

    giac::gen gen(compute, contextptr);

    xcas::PrettyPrint prettyprint((int)windowsize, (int)fontsize, gen);

    int width = prettyprint.getWidth();
    int height = prettyprint.getHeight();

    cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565, width, height);

    try{

        unsigned char* png_array;
        unsigned int lenght;

        prettyprint.draw(cs, &png_array, &lenght, (double)r, (double)g, (double)b);

        std::string base64 = base64_encode(png_array, lenght);

        return env->NewStringUTF(base64.c_str());

    }catch (std::runtime_error & err){
        env->ThrowNew(env->FindClass("java/lang/Exception"), err.what());
    }
}

jobject
Java_org_giac_xcaspad_Calculator_getBitmap(JNIEnv* env, jobject thiz, jint windowsize, jint fontsize, jdouble r, jdouble g, jdouble b, jstring operation){

    AndroidBitmapInfo  info;
    void*              pixels;
    int                ret;

    const char *compute = env->GetStringUTFChars(operation, 0);

    giac::gen gen(compute, contextptr);

    xcas::PrettyPrint prettyprint((int)windowsize, (int)fontsize, gen);

    int width = prettyprint.getWidth();
    int height = prettyprint.getHeight();

    jobject bitmap = createBitmap(env, width, height, "ARGB_8888");

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed: error=%d", ret);
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGB_565 && info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGB_565 or ARGB_8888");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed: error=%d", ret);
    }

    cairo_surface_t *cs = NULL;

    if (info.format == ANDROID_BITMAP_FORMAT_RGB_565) {
        cs = cairo_image_surface_create_for_data(pixels, CAIRO_FORMAT_RGB16_565, info.width, info.height, info.stride);
    } else if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        cs = cairo_image_surface_create_for_data(pixels, CAIRO_FORMAT_RGB24, info.width, info.height, info.stride);
    }

    try{

        prettyprint.draw(cs, (double)r, (double)g, (double)b);

    }catch (std::runtime_error & err){
        env->ThrowNew(env->FindClass("java/lang/Exception"), err.what());
    }

    AndroidBitmap_unlockPixels(env, bitmap);

    return bitmap;
}
