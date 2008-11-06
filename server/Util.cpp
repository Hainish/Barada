#include "Util.h"
#include <stdlib.h>
#include <stdio.h>


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


