package org.giac.xcaspad;

import android.content.res.AssetManager;

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
        System.loadLibrary("freetype");
        System.loadLibrary("png");
        System.loadLibrary("pngwriter");
        System.loadLibrary("giac");
        System.loadLibrary("xcaspad");
    }

    /*this function retrieve the computed result from an math expression*/

    public native static String executeOperation(String operation);

    /*Its needed start the asset manager in order tho get access to the fonts to build the 2d pretty print results */

    public native static void initAssetManager(AssetManager mgr);

    /*This function convert an string into a 2d pretty print */

    public native static byte[] imageBytes(int windowsize, int fontsize, int fontsizefix, int windowfix, int backgroundcolor, double r, double g, double b, String operation);

    /*Its needed to set an work dir to produce temporary png files as 2d pretty print results  that wil be retrieved and sowed as results*/

    public native static void setWorkDir(String file_png_path);

    /*This class holds the two pretty print expressions the input and output.
      Finally it produces the two 2d pretty print expressions into image bytes.
    */

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

    /*  Finally the 2d pretty print image bytes from the JNI C++ code are retrieved from this function.
        For an optimal 2d pretty print image size, the device resolution is checked.
        Some of this parameters are an horrible hacks (fontsizefix and windowfix) that are needed
        to adjust the 2d pretty print image. please fix it.
    */

    private static byte[] getImageBytes(String input, double r, double g, double b) {


        if (AppSpace.density >= 4.0) {
            return imageBytes(9000, 42, 10,12, 65535, r, g, b, input);
        }
        if (AppSpace.density >= 3.0) {
            return imageBytes(9000, 36, 8,10, 65535, r, g, b, input);
        }
        if (AppSpace.density >= 2.0) {
            return imageBytes(9000, 24, 6,6, 65535, r, g, b, input);
        }
        if (AppSpace.density >= 1.5) {
            return imageBytes(9000, 18, 6,6, 65535, r, g, b, input);
        }
        if (AppSpace.density >= 1.0) {
            return imageBytes(9000, 14, 6,6, 65535, r, g, b, input);
        }
        return imageBytes(9000, 14, 6,6, 65535, r, g, b, input);
    }
}
