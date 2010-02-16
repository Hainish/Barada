AM_CXXFLAGS = -Wall -fPIC -ggdb

noinst_HEADERS = HOTPCredentials.h HOTPWindow.h HOTP.h Util.h 

noinst_PROGRAMS = HexTest

bin_PROGRAMS = barada-add

barada_add_SOURCES = barada-add.cpp HOTPCredentials.cpp Util.cpp

barada_add_LDFLAGS = $(BOOST_FILESYSTEM)

pam_barada_sodir = /lib/security/

pam_barada_so_PROGRAMS = pam_barada.so

pam_barada_so_SOURCES = HOTP.cpp HOTPCredentials.cpp HOTPWindow.cpp pam_barada.cpp Util.cpp 

pam_barada_so_LDFLAGS = -shared

HexTest_SOURCES = HexTest.cpp Util.cpp