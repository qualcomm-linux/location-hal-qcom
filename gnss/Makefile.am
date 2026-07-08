ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = \
     $(LOCPLA_CFLAGS) \
     -I$(top_srcdir)/pla/oe \
     $(LIBXML_CFLAGS) \
     -I$(top_srcdir)/utils \
     -I$(top_srcdir)/core \
     -I$(top_srcdir)/core/observer \
     -I$(top_srcdir)/core/data-items \
     -I$(srcdir) \
     -I$(top_srcdir)/location \
     -std=c++14

libgnss_la_SOURCES = \
    location_gnss.cpp \
    GnssAdapter.cpp \
    XtraSystemStatusObserver.cpp \
    Agps.cpp \
    XmlFileParser.cpp \
    NativeAgpsHandler.cpp

if USE_FEATURE_AUTOMOTIVE
AM_CFLAGS += -DFEATURE_AUTOMOTIVE
endif

if USE_TELSDK
AM_CFLAGS += -DUSE_FEATURE_TELSDK
endif

if USE_GLIB
libgnss_la_CFLAGS = -DUSE_GLIB $(AM_CFLAGS) @GLIB_CFLAGS@
libgnss_la_LDFLAGS = -lstdc++ -Wl,-z,defs -lpthread @GLIB_LIBS@ -shared  -version-number @LT_VERSION_NUMBER@
libgnss_la_CPPFLAGS = -DUSE_GLIB $(AM_CFLAGS) $(AM_CPPFLAGS) @GLIB_CFLAGS@
else
libgnss_la_CFLAGS = $(AM_CFLAGS)
libgnss_la_LDFLAGS = -Wl,-z,defs -lpthread -shared  -version-number @LT_VERSION_NUMBER@
libgnss_la_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
endif

libgnss_la_LIBADD = -lstdc++ -ldl ../utils/libgps_utils.la ../core/libloc_core.la ../location/liblocation_api.la ../utils/base_util/src/libloc_base_util.la

#Create and Install libraries
lib_LTLIBRARIES = libgnss.la
