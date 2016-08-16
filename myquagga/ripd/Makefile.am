## Process this file with automake to produce Makefile.in.

AM_CPPFLAGS = -I.. -I$(top_srcdir) -I$(top_srcdir)/lib -I$(top_builddir)/lib
DEFS = @DEFS@ -DSYSCONFDIR=\"$(sysconfdir)/\"
INSTALL_SDATA=@INSTALL@ -m 600

AM_CFLAGS = $(WERROR)

noinst_LIBRARIES = librip.a
sbin_PROGRAMS = ripd

librip_a_SOURCES = \
	ripd.c rip_zebra.c \
	rip_routemap.c rip_peer.c

noinst_HEADERS = \
	ripd.h

ripd_SOURCES = \
	rip_main.c $(librip_a_SOURCES)

ripd_LDADD = ../lib/libzebra.la @LIBCAP@

examplesdir = $(exampledir)
dist_examples_DATA = ripd.conf.sample

EXTRA_DIST = RIPv2-MIB.txt

