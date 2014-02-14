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

package org.kde.necessitas.mucephi.android_xcas.help;

import java.util.ArrayList;

import org.kde.necessitas.mucephi.android_xcas.R;

import android.content.Context;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Filter;
import android.widget.TextView;

public class CustomHelpAdapter extends ArrayAdapter<Function> {
	
	private ArrayList<Function> originalList;
	private ArrayList<Function> functionList;
	private FunctionFilter filter;
	private Context context;
	
	public CustomHelpAdapter(Context context, int textViewResourceId, ArrayList<Function> functionList) {
		super(context, textViewResourceId, functionList);
		this.functionList = new ArrayList<Function>();
		this.functionList.addAll(functionList);
		this.originalList = new ArrayList<Function>();
		this.originalList.addAll(functionList);
		this.context = context;
	}
	@Override
	  public Filter getFilter() {
		if (filter == null) {
			filter  = new FunctionFilter();
		}
		return filter;
	}
	private class ViewHolder {
		TextView function;
	}
	@Override
	  public View getView(int position, View convertView, ViewGroup parent) {
	
		ViewHolder holder = null;
	
		if (convertView == null) {
			LayoutInflater vi = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = vi.inflate(R.layout.items_help_functons, null);
			holder = new ViewHolder();
			holder.function = (TextView) convertView.findViewById(R.id.item_function);
			convertView.setTag(holder);
		} else {
			holder = (ViewHolder) convertView.getTag();
		}
		Function country = functionList.get(position);
		holder.function.setText(country.getFunction());		
		return convertView;
	}
	
	private class FunctionFilter extends Filter {
		
		@Override
		protected FilterResults performFiltering(CharSequence constraint) {
			constraint = constraint.toString().toLowerCase();
			FilterResults result = new FilterResults();
			if(constraint != null && constraint.toString().length() > 0) {
				ArrayList<Function> filteredItems = new ArrayList<Function>();
				for (int i = 0, l = originalList.size(); i < l; i++) {
					Function function = originalList.get(i);
					if(function.getFunction().toLowerCase().contains(constraint))
					      filteredItems.add(function);
				}
				result.count = filteredItems.size();
				result.values = filteredItems;
			} else {
				synchronized(this) {
					result.values = originalList;
					result.count = originalList.size();
				}
			}
			return result;
		}
		@SuppressWarnings("unchecked")
		   @Override
		   protected void publishResults(CharSequence constraint, 
		     FilterResults results) {
			functionList = (ArrayList<Function>)results.values;
			notifyDataSetChanged();
			clear();
			for (int i = 0, l = functionList.size(); i < l; i++)
			     add(functionList.get(i));
			notifyDataSetInvalidated();
		}
	}
}
 

	