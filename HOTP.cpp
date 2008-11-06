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

#include "HOTP.h"
#include "Util.h"

#include <stdlib.h>
#include <string.h>

#include <openssl/hmac.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>

#define DEBUG 0
#define HMAC_SIZE 20
#define OFFSET_BYTE 19

HOTP::HOTP(const pam_handle_t *pamh, const unsigned char *key, 
	   uint32_t keyLength, uint64_t counter, uint32_t pin) 
{
  this->pamh = pamh;

  uint32_t pinActivatedKeyLength = keyLength + sizeof(pin);
  unsigned char *pinActivatedKey = (unsigned char*)malloc(pinActivatedKeyLength);

  memcpy(pinActivatedKey, key, keyLength);
  Util::int32ToArrayBigEndian(pinActivatedKey + keyLength, pin);


#if DEBUG
  for (int i=0;i<pinActivatedKeyLength;i++) {
    pam_syslog(pamh, LOG_EMERG, "Key[%d]: %x", i, pinActivatedKey[i]);
  }
#endif

  unsigned char counterData[sizeof(counter)];
  Util::int64ToArrayBigEndian(counterData, counter);

  unsigned char hmac[HMAC_SIZE];
  HMAC(EVP_sha1(), pinActivatedKey, pinActivatedKeyLength, 
       counterData, sizeof(counterData), hmac, NULL);

#if DEBUG
  for (int i=0;i<sizeof(hmac);i++) {
    pam_syslog(pamh, LOG_EMERG, "HMAC[%d]: %d", i, hmac[i]);
  }
#endif

  otp = truncate(hmac);

#if DEBUG
  pam_syslog(pamh, LOG_EMERG, "OTP Calculated: %d", otp);
#endif

  memset(counterData, 0, sizeof(counterData));
  memset(hmac, 0, sizeof(hmac));
  memset(pinActivatedKey, 0, pinActivatedKeyLength);

  free(pinActivatedKey);
}

uint32_t HOTP::getOneTimePasscode() {
  return otp;
}

uint32_t HOTP::truncate(unsigned char *hmac) {
  uint8_t offset   = hmac[OFFSET_BYTE] & 0xf;
  uint32_t binCode = 
      (hmac[offset] & 0x7f)   << 24
    | (hmac[offset+1] & 0xff) << 16
    | (hmac[offset+2] & 0xff) <<  8
    | (hmac[offset+3] & 0xff) ;

  return binCode % 1000000;
}
