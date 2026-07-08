AM_CFLAGS = \
     -I$(top_srcdir)/utils \
     -I$(top_srcdir)/core \
     -I$(top_srcdir)/location \
     -I$(top_srcdir)/pla/oe \
     -I$(srcdir) \
     -std=c++1y \
     -D__func__=__PRETTY_FUNCTION__ \
     -fno-short-enums

ACLOCAL_AMFLAGS = -I m4

requiredlibs = \
        ../utils/libgps_utils.la \
        ../core/libloc_core.la

h_sources = \
    BatchingAdapter.h

libbatching_la_SOURCES = \
    location_batching.cpp \
    BatchingAdapter.cpp

if USE_GLIB
libbatching_la_CFLAGS = -DUSE_GLIB $(AM_CFLAGS) @GLIB_CFLAGS@
libbatching_la_LDFLAGS = -lstdc++ -g -Wl,-z,defs -lpthread $(requiredlibs) @GLIB_LIBS@ -shared  -version-number @LT_VERSION_NUMBER@
libbatching_la_CPPFLAGS = -DUSE_GLIB $(AM_CFLAGS) $(AM_CPPFLAGS) @GLIB_CFLAGS@
else
libbatching_la_CFLAGS = $(AM_CFLAGS)
libbatching_la_LDFLAGS = -Wl,-z,defs -lpthread $(requiredlibs) -shared  -version-number @LT_VERSION_NUMBER@
libbatching_la_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
endif

library_include_HEADERS = $(h_sources)

library_includedir = $(includedir)

#Create and Install libraries
lib_LTLIBRARIES = libbatching.la
