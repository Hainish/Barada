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

#ifndef __HOTP_H__
#define __HOTP_H__

#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <stdint.h>

class HOTP {

 private:
  const pam_handle_t *pamh;
  uint32_t otp;
  uint32_t truncate(unsigned char *hmac);

 public:
  HOTP(const pam_handle_t *pamh, const unsigned char *key, 
       uint32_t keyLength, uint64_t counter, uint32_t pin);

  uint32_t getOneTimePasscode();
};


#endif
