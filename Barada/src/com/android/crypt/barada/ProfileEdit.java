package com.android.crypt.barada;

import android.app.Activity;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import java.math.BigInteger;

/*
 * Copyright (c) 2008 Moxie Marlinspike
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
public class ProfileEdit extends Activity {

  private EditText profileText;
  private EditText keyText;
  private Button confirmButton;
  private Button cancelButton;

  private BaradaDatabase helper;
  private Long rowId;
  private String key;

  protected void onCreate(Bundle savedInstanceState) {

    super.onCreate(savedInstanceState);
    helper = new BaradaDatabase(this);
    helper.open();
    setContentView(R.layout.profile_edit);

    profileText   = (EditText)findViewById(R.id.new_profile_text);
    keyText       = (EditText)findViewById(R.id.new_key_text);
    confirmButton = (Button)findViewById(R.id.confirm);
    cancelButton  = (Button)findViewById(R.id.cancel);
    rowId         = getRowId(savedInstanceState);

    populateFields();
    registerClick(confirmButton, cancelButton);
  }

  private void populateFields() {
    if (rowId != null) {
      Cursor profile = helper.getProfile(rowId);
      startManagingCursor(profile);

      String key         = BaradaDatabase.getKey(profile);
      String profileName = BaradaDatabase.getProfileName(profile);

      profileText.setText(profileName);
      keyText.setText(key);

      this.key = key;
    }
  }

  protected void onSaveInstanceState(Bundle outState) {
    super.onSaveInstanceState(outState);
    if( rowId != null )
    	outState.putLong(BaradaDatabase.ID, rowId);
  }

  private void saveState() {
    String profile = profileText.getText().toString();
    String key     = keyText.getText().toString();

    if (rowId == null) {
      long id = helper.createProfile(profile, key);
      if (id > 0) rowId = id;
    } else {
      helper.updateProfile(rowId, profile, key, (this.key != key));
    }
  }

  private boolean isProfileFieldValid() {
    if (profileText.getText().toString().length() == 0) {
      Toast.makeText(this, "You must specify a profile name.", Toast.LENGTH_LONG).show();
      return false;
    }
    
    return true;
  }

  private boolean isKeyFieldValid() {
    if (keyText.getText().toString().length() != Barada.KEY_LENGTH*2) {
      Toast.makeText(this, "Key must be " + (Barada.KEY_LENGTH*2) + " characters long.", 
		     Toast.LENGTH_LONG).show();
      return false;
    }

    try {
      new BigInteger(keyText.getText().toString(), 16).toByteArray();
    } catch (NumberFormatException nfe) {
      Toast.makeText(this, "Key contains invalid characers.  Valid characters are [0-9] and [a-f].", 
		     Toast.LENGTH_LONG).show();
      return false;
    }
    
    return true;
  }

  private boolean checkFields() {
    return 
      isProfileFieldValid() && 
      isKeyFieldValid();
  }

  private void registerClick(Button confirm, Button cancel) {
    confirm.setOnClickListener(new View.OnClickListener() {
	public void onClick(View view) {
	  if (checkFields()) {
	    saveState();
	    setResult(RESULT_OK);
	    finish();
	  }
	}
      });

    cancel.setOnClickListener(new View.OnClickListener() {
	public void onClick(View view) {
	  setResult(RESULT_CANCELED);
	  finish();
	}
      });
  }


  private Long getRowId(Bundle savedInstanceState) {
    Long rowId = null;

    if (savedInstanceState != null) {
    	if( savedInstanceState.containsKey(BaradaDatabase.ID ))
    		rowId = savedInstanceState.getLong(BaradaDatabase.ID);
    	else
    		rowId = null;
    }

    if (rowId == null) {
      Bundle extras = getIntent().getExtras();
      if (extras != null) {
	rowId = extras.getLong(BaradaDatabase.ID);
      }
    }

    return rowId;
  }

}