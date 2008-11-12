/*  Barada: Barada Ain't Recognizing Any Devious Adversaries
 *  Two Factor Authentication for Android
 *  
    Copyright (C) 2008  Stuart O. Anderson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.android.crypt.otp;

import java.math.BigInteger;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class NewKeyEntry extends Activity {
	EditText mKeyText;
	byte new_key[];

	private void parse_key_entry() {
		String key_string = mKeyText.getText().toString();
		if( key_string.length() != PinEntry.KEY_LENGTH*2 ) {			
			Toast.makeText(this, "Key must be " + Integer.toString(PinEntry.KEY_LENGTH*2) + " characters", Toast.LENGTH_LONG )
			.show();
			throw new RuntimeException( "Bad Key Length");
		}
		try {
			BigInteger bi = new BigInteger( key_string, 16 );
			new_key = bi.toByteArray();
		}
		catch( NumberFormatException nfe ) {
			Toast.makeText(this, "Key contains invalid characters.  Valid characters are [0-9] and [a-f]", Toast.LENGTH_LONG )
			.show();
			throw new RuntimeException( "Invalid character in key string" );
		}
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.key_entry);
		mKeyText = (EditText) findViewById(R.id.new_key_text);

		Button confirmButton = (Button) findViewById(R.id.new_key_confirm);
		confirmButton.setOnClickListener(new View.OnClickListener() {

			public void onClick(View view) {
				Intent mIntent = new Intent();
				try {
					parse_key_entry();
				} catch (Exception e) {
					return;
				}

				Bundle bundle = new Bundle();
				bundle.putByteArray(PinEntry.KEY_KEY, new_key);
				mIntent.putExtras(bundle);

				setResult(RESULT_OK, mIntent);
				finish();
			}

		});
		
		Button cancelButton = (Button) findViewById(R.id.new_key_cancel);
		cancelButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View view) {
				Intent mIntent = new Intent();
				setResult(RESULT_CANCELED, mIntent);
				finish();
			}
		});
	}
}
