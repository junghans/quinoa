############################################################################
#
#  Program:         SuperLU_DIST
#
#  Module:          Makefile
#
#  Purpose:         Top-level Makefile
#
#  Creation date:   September 1, 1999  version 1.0
#
#  Modified:        
#
############################################################################

############################
# for hypre
############################
all:
	( cd SRC; $(MAKE) all)

install:
	( cd SRC; $(MAKE) install)

clean:
	( cd SRC; $(MAKE) clean)

distclean: clean

############################

#include make.inc

#all: install lib example

#lib: superlulib

example:
	( cd EXAMPLE; $(MAKE) )

#clean: cleanlib cleantesting

#install:
	( cd INSTALL; $(MAKE) )
#	( cd INSTALL; cp lsame.c ../SRC/; \
#	  cp dlamch.c ../SRC/; cp slamch.c ../SRC/ )

blaslib:
	( cd CBLAS; $(MAKE) )

superlulib:
	( cd SRC; $(MAKE) )

cleanlib:
	( cd SRC; $(MAKE) clean )
	( cd CBLAS; $(MAKE) clean )
	( cd lib; rm -f *.a )

cleantesting:
	( cd INSTALL; $(MAKE) clean )
	( cd EXAMPLE; $(MAKE) clean )
	( cd FORTRAN; $(MAKE) clean )
