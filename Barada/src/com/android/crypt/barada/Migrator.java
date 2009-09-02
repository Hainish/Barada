package com.android.crypt.barada;

import java.io.BufferedInputStream;

import android.app.Activity;


/* Copyright 2008, Stuart O. Anderson */

/* Converts the old secret.key and counter to DB entries */

public class Migrator {
	private final String KEY_FILENAME = "secret.key";
	private final String COUNTER_FILENAME = "counter";
	private Activity ctx;
	
	public Migrator( Activity _ctx, BaradaDatabase db ) {
		ctx = _ctx;
		int count = load_counter();
		
		if( count == -1 ) //file didn't exist, we don't need to migrate
			return;
		
		byte key[] = load_key();
		
		long row_id = db.createProfile( "default", Hex.toString(key) );
		db.updateCounter( row_id, count );
		
//		boolean del_success = ctx.deleteFile(COUNTER_FILENAME);
//		boolean del2_success = ctx.deleteFile(KEY_FILENAME);
	}
	
	 private int load_counter() {
         int counter = 1;
         try {
                 BufferedInputStream input = new BufferedInputStream(ctx
                                 .openFileInput(COUNTER_FILENAME));
                 byte data[] = new byte[4];
                 int n_read = input.read(data);
                 if (n_read != 4) {
                         throw new RuntimeException("Counter read failed");
                 }
                 counter = (data[0] << 24) | ((data[1] & 0xff) << 16)
                                 | ((data[2] & 0xff) << 8) | (data[3] & 0xff);
                 input.close();
         } catch (Exception e) {
        	 return -1;
         }
         return counter;
	 }
	 
	 private byte[] load_key() {
		 byte secret[] = null; 
         try {
                 BufferedInputStream input = new BufferedInputStream(ctx
                                 .openFileInput(KEY_FILENAME));
                 int bytes_read = 0;
                 secret = new byte[Barada.KEY_LENGTH];
                 do {
                         int n_read = input.read(secret, 0, Barada.KEY_LENGTH - bytes_read);

                         if (n_read == -1) {
                                 throw new RuntimeException("Key file corrupt");
                         }
                         bytes_read += n_read;
                 } while (bytes_read < Barada.KEY_LENGTH);
                 input.close();
         } catch (Exception e) {
                 return null;
         }
         return secret;
 }


}
