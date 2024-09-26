CFLAGS = -g -Wall -std=c++17 -I./include

iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_OBJ = invIndexFunc.o 
IITEST_OBJ = invIndexTest.o
II_TEST = invIndexTest

$(II_TEST): $(IITEST_OBJ) $(II_OBJ)
	g++ -o $@ $^ $(CFLAGS)

debug : $(IITEST_OBJ) $(II_OBJ)
	g++ -o $(II_TEST) $^ $(CFLAGS); clean $(IITEST_OBJ) $(II_OBJ)

$(IITEST_OBJ) $(II_OBJ) : $(wsHeader) $(iiHeader) 
	g++ -o $@ $^ $(CFLAGS)

.PHONY : clean
clean :
	rm -f invIndexTest *.o





