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

#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <openssl/rand.h>

#include "HOTPCredentials.h"

#define KEY_SIZE 16
#define DEFAULT_PATH "/etc/hotp.d/"

using namespace std;
using namespace boost;
using namespace boost::filesystem;

int printUsage(char *name) {
  fprintf(stderr, "%s <user_name> <pin_number>\n", name);
  return 0;
}

int main(int argc, char **argv) {

  if (argc < 3) return printUsage(argv[0]);

  if (is_directory(path(DEFAULT_PATH) / argv[1])) {
    fprintf(stderr, "Error: User Already Exists in HOTP System...\n");
    return 0;
  }

  unsigned char key[KEY_SIZE];

  if (!(RAND_bytes(key, KEY_SIZE))) {
    fprintf(stderr, "Error: Not enough entropy available to generate key.  Try again later.\n");
    return 0;
  }

  create_directory(path(DEFAULT_PATH) / argv[1]);

  HOTPCredentials credentials(1, lexical_cast<uint32_t>(argv[2]), key);
  credentials.serialize(string(DEFAULT_PATH) + "/" + argv[1]);

  cout << "Added " << argv[1] << " with key:" << endl;

  int i;
  for (i=0;i<sizeof(key);i++) {
    cout << hex << (uint32_t)key[i];
  }

  cout << hex << endl;
}
