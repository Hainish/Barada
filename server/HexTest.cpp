#include "Util.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <string.h>
#include <iostream>

#include <openssl/rand.h>
#include <boost/assert.hpp>

int main(int argc, char **argv) {
  unsigned char buf[16];
  unsigned char res[16];
  
  if (!(RAND_bytes(buf, sizeof(buf)))) {
    fprintf(stderr, "Error: Not enough entropy available to generate key.  Try again later.\n");
    return 0;
  }

  printf("Generated Random Data:\n");
  for (unsigned int i=0;i<sizeof(buf);i++) {
    printf("%X", buf[i]);
  }
  printf("\n");

  std::string hexString = Util::charToHexString(buf, sizeof(buf));
  std::cout << "Hex String:\n" << hexString << "\n";

  Util::hexStringToChar(res, sizeof(res), hexString);

  assert(hexString.length() == (sizeof(buf) * 2));
  
  printf("Got Back Data:\n");
  for (unsigned int i=0;i<sizeof(res);i++) {
    printf("%X", res[i]);
  }
  printf("\n");

  printf("Comparing...\n");

  for (unsigned int i=0;i<sizeof(buf);i++) {
    assert(buf[i] == res[i]);
  }
  
  printf("Done.\n");
}
