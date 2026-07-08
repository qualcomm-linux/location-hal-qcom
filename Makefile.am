# Makefile.am - Automake script for gps loc_api
#

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = pla/oe utils core batching geofence location gnss

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = location-hal.pc
EXTRA_DIST = $(pkgconfig_DATA)
