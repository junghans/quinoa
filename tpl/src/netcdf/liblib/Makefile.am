## This is a automake file, part of Unidata's netCDF package.
# Copyright 2010, see the COPYRIGHT file for more information.

# This Makefile assembles the correct libnetcdf based on various
# configure flags. It is assumed that all the relevant convenience
# libraries have been built (e.g. libsrc, libsrc4, libncdap3, libcdmr,
# libncdap4, fortran).

# Put together AM_CPPFLAGS and AM_LDFLAGS.
include $(top_srcdir)/lib_flags.am

# This is our output, the netcdf library the user will install.
lib_LTLIBRARIES = libnetcdf.la

##
# These linker flags specify libtool version info.
# Note: version-info of "9:0:2" results in soname of libnetcdf.so.7 -> libnetcdf.so.7.2.0
# See http://www.gnu.org/software/libtool/manual/libtool.html#Libtool-versioning
# for information regarding incrementing `-version-info`.
##

libnetcdf_la_LDFLAGS = -version-info 10:0:3 ${NOUNDEFINED}

libnetcdf_la_CPPFLAGS = ${AM_CPPFLAGS}
libnetcdf_la_LIBADD =
CLEANFILES =

# Turn on some extra stuff when building a DLL for windows.
if BUILD_DLL
libnetcdf_la_LDFLAGS += -no-undefined -Wl,--output-def,netcdfdll.def
libnetcdf_la_CPPFLAGS += -DDLL_EXPORT
endif # BUILD_DLL

# The v2 API...
if BUILD_V2
libnetcdf_la_LIBADD += ${top_builddir}/libdispatch/libnetcdf2.la
endif # BUILD_V2

# The output library will always include netcdf3 and dispatch
# libraries
libnetcdf_la_LIBADD += ${top_builddir}/libdispatch/libdispatch.la	\
			${top_builddir}/libsrc/libnetcdf3.la

# + pnetcdf
if USE_PNETCDF
AM_CPPFLAGS += -I${top_srcdir}/libsrcp
libnetcdf_la_LIBADD += ${top_builddir}/libsrcp/libnetcdfp.la
endif # USE_PNETCDF

# + dap
if USE_DAP
AM_CPPFLAGS += -I${top_srcdir}/libdap2 -I${top_srcdir}/oc
libnetcdf_la_LIBADD += ${top_builddir}/libdap2/libdap2.la
libnetcdf_la_LIBADD += ${top_builddir}/oc2/liboc.la
endif # USE_DAP

# NetCDF-4 ...
if USE_NETCDF4

AM_CPPFLAGS += -I${top_srcdir}/libsrc4

libnetcdf_la_LIBADD += ${top_builddir}/libsrc4/libnetcdf4.la

# Not ready for prime time yet
# libnetcdf_la_LIBADD += ${top_builddir}/libdiskless/libdiskless.la

endif #USE_NETCDF4

# We need at least one source file
libnetcdf_la_SOURCES = nc_initialize.c
EXTRA_DIST=CMakeLists.txt 
