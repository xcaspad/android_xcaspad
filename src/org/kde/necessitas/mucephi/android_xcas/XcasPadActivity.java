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

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.app.TabActivity;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.content.res.Configuration;
import android.util.Log;
import android.view.View;
import android.view.inputmethod.InputMethodInfo;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebChromeClient;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.ImageButton;
import org.giac.calculator.calc.*;
import org.giac.calculator.input.CalculatorInputTextView;

//plotfunc([x^2,x^3],x=-2..2,color=[red,yellow],xstep=0.2)

public class XcasPadActivity extends Activity {
	

	ProgressDialog dialogLoading;
	
	private ImageButton executeButton;
	public static WebView webviewMath;	
	private static CalculatorInputTextView editTextMath;	
	
	private static PersistentDataModel persistent;

	private static Handler mainHandler;
	
	private static int expressionID;
	private static ArrayList<String> expressionList;
	
	private static InputMethodManager imeManager;
	
	private static final int ACTIVITY_INPUTMETHOD = 2; 

	static TabActivity parent;
	
	@Override @SuppressLint("SetJavaScriptEnabled")
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		parent = (TabActivity) getParent();
		
		imeManager = (InputMethodManager)getApplicationContext().getSystemService(INPUT_METHOD_SERVICE);
		
		webviewMath = (WebView)findViewById(R.id.webViewMath);                
	    editTextMath = (CalculatorInputTextView)findViewById(R.id.autoCompleteFormulaMath);
	    executeButton = (ImageButton) findViewById(R.id.btnExecuteCalc);  
	    
	    webviewMath.getSettings().setJavaScriptEnabled(true);
	    webviewMath.getSettings().setSupportZoom(true);        
	    webviewMath.getSettings().setSupportMultipleWindows(true); 
	    
	    //webviewMath.getSettings().setUseWideViewPort(true);
	    //webviewMath.getSettings().setLoadWithOverviewMode(true);
	    
	    expressionList = new ArrayList<String>();
	    
	    //miDB = openOrCreateDatabase("xcas.db", Context.MODE_PRIVATE, null); 
	    persistent = new PersistentDataModel(this);
	    
	    dialogLoading = new ProgressDialog(this);
	    dialogLoading.setTitle("Xcas Pad");
	    dialogLoading.setMessage("Working on it...");
	    dialogLoading.setCancelable(true);
	   
	    mainHandler = new Handler(){

	    	@Override
	    	public void handleMessage(Message msg) {
	    		super.handleMessage(msg);
	    		
	    		String[] result = (String[])msg.obj;
	    		
	    		if(result[0].equals("showPlotterFrame")){
	    			parent.getTabHost().setCurrentTab(2);
	    			return;
	    		}
	    		
	    		int index = Integer.parseInt(result[1]);
	    		String computeResult = expressionList.get(index);
	    		
	    		if(result[0].equals("result"))
	    			computeResult = Computer.resultFromGiac(computeResult);
	    		
	    		editTextMath.setText(computeResult);
	    	}
	    };   
	              
	    class htmlXcasInterface{
	    	
	    	@SuppressWarnings("unused")
			public void retriveExpression(String option, String expressionID){             		
	    		Message msg = new Message();
	    		msg.obj = new String[]{ option, expressionID };
	    		mainHandler.sendMessage(msg);
	    	}     
	    	
	    	@SuppressWarnings("unused")
			public String getParsedExpression(int expressionID){
	    		return Computer.parseFromGiacTex(expressionList.get(expressionID));				
	    	}
	    	
	    	@SuppressWarnings("unused")
			public String getResultExpression(int expressionID){
	    		String expression = Computer.resultFromGiacTex(expressionList.get(expressionID));
	    		Log.d("Expression ", expression);
	    		if(expression.equals("$$isgraphic$$")){
	    			Message msg = new Message();
	    			msg.obj = new String[]{ "showPlotterFrame",""};
	    			mainHandler.sendMessage(msg);
	    			return "";
	    		}
	    		return expression;
	    	}	  
	    	
	    	public void sowProgress(){
	    		dialogLoading.show();	    			
	    	}
	    	
	    	public void dismissProgress(){
	    		if(dialogLoading.isShowing())
	    			dialogLoading.dismiss();
	    	}
	    }
	    
	    webviewMath.addJavascriptInterface(new htmlXcasInterface(), "XCAS");
	    
	    webviewMath.setWebChromeClient(new WebChromeClient() {
	    	public void onConsoleMessage(String message, int lineNumber, String sourceID) {
	    		Log.d("xcas debug", message + " -- From line "+ lineNumber + " of " + sourceID);
	    	}		    	
		});   
	    
	    webviewMath.setWebViewClient(new WebViewClient(){
	    		
	    	@Override
	    	public void onPageFinished(WebView view, String url) {	    		
	    		super.onPageFinished(view, url);
	    		initScriptfile();
	    		getStdMathKeyboard();
	    	}
	    });
	    
