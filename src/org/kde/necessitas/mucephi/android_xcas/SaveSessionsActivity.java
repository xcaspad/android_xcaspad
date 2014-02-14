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

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class SaveSessionsActivity extends Activity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_savesession);
		
		TextView txtPath = (TextView)findViewById(R.id.txt_current_path);		
		Button btnSave = (Button)findViewById(R.id.btn_save_session);
		final CheckBox saveResults = (CheckBox) findViewById(R.id.check_save_results);
		final EditText txtFileName = (EditText)findViewById(R.id.txt_file_session);
		
		final String xcaspath = Environment.getExternalStorageDirectory().getPath() + "/xcaspad";
		final File folder = new File(xcaspath);
		
		if(!folder.exists())
			folder.mkdir();
		txtPath.setText(folder.getAbsolutePath());
		
		btnSave.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				String filename = "" + txtFileName.getText().toString();
				if(!filename.equals("")){															    
				    File script = new File(folder, filename);			
					XcasPadActivity.saveCurrentSession(script, saveResults.isChecked());
					finish();
				}
				else{
					Toast.makeText(SaveSessionsActivity.this, R.string.file_hint, Toast.LENGTH_SHORT).show();
				}				
			}
		});
		
	}
}
