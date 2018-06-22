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

import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.view.ContextMenu;
import android.view.MenuItem;
import android.view.View;

import org.kde.necessitas.mucephi.android_xcas.adapteroperations.HolderOperation;

/**
 * Created by leonel on 29/11/17.
 */

public class OperationContextMenu implements View.OnCreateContextMenuListener{

    private Context context;
    private ContextMenuListener contextMenuListener;

    public OperationContextMenu(Context context, ContextMenuListener contextMenuListener){
        this.context = context;
        this.contextMenuListener = contextMenuListener;
    }

    public interface ContextMenuListener{

        void setHolderOperation(boolean isInput);
        void contextZoomIn();
        void contextCopyText();
        void contextAction(String action);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, final View v, ContextMenu.ContextMenuInfo menuInfo) {

        final MenuItem.OnMenuItemClickListener contextMenu = new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem item) {

                if(v.getId() == R.id.img_input){
                    contextMenuListener.setHolderOperation(true);
                }else{
                    contextMenuListener.setHolderOperation(false);
                }

                int id = item.getItemId();

                if (id == R.id.context_zoom_in) {
                    contextMenuListener.contextZoomIn();
                }
                else if (id == R.id.context_copy_text) {
                    contextMenuListener.contextCopyText();
                }
                else if (id == R.id.context_simplify) {
                    contextMenuListener.contextAction("simplify");
                }
                else if (id == R.id.context_normal) {
                    contextMenuListener.contextAction("normal");
                }
                else if (id == R.id.context_factor) {
                    contextMenuListener.contextAction("factor");
                }
                else if (id == R.id.context_approx) {
                    contextMenuListener.contextAction("approx");
                }
                else if (id == R.id.context_exact) {
                    contextMenuListener.contextAction("exact");
                }else if (id == R.id.context_eval) {
                    contextMenuListener.contextAction("eval");
                }

                return true;
            }
        };

        ((Activity)context).getMenuInflater().inflate(R.menu.item_context, menu);

        menu.findItem(R.id.context_zoom_in).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_copy_text).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_simplify).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_normal).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_factor).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_approx).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_exact).setOnMenuItemClickListener(contextMenu);
        menu.findItem(R.id.context_eval).setOnMenuItemClickListener(contextMenu);
    }
}