	    try{
	    	
	    	if(savedInstanceState != null)
	        	webviewMath.restoreState(savedInstanceState);
	    	else
	    		//webviewMath.loadUrl("file:///data/local/tmp/mathframe.html");
	    		webviewMath.loadUrl("file:///android_asset/mathframe.html");
	  
	    }catch (Exception e){
			webviewMath.loadData(e.getMessage(),"text/plain", "UTF-8");
			e.printStackTrace();			
		}    
	    
	    executeButton.setOnClickListener(new View.OnClickListener() {			
			@Override
			public void onClick(View v) {
				performComputation();
			}
		});
	}
	
	
	public static void performComputation(){    	  
		String expression = editTextMath.getText().toString();	
	   	writeMathComputation(expression);
	   	editTextMath.setText("");
	}
	
	public static void runScript(File file){	
		
		String strScript = "";
		InputStream input = null;		
				
		try {
			input = new FileInputStream(file);
			BufferedReader reader = new BufferedReader(new InputStreamReader(input));
			if (input!=null) {							
				while ((strScript = reader.readLine()) != null) {	
				   	writeMathComputation(strScript);					
				}	
			}			
			input.close();
		} catch (FileNotFoundException e) {			
			e.printStackTrace();
		} catch (IOException e) {			
			e.printStackTrace();
		}		
	}
	
	private void initScriptfile(){
							
		try {
			
			final String xcaspath = Environment.getExternalStorageDirectory().getPath() + "/xcaspad";
			final File folder = new File(xcaspath);
			
			if(!folder.exists())
				folder.mkdir();
			
			final File file = new File(folder.getAbsolutePath()+"/init.txt");
			
			//dialogLoading.setMessage("Loading " + file.getName());
			//dialogLoading.show();	
			
			String strScript = "";
			InputStream input = null;	
			
			if(!file.exists())
				file.createNewFile();
			
			input = new FileInputStream(file);
			BufferedReader reader = new BufferedReader(new InputStreamReader(input));
			if (input!=null) {							
				while ((strScript = reader.readLine()) != null) {	
					Computer.resultFromGiacTex(strScript);					
				}	
			}			
			input.close();
			//dialogLoading.dismiss();
		} catch (FileNotFoundException e) {			
			e.printStackTrace();
		} catch (IOException e) {			
			e.printStackTrace();
		}	
	}
	
	private static void writeMathComputation(String expression){
		
		try{       	  
			expressionList.add(expressionID, expression);
			String urlAction = String.format("javascript:(function() {writeMathComputation(%d);})()", expressionID); 
			expressionID++;
			
			webviewMath.loadUrl(urlAction);
	       	  
	  	}catch(Exception ex){        	          		 
	  		ex.printStackTrace();
	  	} 	
	}
	
	public static void saveCurrentSession(File path, boolean saveresults){
		
		try {	
			BufferedOutputStream myOutput = new BufferedOutputStream(new FileOutputStream(path,true));
			for(int i=0; i< expressionList.size(); i++){
				String expression = expressionList.get(i) + "\n";
				myOutput.write(expression.getBytes());
	        	if(saveresults)
	        		myOutput.write(Computer.resultFromGiac(expression).getBytes());
	        }
			myOutput.flush();
			myOutput.close();
		} catch (FileNotFoundException e) {
		    e.printStackTrace();
		} catch (IOException e) {
		    e.printStackTrace();
		}
	}
	
	private void getStdMathKeyboard(){
		

	    String id = Settings.Secure.getString(getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD);
	    
	    if(id.contains("android_xcas")){
	    	return;
	    }
	    
	    for (InputMethodInfo inp : imeManager.getEnabledInputMethodList()){
	    	if(inp.getPackageName().equals("org.kde.necessitas.mucephi.android_xcas")){
	    		imeManager.showInputMethodPicker();
	    		return;
	    	}
	    }
	    
	    new AlertDialog.Builder(this)
		.setIcon(R.drawable.icon_keyb)
		.setTitle("Please, set enable the Xcas Pad Keyboard.")
		.setPositiveButton("Ok", new OnClickListener() {
			@Override
			public void onClick(DialogInterface dialog, int which) {
				Intent inputSettings = new Intent(android.provider.Settings.ACTION_INPUT_METHOD_SETTINGS);
		    	startActivityForResult(inputSettings, ACTIVITY_INPUTMETHOD);
			}
		}).show();
	    
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		
		super.onActivityResult(requestCode, resultCode, data);
		
		if (requestCode == ACTIVITY_INPUTMETHOD){
		    for (InputMethodInfo inp : imeManager.getEnabledInputMethodList()){
		    	if(inp.getPackageName().equals("org.kde.necessitas.mucephi.android_xcas")){
		    		imeManager.showInputMethodPicker(); 
		    	}
		    }
		}
	}
 
	@Override
	protected void onResume() {		
		super.onResume();
		editTextMath.setText(MainTabActivity.AIDE_FUNCTION);
	}
	
	@Override
	protected void onSaveInstanceState(Bundle outState) {    	
		super.onSaveInstanceState(outState);    	
		webviewMath.saveState(outState);    	    	
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {		
		super.onConfigurationChanged(newConfig);
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		expressionID = 0;
	}
}
