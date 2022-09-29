#COMPILER=
#ARCHIVER=
CP=cp
#COMPILER_FLAGS=
#EXTRA_COMPILER_FLAGS=
#LIB=libxil.a

#RELEASEDIR=../../../lib
INCLUDEDIR=../../../include
INCLUDES=-I./. -I${INCLUDEDIR}

INCLUDEFILES=*.h
#LIBSOURCES=*.c
#OUTS = *.o


libs:
#	echo "Adding AD converter serial receiver libraries"
#	$(COMPILER) $(COMPILER_FLAGS) $(EXTRA_COMPILER_FLAGS) $(INCLUDES) $(LIBSOURCES)
#	$(ARCHIVER) -r ${RELEASEDIR}/${LIB} ${OUTS}
#	make clean

include:
	echo "AD converter serial receiver include files"
	 ${CP} ${INCLUDEFILES} ${INCLUDEDIR}

clean:
	rm -rf ${OUTS}

