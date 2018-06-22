package org.kde.necessitas.mucephi.android_xcas.aidehelp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SearchView;
import android.view.Menu;
import android.view.MenuItem;

import org.json.JSONObject;
import org.kde.necessitas.mucephi.android_xcas.AppSpace;
import org.kde.necessitas.mucephi.android_xcas.R;

import java.util.List;

public class HelpActivity extends AppCompatActivity {

    private final static int ACTIVITY_HELP = 0;

    private String index_lang_help;

    private RecyclerView mRecyclerView;
    private RecyclerView.Adapter mAdapter;
    private RecyclerView.LayoutManager mLayoutManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_help);

        SharedPreferences settings = getSharedPreferences(AppSpace.PREFS_XCASPAD, 0);
        index_lang_help = settings.getString("lang_help", "2");


        mRecyclerView = (RecyclerView) findViewById(R.id.recycler_help);

        mLayoutManager = new LinearLayoutManager(this);
        mRecyclerView.setLayoutManager(mLayoutManager);

        mRecyclerView.setHasFixedSize(true);

        List<JSONObject> mDataset = AideParser.getJSONhelpDataset(this, index_lang_help);

        mAdapter = new AdapterHelp(mDataset, new AdapterHelp.RowCickListener() {
            @Override
            public void onRowClick(Bundle params) {

                Intent functionDetailed = new Intent(getApplicationContext(), HelpDetailActivity.class);
                functionDetailed.putExtras(params);
                startActivityForResult(functionDetailed, ACTIVITY_HELP);
            }
        });

        mRecyclerView.setAdapter(mAdapter);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(requestCode == ACTIVITY_HELP){
            if(resultCode == RESULT_OK){
                setResult(RESULT_OK, data);
                finish();
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.search_help, menu);

        final MenuItem searchItem = menu.findItem(R.id.txt_help_search);
        final SearchView searchView = (SearchView) searchItem.getActionView();

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                ((AdapterHelp)mAdapter).filter(newText);
                return true;
            }
        });

        return true;
    }


}
