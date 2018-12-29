package org.kde.necessitas.mucephi.android_xcas.adapteroperations;

import android.graphics.Bitmap;

/**
 * Created by leonel on 24/11/17.
 */

public class HolderOperation {

    private String str_input;
    private String str_output;
    private Bitmap bmp_input;
    private Bitmap bmp_output;

    public String getStrInput() {
        return str_input;
    }

    public void setStrInput(String str_input) {
        this.str_input = str_input;
    }

    public String getStrOutput() {
        return str_output;
    }

    public void setStrOutput(String str_output) {
        this.str_output = str_output;
    }

    public Bitmap getBmpInput() {
        return bmp_input;
    }

    public void setBmpInput(Bitmap bmp_input) {
        this.bmp_input = bmp_input;
    }

    public Bitmap getBmpOutput() {
        return bmp_output;
    }

    public void setBmpOutput(Bitmap bmp_output) {
        this.bmp_output = bmp_output;
    }
}
