AM_CFLAGS = -Wundef \
        -Wno-trigraphs \
        -g -O0 \
        -fno-inline \
        -fno-short-enums \
        -fpic \
        -I$(top_srcdir)/utils \
        -I$(top_srcdir)/core \
        -I$(top_srcdir)/location \
        -I$(top_srcdir)/pla/oe \
        $(LOCPLA_CFLAGS) \
        -D__func__=__PRETTY_FUNCTION__ \
        -std=c++1y

AM_CPPFLAGS = $(AM_CFLAGS)

ACLOCAL_AMFLAGS = -I m4

requiredlibs = \
        ../core/libloc_core.la \
        ../utils/libgps_utils.la

h_sources = \
        GeofenceAdapter.h

c_sources = \
    GeofenceAdapter.cpp \
    location_geofence.cpp

libgeofencing_la_SOURCES = $(c_sources)
if USE_GLIB
libgeofencing_la_CFLAGS  = -DUSE_GLIB @GLIB_CFLAGS@ $(AM_CFLAGS)
libgeofencing_la_CPPFLAGS  = -DUSE_GLIB @GLIB_CFLAGS@ $(AM_CFLAGS) $(AM_CPPFLAGS)
libgeofencing_la_LDFLAGS = -lstdc++ -Wl,-z,defs @GLIB_LIBS@ $(requiredlibs) -shared  -version-number @LT_VERSION_NUMBER@
libgeofencing_la_LIBDADD = $(requiredlibs) -lstdc++ @GLIB_LIBS@
else
libgeofencing_la_CFLAGS  = $(AM_CFLAGS)
libgeofencing_la_CPPFLAGS  = $(AM_CFLAGS) $(AM_CPPFLAGS)
libgeofencing_la_LDFLAGS = -lstdc++ -Wl,-z,defs $(requiredlibs) -shared  -version-number @LT_VERSION_NUMBER@
libgeofencing_la_LIBDADD = $(requiredlibs) -lstdc++
endif

library_include_HEADERS = $(h_sources)

library_includedir = $(includedir)

lib_LTLIBRARIES = libgeofencing.la
