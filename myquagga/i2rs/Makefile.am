## Process this file with automake to produce Makefile.in.

AM_CPPFLAGS = -I.. -I$(top_srcdir) -I$(top_srcdir)/lib -I$(top_builddir)/lib
AM_CFLAGS = $(WERROR)
DEFS = @DEFS@ $(LOCAL_OPTS) -DSYSCONFDIR=\"$(sysconfdir)/\"
INSTALL_SDATA=@INSTALL@ -m 600

noinst_LIBRARIES = libi2rs.a


sbin_PROGRAMS = i2rsd
libi2rs_a_LIBADD = ../lib/libzebra.la

libi2rs_a_SOURCES = \
	i2rs_zebra.c i2rs_vty.c 

i2rsdheaderdir = $(pkgincludedir)/i2rs

i2rsdheader_HEADERS = \
		i2rs_main.h
noinst_HEADERS = \
	  i2rs_zebra.h i2rs_vty.h
i2rsd_SOURCES = i2rs_main.c

i2rsd_LDADD = libi2rs.a ../lib/libzebra.la @LIBCAP@





