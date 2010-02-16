AC_INIT(pam_barada.cpp)
AM_INIT_AUTOMAKE(barada-pam,0.4)
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LANG_CPLUSPLUS
AC_CHECK_HEADER([boost/lexical_cast.hpp],
   [],
   AC_MSG_ERROR([Boost STL Library is needed])) 
BOOST_FILESYSTEM=
AC_CHECK_LIB(boost_filesystem, main, BOOST_FILESYSTEM=-lboost_filesystem, [
 	AC_CHECK_LIB(boost_filesystem-mt, main, BOOST_FILESYSTEM=-lboost_filesystem-mt, [
 	AC_MSG_ERROR("boost-filesystem is needed.")
 	])
 	]) 
AC_SUBST([BOOST_FILESYSTEM])
AC_CHECK_LIB(pam, pam_start, [], AC_MSG_ERROR([PAM Libraries Required]))
AC_CHECK_LIB(crypto, HMAC, [], AC_MSG_ERROR([openssl libraries required]))
AC_CHECK_HEADER([openssl/hmac.h], [], AC_MSG_ERROR([openssl HMAC libraries not found]))
AC_OUTPUT(Makefile)
