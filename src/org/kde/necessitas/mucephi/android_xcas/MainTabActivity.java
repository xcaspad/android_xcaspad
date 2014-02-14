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

import org.kde.necessitas.mucephi.android_xcas.help.HelpActivity;

import android.annotation.SuppressLint;
import android.app.TabActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;

@SuppressLint("NewApi") @SuppressWarnings("deprecation")
public class MainTabActivity extends TabActivity {
	
	public static final int REQ_SESSION = 3;
	
	public static String AIDE_FUNCTION = "";
	public static String PLOT_FUNCTION = "";
	
	public static TabHost tabHost;
    
    @SuppressLint("NewApi") @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getActionBar().hide();
        setContentView(R.layout.main);      
        
        new PersistentDataModel(this);
        
        tabHost = getTabHost();
                
        TabSpec tabMain = tabHost.newTabSpec("Xcas Pad");
        tabMain.setIndicator("Xcas", getResources().getDrawable(R.drawable.content_edit));
        Intent xcasIntent = new Intent(this, XcasPadActivity.class);
        tabMain.setContent(xcasIntent);
        
        TabSpec tabHelp = tabHost.newTabSpec("Xcas Help");
        tabHelp.setIndicator("Help", getResources().getDrawable(R.drawable.action_help));
        Intent helpIntent = new Intent(this, HelpActivity.class);
        tabHelp.setContent(helpIntent);
        
        TabSpec tabPlotter = tabHost.newTabSpec("Xcas Plotter");
        tabPlotter.setIndicator("Plotter", getResources().getDrawable(R.drawable.action_help));
        Intent ploterIntent = new Intent(this, HtmlPlotterActivity.class);
        //Intent ploterIntent = new Intent(this, PlotterActivity.class);
        tabPlotter.setContent(ploterIntent);
        
        TabSpec tabSessions = tabHost.newTabSpec("Xcas Sessions");
        tabSessions.setIndicator("Sessions", getResources().getDrawable(R.drawable.action_help));
        Intent sessionsIntent = new Intent(this, SessionsActivity.class);
        sessionsIntent.putExtra("requestCode", REQ_SESSION); 
        tabSessions.setContent(sessionsIntent);

        tabHost.addTab(tabMain);
        tabHost.addTab(tabHelp); 
        tabHost.addTab(tabPlotter); 
        tabHost.addTab(tabSessions); 		
    }
    
    @Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
    @Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    // Handle item selection
	    switch (item.getItemId()) {
	        case R.id.action_settings:
	        	Intent settings = new Intent(MainTabActivity.this, SettingsActivity.class);
    			startActivityForResult(settings, 0);
	            return true;
	        case R.id.action_savesession:
	        	Intent savession = new Intent(MainTabActivity.this, SaveSessionsActivity.class);
    			startActivityForResult(savession, 1);
	            return true;    
	        case R.id.action_about:
	        	Intent about = new Intent(MainTabActivity.this, AboutActivity.class);
    			startActivityForResult(about, 2);
	            return true; 
	        default:
	            return super.onOptionsItemSelected(item);
	    }
	}
    
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {		
		super.onActivityResult(requestCode, resultCode, data);
	}
}