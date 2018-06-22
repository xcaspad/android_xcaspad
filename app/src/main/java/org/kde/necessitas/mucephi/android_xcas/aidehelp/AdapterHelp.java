package org.kde.necessitas.mucephi.android_xcas.aidehelp;

import android.os.Bundle;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;
import org.kde.necessitas.mucephi.android_xcas.R;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by leonel on 23/11/17.
 */

public class AdapterHelp extends RecyclerView.Adapter<AdapterHelp.ViewHolder>{

    private List<JSONObject> mDataset;
    private List<JSONObject> mCleanCopyDataset;
    private RowCickListener rowCickListener;

    public static class ViewHolder extends RecyclerView.ViewHolder{

        private TextView itemviewfunction;

        public ViewHolder(View itemView) {
            super(itemView);

            itemviewfunction = itemView.findViewById(R.id.txt_item_function);
        }
    }

    public interface RowCickListener{

        public void onRowClick(Bundle params);
    }

    public AdapterHelp(List<JSONObject> mDataset, RowCickListener rowCickListener){

        this.mDataset = mDataset;
        this.mCleanCopyDataset = mDataset;
        this.rowCickListener = rowCickListener;
    }

    @Override
    public AdapterHelp.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.row_item_help, parent, false);


        AdapterHelp.ViewHolder vh = new AdapterHelp.ViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {

        final JSONObject item = mDataset.get(position);

        try {

            holder.itemviewfunction.setText(item.getString("function"));

        } catch (JSONException e) {
            e.printStackTrace();
        }

        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Bundle options = new Bundle();

                try {
                    options.putString("function", item.getString("function"));
                    options.putString("describe", item.getString("describe"));
                    options.putString("args", item.getString("args"));
                    options.putStringArrayList("related", (ArrayList<String>)item.get("related"));
                    options.putStringArrayList("examples", (ArrayList<String>)item.get("examples"));
                } catch (JSONException e) {
                    e.printStackTrace();
                }

                rowCickListener.onRowClick(options);
            }
        });
    }

    public void filter(String charText){

        charText = charText.toLowerCase();

        mDataset = new ArrayList<JSONObject>();

        if (charText.length() == 0) {
            mDataset.addAll(mCleanCopyDataset);
        } else {
            for (JSONObject item : mCleanCopyDataset) {

                try {

                    if (item.getString("function").toLowerCase().contains(charText)) {
                        mDataset.add(item);
                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        }

        notifyDataSetChanged();

    }

    @Override
    public int getItemCount() {
        return mDataset.size();
    }
}
