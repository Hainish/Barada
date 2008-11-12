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

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class PinEntry extends Activity {
	/** Called when the activity is first created. */
	String mPassword;
	public static final String KEY_PASS = "password";
	public static final int ACTIVITY_DISPLAY = 0;
	public static final int ACTIVITY_CREATE = 1;
	public static final int KEY_LENGTH = 16;

	public static final String KEY_KEY = "key";

	public static final int DIALOG_PASSWORD = 0;
	public static final int DIALOG_NEW_KEY = 1;

	public static final int codeDigits = 6;
	private static final String COUNTER_FILENAME = "counter";

	private boolean secret_set = false;
	private byte[] secret = { 0 };
	private int counter = 1, mPWCount = 1;
	private final String KEY_FILENAME = "secret.key";

	EditText mPin;

	private void set_new_key() {
		Intent i = new Intent(this, NewKeyEntry.class);
		startActivityForResult(i, ACTIVITY_CREATE);
	}

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		load_key();
		load_counter();

		setContentView(R.layout.main);
		mPassword = "foobar";
		Button genButton = (Button) findViewById(R.id.generate);
		Button keyButton = (Button) findViewById(R.id.new_key);
		mPin = (EditText) findViewById(R.id.pin);

		keyButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View view) {
				if (secret_set) {
					showDialog(DIALOG_NEW_KEY);
				} else {
					set_new_key();
				}
			}
		});

		genButton.setOnClickListener(new View.OnClickListener() {
			public void onClick(View view) {
				boolean success = false;
				try {
					int pin = Integer.parseInt(mPin.getText().toString());
					byte[] augSecret = new byte[secret.length + 4];
					System.arraycopy(secret, 0, augSecret, 0, secret.length);
					for (int j = 0; j < 4; j++) {
						int this_byte = (pin >> ((3 - j) * 8)) & 0xff;
						augSecret[j + secret.length] = (byte) this_byte;
					}
					mPassword = OneTimePasswordAlgorithm.generateOTP(augSecret,
							counter, codeDigits, false, -1);
					mPWCount = counter;
					counter = counter + 1;
					save_counter();
					success = true;
				} catch (NumberFormatException nfe) {
					Toast mToast = Toast.makeText(PinEntry.this,
							R.string.non_integer_pin, Toast.LENGTH_LONG);
					mToast.show();
				} catch (Exception e) {
					mPassword = "fail";
				}
				// }
				mPin.setText("");
				if (success) {
					removeDialog(DIALOG_PASSWORD);
					showDialog(DIALOG_PASSWORD);
				}
			}
		});
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		switch (id) {
		case DIALOG_PASSWORD:
			return new AlertDialog.Builder(PinEntry.this).setTitle(
					R.string.password_dialog_title).setCancelable(true)
					.setMessage("Count: " + mPWCount + "\nPassword: " + mPassword).create();
		case DIALOG_NEW_KEY:
			return new AlertDialog.Builder(PinEntry.this).setTitle(
					R.string.key_gen_check_title).setCancelable(true)
					.setMessage(R.string.key_gen_check_message)
					.setPositiveButton("OK",
							new DialogInterface.OnClickListener() {
								@Override
								public void onClick(DialogInterface arg0,
										int arg1) {
									set_new_key();
								}
							}).create();
		}
		return null;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode,
			Intent intent) {
		super.onActivityResult(requestCode, resultCode, intent);
		Bundle extras = intent.getExtras();

		switch (requestCode) {
		case ACTIVITY_CREATE:
			if (resultCode == RESULT_CANCELED) {
				return;
			}
			secret = extras.getByteArray(KEY_KEY);
			save_key();
		}
	}

	private void save_key() {
		try {
			BufferedOutputStream output = new BufferedOutputStream(this
					.openFileOutput(KEY_FILENAME, MODE_PRIVATE));
			output.write(secret);
			output.flush();
			output.close();
		} catch (Exception e) {
			Toast.makeText(this, "Failed to save key: " + e.getMessage(),
					Toast.LENGTH_SHORT).show();
		}
	}

	private void load_counter() {
		counter = 1;
		try {
			BufferedInputStream input = new BufferedInputStream(this
					.openFileInput(COUNTER_FILENAME));
			byte data[] = new byte[4];
			int n_read = input.read(data);
			if (n_read != 4) {
				throw new RuntimeException("Counter read failed");
			}
			counter = (data[0] << 24) | ((data[1] & 0xff) << 16)
					| ((data[2] & 0xff) << 8) | (data[3] & 0xff);
		} catch (Exception e) {
			Toast.makeText(this, "Failed to load counter, reset to 1",
					Toast.LENGTH_LONG).show();
		}
	}
	
	private void save_counter() {
		try {
			BufferedOutputStream output = new BufferedOutputStream(this
					.openFileOutput(COUNTER_FILENAME, MODE_PRIVATE) );
			byte data[] = new byte[4];
			data[0] = (byte)(counter >> 24);
			data[1] = (byte)(counter >> 16);
			data[2] = (byte)(counter >> 8);
			data[3] = (byte)(counter);
			
			output.write(data);
			output.flush();
			output.close();			
			
		} catch (Exception e) {
			Toast.makeText(this, "Failed to save counter: " + e.getMessage() ,
					Toast.LENGTH_LONG).show();
		}
	}

	private void load_key() {
		try {
			BufferedInputStream input = new BufferedInputStream(this
					.openFileInput(KEY_FILENAME));
			int bytes_read = 0;
			secret = new byte[KEY_LENGTH];
			do {
				int n_read = input.read(secret, 0, KEY_LENGTH - bytes_read);

				if (n_read == -1) {
					throw new RuntimeException("Key file corrupt");
				}
				bytes_read += n_read;
			} while (bytes_read < KEY_LENGTH);
		} catch (Exception e) {
			Toast.makeText(this, "Failed to load key:" + e.getMessage(),
					Toast.LENGTH_LONG).show();
		}
	}

}