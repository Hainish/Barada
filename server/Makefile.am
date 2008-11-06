AM_CXXFLAGS = -Wall -fPIC -ggdb

noinst_HEADERS = HOTPCredentials.h HOTPWindow.h HOTP.h Util.h 

bin_PROGRAMS = hotpadd

hotpadd_SOURCES = hotpadd.cpp HOTPCredentials.cpp Util.cpp

hotpadd_LDFLAGS = -lboost_filesystem

pam_hotp_sodir = /lib/security/

pam_hotp_so_PROGRAMS = pam_hotp.so

pam_hotp_so_SOURCES = HOTP.cpp HOTPCredentials.cpp HOTPWindow.cpp pam_hotp.cpp Util.cpp 

pam_hotp_so_LDFLAGS = -shared
