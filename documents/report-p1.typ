#import "../template/template.typ": *
#import "@preview/algo:0.3.3": algo, i, d, comment, code

#show: project.with(
  theme: "project",
  course: "Advanced Data Structures and Algorithm Analysis",
  title: "Project 1: Roll Your Own Mini Search Engine",
  cover_image_size: 50%,
  semester: "2024-2025 Autumn&Winter",
  author: none,
  school_id: "",
  date: "2024-09-30",
  table_of_contents: true
)

= Chapter 1: Introduction

== Problem Description

The project required us to create a *mini search engine* which can handle inquiries over "The Complete Works of William Shakespeare".

Here are some specific requirements:

- Run a word count over the Shakespeare set, extract all words from the documents by word stemming and try to identify the stop words.

- Create a customized inverted index over the Shakespeare set with word stemming. The stop words identified must not be included.

- Write a query program on top of the inverted file index, which will accept a user-specified word (or phrase) and return the IDs of the documents that contain that word.

- Run tests to show how the thresholds on query may affect the results.

#v(1em)

== Purpose of Report

#v(.5em)

- Show the details of the implementation of the mini search engine by showcasing essential data structures and algorithms.

- Demonstrate the correctness and efficiency of the program by analysis based on testing data and diagrams.

- Summarize the whole project, analyze the pros and cons of the mini search engine, and put forward the prospect of further improvement.

#v(1em)

== Backgound of Data Structures and Algorithms

#v(.5em)

1. *B+ Trees*: It's a improved version of search trees, widely used in the relational database and file management in operating systems. We will use this data structure to store and access to the inverted index.
2. *Queue*: 
3. *Hashing*:

#v(1.5em)

= Chapter 2: Data Structure / Algorithm Specification

== Algorithm Architecture

The overall algorithm architecture in the program is shown below:

In the following sections, I will introduce these algorithms from top to down, but with some slight adjustement, in the hope that you can gain a deeper insight into my whole program.

== The Main Program

== Word Count

== Stop Words

== Word Stemming 

== Inverted Index

Maybe this is the most complicated part of the whole program, because in this part we have a relatively complex algorithm architecture, and we use a couple of data structrues and algorithms, such as B+ trees, implicit queue ADT and linked list ADT. Here is the diagram of the functions used in the inverted index:

#figure(
  image("../template/images/1.png"), caption: "Relation diagram of all functions for inverted index"
)

- Red: Overall Functions
- Blue: B+ Tree Operations
- Green: Hashing Functions
- Yellow: Other Functions

We'll introduce these functions in detail below.

=== Overall Functions

*(1) InvertedIndex*

#fakebold[Function]: The highest-level function, which users can call it directly.

#algo(
  header: [
    #fakebold[Inputs]:
    - _isTest_: `-t` or `--test` mode, just use one particular file
    - _containStopWords_:  `-s` or `--stopwords` mode, contain stop words when building inverted index
    
    #fakebold[Outputs]: 
    - _InvIndex_: A B+ tree containing the inverted index 

    #fakebold[Procedure]: InvertedIndex(_isTest_: #fakebold[bool], _containStopWords_: #fakebold[bool]) 
  ],
  block-align: left,
)[
  Begin #i\ 
  _InvIndex_ $<-$ CreateBP() \ 
  askforFilePos(_dir_, _fname_, _isTest_) \
  
  #comment[_dir_: directory name, _fname_: filename] \ 

  _InvIndex_ $<-$ fileTraversaler(_InvIndex_, _dir_, _fname_, _isTest_, _containStopWords_) \ 

  if _InvIndex$->$size_ > 0 then #i\ 
    print("Build successfully!") #d\ 
  else #i\ 
    print("Fail to build an inverted index!") #d\
  endif \
  return _InvIndex_ #d\
  End
]

*(2) askforFilePos*

#fakebold[Function]: Ask for the position of the directory or file.

#algo(
  header: [
    #fakebold[Inputs]:
    - _dir_: directory name
    - _fname_:  file name
    - _isTest_: `-t` or `--test` mode, just use one particular file
    
    #fakebold[Outputs]: None, but will update either _dir_ or _fname_

    #fakebold[Procedure]: askforFilePos(dir: #fakebold[string], fname: #fakebold[string], isTest: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
  if _isTest_ is true then #i\
    print("Now testing the correctness of inverted Index:") \
    print("Please input the name of the input sample file:") \
    input("Name:", _fname_) #d\
  else #i\
    print("Now building an inverted Index:") \
    print("Please input the directory of the documents:") \
    input("Path:", _dir_) #d\
  endif #d\
  End
]

