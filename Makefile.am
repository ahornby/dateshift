
# Packaging
SUBDIRS = config
EXTRA_DIST = dateshift.1 autogen.sh m4
ACLOCAL_AMFLAGS = -I m4

# Doco
man_MANS = dateshift.1

# Libs
lib_LTLIBRARIES = libdsintercept.la
libdsintercept_la_SOURCES = dsintercept.c
libdsintercept_la_LIBADD = -ldl

# Program
bin_PROGRAMS = dateshift
dateshift_SOURCES = dateshift.c

AM_CPPFLAGS = -DLIBDIR=\"$(libdir)\"

CLEANFILES = *~ *.tar.gz
