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

import android.Manifest;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;

import org.giac.xcaspad.Calculator;
import org.kde.necessitas.mucephi.android_xcas.adapteroperations.AdapterOperations;
import org.kde.necessitas.mucephi.android_xcas.adapteroperations.HolderOperation;
import org.kde.necessitas.mucephi.android_xcas.adapteroperations.TouchCallback;
import org.kde.necessitas.mucephi.android_xcas.aidehelp.AideParser;
import org.kde.necessitas.mucephi.android_xcas.aidehelp.HelpActivity;

import java.util.ArrayList;
import java.util.List;


public class XcasPadActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private final static int ACTIVITY_HELP = 0;
    private final static int ACTIVITY_SETTINGS = 1;
    private final static int ACTIVITY_ZOOMIN = 2;
    private static final int PERMISSIONS_REQUEST_WRITE_EXTERNAL = 0;

    private RecyclerView mRecyclerView;
    private AdapterOperations mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;
    private List<HolderOperation> operations = new ArrayList<HolderOperation>();
    private HolderOperation contextHolderOperation;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_xcas_pad);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        final EditText txtInputOperation = findViewById(R.id.txt_input);

        mRecyclerView = findViewById(R.id.recycler_outputs);
        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);
        mRecyclerView.setHasFixedSize(true);

        /*  For every operation we have two components: entry to do calculus and his result.
            This adapter perform the contextual operations like to retrieve the text result into the input operation text box
         */

        AdapterOperations.InputListener inputListener = new AdapterOperations.InputListener() {
            @Override
            public void onInputClick(String result) {
                txtInputOperation.setText(result);
            }

            @Override
            public void onOutputClick(String result) {
                txtInputOperation.setText(result);
            }

            @Override
            public void onInputLongClick(View v, int position) {
                contextHolderOperation = mAdapter.getItem(position);
                v.showContextMenu();
            }

            @Override
            public void onOutputLongClick(View v, int position) {
                contextHolderOperation = mAdapter.getItem(position);
                v.showContextMenu();
            }
        };

        /*  This adapter perform the contextual operations that appears on a long click input or output
         */

        OperationContextMenu operationContextMenu = new OperationContextMenu(this, new OperationContextMenu.ContextMenuListener() {

            Bitmap img;
            String operation;

            @Override
            public void setHolderOperation(boolean isInput) {

                img = contextHolderOperation.getBmpOutput();
                operation = contextHolderOperation.getStrOutput();

                if(isInput){
                    img = contextHolderOperation.getBmpInput();
                    operation = contextHolderOperation.getStrInput();
                }
            }

            @Override
            public void contextZoomIn() {
                Intent intentZoomIn = new Intent(getApplicationContext(), ZoomInActivity.class);
                intentZoomIn.putExtra("operation", operation);
                startActivityForResult(intentZoomIn, ACTIVITY_ZOOMIN);
            }

            @Override
            public void contextCopyText() {
                ClipboardManager clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                ClipData clip = ClipData.newPlainText("xcas pad", operation);
                clipboard.setPrimaryClip(clip);
            }

            @Override
            public void contextAction(String action) {
                performOperation(String.format("%s(%s)",action, operation));
            }
        });

        /* The adapter that handles the list of input and their output operations.
         */

        mAdapter = new AdapterOperations(operations, inputListener, operationContextMenu);
        mRecyclerView.setAdapter(mAdapter);

        new ItemTouchHelper(new TouchCallback(mAdapter)).attachToRecyclerView(mRecyclerView);

        /*This function loads the session from one file .cas */

        SessionFromSender.load(this, new SessionFromSender.OnLoadFromSender() {
            @Override
            public void loadInBackground(List<String> lists) {
                for(String op: lists){
                    operations.add(Calculator.prettyPrint(op));
                }
            }

            @Override
            public void onFinishLoading() {
                mAdapter.notifyDataSetChanged();
            }
        });

        /*This is the button that perform the operations.*/

        findViewById(R.id.btn_doit).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                String input = txtInputOperation.getText().toString();
                //input = "some_tests";
                if(input.equals("some_tests")) {
                    for (String op : TestsOperations.operations) {
                        performOperation(op);
                    }
                }
                else if(input.contains("plot")){
                    showMessage("Graphics are not longer supported by this version.");
                }
                else {
                    performOperation(input);
                    txtInputOperation.setText("");
                }
            }
        });
    }

    /* This is the main function that reads the input an makes the operation through the callings to the JNI
     */
    private void performOperation(String input){
        HolderOperation operation = Calculator.prettyPrint(input);
        operations.add(operation);
        mAdapter.notifyDataSetChanged();
        mRecyclerView.scrollToPosition(mAdapter.getItemCount()-1);
    }


    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.xcas_pad, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.action_settings) {
            Intent intentSettings = new Intent(getApplicationContext(), SettingsActivity.class);
            startActivityForResult(intentSettings, ACTIVITY_SETTINGS);
            return true;
        }
        else if (id == R.id.action_save_session){
            if(requestWritePermission())
                SaveSession.download(this, operations);
        }
        else if(id == R.id.action_clear_session){
            operations.clear();
            mAdapter.notifyDataSetChanged();
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.nav_help) {

            Intent intentHelp = new Intent(getApplicationContext(), HelpActivity.class);
            startActivityForResult(intentHelp, ACTIVITY_HELP);

        } else if (id == R.id.nav_settings) {
            Intent intentSettings = new Intent(getApplicationContext(), SettingsActivity.class);
            startActivityForResult(intentSettings, ACTIVITY_SETTINGS);
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(requestCode == ACTIVITY_HELP){
            if(resultCode == RESULT_OK){
                ((EditText)findViewById(R.id.txt_input)).setText(data.getStringExtra("function"));
            }
        }
        else if(requestCode == ACTIVITY_SETTINGS){
            if(resultCode == RESULT_OK){
                if(data.getStringExtra("changed").equals("lang")){
                    AideParser.reset();
                }
            }
        }
    }

    private boolean requestWritePermission() {

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                    Manifest.permission.WRITE_EXTERNAL_STORAGE)) {

            } else {

                ActivityCompat.requestPermissions(this,
                        new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                        PERMISSIONS_REQUEST_WRITE_EXTERNAL);
            }

            return false;
        }

        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case PERMISSIONS_REQUEST_WRITE_EXTERNAL: {

                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {

                    SaveSession.download(this, operations);

                } else {

                }
                return;
            }
        }
    }

    private void showMessage(String message){
        AlertDialog.Builder builder = new AlertDialog.Builder(XcasPadActivity.this);

        builder.setMessage(message)
                .setTitle(R.string.app_name);

        AlertDialog dialog = builder.create();

        dialog.show();
    }
}
