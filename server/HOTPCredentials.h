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

#ifndef __HOTP_CREDENTIALS_H__
#define __HOTP_CREDENTIALS_H__

#include <stdint.h>
#include <string>

#include <security/pam_modules.h>
#include <security/pam_ext.h>

using namespace std;

#define KEY_SIZE 16

class HOTPCredentials {

 private:
  const pam_handle_t *pamh;
  unsigned char key[KEY_SIZE];
  uint32_t pin;
  uint64_t counter;

  void loadCounter(const string &path);
  void loadKey(const string &path);
  void loadPin(const string &path);

  void serializePin(const string &path);
  void serializeKey(const string &path);

 public:
  HOTPCredentials(uint64_t counter, uint32_t pin, unsigned char *key);
  HOTPCredentials(const pam_handle_t *pamh, const string &path);
  void setCounter(uint64_t counter);
  void serializeCounter(const string &path);
  void serialize(const string &path);
  uint64_t getCounter();
  uint32_t getPin();
  const unsigned char* getKey();
  uint32_t getKeyLength();
};
#endif
