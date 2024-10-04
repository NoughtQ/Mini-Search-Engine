g++ -g -c main.cpp -o obj\main.o
g++ -g -c porter2_stemmer.cpp -o  obj\porter2_stemmer.o
g++ -g -c insert_index.cpp -o obj\insert_index.o
g++ -g -c search_word.cpp -o obj\search_word.o
g++ -g obj\main.o obj\porter2_stemmer.o obj\insert_index.o obj\search_word.o -o main.exe