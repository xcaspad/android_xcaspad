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

import org.giac.calculator.calc.Computer;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class HtmlPlotterActivity extends Activity {
	
	public static WebView webviewPlotter;	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_plotter_html);
		
		webviewPlotter = (WebView)findViewById(R.id.webViewPlotter);                 
	    
	    webviewPlotter.getSettings().setJavaScriptEnabled(true);
	    webviewPlotter.getSettings().setSupportZoom(true);        
	    webviewPlotter.getSettings().setSupportMultipleWindows(true);  
	    
	    webviewPlotter.getSettings().setUseWideViewPort(true);
	    webviewPlotter.getSettings().setLoadWithOverviewMode(true);
	    
	    class htmlPlotInterface{
	    	
	    	public String getPlot(){
	    		//Log.d("Grafico", Computer.getCurrentGraph());
				return Computer.getCurrentGraph();    		
	    	}
	    	
	    }
	    
	    webviewPlotter.addJavascriptInterface(new htmlPlotInterface(), "XCAS");
	    
	    webviewPlotter.setWebChromeClient(new WebChromeClient() {
	    	public void onConsoleMessage(String message, int lineNumber, String sourceID) {
	    		Log.d("xcas debug", message + " -- From line "+ lineNumber + " of " + sourceID);
	    	}	
		});   
	    
	    webviewPlotter.setWebViewClient(new WebViewClient(){
	    		
	    	@Override
	    	public void onPageFinished(WebView view, String url) {	    		
	    		super.onPageFinished(view, url);
	    		//webviewPlotter.loadUrl("javascript:plotFunction();");
	    	}
	    });
	    
	    try{
	    	webviewPlotter.loadUrl("file:///android_asset/plotterframe.html");
	    }catch (Exception e){
			webviewPlotter.loadData(e.getMessage(),"text/plain", "UTF-8");
			e.printStackTrace();			
		}		
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		//webviewPlotter.loadUrl("javascript:plotFunction();");
		webviewPlotter.loadUrl("file:///android_asset/plotterframe.html");
	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if ((keyCode == KeyEvent.KEYCODE_BACK)) {
			MainTabActivity.tabHost.setCurrentTab(0);
	    }
		return true;
	}
}
