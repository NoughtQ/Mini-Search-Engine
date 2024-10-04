# The Manual of the Mini Search Engine

First of all, welcome to evaluate our project.

Before starting the evaluation, I hope you can read the following content, which may solve some of your problems:

1. Please make sure you are using a Unix-like system, such as Linux or MacOS, otherwise our program may not run properly (for example, it reports an error and cannot open a certain file)

2. Enter the code folder of this project in the CLI (command line interface) and enter the "make" command to compile all programs written in C/C++, and finally generate 4 executable files in the code directory, namely:

- miniSearchEngine: search engine program, providing search function.
Parameter threshold description: threshold, 0 is the minimum and 1 is the maximum. When set to 0, no word will be added to the search (but in fact I force the program to ensure that the words in the search list will not be completely filtered out), and when set to 1, all valid words will be added to the search.

- searchTest: a detection program for testing search capabilities. We have provided you with 6 test samples in advance, located in the code/data/search_test directory, and you can modify the contents of the files freely. When running the program, enter the serial number after input to load it.

    If you need to add new test sample files, please also put the files in this directory, and start with "input" and end with a serial number.

- invIndexTest: A program for testing the correctness of the inverted index. We provide command line parameters that can specify different modes for testing. For details, see README.pdf

- getStopWord: A program for getting stop words. It will generate four files that our project depends on, file_word_count.txt, stop_words.txt, word_count.txt, and word_docs.txt in the code/data folder.

    Also in file_word_count.txt, we completed the word count of the article and finally applied it to the algorithm for sorting articles.

3. We crawled the txt source file using the python program, which is located in the code/scripts/pycode folder. You can run the program. But please note that we assume that you are running the program in the code directory. If you run it in other directories, please modify the path in the program.

    This program will crawl all .html files in the code/data/shakespeare-master folder, select chapters according to our needs (we discard all full.html and index.html and some irrelevant content), and finally output them to the code/data/shakespeare_works folder. As a byproduct, the program outputs all article titles in txt_title.txt, which will be used in the getStopWord program. 

**Finally, I would like to add that some of the content  below may be outdated, so please refer to the actual running results.**

## Test on Inverted Index

To test the inverted index, you should follow the steps below:

1. Open the CLI(Command Line Interface, such as cmd、powershell or other terminals), change the directory(cd) to `code`.
2. Complie the inverted-index-test file: input the instruction

    ``` sh
    make
    ```

    then you will get the executable file called `invIndexTest`.

3. Run the program: input the instruction

    ``` sh
    ./invIndexTest
    ```

    If everything is OK, you will see the info below:

    ``` sh
    ./invIndexTest
    Now building an inverted Index:
    Please input the directory of the documents:
    Path: 
    ```

    You should input the correct directory containing some documentation(It's recommended that the directory is positioned in the `code` directory(the current directory)), like `tests/input3`(no quotation mark!). 

    Result:

    ``` sh
    ./invIndexTest
    Now building an inverted Index:
    Please input the directory of the documents:
    Path: tests/input3
    Build successfully!
    ```

    Failure(the incorrect directory):

    ```
    ./invIndexTest
    Now building an inverted Index:
    Please input the directory of the documents:
    Path: haha
    Could not open directory: No such file or directory
    Fail to build an inverted index!
    ```

4. We have offered some useful CLI parameters for you to check more information about our inverted index. Here are the specific function of these parameters:

    - `-f=n` or `--find=n`: You can search **single** word per time, and you totally have n times. If you omit `=n`，like `-f`，you have 1 time(default value)

    ``` sh 
    ./invIndexTest -f=3
    Now building an inverted Index:
    Please input the directory of the documents:
    Path: tests/input3
    Build successfully!

    Finding Words Mode(only supports single word finding):
    Find 1: great
    Successfully find the word!
    The word was found in files below:
    1henryiv.1.3.txt: 6 times
    Frequency: 6
    -----------------------------------
    Find 2: haha
    Sorry, no such word in the inverted index!
    -----------------------------------
    Find 3: you
    Sorry, no such word in the inverted index!
    -----------------------------------
    ```

    - `-p` or `--print`：Print the whole B+ trees containing the inverted index. **You should use this parameter only when the word count is small**.

    - `-s` or `--stopwords`: If you want to involve all stopwords to the inverted index, you can add this parameter to your command.

    - `-t` or `--test`：Instead of make the traversal in the directory, you can make an inverted index just for a single document in the particular directory(the default directory is `tests`)

    ``` sh
    ./invIndexTest -p -t
    Now testing the correctness of inverted Index:
    Please input the name of the input sample file:
    Name: input2.txt
    Build successfully!
    B+ Tree of Inverted Index:
    [ox, pig]
    [ant, bear, cat, dog][ox, pear][pig, snake, whale]
    ```

    - `-tr` or `--time`：Record the duration for both the building of the inverted index and search a single word.

    ``` sh
    ./invIndexTest -f -tr
    Now building an inverted Index:
    Please input the directory of the documents:
    Path: docs  
    Build successfully!
    Ticks: 2152143
    Duration: 2.15s

    Finding Words Mode(only supports single word finding):
    Find 1: ice 
    Successfully find the word!
    The word was found in files below:
    allswell.2.3.txt: 1 time
    asyoulikeit.3.4.txt: 1 time
    # Delebrate Omission 
    troilus_cressida.3.3.txt: 1 time
    two_gentlemen.3.2.txt: 1 time
    Frequency: 18
    -----------------------------------
    Ticks: 102
    Duration: 0.00s
    ```