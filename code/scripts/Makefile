CXX = g++
iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_FILE = invIndexFunc.cpp invIndexTest.cpp
ST_FILE = getStopWord.cpp

II_OBJS = ../build/invIndexFunc.o ../build/invIndexTest.o
# ST_OBJ = ../build/getStopWord.o
II_TEST = invIndexTest
ST = getStopWord

$(II_TEST): $(II_OBJS)	
	$(CXX) -o $@ $^ 

testII : 
	$(CXX) -o $(II_TEST) $(II_FILE) -g

stopwords:
	$(CXX) -o $(ST) $(ST_FILE)

../build/invIndexFunc.o : invIndexFunc.cpp $(wsHeader) $(iiHeader) 
	$(CXX) -c $< -o $@

../build/invIndexTest.o : invIndexTest.cpp $(wsHeader) $(iiHeader) 
	$(CXX) -c $< -o $@

.PHONY : clean
clean :
	rm -f $(II_TEST) $(II_OBJS)





