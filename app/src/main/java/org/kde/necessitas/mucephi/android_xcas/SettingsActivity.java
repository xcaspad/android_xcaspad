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

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import org.kde.necessitas.mucephi.android_xcas.aidehelp.AideParser;

import java.util.LinkedList;

public class SettingsActivity extends AppCompatActivity {

    private class Lang {

        int id;
        String lang;

        public Lang(int id, String lang) {
            this.id = id;
            this.lang = lang;
        }

        @Override
        public String toString(){
            return lang;
        }

        public int getId(){
            return id;
        }
    }

    final Intent RETURNSETTINGS = new Intent();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        Spinner splangs = findViewById(R.id.spinner_langs);

        final SharedPreferences settings = getSharedPreferences(AppSpace.PREFS_XCASPAD, 0);

        LinkedList<Lang> langs = new LinkedList<Lang>();

        langs.add(new Lang(1, "French"));
        langs.add(new Lang(2, "English"));
        langs.add(new Lang(3, "Spanish"));
        langs.add(new Lang(4, "Greek"));
        langs.add(new Lang(8, "Chinese"));
        langs.add(new Lang(9, "Russian"));

        ArrayAdapter<Lang> spiner_adapter = new ArrayAdapter<Lang>(this, android.R.layout.simple_spinner_item, langs);
        spiner_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        splangs.setAdapter(spiner_adapter);

        String index_lang_help = settings.getString("lang_help", "2");

        for(int pos = 0; pos < langs.size(); pos++) {
            int sel = new Integer(index_lang_help).intValue();
            if(langs.get(pos).id == sel) {
                splangs.setSelection(pos);
                break;
            }
        }

        splangs.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {

            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                if (parent.getId() == R.id.spinner_langs) {
                    String lid = "" + ((Lang) parent.getItemAtPosition(pos)).getId();
                    SharedPreferences.Editor editor = settings.edit();
                    editor.putString("lang_help", lid);
                    editor.commit();
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> view) {

            }

        });

        findViewById(R.id.btn_settings).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                RETURNSETTINGS.putExtra("changed", "lang");
                setResult(RESULT_OK, RETURNSETTINGS);
                finish();
            }
        });
    }
}
