iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_OBJ = invIndexFunc.o 
IITEST_OBJ = invIndexTest.o

invIndexTest : $(IITEST_OBJ) $(II_OBJ)
	g++ -o $@ $^ -std=c++17

$(IITEST_OBJ) $(II_OBJ) : $(wsHeader) $(iiHeader) 

.PHONY : clean
clean :
	rm -f invIndexTest *.o





