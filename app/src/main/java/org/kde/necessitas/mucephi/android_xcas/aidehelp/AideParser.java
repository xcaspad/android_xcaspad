package org.kde.necessitas.mucephi.android_xcas.aidehelp;

import android.content.Context;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by leonel on 24/11/17.
 */

public final class AideParser {

    private static List<JSONObject> mDataset = null;
    private static AideParser instance = null;

    public static List<JSONObject> getDataset(Context context, String index_lang_help){

        if(instance == null){
            instance = new AideParser();
        }

        return instance.getJSONhelpDataset(context, index_lang_help);
    }

    public static void reset(){
        mDataset = null;
    }

    public static List<JSONObject> getJSONhelpDataset(Context context, String index_lang_help){

        if(mDataset == null){
            mDataset = new ArrayList<JSONObject>();
        }else{
            return mDataset;
        }

        try {

            InputStream instream = context.getAssets().open("help_xcas.json");

            if (instream != null) {

                InputStreamReader inputreader = new InputStreamReader(instream);
                BufferedReader buffreader = new BufferedReader(inputreader);

                String line;

                while ((line = buffreader.readLine()) != null){
                    JSONObject function = new JSONObject(line);
                    function.put("describe", function.getJSONObject("langs").getString(index_lang_help));
                    function.put("related", JArrayToList(function.getJSONArray("related")));
                    function.put("examples", JArrayToList(function.getJSONArray("examples")));
                    mDataset.add(function);
                }

                instream.close();
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return mDataset;
    }

    private static List<String> JArrayToList(JSONArray array){


        List<String> list = new ArrayList<String>();

        for (int i=0; i<array.length(); i++){
            try {
                list.add(array.getString(i));
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }

        return list;
    }
}
