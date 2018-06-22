package org.kde.necessitas.mucephi.android_xcas.aidehelp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ExpandableListView;
import android.widget.LinearLayout;
import android.widget.TextView;


import org.json.JSONException;
import org.json.JSONObject;
import org.kde.necessitas.mucephi.android_xcas.AppSpace;
import org.kde.necessitas.mucephi.android_xcas.R;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;


public class HelpDetailActivity extends AppCompatActivity {

    private static final int EXAMPLES = 0;
    private static final int RELATED = 1;

    private TextView lblDetFunctionDef;
    private LinearLayout layoutFunctions;
    private ExpandableListView listDetailFunctions;

    private AdapterHelpDetail adapter;

    ArrayList<AdapterParentList> arrayParents;
    ArrayList<String> arrayChildren;

    final Intent RETURNFUNCTION = new Intent();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_help_detail);

        lblDetFunctionDef = findViewById(R.id.lbl_det_definition);
        layoutFunctions = findViewById(R.id.layout_function);
        listDetailFunctions = findViewById(R.id.list_details_function);

        buildAideDetail(getIntent());
    }

    private void buildAideDetail(Intent intent){

        arrayParents = new ArrayList<AdapterParentList>();

        layoutFunctions.removeAllViews();

        try {

            String[] functions = intent.getStringExtra("function").split(" ");
            String describe = intent.getStringExtra("describe");
            String args = intent.getStringExtra("args");

            List<String> examples = intent.getStringArrayListExtra("examples");
            List<String> related = intent.getStringArrayListExtra("related");
            List<String> arguments = new ArrayList<String>();
            arguments.add(args);

            for(String function: functions ) {
                addFunctionButton(function);
            }

            lblDetFunctionDef.setText(describe);

            buildParentItems("Examples", examples);
            buildParentItems("Related", related);
            buildParentItems("Arguments", arguments);

        }catch (Exception e){
            e.printStackTrace();
        }

        adapter = new AdapterHelpDetail(this, arrayParents);
        listDetailFunctions.setAdapter(adapter);

        listDetailFunctions.setOnChildClickListener(new ExpandableListView.OnChildClickListener() {

            @Override
            public boolean onChildClick(ExpandableListView parent, View v, int groupPosition, int childPosition, long id) {

                String itemValue = (String) adapter.getChild(groupPosition, childPosition);

                switch(groupPosition){
                    case EXAMPLES:
                        RETURNFUNCTION.putExtra("function", itemValue);
                        setResult(RESULT_OK, RETURNFUNCTION);
                        finish();
                        break;
                    case RELATED:
                        buildRelatedDetail(itemValue);
                        break;
                    case 2:
                        break;
                }
                return false;
            }
        });
    }

    private void buildParentItems(String label, List<String> items){

        AdapterParentList parent = new AdapterParentList();
        parent.setTitle(label);
        arrayChildren = new ArrayList<String>();

        arrayChildren.addAll(items);

        parent.setArrayChildren(arrayChildren);
        arrayParents.add(parent);
    }

    private void buildRelatedDetail(String function) {

        SharedPreferences settings = getSharedPreferences(AppSpace.PREFS_XCASPAD, 0);
        String index_lang_help = settings.getString("lang_help", "2");

        List<JSONObject> mDataset = AideParser.getDataset(this, index_lang_help);

        JSONObject compare = new JSONObject();

        try {
            compare.put("function", function);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        int index = Collections.binarySearch(mDataset, compare, new Comparator<JSONObject>() {
            @Override
            public int compare(JSONObject o1, JSONObject o2) {
                try {
                    return o1.getString("function").compareToIgnoreCase(o2.getString("function"));
                } catch (JSONException e) {
                    e.printStackTrace();
                }

                return 0;
            }
        });

        if(index < 0)
            return;

        JSONObject related = mDataset.get(index);

        Bundle options = new Bundle();

        try {
            options.putString("function", related.getString("function"));
            options.putString("describe", related.getString("describe"));
            options.putString("args", related.getString("args"));
            options.putStringArrayList("related", (ArrayList<String>)related.get("related"));
            options.putStringArrayList("examples", (ArrayList<String>)related.get("examples"));
        } catch (JSONException e) {
            e.printStackTrace();
        }

        Intent intentRelated = new Intent();
        intentRelated.putExtras(options);

        buildAideDetail(intentRelated);

    }

    private void addFunctionButton(final String function){

        Button btnFunction = new Button(this);
        btnFunction.setText(function);
        btnFunction.setLayoutParams(new ViewGroup.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT,
                ViewGroup.LayoutParams.WRAP_CONTENT));

        btnFunction.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View arg0) {
                RETURNFUNCTION.putExtra("function", function);
                setResult(RESULT_OK, RETURNFUNCTION);
                finish();
            }
        });

        layoutFunctions.addView(btnFunction);
    }
}
