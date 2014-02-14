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

import java.util.LinkedList;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;

import org.kde.necessitas.mucephi.android_xcas.PersistentDataModel.Langs;

public class SettingsActivity extends Activity{
	
	private PersistentDataModel persistent;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_settings);
		
		Spinner splangs = (Spinner)findViewById(R.id.spinner_langs);
		final EditText axisx = (EditText)findViewById(R.id.txt_axis_x);
		//final EditText axisy = (EditText)findViewById(R.id.txt_axis_y);
		//Button saveAxis = (Button)findViewById(R.id.btnSavePlotaxis); 
		
		persistent = new PersistentDataModel(this);
		
		LinkedList<Langs> langs = persistent.geListLang();
		
		ArrayAdapter<Langs> spiner_adapter = new ArrayAdapter<Langs>(this, android.R.layout.simple_spinner_item, langs);
		spiner_adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		splangs.setAdapter(spiner_adapter);
		
		int select = persistent.getActiveLang();
		splangs.setSelection(select);
		
		//axisx.setText(persistent.getPlotWidth());
		//axisy.setText(persistent.getPlotheight());
		
		splangs.setOnItemSelectedListener(new OnItemSelectedListener() {

			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int pos,long id) {
				if (parent.getId() == R.id.spinner_langs) {
					int lid = ((Langs) parent.getItemAtPosition(pos)).getId();
					persistent.setActiveLang(lid);
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> view) {
				
			}
			
		});
		
		/*saveAxis.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				persistent.setPlotAxis(axisx.getText().toString(), axisy.getText().toString());
				finish();
			}
		});*/
	}
}
