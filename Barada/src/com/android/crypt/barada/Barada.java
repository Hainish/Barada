package com.android.crypt.barada;

import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

/*
 * Copyright (c) 2008 Stuart O. Anderson, Moxie Marlinspike
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 */

/**
 * I, MOXIE, am too lazy to write a single sentence about what this class is for.
 */
public class Barada extends ListActivity {

  public static final int KEY_LENGTH = 16;

  private static final int ADD_ID    = Menu.FIRST;
  private static final int DELETE_ID = Menu.FIRST + 1;

  private BaradaDatabase helper;

  public void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.profile_list);

    helper = new BaradaDatabase(this);
    helper.open();
    
    new Migrator( this, helper );
    
    redrawList();
  }

  public boolean onCreateOptionsMenu(Menu menu) {
    super.onCreateOptionsMenu(menu);
    menu.add(0, Menu.FIRST, 0, R.string.profile_add);
    menu.add(0, Menu.FIRST+1, 0, R.string.profile_delete);

    return true;
  }

  public boolean onMenuItemSelected(int id, MenuItem item) {
    switch (item.getItemId()) {
    case ADD_ID:
      createNewProfile();
      return true;
    case DELETE_ID:
      deleteProfile(getListView().getSelectedItemId());
      return true;
    }

    return super.onMenuItemSelected(id, item);
  }

  protected void onListItemClick(ListView l, View v, int position, long id) {
    super.onListItemClick(l, v, position, id);
    Intent i = new Intent(this, ProfileUse.class);
    i.putExtra(BaradaDatabase.ID, id);
    startActivityForResult(i, 1);    
  }

  protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
    super.onActivityResult(requestCode, resultCode, intent);
    redrawList();
  }

  private void redrawList() {
    Cursor profiles = helper.getAllProfiles();
    startManagingCursor(profiles);

    String[] databaseColumnsToDisplay = {BaradaDatabase.PROFILE};
    int[] listColumnsMap              = {R.id.profile_title};

    SimpleCursorAdapter adapter = 
      new SimpleCursorAdapter(this, R.layout.profile_row, profiles, 
			      databaseColumnsToDisplay, listColumnsMap);
    setListAdapter(adapter);
  }

  private void createNewProfile() {
    Intent i = new Intent(this, ProfileEdit.class);
    startActivityForResult(i, 0);
  }

  private void deleteProfile(long id) {
    helper.deleteProfile(id);
    redrawList();
  }

}