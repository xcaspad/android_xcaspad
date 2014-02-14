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

import org.kde.necessitas.mucephi.android_xcas.R;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.ExpandableListView.OnChildClickListener;
import android.widget.LinearLayout;
import android.widget.TextView;

public class FunctionDetailActivity extends Activity{
	
	SQLiteDatabase miDb;
	String dbpath;

	private TextView lblDetFunctionDef;
	private LinearLayout layoutFunctions;
	private ExpandableListView listDetailFunctions;
	
	private CustomDetailFunctionAdapter adapter;
	
	ArrayList<Parent> arrayParents;
	ArrayList<String> arrayChildren;
	
	Intent intent;
	final Intent resultfunction = new Intent();
	
	String funcdef = "select definition from xcas_config c, aide_languajes l where option ='lang' and c.value = l.id_languaje and id_function = ";
	String related = "select related from  aide_related where id_function = ";
	String examples = "select example from aide_examples where id_function = ";
	String args = "select arguments from  aide_arguments where id_function = ";
	String functions = "select function from  aide_functions where id = ";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_help_detail);
		
		lblDetFunctionDef = (TextView)findViewById(R.id.lbl_det_definition);		
		layoutFunctions = (LinearLayout)findViewById(R.id.layout_function);		
		listDetailFunctions = (ExpandableListView)findViewById(R.id.list_details_function);
		
		intent = getIntent();
		String id = intent.getStringExtra("id");
		String name = intent.getStringExtra("function");
		
		dbpath = this.getFilesDir().getPath() + "/xcas.db";	
		
		buildDetailedfunction(id, name);
		
	}
	
	private void buildDetailedfunction(String id, String name){
		
		
		layoutFunctions.removeAllViews();
		
		arrayParents = new ArrayList<Parent>();  
		
		functions(id);
		detFunctionDef(id);

		buildParentItems(id, examples, "Examples");
		buildParentItems(id, related, "Related");
		buildParentItems(id, args, "Arguments");
		
		adapter = new CustomDetailFunctionAdapter(this,arrayParents);
		listDetailFunctions.setAdapter(adapter);
		
		listDetailFunctions.setOnChildClickListener(new OnChildClickListener() {
			
			@Override
			public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {
				
				String itemValue = (String) adapter.getChild(groupPosition, childPosition);
				
				switch(groupPosition){
					case 0:												
						resultfunction.putExtra("function", itemValue);
						setResult(RESULT_OK, resultfunction);
						finish();	
						break;
					case 1:						
						String id_related = findRelated(itemValue.trim());
						buildDetailedfunction(id_related,itemValue);						
						break;
					case 2:
						break;
				}
				return false;
			}
		});
	}
	
	private String findRelated(String related){
		
		String id_related = "";
		miDb = openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
		
		try{		    			
		    Cursor cur = miDb.rawQuery("SELECT id FROM aide_functions WHERE function = \"" + related + "\"", null);	
		    if(cur.getCount() > 0){
		    	cur.moveToNext();
		    	id_related += cur.getString(0);		
				cur.close();				
			}
		    miDb.close();
	    }
	    catch(Exception ex){
	    	ex.printStackTrace();
	    }
		return id_related;
	}
	
	private void functions(String id){
		
		miDb = openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
		
		try{		    
		    Cursor cur = miDb.rawQuery(functions + id,null);		    
		    if(cur.getCount() > 0){
				while(cur.moveToNext()){							        	
					addFunctionButton(cur.getString(0));
				}
				cur.close();				
			}
		    miDb.close();
	    }
	    catch(Exception ex){
	    	ex.printStackTrace();
	    }
	}
	
	private void addFunctionButton(final String function){
		
		Button btnFunction = new Button(this);
		btnFunction.setText(function);
		btnFunction.setLayoutParams(new LayoutParams(
	        ViewGroup.LayoutParams.WRAP_CONTENT,
	            ViewGroup.LayoutParams.WRAP_CONTENT));
		
		btnFunction.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {					
				resultfunction.putExtra("function", function);
				setResult(RESULT_OK, resultfunction);
				finish();					
			}
		});
		
		layoutFunctions.addView(btnFunction);
	}
	
	private void detFunctionDef(String id){
		
		miDb = openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
		
		try{		    
		    Cursor cur = miDb.rawQuery(funcdef + id,null);		    
		    if(cur.getCount() > 0){
				while(cur.moveToNext()){							        	
					lblDetFunctionDef.setText(cur.getString(0));
				}
				cur.close();				
			}
		    miDb.close();
	    }
	    catch(Exception ex){
	    	ex.printStackTrace();
	    }	
	}
	
	private void buildParentItems(String id, String query, String label){
		
		miDb = openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
		
		try{		    
			Parent parent = new Parent();
            parent.setTitle(label);     
            arrayChildren = new ArrayList<String>();
            
		    Cursor cur = miDb.rawQuery(query + id,null);	
		   
		    if(cur.getCount() > 0){
				while(cur.moveToNext()){							        	
					arrayChildren.add(cur.getString(0));
				}
				cur.close();	
				parent.setArrayChildren(arrayChildren);
				arrayParents.add(parent);
			}
		    miDb.close();
	    }
	    catch(Exception ex){
	    	ex.printStackTrace();
	    }
	}
	
}
