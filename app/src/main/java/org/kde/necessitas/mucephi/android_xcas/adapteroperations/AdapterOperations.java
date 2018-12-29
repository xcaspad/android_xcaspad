package org.kde.necessitas.mucephi.android_xcas.adapteroperations;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import org.kde.necessitas.mucephi.android_xcas.R;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Created by leonel on 1/11/17.
 */

public class AdapterOperations extends RecyclerView.Adapter<AdapterOperations.ViewHolder> {

    private List<HolderOperation> mDataset = new ArrayList<HolderOperation>();
    private InputListener inputListener;
    private View.OnCreateContextMenuListener menuListener;


    public static class ViewHolder extends RecyclerView.ViewHolder {

        public ImageView imgInput;
        public ImageView imgOutput;

        public ViewHolder(View v) {
            super(v);
            imgInput = v.findViewById(R.id.img_input);
            imgOutput = v.findViewById(R.id.img_ouput);
        }
    }

    public interface InputListener{

        public void onInputClick(String result);
        public void onOutputClick(String result);
        public void onInputLongClick(View v, int position);
        public void onOutputLongClick(View v, int position);
    }

    public AdapterOperations(List<HolderOperation> myDataset, InputListener inputListener, View.OnCreateContextMenuListener menuListener) {
        mDataset = myDataset;
        this.inputListener = inputListener;
        this.menuListener = menuListener;
    }


    @Override
    public AdapterOperations.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {

        View v = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.row_prettyprint_operation, parent, false);


        ViewHolder vh = new ViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, final int position) {


        Bitmap bitmapInput = mDataset.get(position).getBmpInput();

        if(bitmapInput != null) {
            holder.imgInput.setImageBitmap(bitmapInput);
        }

        Bitmap bitmapOutput = mDataset.get(position).getBmpOutput();

        if(bitmapOutput != null) {
            holder.imgOutput.setImageBitmap(bitmapOutput);
        }

        holder.imgInput.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                inputListener.onInputClick(mDataset.get(position).getStrInput());
            }
        });

        holder.imgOutput.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                inputListener.onOutputClick(mDataset.get(position).getStrOutput());
            }
        });

        holder.imgInput.setOnCreateContextMenuListener(menuListener);
        holder.imgOutput.setOnCreateContextMenuListener(menuListener);

        holder.imgInput.setOnLongClickListener(new View.OnLongClickListener() {

            @Override
            public boolean onLongClick(View v) {
                inputListener.onInputLongClick(v, position);
                return true;
            }
        });

        holder.imgOutput.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                inputListener.onOutputLongClick(v, position);
                return true;
            }
        });

    }

    public void remove(int position){
        if (position < 0 || position >= mDataset.size()) {
            return;
        }
        mDataset.remove(position);
        notifyDataSetChanged();
    }

    public void swap(int i, int j){
        Collections.swap(mDataset, i, j);
    }

    @Override
    public int getItemCount() {
        return mDataset.size();
    }

    public HolderOperation getItem(int position){
        return mDataset.get(position);
    }
}

