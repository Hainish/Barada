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

#include "Util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>

#include <openssl/rand.h>
#include <boost/assert.hpp>

#undef DEBUG

void Util::int64ToArrayBigEndian(unsigned char *a, uint64_t i) {
  a[0] = (i >> 56) & 0xFF;
  a[1] = (i >> 48) & 0xFF;
  a[2] = (i >> 40) & 0xFF;
  a[3] = (i >> 32) & 0xFF;
  a[4] = (i >> 24) & 0xFF;
  a[5] = (i >> 16) & 0xFF;
  a[6] = (i >> 8)  & 0xFF;
  a[7] = i         & 0xFF;
}


void Util::int32ToArrayBigEndian(unsigned char *a, uint32_t i) {
  a[0] = (i >> 24) & 0xFF;
  a[1] = (i >> 16) & 0xFF;
  a[2] = (i >> 8)  & 0xFF;
  a[3] = i         & 0xFF;
}

void Util::hexDump(const pam_handle_t *pamh, unsigned char *buf, int length) {
  char *output = (char*)malloc((length * 3) + 1);
  int i;
  for (i=0;i<length;i++) {
    sprintf(output+(i*3), "%02X ", *(buf+i));
  }

  pam_syslog(pamh, LOG_EMERG, output);

  free(output);
}

void Util::hexStringToChar(unsigned char *buffer, int len, std::string &hexString) {
  assert(hexString.length() / 2 == (unsigned int)len);

  char nibble[9];
  nibble[8] = '\0';

  uint32_t nibbleConversion;

  for (int i=0;i<len/4;i++) {
    memcpy(nibble, hexString.c_str()+(i*8), 8);
    sscanf(nibble, "%x", &nibbleConversion);
    Util::int32ToArrayBigEndian(buffer+(i*4), nibbleConversion);
  }

  memset(nibble, 0, sizeof(nibble));
  nibbleConversion = 0;
}

std::string Util::charToHexString(const unsigned char* buffer, size_t size) {
  std::stringstream str;
  str.setf(std::ios_base::hex, std::ios::basefield);
  str.fill('0');

  for (size_t i=0;i<size;++i) {
    str << std::setw(2) << (unsigned short)(unsigned char)buffer[i];
  }

  return str.str();
}
