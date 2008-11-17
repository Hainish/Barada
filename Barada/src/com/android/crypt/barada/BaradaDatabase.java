package com.android.crypt.barada;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

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
public class BaradaDatabase {
  
  private DatabaseHelper helper;
  private SQLiteDatabase db;

  public static final String ID             = "_id";
  public static final String PROFILE        = "profile";
  public static final String KEY            = "key";
  public static final String COUNTER        = "counter";

  private static final String DATABASE_NAME  = "barada";
  private static final String DATABASE_TABLE = "barada";
  private static final int DATABASE_VERSION  = 1;

  private static final String DATABASE_CREATE =
    "CREATE TABLE " + DATABASE_TABLE + " (_id INTEGER PRIMARY KEY AUTOINCREMENT, " + 
    PROFILE + " TEXT NOT NULL, " + KEY + " TEXT NOT NULL, " + COUNTER + " MEDIUMINT NOT NULL);";

  private final Context context;

  private static class DatabaseHelper extends SQLiteOpenHelper {

    DatabaseHelper(Context context) {
      super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    public void onCreate(SQLiteDatabase db) {      
      db.execSQL(DATABASE_CREATE);
    }

    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
      // FIXME 2008-11-17 <moxie> -- We've got to do something here.
      Log.w("BaradaDatabase", "Upgrading database from version " + oldVersion + " to "
	    + newVersion + ", which will destroy all old data");
      db.execSQL("DROP TABLE IF EXISTS barada");
      onCreate(db);
    }
  }  

  public BaradaDatabase(Context context) {
    this.context = context;
  }

  public BaradaDatabase open() throws SQLException {
    helper = new DatabaseHelper(context);
    db     = helper.getWritableDatabase();
    return this;
  }

  public void close() {
    helper.close();
  }

  public long createProfile(String profile, String key) {
    ContentValues profileValues = new ContentValues();
    profileValues.put(PROFILE, profile);
    profileValues.put(KEY, key);
    profileValues.put(COUNTER, "1");

    return db.insert(DATABASE_TABLE, null, profileValues);
  }

  public boolean deleteProfile(long id) {
    return db.delete(DATABASE_TABLE, ID + " = " + id, null) > 0;
  }

  public Cursor getProfile(long id) {
    String columns[] = {ID, PROFILE, KEY, COUNTER};
    Cursor cursor = db.query(true, DATABASE_TABLE, columns, ID + " = " + id, 
			     null, null, null, null, null);

    if (cursor != null) cursor.moveToFirst();
    return cursor;
  }

  public Cursor getAllProfiles() {
    String[] columns = {ID, PROFILE, KEY, COUNTER};
    return db.query(DATABASE_TABLE, columns, null, null, null, null, null);
  }

  public boolean updateCounter(long id, long counter) {
    ContentValues args = new ContentValues();
    args.put(COUNTER, counter);

    return db.update(DATABASE_TABLE, args, ID + " = " + id, null) > 0;
  }

  public boolean updateProfile(long id, String profile, String key, boolean resetCounter) {
    ContentValues args = new ContentValues();
    args.put(PROFILE, profile);
    args.put(KEY, key);

    if (resetCounter)
      args.put(COUNTER, "1");

    return db.update(DATABASE_TABLE, args, ID + " = " + id, null) > 0;
  }

  public static String getKey(Cursor cursor) {
    return cursor.getString(cursor.getColumnIndexOrThrow(KEY));
  }
  
  public static String getProfileName(Cursor cursor) {
    return cursor.getString(cursor.getColumnIndexOrThrow(PROFILE));
  }

  public static long getCounter(Cursor cursor) {
    return cursor.getLong(cursor.getColumnIndexOrThrow(COUNTER));
  }
}