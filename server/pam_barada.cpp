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

#include <boost/lexical_cast.hpp>
#include <string.h>
#include <stdint.h>
#include <string>
#include <unistd.h>

#include <security/pam_modules.h>
#include <security/pam_ext.h>

#include <syslog.h>

#include "HOTPCredentials.h"
#include "HOTPWindow.h"

#define DEFAULT_PATH "/etc/barada.d/"

#undef DEBUG

using namespace std;
using namespace boost;

static int need_username (pam_handle_t *pamh, char **name)
{
  const void *void_name;
  int retval;

  retval = pam_get_item(pamh, PAM_USER, &void_name);
  if (retval != PAM_SUCCESS || void_name == NULL) {
    return 1;
  } else {
    *name = strdup((char*)void_name);
    return 0;
  }
}

static int need_password (pam_handle_t *pamh, char **password)
{
  const void *void_pass;
  int retval;

  retval = pam_get_item (pamh, PAM_AUTHTOK, &void_pass);
  if (retval != PAM_SUCCESS || void_pass == NULL) {
    return 1;
  }

  *password = strdup((char*)void_pass);
  return 0;
}

static string getLogin(pam_handle_t *pamh) {
  char *login=NULL; 
  
  if (need_username(pamh, &login)) {
    pam_prompt(pamh, PAM_PROMPT_ECHO_ON, &login, "login: ");
  }

  if (login != NULL) {
    pam_set_item(pamh, PAM_USER, login);
    string loginString(login);
    free(login);
    
    return loginString;
  }

  return "";
}

static uint32_t getPasscode(pam_handle_t *pamh) {
  char *password=NULL;

  if (need_password(pamh, &password)) {
    pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &password, "Password: ");
  }

  if (password != NULL) {
    pam_set_item(pamh, PAM_AUTHTOK, password);
    string passwordString(password);
    free(password);

    try {
      return lexical_cast<uint32_t>(passwordString);
    } catch (bad_lexical_cast &exception) {
      return 0;
    }
  }

  return 0;
}


int pam_sm_authenticate(pam_handle_t *pamh, int flags, 
			int argc, const char **argv)
{
  string login;
  uint32_t passcode;
  uint64_t counter;

  if ((login = getLogin(pamh)) == "") {
#ifdef DEBUG
    pam_syslog(pamh, LOG_EMERG, "HOTP: Unable To Get Login...");
#endif
    return PAM_USER_UNKNOWN;
  }

#ifdef DEBUG
  pam_syslog(pamh, LOG_EMERG, ("Got Login: " + login).c_str());
#endif

  if ((passcode = getPasscode(pamh)) == 0) {
#ifdef DEBUG
    pam_syslog(pamh, LOG_EMERG, "HOTP: Unable To Get Passcode...");
#endif
    return PAM_AUTH_ERR;
  }

#ifdef DEBUG
  pam_syslog(pamh, LOG_EMERG, "Got Passcode: %d", passcode);
#endif

  HOTPCredentials user(pamh, DEFAULT_PATH + login);
  HOTPWindow userWindow(pamh, 20, user);

#ifdef DEBUG
  pam_syslog(pamh, LOG_EMERG, "Checking HOTP Window For User...");
#endif
  
  if ((counter = userWindow.verify(passcode))) {
    user.setCounter(counter+1);
    user.serializeCounter(DEFAULT_PATH + login);
    pam_syslog(pamh, LOG_NOTICE, "pam_barada: authentication successful for %s\n", login.c_str());
    return PAM_SUCCESS;
  } else {
    return PAM_AUTH_ERR;
  }
}
int pam_sm_setcred(
    pam_handle_t    *pamh,
    int              flags,
    int              argc,
    const char     **argv
    ){ return PAM_CRED_UNAVAIL; }