*(3) fileTraversaler*

#fakebold[Function]: Make a traversal of all files(or a single file) and build the inverted index from them(or it).

#algo(
  header: [
    #fakebold[Inputs]:
    - _T_: A B+ tree containing the inverted index
    - _dir_: directory name
    - _fname_:  file name
    - _isTest_: `-t` or `--test` mode, just use one particular file
    - _containStopWords_: `-s` or `--stopwords` mode, contain stop words when building inverted index
    
    #fakebold[Outputs]: An updated B+ tree _T_

    #fakebold[Procedure]: fileTraversaler(T: #fakebold[BplusTree], _dir_: #fakebold[string], _fname_: #fakebold[string], _isTest_: #fakebold[boolean], _containStopWords_: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    _docCnt_ = 0 #comment[Count the number of documents and act as the index of the documents at the same time]\

    if _isTest_ == false then #i\
      if _dir_ exists then #i\
        for _file_ in the _dir_ directory do #i\
          _filename_ $<-$ the name of _file_ #comment[string] \
          _docNames_[_docCnt_] $<-$ _filename_ \
          #comment[_docNames_: an array containing names of documents(global variable)] \
          _wholePath_ $<-$ _dir_ + "/" + _filename_ #d\
          #comment[_wholePath_: the complete path of the file to be read] \
        end \

        closefile(_fp_) #d\
        else #i\
          Error("Could not open directory!") #d\
        endif #d\
      else #i\
        _docNames_[_docCnt_] $<-$ _fname_ \
        _dir_ $<-$ _DEFAULTFILEPOS_ #comment[Constant: "tests"(string)] \
        _wholePath_ $<-$ _dir_ + "/" + _fname_ #d\
      endif \

      _fp_ $<-$ openfile(_wholePath_, "r") #comment[read mode] \ 
      #comment[_fp_: the pointer to the file] \
      _T_ $<-$ UpdateInvertedIndex(_T_, _docCnt_, _fp_, _containStopWords_) \
      return _T_ #d\
  End
]

*(4) UpdateInvertedIndex*

#fakebold[Function]: Update the Inverted Index while reading a new document.

#algo(
  header: [
    #fakebold[Inputs]:
    - _T_: A B+ tree containing the inverted index
    - _docCnt_: the index of the document
    - _fp_: pointer to the file
    - _containStopWords_: `-s` or `--stopwords` mode, contain stop words when building inverted index
    
    #fakebold[Outputs]: An updated B+ tree _T_

    #fakebold[Procedure]: UpdateInvertedIndex(T: #fakebold[BplusTree], _docCnt_: #fakebold[integer], _fp_: #fakebold[filePointer], _containStopWords_: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    _H_ $<-$ GenerateHashTb() \
    while reading texts in the file pointed by _fp_ do #i\
      if find an English word then #i\
        _term_ $<-$ the English word \
        if _containStopWords_ == false and FindHashSW(_term_, _H_, true) >= 0 then #i\
          continue #d\
        endif \
        _term_ $<-$ WordStemming(_term_) \
        _isDuplicated_ $<-$ false \ 
        _nodebp_ $<-$ FindBP(_term_, _docCnt_, _T_, _isDuplicated_) \
        if _isDuplicated_ == false then #i\
          _T_ = InsertBP(_term_, _docCnt_, _nodebp_, _T_) #d\
        endif #d\
      else #i\
        continue searching for next English word. #d\
      endif #d\
    end \

    return _T_ #d\
  End
]

=== B+ Tree Operations

*(1) CreateBP*

#fakebold[Function]: Create a B+ tree.

#algo(
  header: [
    #fakebold[Inputs]: None
    
    #fakebold[Outputs]: A new and initialized B+ Tree

    #fakebold[Procedure]: CreateBP()
  ],
  block-align: left,
)[
  Begin #i\ 
    Allocate a memory block for new B+ tree _T_ \

    for all data and children in _T_ do #i\
      Allocate memory blocks for _term_ and _poslist_ of the data, and children \ 
      #comment[Use CreatePL() to intialize the _poslist_] #d\
    end \

    _T_$->$_size_ $<-$ 0 \ 
    _T_$->$_childrenSize_ $<-$ 0  \ 
    _T_$->$_parent_ $<-$ _NULL_  \

    return _T_ #d\
  End
]

*(2) FindBP*

#fakebold[Function]: Find a term in B+ tree.

