AC_INIT(pam_hotp.cpp)
AM_INIT_AUTOMAKE(barada-pam,0.2)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER([boost/lexical_cast.hpp],
   [],
   AC_MSG_ERROR([Boost STL Library is needed])) 
AC_CHECK_LIB(pam, pam_start, [], AC_MSG_ERROR([PAM Libraries Required]))
AC_CHECK_LIB(crypto, HMAC, [], AC_MSG_ERROR([openssl libraries required]))
AC_CHECK_HEADER([openssl/hmac.h], [], AC_MSG_ERROR([openssl HMAC libraries not found]))
AC_OUTPUT(Makefile)
