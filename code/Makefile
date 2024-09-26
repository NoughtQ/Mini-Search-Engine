invIndexHeader = invIndexHeader.h wordStem/english_stem.h wordStem/stemming.h

invIndexFile = invIndexFunc.c invIndexGenerator.cpp invIndexMain.c

UNAME := $(shell uname)
ifeq ($(UNAME),Linux)
    CFLAGS += -DLINUX
    OS = invIndexUnix.c
else ifeq ($(UNAME),Darwin)
    CFLAGS += -DMACOS
    OS = invIndexUnix.c
else ifeq ($(UNAME),Windows_NT)
    CFLAGS += -DWINDOWS
    OS = invIndexWin.c
endif

objects = invIndexTest.o

invIndexTest : invIndexTest.o
	cc -o invIndexTest invIndexTest.o

invIndexTest.o : $(invIndexHeader)
	cc -c $(invIndexFile) $(OS) invIndexTest.c

clean :
	rm -f *.o