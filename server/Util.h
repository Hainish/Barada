#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdint.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <syslog.h>
#include <string>
#include <string.h>

class Util {

 public:
  static void int64ToArrayBigEndian(unsigned char *a, uint64_t i);
  static void int32ToArrayBigEndian(unsigned char *a, uint32_t i);
  static void hexDump(const pam_handle_t *pamh, unsigned char *buf, int length);
  static void hexStringToChar(unsigned char *buffer, int len, std::string &hexString);
  static std::string charToHexString(const unsigned char* buffer, size_t size);
};

#endif
