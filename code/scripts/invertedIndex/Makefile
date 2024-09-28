CXX = g++
iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_FILE = invIndexFunc.cpp invIndexTest.cpp

II_OBJ = invIndexFunc.o 
IITEST_OBJ = invIndexTest.o
II_TEST = invIndexTest

$(II_TEST): $(IITEST_OBJ) $(II_OBJ)
	$(CXX) -o $@ $^ 

testII : 
	$(CXX) -o $(II_TEST) $(II_FILE) -g

$(IITEST_OBJ) $(II_OBJ) : $(wsHeader) $(iiHeader) 

.PHONY : clean
clean :
	rm -f invIndexTest *.o





