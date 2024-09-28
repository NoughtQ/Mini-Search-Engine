CXX = g++
CFLAGS = -g
iiHeader = invIndexHeader.h
wsHeader = wordStem/english_stem.h wordStem/stemming.h

II_FILE = invIndexFunc.cpp invIndexSearch.cpp main.cpp

TARGET = miniSearchEngine
OBJS = ../bulid/invIndexFunc.o ../bulid/invIndexSearch.o ../bulid/main.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(CFLAGS)

../bulid/invIndexFunc.o: invIndexFunc.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

../bulid/invIndexSearch.o: invIndexSearch.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

../bulid/main.o: main.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

.PHONY : clean
clean:
	rm ../bulid/*.o
	rm $(TARGET)