#algo(
  header: [
    #fakebold[Inputs]:
      - _term_: term
      - _docCnt_: the index of the document
      - _T_: inverted index
      - _flag_: true if the term is found, false otherwise
      - _isSearch_: mark the find mode(-f or --find)
    #fakebold[Outputs]: the updated B+ tree _T_ or recursively call itself again

    #fakebold[Procedure]: FindBP(_term_: #fakebold[string], _docCnt_: #fakebold[integer], _T_: #fakebold[BplusTree], _flag_: #fakebold[booleanPointer], _isSearch_: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _T_$->$childrenSize == 0 then #i\
      isSameTerm(_term_, _docCnt_, _T_, _flag_, _isSearch_) \
      return _T_ #d\
    endif \

    _pos_ $<-$ -1 \
    for _i_ in range(0, _T_$->$_size_) do #comment[not contains _T_$->$_size_] #i\
      if _term_ has less lexicographical order than _T_$->$_data_[_i_]$->$_term_ then #i\
        _pos_ $<-$ _i_ \
        break #d\
      endif #d\
    end \

    if _pos_ == -1 then #i\
      _pos_ $<-$ _i_ #d\
    endif

    return FindBP(_term_, _docCnt_, _T_$->$_children_[_pos_], _isSearch_) #d\
  End
]

*(3) isSameTerm*

#fakebold[Function]: Check if the term exists in the B+ tree.

#algo(
  header: [
    #fakebold[Inputs]:
      - _term_: term
      - _docCnt_: the index of the document
      - _nodebp_: the appropriate node where the term may exists or will exists after insertion
      - _flag_: true if the term is found, false otherwise
      - _isSearch_: mark the find mode(-f or --find)  
    #fakebold[Outputs]: None, but may update the flag and print some information regarding _term_

    #fakebold[Procedure]: isSameTerm(_term_: #fakebold[string], _docCnt_: #fakebold[integer], _nodebp_: #fakebold[NodeBP], _flag_: #fakebold[booleanPointer], _isSearch_: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _nodebp_$->$_size_ > 0 then #i\
      for _i_ in range(0, _T_$->$_size_) do #comment[not contains _T_$->$_size_] #i\
        if _term_ == _nodebp_$->$_data_[_i_]$->$_term_ then #i\
          if _isSearch_ == false then #i\
            EnqueuePL(_docCnt_, _nodebp_$->$_data_[_i_]$->$_poslist_) #d\
          else #i\
            _poslist_ $<-$ _nodebp_$->$_data_[_i_]$->$_poslist_ \
            _size_ $<-$ _poslist_$->$_size_ \
            _cnt_ $<-$ 0 \

            print("Successfully find the word!") \
            print("The word was found in files below:") \

            _posArr_ $<-$ RetrievePL(_poslist_) \

            for _j_ in range(0, _size_) do #comment[not contains _size_] #i\
              if _posArr_[_j_][1] <= 1 then #i\
                print("{_docNames_[_posArr_[_j_][0]]}: {_posArr_[_j_][1]} time") #d\
              else #i\
                print("{_docNames_[_posArr_[_j_][0]]}: {_posArr_[_j_][1]} times") #d\
              endif \
              cnt $<-$ cnt + _posArr_[_j_][1] #d\
            end \
            print("Frequency: {_cnt_}") \ 
            print("---------------------------------------") #d\
          endif \

          _flag_ $<-$ true \
          break #d\
        endif #d\
      end #d\
    endif #d\
  End
]

*(4) InsertBP*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(5) SplitBP*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(6) PrintBPTree*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

=== Hashing Operations

*(1) GenerateHashTb*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(2) InitHashTb*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(3) FindHashSW*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(4) InsertHashSW*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(5) HashFunc*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

=== Other Functions

*(1) CreateQueueBP*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(2) EnqueueBP*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(3) DequeueBP*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(4) CreatePL*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(5) EnqueuePL*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

*(6) RetrievePL*

#fakebold[Function]: 

#algo(
  header: [
    #fakebold[Inputs]:
    
    #fakebold[Outputs]: 

    #fakebold[Procedure]: 
  ],
  block-align: left,
)[
  Begin #i\ 

  End
]

== Query

= Chapter 3: Testing Results

== Test 1: Inverted Index

== Test 2: Thresholds for Queries

== Test 3: Speed Test

== (Maybe)Debug Mode

= Chapter 4: Analysis comments

== Time Complexity

== Space Complexity

= Appendix: Source code

== File Structure

= References

= Declaration

#v(.5em)

#fakebold[#fakeitalic[We hereby declare that all the work done in this project titled "Roll Your Own Mini Search Engine" is of our independent effort as a group.]]

