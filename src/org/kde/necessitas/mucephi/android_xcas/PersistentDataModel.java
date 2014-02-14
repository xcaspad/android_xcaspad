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
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.LinkedList;

import org.kde.necessitas.mucephi.android_xcas.help.Function;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

public class PersistentDataModel {
	
	SQLiteDatabase miDB;
	private Context context;
	private static String dbpath;
	
	public class Langs{
		int id;
		String lang;
		
		public Langs(int id, String lang){
			super();
			this.id= id;
			this.lang= lang;
		}
		
		@Override
		public String toString(){
			return lang;
		}
		
		public int getId(){
			return id;
		}
	}
	
	public PersistentDataModel(Context context) {
		this.context = context;
		dbpath = context.getFilesDir().getPath() + "/xcas.db";
		setupDatabase(dbpath);
		miDB = context.openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
		miDB.close();
	}
	
	private void openDatabase(){
		miDB = context.openOrCreateDatabase(dbpath, Context.MODE_PRIVATE, null);
	}
	
	
	public LinkedList<Langs> geListLang(){
		openDatabase();
		
		LinkedList<Langs> langs = new LinkedList<Langs>();
		
		Cursor  c = miDB.rawQuery("select _id, lang from list_langs", null);
		c.moveToFirst();
		do{
			langs.add(new Langs(c.getInt(0), c.getString(1)));
		}while(c.moveToNext());
		
		c.close();
		miDB.close();
		return langs;
		
	}
	
	public int getActiveLang(){		
		openDatabase();
		Cursor c = miDB.rawQuery("select rowid from list_langs where _id = (select value from xcas_config where option = 'lang')", null);
		c.moveToFirst();
		miDB.close();
		return c.getInt(0)-1;
	}
	
	public void setActiveLang(int lid){		
		openDatabase();
		miDB.execSQL("UPDATE xcas_config SET value = '" + lid + "' WHERE option = 'lang'");
		miDB.close();
	}
	
	public String getPlotWidth(){
		openDatabase();
		Cursor c = miDB.rawQuery("select value from xcas_config where option = 'plot_width'", null);
		c.moveToFirst();
		miDB.close();
		return c.getString(0);
	}
	
	public String getPlotheight(){
		openDatabase();
		Cursor c = miDB.rawQuery("select value from xcas_config where option = 'plot_height'", null);
		c.moveToFirst();
		miDB.close();
		return c.getString(0);
	}
	
	public void setPlotAxis(String x, String y){
		openDatabase();
		miDB.execSQL("UPDATE xcas_config SET value = '" + x + "' WHERE option = 'plot_width'");
		miDB.execSQL("UPDATE xcas_config SET value = '" + y + "' WHERE option = 'plot_height'");
		miDB.close();
	}
	
	public ArrayList<Function> getLisFunctions(){
		
		openDatabase();
		
		ArrayList<Function> functionList = new ArrayList<Function>();
		
		try{
			   
		    Cursor cur = miDB.rawQuery("SELECT * FROM aide_functions ORDER BY function",null);
		    
		    if(cur.getCount() > 0){
				while(cur.moveToNext()){					
		        	Function function = new Function();
		        	function.setId(cur.getString(0));
		        	function.setFunction(cur.getString(1));
		    		functionList.add(function);
				}
				cur.close();				
			}
	    }
	    catch(Exception ex){
	    	ex.printStackTrace();
	    }
		finally{
			miDB.close();
	    }
		
		return functionList;
	}
	
	public String getFirstRun(){		
		openDatabase();
		Cursor c = miDB.rawQuery("select value from xcas_config where option = 'firstrun'", null);
		c.moveToFirst();
		miDB.close();
		return c.getString(0);
	}
	
	public void setFirstRun(int first){		
		openDatabase();
		miDB.execSQL("UPDATE xcas_config SET value = '" + first + "' WHERE option = 'firstrun'");
		miDB.close();
	}
	
	private void setupDatabase(String dbpath){
		try {
            File f = new File(dbpath);
            if(!f.exists()){            	
	            Log.d("db copy","File Not Exist");
	            InputStream in = context.getAssets().open("xcas.db");
	            OutputStream out = new FileOutputStream(dbpath);
	
	            byte[] buffer = new byte[1024];
	            int length;
	            while ((length = in.read(buffer)) > 0) {
	                out.write(buffer, 0, length);
	            }
	            in.close();
	            out.close();
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            Log.v("db copy","ioexeption");
            e.printStackTrace();
        }
	}
}
