# This script creates the CommoTalkie Arduino based library files.

COMMOTALKIEDIRNAME=CommoTalkie
RELEASEDIRNAME=prebuild
CONFIGDIRNAME=config

build: library.properties.template ../include/application/Publish.h ../include/application/Pull.h ../src/application/Publish.c ../src/application/Pull.c
	mkdir -p "${RELEASEDIRNAME}/${COMMOTALKIEDIRNAME}"
	find ../include/ -type f -name *.h -exec cp {} "${RELEASEDIRNAME}/${COMMOTALKIEDIRNAME}/" \;
	find ../src/ -type f -name *.c -exec cp {} "${RELEASEDIRNAME}/${COMMOTALKIEDIRNAME}/" \;
	mv "${RELEASEDIRNAME}/${COMMOTALKIEDIRNAME}/messageconfig.h" "${RELEASEDIRNAME}/"
	cp ./library.properties.template "${RELEASEDIRNAME}/${COMMOTALKIEDIRNAME}/library.properties"

clean:
	rm -Rf "${RELEASEDIRNAME}"
