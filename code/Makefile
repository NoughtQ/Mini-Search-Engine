CXX = g++
CFLAGS = -g

BULID_DIR = ./build
SRC_DIR = ./scripts

# Header files
iiHeader = $(SRC_DIR)/invIndexHeader.h
wsHeader = $(SRC_DIR)/wordStem/english_stem.h $(SRC_DIR)/wordStem/stemming.h

# Target files
TARGET = miniSearchEngine
STOPWORDS = getStopWord
TEST_SEARCH = searchTest
TEST_II = invIndexTest

# Object files
SEAM_OBJS = $(BULID_DIR)/invIndexFunc.o $(BULID_DIR)/invIndexSearch.o $(BULID_DIR)/search_main.o
SEAT_OBJS = $(BULID_DIR)/invIndexFunc.o $(BULID_DIR)/invIndexSearch.o $(BULID_DIR)/search_test.o
II_OBJS = $(BULID_DIR)/invIndexFunc.o $(BULID_DIR)/invIndexTest.o

all: $(TARGET) $(STOPWORDS) $(TEST_SEARCH) $(TEST_II)


$(TARGET): $(SEAM_OBJS) $(iiHeader) $(wsHeader)
	$(CXX) -o $(TARGET) $(SEAM_OBJS) $(CFLAGS)

$(TEST_SEARCH): $(SEAT_OBJS) $(iiHeader) $(wsHeader)
	$(CXX) -o $(TEST_SEARCH) $(SEAT_OBJS) $(CFLAGS)

$(TEST_II): $(II_OBJS) $(iiHeader) $(wsHeader)
	$(CXX) -o $(TEST_II) $(II_OBJS) $(CFLAGS)

$(STOPWORDS): $(BULID_DIR)/stopwords.o
	$(CXX) -o $(STOPWORDS) $< $(CFLAGS)

$(BULID_DIR)/invIndexFunc.o: $(SRC_DIR)/invIndexFunc.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

$(BULID_DIR)/invIndexSearch.o: $(SRC_DIR)/invIndexSearch.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

$(BULID_DIR)/search_main.o: $(SRC_DIR)/search_main.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

$(BULID_DIR)/search_test.o: $(SRC_DIR)/search_test.cpp $(iiHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

$(BULID_DIR)/invIndexTest.o: $(SRC_DIR)/invIndexTest.cpp $(iiHeader) $(wsHeader)
	$(CXX) -c $< -o $@ $(CFLAGS)

$(BULID_DIR)/stopwords.o: $(SRC_DIR)/getStopWord.cpp
	$(CXX) -c $< -o $@ $(CFLAGS)

.PHONY : clean
clean:
	rm $(BULID_DIR)/*.o
	rm $(TARGET) $(STOPWORDS) $(TEST_SEARCH) $(TEST_II)

