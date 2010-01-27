package com.android.crypt.barada;

import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.database.Cursor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

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
public class ProfileUse extends Activity {

  private static final int CODE_LENGTH = 6;
  private Context mContext;
  private EditText pinText;
  private Button generateButton;

  private BaradaDatabase helper;
  private Long rowId;

  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    mContext = this;
    helper = new BaradaDatabase(this);
    helper.open();
    setContentView(R.layout.profile_use);

    pinText        = (EditText)findViewById(R.id.pin_text);
    generateButton = (Button)findViewById(R.id.generate);
    rowId          = getRowId(savedInstanceState);

    assert(rowId != null);

    registerGenerateClick(generateButton);
  }

  protected void onSaveInstanceState(Bundle outState) {
    super.onSaveInstanceState(outState);
    outState.putLong(BaradaDatabase.ID, rowId);
  }

  private void registerGenerateClick(Button confirm) {
    generateButton.setOnClickListener(new View.OnClickListener() {
	public void onClick(View view) {
	  try {
	    Cursor profile = helper.getProfile(rowId);
	    startManagingCursor(profile);

	    long pin     = 0;
	    try { 
	    	pin = Long.parseLong(pinText.getText().toString());	    
	    }
	    catch( NumberFormatException e ) {
	    	Toast.makeText(mContext, "That pin is not a valid number", Toast.LENGTH_LONG ).show();
	    	return;
	    }
	    String key   = BaradaDatabase.getKey(profile);
	    long counter = BaradaDatabase.getCounter(profile);
	    
	    String otp   = OneTimePasswordAlgorithm.generateOTP(key, pin, counter, 
								CODE_LENGTH, false, -1);
	    helper.updateCounter(rowId, ++counter);
	    displayPassword(otp);
	  } catch (NoSuchAlgorithmException nsae) {
	    throw new AssertionError(nsae);
	  } catch (InvalidKeyException ike) {
	    throw new AssertionError(ike);
	  }
	}
      });
  }

  private void displayPassword(String otp) {
    new AlertDialog.Builder(this)
      .setIcon(R.drawable.icon)
      .setTitle("Generating OTP...")
      .setMessage("Password: " + otp)
      .setPositiveButton("Thanks!", new DialogInterface.OnClickListener() {
	  public void onClick(DialogInterface dialog, int whichButton) {
	    setResult(RESULT_OK);
	    finish();
	  }
	}).create().show();
  }

  private Long getRowId(Bundle savedInstanceState) {
    Long rowId = null;

    if (savedInstanceState != null) {
      rowId = savedInstanceState.getLong(BaradaDatabase.ID);
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