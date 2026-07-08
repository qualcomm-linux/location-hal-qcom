AM_CFLAGS = \
     -I$(top_srcdir)/utils/base_util \
     -I$(top_srcdir)/utils \
     -D__func__=__PRETTY_FUNCTION__ \
     -fno-short-enums

if USE_G_STRLCPY
    AM_CFLAGS += -Dstrlcpy=g_strlcpy -Dstrlcat=g_strlcat
endif

h_sources = \
        ../config_file.h \
        ../log.h \
        ../postcard.h \
        ../sync.h \
        ../queue.h \
        ../list.h \
        ../memorystream.h \
        ../nvparam_mgr.h \
        ../time_routines.h \
        ../vector.h \
        ../array.h \
        ../string_routines.h \
        ../shutdown_timer.h \
        ../loc_common.h

libloc_base_util_la_SOURCES = \
        config_file.cpp \
        log.cpp \
        memorystream.cpp \
        nvparam_mgr.cpp \
        postcard.cpp \
        sync.cpp \
        string_routines.cpp \
        time_routines.cpp
library_includedir = $(includedir)/base_util
library_include_HEADERS = $(h_sources)

if USE_GLIB
libloc_base_util_la_CFLAGS = -DUSE_GLIB $(AM_CFLAGS) @GLIB_CFLAGS@ -include glib.h
libloc_base_util_la_LDFLAGS = -lstdc++ -Wl,-z,defs -lpthread @GLIB_LIBS@ -shared -version-info 1:0:0
libloc_base_util_la_CPPFLAGS = -DUSE_GLIB $(AM_CFLAGS) $(AM_CPPFLAGS) @GLIB_CFLAGS@  -include glib.h
else
libloc_base_util_la_CFLAGS = $(AM_CFLAGS)
libloc_base_util_la_LDFLAGS = -Wl,-z,defs -lpthread -shared  -version-number @LT_VERSION_NUMBER@
libloc_base_util_la_CPPFLAGS = $(AM_CFLAGS) $(AM_CPPFLAGS)
endif

libloc_base_util_la_LIBADD = -lstdc++ -ldl -lsqlite3

#Create and Install libraries
lib_LTLIBRARIES = libloc_base_util.la
