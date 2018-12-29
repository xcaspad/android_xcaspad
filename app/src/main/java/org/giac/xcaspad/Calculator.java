package org.giac.xcaspad;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Base64;

import org.kde.necessitas.mucephi.android_xcas.AppSpace;
import org.kde.necessitas.mucephi.android_xcas.adapteroperations.HolderOperation;

/**
 * Created by leonel on 17/10/17.
 *
 * This is the interface to call the native functions from C++ to Java.
 * This turns the input expressions into 2d pretty print results
 */


public class Calculator {

    static {
        System.loadLibrary("xcaspad");
    }

    /*this function retrieve the computed result from an math expression*/

    public native static String executeOperation(String operation);

    public native static Bitmap getBitmap(int windowsize, int fontsize, double r, double g, double b, String operation);

    public native static String getImageBase64(int windowsize, int fontsize, double r, double g, double b, String operation);

    public native static byte[] getImageBytes(int windowsize, int fontsize, double r, double g, double b, String operation);

    public native static byte[] getImageBytesBase64(int windowsize, int fontsize, double r, double g, double b, String operation);

    public static HolderOperation prettyPrint(String input){

        HolderOperation operation = new HolderOperation();

        try{

            String result = executeOperation(input);

            operation.setStrInput(input);
            operation.setStrOutput(result);

            operation.setBmpInput(getImageBytes(input, 0.169, 0.282, 0.498));
            operation.setBmpOutput(getImageBytes(result, 0.204, 0.369, 0.047));
        }
        catch (Exception ex){
            ex.printStackTrace();
        }

        return operation;
    }

    public static Bitmap getImageBytes(String input, double r, double g, double b) {

        return getImageByMethod("array", input, r, g, b);

    }

    private static Bitmap getImageByMethod(String method, String input, double r, double g, double b){

        Bitmap bitmap = null;
        int fontSize = getFontSize();

        if(method.equals("base64")){
            String encodedImage = getImageBase64(9000, fontSize, r, g, b, input);
            byte[] decodedString = Base64.decode(encodedImage, Base64.DEFAULT);
            bitmap = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.length);
        }
        else if(method.equals("array")) {
            byte[]  raw_bytes = getImageBytes(9000, fontSize, r, g, b, input);
            bitmap = BitmapFactory.decodeByteArray(raw_bytes, 0, raw_bytes.length);
        }
        else if(method.equals("bitmap")) {
            bitmap = getBitmap(9000, fontSize, r, g, b, input);
        }

        return bitmap;
    }

    private static int getFontSize(){

        if (AppSpace.density >= 4.0) {
            return 42;
        }
        if (AppSpace.density >= 3.0) {
            return 36;
        }
        if (AppSpace.density >= 2.0) {
            return 24;
        }
        if (AppSpace.density >= 1.5) {
            return 18;
        }
        if (AppSpace.density >= 1.0) {
            return 14;
        }
        return 14;
    }
}
