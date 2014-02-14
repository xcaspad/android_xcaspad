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
import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class FileAdapter extends BaseAdapter{
	
	private List<String> item;
	private List<String> path;
	private static LayoutInflater inflater;
	
	public FileAdapter(Context context, List<String> item, List<String> path) {
		this.item = item;
		this.path = path;
		inflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	@Override
	public int getCount() {
		return item.size();
	}

	@Override
	public Object getItem(int position) {		
		return position;
	}

	@Override
	public long getItemId(int position) {		
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View vi=convertView;
        if(convertView==null)
            vi = inflater.inflate(R.layout.row_file_item, null);

        TextView title = (TextView)vi.findViewById(R.id.txt_file_name); // title       
        ImageView thumb_image=(ImageView)vi.findViewById(R.id.img_file_type); // thumb image
        
   
        // Setting all values in listview
        title.setText(item.get(position));
        
        File file = new File(path.get(position));
		
		if (file.isDirectory()){
			thumb_image.setImageResource(R.drawable.folder);
		}
		else {
			thumb_image.setImageResource(R.drawable.doc);
		}
		
        return vi;
	}
	

}
