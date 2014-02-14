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

package org.kde.necessitas.mucephi.android_xcas.help;

import java.util.ArrayList;
import android.app.Activity;
import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.AdapterView.OnItemClickListener;

import org.kde.necessitas.mucephi.android_xcas.MainTabActivity;
import org.kde.necessitas.mucephi.android_xcas.PersistentDataModel;
import org.kde.necessitas.mucephi.android_xcas.R;

public class HelpActivity extends Activity{
	
	CustomHelpAdapter dataAdapter = null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {	
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_help);
		
		PersistentDataModel database = new PersistentDataModel(this);
		ArrayList<Function> functionList = database.getLisFunctions();
		
		dataAdapter = new CustomHelpAdapter(this,R.layout.items_help_functons, functionList);
		ListView listView = (ListView) findViewById(R.id.listViewfunctions);
		
		listView.setAdapter(dataAdapter);
		
		listView.setTextFilterEnabled(true);
		listView.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View view,int position, long id) {
				
				Function function = (Function) parent.getItemAtPosition(position);

				MainTabActivity.AIDE_FUNCTION = function.getFunction();
				
				Intent intentDetail = new Intent(HelpActivity.this, FunctionDetailActivity.class);
				intentDetail.putExtra("id", function.getId());
				intentDetail.putExtra("function", function.getFunction());
				startActivityForResult(intentDetail, 0);
			}
		}
		);
		
		EditText myFilter = (EditText) findViewById(R.id.txt_axis_x);
		
		myFilter.addTextChangedListener(new TextWatcher() {
			public void afterTextChanged(Editable s) {
			}
			public void beforeTextChanged(CharSequence s, int start, int count, int after) {
			}
			public void onTextChanged(CharSequence s, int start, int before, int count) {
				dataAdapter.getFilter().filter(s.toString());
			}
		});
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {			
		super.onActivityResult(requestCode, resultCode, data);
		
		if (requestCode == 0){

	        if (resultCode == RESULT_OK){
	        	MainTabActivity.AIDE_FUNCTION = data.getStringExtra("function");	       
	        	TabActivity parent = (TabActivity) getParent();
	        	parent.getTabHost().setCurrentTab(0);	        	
	        }
	    }
	}
	
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if ((keyCode == KeyEvent.KEYCODE_BACK)) {
			MainTabActivity.tabHost.setCurrentTab(0);
	    }
		return true;
	}

}
