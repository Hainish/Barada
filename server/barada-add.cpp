/*
 * Copyright (c) 2008-2010 Moxie Marlinspike
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
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

#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/rand.h>

#include <sys/stat.h>

#include "HOTPCredentials.h"
#include "Util.h"

#define KEY_SIZE 16
#define DEFAULT_PATH "/etc/barada.d/"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

int printUsage(char *name) {
  cerr << endl << "You're running barada " << VERSION << endl << endl;
  
  cerr << "Usage: " << name << " <user_name> <pin_number>" << endl;
  cerr << "user_name:\tA system login name." << endl;
  cerr << "pin_number:\tA multi-digit number." << endl;

  return 0;
}

int main(int argc, char **argv) {

  if (argc != 3) return printUsage(argv[0]);

  try {
    if (is_directory(path(DEFAULT_PATH) / argv[1])) {
      fprintf(stderr, "Error: Barada user already exists...\n");
      return 0;
    }
  } catch( filesystem_error err ) {
    //This is the desired behavior (the user dir does not yet exist)
  }

  unsigned char key[KEY_SIZE];

  if (!(RAND_bytes(key, sizeof(key)))) {
    fprintf(stderr, "Error: Not enough entropy available to generate key.  Try again later.\n");
    return 0;
  }

  create_directory(path(DEFAULT_PATH));
  chmod(DEFAULT_PATH, S_IRUSR | S_IWUSR);

  create_directory(path(DEFAULT_PATH) / argv[1]);
  chmod((string(DEFAULT_PATH) + "/" + argv[1]).c_str(), S_IRUSR | S_IWUSR);

  HOTPCredentials credentials(1, lexical_cast<uint32_t>(argv[2]), key);
  credentials.serialize(string(DEFAULT_PATH) + "/" + argv[1]);

  cout << "Added " << argv[1] << " with key:" << endl;

  std::string keyString = Util::charToHexString(key, sizeof(key));
  cout << keyString << endl;
}
