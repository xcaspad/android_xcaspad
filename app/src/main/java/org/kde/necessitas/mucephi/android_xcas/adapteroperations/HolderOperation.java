package org.kde.necessitas.mucephi.android_xcas.adapteroperations;

/**
 * Created by leonel on 24/11/17.
 */

public class HolderOperation {

    private String str_input;
    private String str_output;
    private byte[] bmp_input;
    private byte[] bmp_output;

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

    public byte[] getBmpInput() {
        return bmp_input;
    }

    public void setBmpInput(byte[] bmp_input) {
        this.bmp_input = bmp_input;
    }

    public byte[] getBmpOutput() {
        return bmp_output;
    }

    public void setBmpOutput(byte[] bmp_output) {
        this.bmp_output = bmp_output;
    }
}
