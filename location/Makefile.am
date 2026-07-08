ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = \
     -I$(srcdir) \
     -I$(top_srcdir)/utils \
     -I$(top_srcdir)/pla/oe \
     $(LOCPLA_CFLAGS) \
     -std=c++11

liblocation_api_la_SOURCES = \
    LocationAPI.cpp \
    LocationAPIClientBase.cpp

if USE_EXTERNAL_AP
AM_CFLAGS += -DFEATURE_EXTERNAL_AP
endif

if USE_GLIB
liblocation_api_la_CFLAGS = -DUSE_GLIB $(AM_CFLAGS) @GLIB_CFLAGS@
liblocation_api_la_LDFLAGS = -lstdc++ -Wl,-z,defs -lpthread @GLIB_LIBS@ -shared  -version-number @LT_VERSION_NUMBER@
liblocation_api_la_CPPFLAGS = -DUSE_GLIB $(AM_CFLAGS) $(AM_CPPFLAGS) @GLIB_CFLAGS@
else
liblocation_api_la_CFLAGS = $(AM_CFLAGS)
liblocation_api_la_LDFLAGS = -Wl,-z,defs -lpthread -shared  -version-number @LT_VERSION_NUMBER@
liblocation_api_la_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
endif

liblocation_api_la_LIBADD = -lstdc++ -ldl ../utils/libgps_utils.la

library_include_HEADERS = \
    LocationAPI.h \
    LocationAPIClientBase.h \
    location_interface.h \
    LocationDataTypes.h \
    ILocationAPI.h

#Create and Install libraries
lib_LTLIBRARIES = liblocation_api.la

library_includedir = $(includedir)
