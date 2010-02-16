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

#include <stdint.h>

#include "HOTPWindow.h"
#include "HOTP.h"
#include "HOTPCredentials.h"

#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <syslog.h>

#undef DEBUG

HOTPWindow::HOTPWindow(const pam_handle_t *pamh, int windowSize, HOTPCredentials &credentials) {
  this->pamh       = pamh;
  this->windowSize = windowSize;
  this->key        = credentials.getKey();
  this->keyLength  = credentials.getKeyLength();
  this->counter    = credentials.getCounter();
  this->pin        = credentials.getPin();
}

uint64_t HOTPWindow::verify(uint32_t remoteOtp) {
  int i;
  
  for (i=0;i<windowSize;i++) {
    HOTP localOtp(pamh, key, keyLength, counter+i, pin);

#ifdef DEBUG
    pam_syslog(pamh, LOG_EMERG, "Counter: %d, Given OTP: %d -- Computed OTP: %d", 
	       counter+i, remoteOtp, localOtp.getOneTimePasscode());
#endif

    if (localOtp.getOneTimePasscode() == remoteOtp) {
      return counter+i;
    }
  }
  
  return 0;
}

