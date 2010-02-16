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

#ifndef __HOTP_WINDOW_H__
#define __HOTP_WINDOW_H__

#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include "HOTPCredentials.h"

class HOTPWindow {

 private:
  const pam_handle_t *pamh;
  int windowSize;

  uint64_t counter;
  const unsigned char* key;
  uint32_t keyLength;
  uint32_t pin;

 public:
  HOTPWindow(const pam_handle_t *pamh, int windowSize, HOTPCredentials &credentials);
  uint64_t verify(uint32_t remoteOtp);
};


#endif
