CXX = g++
CFLAGS = -g
iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_FILE = invIndexFunc.cpp invIndexSearch.cpp search_main.cpp

TARGET = miniSearchEngine
OBJS = ../bulid/invIndexFunc.o ../bulid/invIndexSearch.o ../bulid/search_main.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CFLAGS)

search_test: ../bulid/invIndexFunc.o ../bulid/invIndexSearch.o ../bulid/search_test.o
	$(CXX) -o $@ $^ $(CFLAGS)

../bulid/invIndexFunc.o: invIndexFunc.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

../bulid/invIndexSearch.o: invIndexSearch.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

../bulid/search_main.o: search_main.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

../bulid/search_test.o: search_test.cpp $(iiHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

.PHONY : clean test
clean:
	rm ../bulid/*.o
	rm $(TARGET)
test:
	./search_test

