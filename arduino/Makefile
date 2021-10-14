# This script creates the CommoTalkie Arduino based library files.

COMMOTALKIEDIRNAME=CommoTalkie
RELEASEDIRNAME=prebuild
CONFIGDIRNAME=config

.DEFAULT_GOAL := build

build: library.properties.template ../include/application/Publish.h ../include/application/Pull.h ../src/application/Publish.c ../src/application/Pull.c
	@echo "> Preparing header and source files"
	mkdir -p "$(RELEASEDIRNAME)/$(COMMOTALKIEDIRNAME)"
	find ../include/ -type f -name *.h -exec cp {} "$(RELEASEDIRNAME)/$(COMMOTALKIEDIRNAME)/" \;
	find ../src/ -type f -name *.c -exec cp {} "$(RELEASEDIRNAME)/$(COMMOTALKIEDIRNAME)/" \;
	cp ./library.properties.template "$(RELEASEDIRNAME)/$(COMMOTALKIEDIRNAME)/library.properties"

clean:
	@echo "> Cleaning header and source files"
	rm -Rf "$(RELEASEDIRNAME)"
