CXX = g++
iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_FILE = invIndexFunc.cpp invIndexTest.cpp

OBJS = ../build/invIndexFunc.o ../build/invIndexTest.o
II_TEST = invIndexTest

$(II_TEST): $(OBJS)	
	$(CXX) -o $@ $^ 

testII : 
	$(CXX) -o $(II_TEST) $(II_FILE) -g

../build/invIndexFunc.o : invIndexFunc.cpp $(wsHeader) $(iiHeader) 
	$(CXX) -c $< -o $@

../build/invIndexTest.o : invIndexTest.cpp $(wsHeader) $(iiHeader) 
	$(CXX) -c $< -o $@

.PHONY : clean
clean :
	rm -f $(II_TEST) $(OBJS)





