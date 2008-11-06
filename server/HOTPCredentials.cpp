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

#include "HOTPCredentials.h"
#include "Util.h"

#include <string>
#include <vector>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <boost/lexical_cast.hpp>

#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <syslog.h>
#include <string.h>

using namespace std;
using namespace boost;

#define COUNTER_FILE "counter"
#define PIN_FILE "pin"
#define KEY_FILE "key"

#define DEBUG 0

HOTPCredentials::HOTPCredentials(uint64_t counter, uint32_t pin, unsigned char *key) {
  this->counter   = counter;
  this->pin       = pin;
  memcpy(this->key, key, sizeof(this->key));
} 

HOTPCredentials::HOTPCredentials(const pam_handle_t *pamh, const string &path) {
  this->pamh = pamh;
  loadCounter(path);
  loadKey(path);
  loadPin(path);
}

void HOTPCredentials::serialize(const string &path) {
  serializeCounter(path);
  serializeKey(path);
  serializePin(path);
}

void HOTPCredentials::setCounter(uint64_t counter) {
  this->counter = counter;
}

uint64_t HOTPCredentials::getCounter() {
  return counter;
}

uint32_t HOTPCredentials::getPin() {
  return pin;
}

const unsigned char* HOTPCredentials::getKey() {
  return key;
}

uint32_t HOTPCredentials::getKeyLength() {
  return sizeof(key);
}

void HOTPCredentials::serializePin(const string &path) {
  string pinPath = path + "/" + PIN_FILE;
  ofstream pinFile(pinPath.c_str());

  pinFile << this->pin << endl;
  pinFile.close();
}

void HOTPCredentials::serializeKey(const string &path) {
  string keyPath = path + "/" + KEY_FILE;
  ofstream keyFile(keyPath.c_str());
  uint32_t i;

  for (i=0;i<sizeof(key);i++) {
    keyFile << hex << (uint32_t)key[i];
  }

  keyFile << hex << endl;
  keyFile.close();
}

void HOTPCredentials::serializeCounter(const string &path) {
  string counterPath = path + "/" + COUNTER_FILE;
  ofstream counterFile(counterPath.c_str());

  counterFile << this->counter << endl;
  counterFile.close();
}

void HOTPCredentials::loadCounter(const string &path) {
  string counterPath = path + "/" + COUNTER_FILE;

  ifstream counterFile;
  counterFile.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
  counterFile.open(counterPath.c_str());

  string line;
  getline(counterFile, line);

#if DEBUG
  pam_syslog(pamh, LOG_EMERG, "Read Counter Line: %s", line.c_str());  
#endif

  this->counter = lexical_cast<uint64_t>(line);

#if DEBUG
  pam_syslog(pamh, LOG_EMERG, "Cast To Counter: %d", this->counter);
#endif
}

void HOTPCredentials::loadKey(const string &path) {
  string keyPath = path + "/" + KEY_FILE;
  ifstream keyFile;
  keyFile.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
  keyFile.open(keyPath.c_str());

  string line;
  getline(keyFile, line);

#if DEBUG
  pam_syslog(pamh, LOG_EMERG, "Read Key: %s", line.c_str());
#endif

  char nibble[9];
  nibble[8] = '\0';

  uint32_t nibbleConversion;

  for (int i=0;i<sizeof(key)/4;i++) {
    memcpy(nibble, line.c_str()+(i*8), 8);
    sscanf(nibble, "%x", &nibbleConversion);
    Util::int32ToArrayBigEndian(key+(i*4), nibbleConversion);
  }

  memset(nibble, 0, sizeof(nibble));
  nibbleConversion = 0;
  line.clear();
}

void HOTPCredentials::loadPin(const string &path) {
  string pinPath = path + "/" + PIN_FILE;
  ifstream pinFile;
  pinFile.exceptions ( ifstream::eofbit | ifstream::failbit | ifstream::badbit );
  pinFile.open(pinPath.c_str());

  string line;
  getline(pinFile, line);

  this->pin = lexical_cast<uint32_t>(line);
  
#if DEBUG
  pam_syslog(pamh, LOG_EMERG, "Read Pin: %d", this->pin);
#endif
}
