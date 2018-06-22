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

package org.kde.necessitas.mucephi.android_xcas;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by leonel on 29/11/17.
 */

public class SessionFromSender {


    public interface OnLoadFromSender{
        void loadInBackground(List<String> lists);
        void onFinishLoading();
    }

    public static void load(final Context context, final OnLoadFromSender onLoadFromSender) {


        Intent intent = ((Activity) context).getIntent();
        String action = intent.getAction();
        String type = intent.getType();

        if (Intent.ACTION_VIEW.equals(action) && type != null) {
            if ("application/cas".equals(type)) {

                List<String> listOperations = loadFromIntent(context);

                final ProgressDialog progress = new ProgressDialog(context);
                progress.setCancelable(false);
                progress.setMessage("Loading session...");


                class loading extends AsyncTask<List<String>, Void, Void>{


                    @Override
                    protected void onPreExecute() {
                        super.onPreExecute();
                        progress.show();
                    }

                    @Override
                    protected Void doInBackground(List<String>[] lists) {
                        onLoadFromSender.loadInBackground(lists[0]);
                        return null;
                    }

                    @Override
                    protected void onPostExecute(Void aVoid) {
                        super.onPostExecute(aVoid);
                        onLoadFromSender.onFinishLoading();
                        progress.dismiss();
                    }
                };

                new loading().execute(listOperations);
            }
        }
    }

    private static List<String> loadFromIntent(Context context) {

        BufferedReader br = null;
        List<String> list = new ArrayList<String>();

        Uri data = ((Activity) context).getIntent().getData();

        try {
            String line;
            InputStream is = context.getContentResolver().openInputStream(data);
            br = new BufferedReader(new InputStreamReader(is));

            while ((line = br.readLine()) != null) {
                list.add(line);
                br.readLine();
            }

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                if (br != null) br.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return list;
    }
}
