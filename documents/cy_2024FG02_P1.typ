#import "../template/template.typ": *
#import "@preview/algo:0.3.3": algo, i, d, comment, code

#show: project.with(
  theme: "project",
  course: "Advanced Data Structures and Algorithm Analysis",
  title: "Project 1: Roll Your Own Mini Search Engine",
  cover_image_size: 50%,
  cover_image_padding: 1em,
  semester: "2024-2025 Autumn&Winter",
  author: [Huang Xingyao\ $quad quad$ Qian Ziyang],
  school_id: "",
  date: "2024-10-04",
  table_of_contents: true
)

= Chapter 1: Introduction

== Problem Description

The project requires us to implement a mini search engine based on _The Complete Works of William Shakespeare_, which can return the corresponding articles according to the words users desire to query.

In addition, this search engine has the features below:

- Word count for over the article set and attain the stop words(i.e. meaningless words while searching) from this set.

- Use the #fakebold[inverted file index] to store and access all words with their frequency and location.

- The precision and recall can be adjusted by setting the threshold of queries.

#v(1em)

== Purpose of Report

#v(.5em)

- Show the details of the implementation of the mini search engine by showcasing essential data structures and algorithms.

- Demonstrate the correctness and efficiency of the program by analysis based on testing data and diagrams.

- Summarize the whole project, analyze the pros and cons of the mini search engine, and put forward the prospect of further improvement.

#v(1em)

== Backgound of Data Structures and Algorithms

#v(.5em)

1. *B+ Trees*: It's an improved version of search trees, widely used in the relational database and file management in operating systems. We will use this data structure to store and access to the inverted index.

2. *Hashing*: Hash tables have an excellent performance in searching data(only cost O(1) time), hence we take advantage of this data structure for finding stopwords when building an inverted index.

3. *Queue*: The Queue ADT is one of the most basic data structrues used in printing the B+ tree, storing the positions for terms, etc.

4. *TF-IDF Algorithm*: 

*tf–idf* (also *TF\*IDF*, *TFIDF*, *TF–IDF*, or *Tf–idf*), short for *term frequency–inverse document frequency*, is a measure of importance of a word to a document in a collection or corpus, adjusted for the fact that some words appear more frequently in general.

It was often used as a weighting factor in searches of information retrieval, text mining, and user modeling.

One of the simplest ranking functions is computed by summing the tf–idf for each query term; many more sophisticated ranking functions are variants of this simple model.

There are many variations of how tf-idf is calculated. In our project, we use the following formula to calculate tf , idf and tf-idf.

$
"tf"("word", "doc") = frac("num of word in the doc", "total word count of the doc")
$

$
"idf"("word", "doc") = log frac("total num of documents", 1+ |"num of documents that include the word"|)
$

$
"ti-idf"("word", "doc") = "tf" times "idf"
$

#v(1.5em)

= Chapter 2: Data Structure / Algorithm Specification

== Algorithm Architecture

The overall algorithm architecture in the program is shown below:

#table(columns: 1fr, align: left)[

```
the main program
├── word count
├── stop words
├── inverted index
│   ├── askforFilePos
│   └── fileTraversaler
│       ├── generate B+ tree
│       │   ├── CreateBP
│       │   ├── FindBP
│       │   │   └──isSameTerm
│       │   ├── InsertBP
│       │   ├── SplitBP
│       │   └── PrintBPTree
│       └── generate hash table
│           ├── InitHashTb
│           ├── FindHashSW
│           ├── InsertHashSW
│           ├── HashFunc
│           └── PrintHashTb
└── search
    ├── loadWordIdf
    ├── loadFileWordsNum
    └── loadStopWords
```
]

== The Main Program

#algo(
  header: [
    #fakebold[Inputs]: 
    
    - All files of _The Complete Works of Shakespeare_
    - User's queries

    #fakebold[Outputs]: 

    - The results of queries

    #fakebold[Procedure]: main()
  ],
  block-align: left,
  strong-keywords: false
)[
*Begin* #i\ 
  load necessary resources and establish inverted index \

  set values for pageSize and threshold from user input \

  *while* search_allow *do* #i\
      get query from user input \
      Calling the search function \
      ask user if they want to continue searching or not and set search_allow accordingly #d\
  *end* #d\
*End*
]

The main idea of the code is to serve as the entry point for a search application named "ShakespeareFinder." It involves loading required resources, setting up an inverted index, and managing user queries.

== Word Count & Stop Words

#v(.5em)

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #note(name: [Note])[
    Because the original open resources of _The Works_ are written in HTML format with too many markups, we have converted them into TXT format, which is more readable and convenient for us to handle. And all titles of these articles are extracted to a TXT file at the same time.
  ]

  #v(1em)
]
#render("thickness")

The word count and stop words detection are relatively simple, we combined their functions into one program to cope with problems together. 

#algo(
  header: [
    #fakebold[Inputs]: No obvious input parameters, but the program will read in: 
    
    - A file(`code/data/txt_title.txt`) contains all titles of articles in _The Works_
    - A directory(`code/data/shakespeare_work`) includes _The Works_ in TXT format

    #fakebold[Outputs]: 3 files

    - `code/data/stop_words.txt`: Record all selected stopwords
    - `code/data/word_count.txt`: Count for each word in all files
    - `code/data/word_docs.txt`: Count the words for each file

    #fakebold[Procedure]: getStopWord() 
  ],
  block-align: left,
)[
  Begin #i\ 
    Read in the file `txt_title.txt` as _infile_\
    Prepare the output file (_outfile_) named `file_word_count.txt` \
    while reading in the content of _infile_ do #i\
      #comment[_file_: one line content in _infile_, i.e. the title of each file] \
      Read in the file `"shakespeare_works/"` + _file_ + `".txt"` as _in_ \
      while reading each line(_line_) in file _in_ do #i\
        if find an English word (called _word_) then #i\
          Do word stemming \
          _wordList_[_word_] $<-$ _wordList_[_word_] + 1 \
          _wordNumOfDoc_[_file_] $<-$ _wordNumOfDoc_[_file_] + 1\
          _wordDocs_[_word_] $<-$ _file_ #d\
        endif #d\
      end \

      Output the _wordNumOfDoc_ to _outfile_ \
      closefile(_in_) #d\
    end \

    closefile(_infile_) \
    Sort(_wordDocs_)
    Prepare the output file (_out_) named `word_count.txt` \
    Prepare the output file (_out2_) named `stop_words.txt` \
    Prepare the output file (_out3_) named `word_docs.txt` \

    for each item(_word_) in _wordDocs_ do #i\
      Output the _word_$->$_content_ and _word_$->$_frequency_ to _out3_ \
      if _word_$->$_frequency_ > _THRESHOLD_ then #i\
        Output the _word_$->$_content_ to _out2_ #d\
      endif \
      Output the _word_$->$_content_ and _wordList_[_word_$->$_content_] to _out_ #d\
    end \
    closefile(_out_) \
    closefile(_out2_) \   
    closefile(_out3_) #d\ 
  End
]

== Word Stemming 

We tap into the codes from a GitHub repository called "OleanderStemmingLibrary" by the author Blake-Madden. The codes are stored in the directory `code/scripts/wordStem`, and the link of repository is listed in the *References* section below.

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #warning(name: [Warning])[
    We have to admitted that this word stemming program is kind of clumsy, especially for nouns, because the program will continue doing word stemming even though the word is in the simplest and the most common form. For example, for a simple English word "orange", it will convert it to another word "orang", which means "gorilla".

    Owing to the time and capability limitation, we couldn't find a better word stemming programs in C/C++ version or convert other languages version to C/C++ version. We hope that we will use a smarter word stemming program in the foreseeable future.
  ]

  #v(1em)
]
#render("thickness")

== Inverted Index

Maybe this is the most complicated part of the whole program, because in this part we have a relatively complex algorithm architecture, and we use a couple of data structrues and algorithms, such as B+ trees, implicit queue ADT and linked list ADT. Here is the diagram of the functions used in the inverted index:

#figure(
  image("../template/images/1.png"), caption: "Relation diagram of all functions for inverted index"
)

- #text(fill: red)[Red]: Overall Functions
- #text(fill: blue)[Blue]: B+ Tree Operations
- #text(fill: green)[Green]: Hashing Functions
- #text(fill: yellow)[Yellow]: Other Functions

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
      _dir_ $<-$ _SHAKESPEAREDIR_ \
      print("Now building an inverted Index:") \
      print("Do you want to search something in the default path({_dir_})?") \ 
      print("Please input yes or no: ") \
      input(_choice_) \
      switch _choice_ #i\
        case 'y': case 'Y': #i\
          break #d\
        case 'n': case 'N': #i\
          print("Please input the directory of the documents:") \
          print("Path: ") \
          input(_dir_) \
          break #d\
        default: #i\
          print("Choice Error!") \
          exit(1) \
          break #d\
      end #d\
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

    if _isTest_ is false then #i\
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
      if find an English word then do #i\
        _term_ $<-$ the English word \
        if _containStopWords_ is false and FindHashSW(_term_, _H_, true) >= 0 then #i\
          continue #d\
        endif \
        _term_ $<-$ WordStemming(_term_) \
        _isDuplicated_ $<-$ false \ 
        _nodebp_ $<-$ FindBP(_term_, _docCnt_, _T_, _isDuplicated_) \
        if _isDuplicated_ is false then #i\
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

#v(1em)

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #note(name: [Note])[
    - The order of our B+ tree is 4.
  ]

  #v(1em)
]
#render("thickness")

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
    #fakebold[Outputs]: the (possibly updated) B+ tree _T_ or recursively call itself again

    #fakebold[Procedure]: FindBP(_term_: #fakebold[string], _docCnt_: #fakebold[integer], _T_: #fakebold[BplusTree], _flag_: #fakebold[booleanPointer], _isSearch_: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _T_$->$childrenSize = 0 then #i\
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

    if _pos_ = -1 then #i\
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
        if _term_ = _nodebp_$->$_data_[_i_]$->$_term_ then #i\
          if _isSearch_ is false then #i\
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

#fakebold[Function]: Insert a term into the B+ tree.

#algo(
  header: [
    #fakebold[Inputs]:
    - _term_: term
    - _docCnt_: the index of the document
    - _nodebp_: the appropriate node where the term will be inserted
    - _Tree_: B+ tree containing the inverted index
        
    #fakebold[Outputs]: the updated B+ tree _Tree_

    #fakebold[Procedure]: InsertBP(_term_: #fakebold[string], _docCnt_: #fakebold[integer], _nodebp_: #fakebold[NodeBP], _Tree_: #fakebold[BplusTree])
  ],
  block-align: left,
)[
  Begin #i\ 
    _nodebp_$->$_data_[_nodebp_$->$_size_]$->$_term_ $<-$ _term_ \
    EnqueuePL(_docCnt_, _nodebp_$->$_data_[_nodebp_$->$_size_]$->$_poslist_) \
    _nodebp_$->$_size_ $<-$ _nodebp_$->$_size_ + 1 \
    Sort(_nodebp_$->$_data_) \
    _Tree_ $<-$ SplitBP(_nodebp_, _Tree_) \
    return _Tree_ #d\
  End
]

*(5) SplitBP*

#fakebold[Function]: Split the node when the node is full.

#algo(
  header: [
    #fakebold[Inputs]:

    - _nodebp_: the appropriate node where the term will be inserted
    - _Tree_: B+ tree containing the inverted index
    
    #fakebold[Outputs]: The updated B+ tree _Tree_, or recursively call itself to split _nodebp_'s parent node

    #fakebold[Procedure]: SplitBP(_nodebp_: #fakebold[NodeBP], _Tree_: #fakebold[BplusTree])
  ],
  block-align: left,
)[
  Begin #i\ 
    #comment[_ORDER_: (constant)the order of B+ trees]

    if (_nodebp_$->$_childrenSize_ = 0 and _nodebp_$->$_size_ <= _ORDER_) or (_nodebp_$->$_childrenSize_ > 0 and _nodebp_$->$_size_ < _ORDER_) then #i\
      return _Tree_ #d\
    endif \
    \ 
    #comment[_lnodebp_, _rnodebp_: the left and right part of the split node] \
    #comment[_tmpNodebp_: store the node temporarily] \
    #comment[_parent_: the parent node of nodebp] \
    #comment[_cut_: the position of the middle data] \
    \

    _parent_ $<-$ _nodebp_$->$_parent_ \

    if _parent_ = NULL then #i\
      _tmpNodebp_ $<-$ CreateBP() \
      Allocate memory for _parent_ \
      _Tree_ $<-$ _parent_ $<-$ _tmpNodebp_ #d\
    endif \

    _lnodebp_ $<-$ CreateBP() \
    _rnodebp_ $<-$ CreateBP() \
    _lnodebp_$->$_parent_ $<-$ _rnodebp_$->$_parent_ $<-$ _parent_ \

    if _nodebp_$->$_childrenSize_ = 0 then #i\
      _cut_ $<-$ _LEAFCUT_ #comment[constant: (ORDER / 2 + 1)] \
      for _i_ in range(0, _cut_) do #comment[not contains _cut_] #i\
        _lnodebp_$->$_data_[_i_] $<-$ _nodebp_$->$_data_[_i_] #d\
      end \
      _lnodebp_$->$_size_ $<-$ _cut_ \ 

      for _j_ in range(_cut_, _nodebp_$->$_size_) do #comment[not contains _nodebp_$->$_size_] #i\
        _rnodebp_$->$_data_[_j_ - _cut_] $<-$ _nodebp_$->$_data_[_j_] #d\
      end \
      _rnodebp_$->$_size_ $<-$ _nodebp_$->$_size_ - _cut_ #d\
    else #i\
      _cut_ $<-$ _NONLEAFCUT_ #comment[constant: (ORDER / 2)] \
      for _i_ in range(0, _cut_ + 1) do #comment[not contains _cut_ + 1] #i\
        if _i_ $eq.not$ _cut_ then #i\
          _lnodebp_$->$_data_[_i_] $<-$ _nodebp_$->$_data_[_i_] #d\
        endif \
        _lnodebp_$->$_children_[_i_] $<-$ _nodebp_$->$_children_[_i_] \
        _lnodebp_$->$_children_$->$_parent_ $<-$ _lnodebp_ #d\
      end \
      _lnodebp_$->$_size_ $<-$ _cut_ \ 
      _lnodebp_$->$_childrenSize_ $<-$ _cut_ + 1\

      for _j_ in range(_cut_ + 1, _nodebp_$->$_size_) do #comment[not contains _nodebp_$->$_size_] #i\
        _rnodebp_$->$_data_[_j_ - _cut_ - 1] $<-$ _nodebp_$->$_data_[_j_] #d\
      end \

      for _j_ in range(_cut_ + 1, _nodebp_$->$_childrenSize_) do #comment[not contains _nodebp_$->$_childrenSize_] #i\
        _rnodebp_$->$_children_[_j_ - _cut_ - 1] $<-$ _nodebp_$->$_children_[_j_] \
        _rnodebp_$->$_children_[_j_ - _cut_ - 1]$->$_parent_ $<-$ _rnodebp_ #d\        
      end \      

      _rnodebp_$->$_size_ $<-$ _nodebp_$->$_size_ - _cut_ - 1 
      _rnodebp_$->$_childrenSize_ $<-$ _nodebp_$->$_childrenSize_ - _cut_ - 1#d\
    end \

    _parent_$->$_data_[_parent_$->$_size_] $<-$ _nodebp_$->$_data_[_cut_] \
    _parent_$->$_size_ $<-$ _parent_$->$_size_ + 1 \
    if _parent_$->$_childrenSize_ > 0 then #i\
      for _i_ in range(0, _parent_$->$_childrenSize_) do #comment[not contains _parent_$->$_childrenSize_] #i\
        if _parent_$->$_children_[_i_] = _nodebp_ then #i\
          _parent_$->$_children_[_i_] $<-$ _lnodebp_ \
          break #d\
        endif #d\
      end #d\
    else #i\
      _parent_$->$_children_[_parent_$->$_childrenSize_] $<-$ _lnodebp_ \
      _parent_$->$_childrenSize_ $<-$ _parent_$->$_childrenSize_ + 1 #d\ 
    endif \
    _parent_$->$_children_[_parent_$->$_childrenSize_] $<-$ _rnodebp_ \
    _parent_$->$_childrenSize_ $<-$ _parent_$->$_childrenSize_ + 1 \
    Sort(_parent_$->$_data_) \
    Sort(_parent_$->$_children_) \

    _Tree_ $<-$ SplitBP(_parent_, _Tree_) \

    return _Tree_ #d\
  End
]

*(6) PrintBPTree*

#fakebold[Function]: Print the B+ tree(level-order traversal).

#algo(
  header: [
    #fakebold[Inputs]:
    
    - _T_: B+ tree containing the inverted index

    #fakebold[Outputs]: None, but will print the whole B+ tree

    #fakebold[Procedure]: PrintBPTree(_T_: #fakebold[BplusTree])
  ],
  block-align: left,
)[
  Begin #i\ 
    print("B+ Tree of Inverted Index:") \

    _q_ $<-$ CreateQueueBP() \
    EnqueueBP(_T_, _q_) \
    EnqueueBP(_NULL_, _q_) \

    while _q_$->$_size_ > 0 do #i\
      _nodebp_ $<-$ DequeueBP(q) \
      if _nodebp_ is _NULL_ then #i\
        change to a newline \
        if _q_$->$_size_ > 0 then #i\
          EnqueueBP(_NULL_, _q_) #d\
        endif #d\
      else #i\
        print("[") \
        for _i_ in range(0, _nodebp_$->$_size_) do #comment[not contains _nodebp_$->$_size_] #i\
          if _i_ = 0 then #i\
            print(_nodebp_$->$_data_[_i_]$->$_term_) #d\
          else #i\
            print(", {_nodebp_$->$_data_[_i_]$->$_term_}") #d\
          endif #d\
        end \
        print("]") #d\
      endif \

      if nodebp is not _NULL_ then #i\
        for _i_ in range (0, _nodebp_$->$_childrenSize_) do #comment[not contains _nodebp_$->$_childrenSize_] #i\
          EnqueueBP(_nodebp_$->$_children_[_i_], _q_) #d\
        end #d\
      endif #d\
    end #d\
  End
]

=== Hashing Operations

*(1) GenerateHashTb*

#fakebold[Function]: Build a hash table.

#algo(
  header: [
    #fakebold[Inputs]: None
    
    #fakebold[Outputs]: A new hash table _H_, containing stopwords from the file

    #fakebold[Procedure]: GenerateHashTb()
  ],
  block-align: left,
)[
  Begin #i\ 
    _H_ $<-$ InitHashTb() \
    _fname_ $<-$ _STOPWORDPATH_ #comment[constant: "stop_words.txt"] \
    _fp_ $<-$ openfile(_fname_, "r") #comment[read mode] \
    if _fp_ is _NULL_ then #i\
      Error("Fail to open the file of stopwords!") #d\
    endif \

    while reading texts in the file pointed by _fp_ do #i\
      if find an English word then do #i\
        _term_ $<-$ the English word \
        InsertHashSW(_term_, _H_) #d\
      endif #d\
    end \

    closefile(_fp_) \
    return _H_ #d\
  End
]

*(2) InitHashTb*

#fakebold[Function]: Initialization of the hash table.

#algo(
  header: [
    #fakebold[Inputs]: None
    
    #fakebold[Outputs]: A new initialized hash table

    #fakebold[Procedure]: InitHashTb()
  ],
  block-align: left,
)[
  Begin #i\ 
    Allocate memory block for _H_ #comment[HashTb] \
    if _H_ is _NULL_ then #i\
      Error("Fail to create a hash table for stopwords!") #d\
    end \

    _H_$->$_size_ $<-$ _STOPWORDSUM_ #comment[maximum size] \

    for i in range(0, _H_$->$_size_) do #comment[not contains _H_$->$_size_] #i\
      Allocate memory block for _H_$->$_data_[_i_] \
      if _H_$->$_data_[_i_] is _NULL_ then #i\
        Error("Fail to create a hash table for stopwords!") #d\
      end \
      _H_$->$_data_[_i_]$->$_stopword_ \
      _H_$->$_data_[_i_]$->$_info_ $<-$ _Empty_ #comment[constant: 0] #d\
    end \
    return _H_ #d\
  End
]

*(3) FindHashSW*

#fakebold[Function]: Find the stopwords or other words in the hash table.

#algo(
  header: [
    #fakebold[Inputs]:
    - _stopword_: stop word
    - _H_: hash table containing the stop words
    - _justSearch_: find the term without subsequent insertion    
    
    #fakebold[Outputs]: A appropriate position _pos_ in hash table for stopword, or just search the term in the hash table

    #fakebold[Procedure]: FindHashSW(stopword: #fakebold[string], H: #fakebold[HashTb], justSearch: #fakebold[boolean])
  ],
  block-align: left,
)[
  Begin #i\ 
    _collisionNum_ $<-$ 0 \
    _pos_ $<-$ HashFunc(_stopword_, _H_$->$_size_) \

    if _justSearch_ is true and (_H_$->$_data_[_pos_]$->$_info_ = _Empty_ or _H_$->$_data_[_pos_]$->$_stopword_ = _stopword_) then #i\
      return -1 #d\
    endif \

    while _H_$->$_data_[_pos_]$->$_info_ $eq.not$ _Empty_ and _H_$->$_data_[_pos_]$->$_stopword_ = _stopword_ do #i\
      _collisionNum_ $<-$ _collisionNum_ + 1
      _pos_ $<-$ _pos_ + 2 \* _collisionNum_ - 1 \
      if _pos_ >= _H_$->$_size_ then #i\
        _pos_ $<-$ _pos_ - _H_$->$_size_ #d\
      endif #d\
    end \
    return _pos_ #d\
  End
]

*(4) InsertHashSW*

#fakebold[Function]: Insert a new stopword in hash table.

#algo(
  header: [
    #fakebold[Inputs]:
    - _stopword_: stop word
    - _H_: hash table containing the stop words

    #fakebold[Outputs]: None, but will update the hash table _H_

    #fakebold[Procedure]: InsertHashSW(_stopword_: #fakebold[string], _H_: #fakebold[HashTb])
  ],
  block-align: left,
)[
  Begin #i\ 
    _pos_ $<-$ FindHashSW(_stopword_, _H_, false) \

    #comment[_Legitimate_: (constant) 1] \
    if (_H_$->$_data_[_pos_]$->$_info_ $eq.not$ _Legitimate_) then #i\
      _H_$->$_data_[_pos_]$->$_info_ $<-$ _Legitimate_ \
      _H_$->$_data_[_pos_]$->$_stopword_ $<-$ _stopword_ #d\
    endif #d\
  End
]

*(5) HashFunc*

#fakebold[Function]: Hashing function.

#algo(
  header: [
    #fakebold[Inputs]:

    - _stopword_: stop word
    - _size_: the maximum size of the hash table
    
    #fakebold[Outputs]: A hash value to _stopword_

    #fakebold[Procedure]: HashFunc(_stopword_: #fakebold[string], _size_: #fakebold[integer])
  ],
  block-align: left,
)[
  Begin #i\ 
    _val_ $<-$ 0 \
    for each character _ch_ in _stopword_ do #i\
      _val_ = (_val_ << 5) + integer(_ch_) #d\
    end \
    return _val_ % _size_ #d\
  End
]

=== Other Functions

*(1) CreateQueueBP*

#fakebold[Function]: Create the queue

#algo(
  header: [
    #fakebold[Inputs]: None
    
    #fakebold[Outputs]: A new queue _Q_

    #fakebold[Procedure]: CreateQueueBP()
  ],
  block-align: left,
)[
  Begin #i\ 
    Allocate memory block for the queue _Q_ \
    _Q_$->$_size_ $<-$ 0 \
    _Q_$->$_front_ $<-$ _Q_$->$_rear_ $<-$ 0 \
    return _Q_ #d\
  End
]

*(2) EnqueueBP*

#fakebold[Function]: Put the node of B+ tree into the queue.

#algo(
  header: [
    #fakebold[Inputs]:

    - _nodebp_: the newly added node
    - _Q_: the queue
    
    #fakebold[Outputs]: None, but will update _Q_

    #fakebold[Procedure]: EnqueueBP(_nodebp_: #fakebold[NodeBP], _Q_: #fakebold[QueueBP])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _Q_$->$_size_ >= _SIZE_ then #i\
      Error("Full B+-tree-item queue!") #d\
    endif \
    _Q_$->$_data_[_Q_$->$_rear_] $<-$ _nodebp_ \
    _Q_$->$_rear_ $<-$ _Q_$->$_rear_ + 1 \
    _Q_$->$_size_ $<-$ _Q_$->$_size_ + 1 #d\
  End
]

*(3) DequeueBP*

#fakebold[Function]: Get the front node and delete it from the queue.

#algo(
  header: [
    #fakebold[Inputs]:

    - _Q_: the queue
    
    #fakebold[Outputs]: the front node _returnNodeBP_

    #fakebold[Procedure]: DequeueBP(_Q_: #fakebold[QueueBP])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _Q_$->$_size_ = 0 then #i\
      Error("Empty B+-tree-item queue!") #d\
    endif \
    _returnNodeBP_ $<-$ _Q_$->$_data_[_Q_$->$_front_] \
    _Q_$->$_front_ $<-$ _Q_$->$_front_ + 1 \
    _Q_$->$_size_ $<-$ _Q_$->$_size_ - 1 \
    return _returnNodeBP_ #d\
  End
]

*(4) CreatePL*

#fakebold[Function]: Create the poslist

#algo(
  header: [
    #fakebold[Inputs]: None
    
    #fakebold[Outputs]: A new PosList _L_

    #fakebold[Procedure]: CreatePL()
  ],
  block-align: left,
)[
  Begin #i\ 
    Allocate memory blocks for _L_(#fakebold[PosList]), _L_$->$_front_(#fakebold[PosData]), _L_$->$_rear_(#fakebold[PosData]) \
    _L_$->$_size_ $<-$ 0 \
    _L_$->$_front_ $<-$ _L_$->$_rear_ \
    _L_$->$_rear_$->$_pos_ $<-$ -1 \

    return _L_ \
  End
]

*(5) EnqueuePL*

#fakebold[Function]: Add new position.

#algo(
  header: [
    #fakebold[Inputs]:

    - _pos_: the position
    - _L_: the position list
    
    #fakebold[Outputs]: None, but will update _L_

    #fakebold[Procedure]: EnqueuePL(_pos_: #fakebold[integer], _L_: #fakebold[PosList])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _L_$->$_rear_$->$_pos_ $eq.not$ _pos_ then #i\
      Allocate memory block for _tmp_(#fakebold[PosData]) \
      if _tmp_ is _NULL_ then #i\
        Error("Fail to create a new position data!") #d\
      endif \
      _tmp_$->$_pos_ $<-$ _pos_ \
      _tmp_$->$_time_ $<-$ 1 \
      _tmp_$->$_next_ $<-$ _L_$->$_rear_$->$_next_ \
      _L_$->$_rear_$->$_next_ $<-$ _tmp_ \
      _L_$->$_rear_ $<-$ _tmp_ \
      _L_$->$_size_ $<-$ _L_$->$_size_ + 1 #d\
    else #i\
      _L_$->$_rear_$->$_time_ $<-$ _L_$->$_rear_$->$_time_ + 1 #d\
    endif #d\
  End
]

*(6) RetrievePL*

#fakebold[Function]: Retrieve all position in the list.

#algo(
  header: [
    #fakebold[Inputs]:

    - _L_: the position list

    #fakebold[Outputs]: An 2D array _posArr_ containing the all position in _L_, and each data contains two attributes: document index and the frequency in that document

    #fakebold[Procedure]: RetrievePL(_L_: #fakebold[PosList])
  ],
  block-align: left,
)[
  Begin #i\ 
    if _L_$->$_size_ = 0 then #i\
      Error("Empty position-data queue!") #d\
    endif \

    Allocate memory block for _posArr_
    _cur_ $<-$ _L_$<-$_front_$<-$_next_ \
    _i_ $<-$ 0 \
    while _cur_ $eq.not$ _NULL_ do #i\
      _posArr_[_i_][0] $<-$ _cur_$->$_pos_ \
      _posArr_[_i_][1] $<-$ _cur_$->$_time_ \
      _cur_ $<-$ _cur_$->$_next_ \
      _i_ $<-$ _i_ + 1 #d\
    end \

    return _posArr_ #d\
  End
]

== Query

=== Search Function

#algo(
  header: [
    #fakebold[Inputs]:

    - _query_: User's query in string form
    - _T_: Inverted index
    - _pageSize_: Number of documents to be displayed per query
    - _threshold_: The proportion of search terms to the total number of terms

    #fakebold[Outputs]: The query result

    #fakebold[Procedure]: search(_query_: #fakebold[string], _T_: #fakebold[BplusTree], _pageSize_: #fakebold[integer], _threshold_: #fakebold[double])
  ],
  block-align: left,
)[
Begin #i\ 

  for read every character in _query_ do #i\
    remove the Punctuation and Whitespace \
    if get the word then #i\
      store the word itself and the _idf_ of word in a vector #d\
    endif #d\
  end \
  
  sort the vector in descending order \

  if no word then #i\
    return #d\
  else if one word then #i\
    search the word in _T_ \
    sort the file name by _tf_ \
    print(_file_, _tf_) #d\
  else #i\
    according to the threshold get the word needed to searched \
    for every word do #i\
      search the word in _T_ \
      store the result in a hash table \
      (the hash table store the file name and their _tf-idf_) #d\
    end \
    print(_file_, _td-idf_) #d\
  endif #d\
End
]

The main idea of the function is to implement a search functionality that retrieves documents related to a given query from a B+ tree. Here is a more detailed summary of the function logic:

1. *Query parsing*: The code starts by loading and analyzing the query string, breaking it down into words while converting each to lowercase and stemming. It checks if each word exists in a stop words list or if it is present in the inverted index.

2. *Valid word collection*: Valid words are added to a vector, while warnings are displayed for invalid words (such as stop words or words not found in the inverted index).

3. *Search condition assessment*: Depending on the number of valid words, the processing is divided into two modes. If there is only one valid word, it searches directly in the inverted index and returns the matching documents with their TF values. For multiple valid words, the code calculates their TF-IDF values.

4. *Inverted index lookup*: For multi-word queries, the code traverses the inverted index for each valid word, using a hash table to store document IDs and their corresponding TF-IDF scores while ensuring it only retains documents that contain all the query words.

5. *Result sorting and output*: Finally, the results are sorted based on the TF-IDF values, and the document names along with their TF-IDF scores are printed, adhering to a specified limit on the number of results (page size).

Overall, this function provides an efficient document retrieval mechanism through valid word filtering and TF-IDF score calculation, suitable for the context of search engines or information retrieval systems.

=== Search Test

#algo(
  header: [
    #fakebold[Inputs]: Files of _The Works_

    #fakebold[Outputs]: The results of queries

    #fakebold[Procedure]: SearchTest()
  ],
  block-align: left,
  strong-keywords: false
)[
*Begin* #i\ 
  load necessary resources and establish inverted index \

  *while* test_allow *do* #i\
    get serial number of the test input form user \
 
    get the query from the input file \

    get important parameters from user's input \

    call the search function with the query and parameters \

    ask user if they want to continue testing or not and set test_allow accordingly #d\
  *end* #d\
*End*
]

The main idea of the code is to set up a testing framework for the "ShakespeareFinder" search application. This includes loading necessary resources, allowing users to input parameters from a test file, executing search queries multiple times to evaluate performance, and managing repeated tests.

=== Some helper functions

*(1) Load function: loadWordIdf & loadFileWordsNum & loadStopWords*

These functions have a similar structure. The main idea is to load resources from existing files. See the pseudocode for details.

#algo(
  header: [
    #fakebold[Inputs]: Files of _The Works_

    #fakebold[Outputs]: The results of queries

    #fakebold[Procedure]: SearchTest()
  ],
  block-align: left,
  strong-keywords: false
)[
*Begin* #i\ 
  load the input FILE \
  read resource from the file \
  load the resource into a hash table #d\
*End*
]

*(2) Search helper: FindBP2 & isSameTerm2 & RetrievePL2*

These functions are very similar to their versions without the "2", but in order to better cooperate with the search function, the type of data they return is changed to `vector<pair<docId, tf>>`, and the logic of getting data is changed accordingly.

= Chapter 3: Testing Results

== Inverted Index Tests

To verify the correctness of our inverted index, we have devised several tests from different aspects. Here is the #fakebold[purpose] of each test: 

- Check if every word in document(s) is inserted into the inverted index correctly.
- Build an inverted index from a single file, or a directory with a bunch of files.
- Check if the inverted index can eliminate all stopwords.

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #warning(name: [Warning])[
    In the following tests, we will use the test programs to verify the correctness of our sub-programs seperately, and we #fakebold[won't tell you the usage of these intructions] we use below. If you are curious about it, please read the `README.md` file in the directory `code`, which will guide you how to run these instructions.
  ]

  #v(1em)
]
#render("thickness")

=== Word Insertion Test

We have two method to accomplish the first purpose: printing the whole inverted index(when the size is small), and finding the words existing in the inverted index(if words were correctly inserted). 

*(1) Printing the inverted index*

#table(columns: 1fr, align: left)[Case 1: very simple example][
  ```bash $ ./invIndexTest -t -p``` 
` 
Now testing the correctness of inverted Index: 
Please input the name of the input sample file:
Name:` ```bash input1.txt``` \
`Build successfully!
B+ Tree of Inverted Index:
[beauti, ice, peach]
[appl, are, banana][beauti, cherri][ice, icecream, orang][peach, pear, strawberri, watermelon]
`
][input1.txt][
ice \
strawberry \
orange \
banana \
peach \
apple \ 
pear \ 
watermelon \
cherry icecream you are beautiful 
]

#v(1em)

#table(columns: 1fr, align: left)[Case 2: simple example][
```bash $ ./invIndexTest -t -p``` \
`Now testing the correctness of inverted Index:
Please input the name of the input sample file:
Name:` ```bash input2.txt``` \
`Build successfully!
B+ Tree of Inverted Index:
[et, lorem, nullam]
[consectetur, dolor, elit][id, ipsum][nec][pretium, sed, ut]
[adipisc, amet, at, congu][consectetur, consequat, dapibus, diam][dolor, e, eget][elit, erat][et, etiam, facilisi, fringilla][id, interdum][ipsum, lacus, lectus][lorem, metus, mi][nec, nulla][nullam, orci, pellentesqu][pretium, purus, rhoncus][sed, sit, sollicitudin, tincidunt][ut, vita]`
][input2.txt][
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam nec erat sed nulla rhoncus dapibus et at lectus. Etiam in congue diam, ut interdum metus. Nullam pretium orci id mi pellentesque, vitae consequat lacus tincidunt. Pellentesque fringilla purus eget nulla facilisis sollicitudin.   
]

*(2) Finding words in the inverted index*

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #note(name: [Note])[
    - This test is just used in checking the correctness of word insertion, which is similar to a simple query function, but the implementation is totally different from our formal query program, so you shouldn't mix them together.
    - The texts in the following tests are too long, therefore we won't show these text in our report, but you can see them in the files positioned in the directory called `code/data/tests`.
  ]

  #v(1em)
]
#render("thickness")

#table(columns: 1fr, align: left)[Case 3: intermediate-level example][
```bash  ./invIndexTest -f=3``` \
`Now building an inverted Index:
Please input the directory of the documents:
Path:` ```bash tests/input3``` \
`Build successfully!

Finding Words Mode(only supports single word finding):
Find 1:` ```bash same``` \
`Successfully find the word!
The word was found in files below:
1henryiv.1.2.txt: 1 time
1henryiv.1.3.txt: 4 times
Frequency: 5
-----------------------------------
Find 2:` ```bash star``` \
`Successfully find the word!
The word was found in files below:
1henryiv.1.2.txt: 1 time
Frequency: 1
-----------------------------------
Find 3:` ```bash fantastic``` \
`Sorry, no such word in the inverted index!
----------------------------------- `
][Verification by using finding function in Visual Studio Code][
Find 1: same

#grid(
  columns: 1,
  image("../template/images/2.png"), 
  image("../template/images/3.png"),
  image("../template/images/4.png"), 
)

#v(1em)

Find 2: star

#grid(
  columns: 1,
  image("../template/images/5.png"), 
  image("../template/images/6.png"),
  image("../template/images/7.png"), 
)

#v(1em)

Find 3: fantastic

#grid(
  columns: 1,
  image("../template/images/8.png"), 
  image("../template/images/9.png"),
  image("../template/images/10.png"), 
)

]

In a nutshell, our inverted index program successfully #text(fill: green)[passes] the first test.

=== Single File to Multiple Files Test

We executed our first test based on a single file and several files,  but our ultimate goal is to let our mini search engine to search something from a dozens of files(i.e. _the Complete Works of Shakespeare_). So it's necessary for us to test whether the inverted index can be built from tons of files. Note that `the Works` is in the directory called `code/data/shakespeare_works`.

#table(columns: 1fr, align: left)[Case 1: Search some dedicated words from piles of files][
```bash ./invIndexTest -f=3``` \
`Now building an inverted Index:
Please input the directory of the documents:
Path:` ```bash ../data/shakespeare_works``` \
`Build successfully!

Finding Words Mode(only supports single word finding):
Find 1:` ```bash hamlet``` \
`Successfully find the word!
The word was found in files below:
hamlet.1.1.txt: 3 times
hamlet.1.2.txt: 42 times`
```bash
# Deleberate omisssion
```
`hamlet.5.1.txt: 45 times
hamlet.5.2.txt: 83 times
Frequency: 470
-----------------------------------
Find 2:` ```bash juliet``` \
`Successfully find the word!
The word was found in files below:
measure.1.2.txt: 3 times
measure.1.4.txt: 1 time`
```bash
# Deleberate omisssion
```
`romeo_juliet.5.2.txt: 1 time
romeo_juliet.5.3.txt: 19 times
Frequency: 199
-----------------------------------
Find 3:` ```bash macbeth``` \
`Successfully find the word!
The word was found in files below:
macbeth.1.1.txt: 1 time
macbeth.1.2.txt: 4 times`
```bash
# Deleberate omisssion
```
`macbeth.5.7.txt: 8 times
macbeth.5.8.txt: 7 times
Frequency: 285
-----------------------------------`
]


#table(columns: 1fr, align: left)[Case 2: Search some universal words from piles of files][
```bash ./invIndexTest -f=3``` \
`Now building an inverted Index:
Please input the directory of the documents:
Path:` ```bash ../data/shakespeare_works``` \
`Build successfully!

Finding Words Mode(only supports single word finding):
Find 1:` ```bash moon``` \
`Successfully find the word!
The word was found in files below:
1henryiv.1.2.txt: 5 times
1henryiv.1.3.txt: 1 time`
```bash
# Deleberate omisssion
```
`winters_tale.4.3.txt: 1 time
winters_tale.4.4.txt: 1 time
Frequency: 152
-----------------------------------
Find 2:` ```bash happy``` \
`Successfully find the word!
The word was found in files below:
1henryiv.2.2.txt: 1 time
1henryiv.4.3.txt: 1 time`
```bash
# Deleberate omisssion
```
`winters_tale.1.2.txt: 2 times
winters_tale.4.4.txt: 3 times
Frequency: 278
-----------------------------------
Find 3:` ```bash hit``` \
`Successfully find the word!
The word was found in files below:
1henryiv.2.4.txt: 1 time
2henryiv.1.1.txt: 1 time`
```bash
# Deleberate omisssion
```
`VenusAndAdonis.txt: 2 times
winters_tale.5.1.txt: 1 time
Frequency: 74
-----------------------------------`  
]

In a nutshell, our inverted index program successfully #text(fill: green)[passes] the second test.

=== Stopwords Test

Finally, we should confirm whether our program can eliminate the stopwords we have selected in advance. So we can make a comparison with two test program: one includes the stopwords, while the other doesn't include them.

#table(columns: 1fr, align: left)[Case 1: stopwords #fakebold[not included]\(defualt situation\)][
```bash ./invIndexTest -f=3``` \
`Now building an inverted Index:
Please input the directory of the documents:
Path:` ```bash ../data/shakespeare_works``` \
`Build successfully!

Finding Words Mode(only supports single word finding):
Find 1:` ```bash much``` \
`Sorry, no such word in the inverted index!
-----------------------------------
Find 2:` ```bash you``` \
`Sorry, no such word in the inverted index!
-----------------------------------
Find 3:` ```bash great``` \
`Sorry, no such word in the inverted index!
-----------------------------------`
]

#table(columns: 1fr, align: left)[Case 2: stopwords #fakebold[included]][
```bash ./invIndexTest -f=3 -s``` \
`Now building an inverted Index:
Please input the directory of the documents:
Path:` ```bash ../data/shakespeare_works``` 
```bash
# Delebrate omission for the display of all stopwords
```
`Build successfully!

Finding Words Mode(only supports single word finding):
Find 1:` ```bash much``` 
```bash
# Delebrate omission for the very long position list
```
`winters_tale.5.1.txt: 4 times
winters_tale.5.2.txt: 2 times
winters_tale.5.3.txt: 7 times
Frequency: 1070
-----------------------------------
Find 2:` ```bash you``` 
```bash
# Delebrate omission for the very long position list
```
`winters_tale.5.1.txt: 40 times
winters_tale.5.2.txt: 19 times
winters_tale.5.3.txt: 29 times
Frequency: 14249
-----------------------------------
Find 3:` ```bash great``` 
```bash
# Delebrate omission for the very long position list
```
`winters_tale.5.1.txt: 3 times
winters_tale.5.2.txt: 1 time
winters_tale.5.3.txt: 1 time
Frequency: 1032
-----------------------------------`
]

Actually, the inverted index can eliminate all stopwords, but due to space limitation, we won't list all tests about them.

In a nutshell, our inverted index program successfully #text(fill: green)[passes] the third test.

Although we can't make a thorough test for the inverted index, but from the above tests, we can assure that our inverted index have no obvious error(maybe there're several small bugs existing).


=== Speed Test

#v(.5em)

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #note(name: [Note])[
    The specific analysis and comments about speed tests are written in *Chapter 4*.
  ]

  #v(1em)
]
#render("thickness")

To analyze the time complexity of the inverted index, especially the algorithms regarding the #fakebold[finding] and #fakebold[insertion] operations of B+ tree, we devise some timing tests for #fakebold[diffrent numbers of words] in _The Works_. The results are shown below:


#figure(
  table( 
    fill: (x, y) =>
    if x == 0 {
      gray.lighten(80%)
    },
    columns: (9em, 1fr, 1fr, 1fr, 1fr, 1fr, 1fr), [Number of Words(roughly)], [100,000], [200,000], [400,000], [600,000], [800,000], [880,000], [Iterations], [10], [10], [10], [10], [5], [5], [Ticks], [1988219], [3704927], [7317245], [11594770], [7985568], [8438331], [Total Time(s)], [1.99], [3.70], [7.32], [11.59], [7.99], [8.44], [Duration(s)], [0.199], [0.370], [0.732], [1.159], [1.598], [1.688]), caption: "Speed Tests for Inverted Index"
)

#figure(
  image("../template/images/iist_diagram.png", width: 120%), caption: "Curve Chart of the Speed Tests for Inverted Index"
)

As you can see, these data points can be fitted with the product of linear function and #fakebold[logarithmic function] (although the curve looks like a straight line due to the limitation of aspect ratio). So the result indicates that the time complexity of building inverted index approaches O($log N$), and you will see the detailed explanation in Chapter 4.


== Query Tests

Different from the oriented-procedure inverted index tests, the query tests are based on distinct and typical #fakebold[input data], which consider the performance, speed and threshold analysis simultaneously.



=== input0

#v(.5em)

- *content* : 1000\*'a'
- *purpose*: Test for illegal words of extreme length
- *expected results*:

  #table(align: left, columns: 1fr)[
    ```shell
    Warning: "a...a" is not in the inverted index and will be ignored.
    Ops, your query is in StopList or empty, so there are no documents retrieved.
    ```
  ]

- *test results*:

  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][155.835][1.6e-4]

  The time is averaged from 1000 runs. Since irrelevant words will not enter the search phase, the threshold is irrelevant to this test.

=== input1

#v(.5em)

- *content* : peas and beans

- *purpose*: One stop word, two rare words (each appearing in only two documents), but all appearing in one document. This tests the retrieval correctness of the program and its handling of stop words.

- *expected results*：
  
  #table(align: left, columns: 1fr)[
    ```shell
    Warning: "and" is in the stop list and will be ignored.
    Your query has multiple valid words, so we will search for them in the inverted index.
    The words were found in files below:
    File name                       Tf-Idf
    1henryiv.2.1.txt                0.014780
    ```
  ]

- *test result*:
  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][79.439][8e-5]

  The time is averaged from 1000 runs. Since the test purpose does not include the threshold, the threshold is set to 1 here.

=== input2

#v(.5em)

- *content* : blank input

- *purpose*: Testing for blank input

- *expected results*：

  #table(align: left, columns: 1fr)[
    ```shell
    Ops, your query is in StopList or empty, so there are no documents retrieved. 
    ```
  ]

- *Test result*

  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][15.246][2e-5]  
  
  The time is averaged from 1000 runs.Since irrelevant words will not enter the search phase, the threshold is irrelevant to this test.

=== input3

#v(.5em)

- *content* : three stop words

- *purpose*: Testing for stop words

- *expected results*：
  
  #table(align: left, columns: 1fr)[
    ```shell
    Warning: "call" is in the stop list and will be ignored.
    Warning: "other" is in the stop list and will be ignored.
    Warning: "man" is in the stop list and will be ignored.
    Ops, your query is in StopList or empty, so there are no documents retrieved. 
    ```
  ]

- *test result*:
  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][51.027][5e-5]    
  
  The time is averaged from 1000 runs.Since irrelevant words will not enter the search phase, the threshold is irrelevant to this test.

=== input4

#v(.5em)

- *content* : Complete 1henryiv.1.2

- *purpose*: Test detection of extreme length correct text. Test the performance of searches at different thresholds

- *expected results*(threshold=1.0):
  
  #table(align: left, columns: 1fr)[
    ```shell
    Warning:...
    ...(omit numorous warning about stop words)
    Your query has multiple valid words, so we will search for them in the inverted index.
    The words were found in files below:
    File name                       Tf-Idf
    1henryiv.1.2.txt                5.785513
    ```
  ]

- *test result*:

  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][129114.1][0.13]

  The time is averaged from 10 runs, and `threshold=1`
  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (1fr, 1fr, 1fr, 1fr), table.header([threshold],[rank],[tf-idf], [search time]))[0.01][1/1][0.024940][0.02]

  No more thresholds were tested here because the situation was too extreme and the correct documents were already filtered out at a threshold of 0.01.Increasing the threshold will only increase the running time, so it is not tested here..

=== input5

#v(.5em)

- *content* : All sects, all ages smack of this vice; and he To die for't!

- *purpose*: This sentence is choose from measure.2.2. Select this to test the effect of the threshold on search results.

- *expected results*(threshold=1.0):
  
  #table(align: left, columns: 1fr)[
    ```shell
    Warning: "all" is in the stop list and will be ignored.
    Warning: "all" is in the stop list and will be ignored.
    Warning: "of" is in the stop list and will be ignored.
    Warning: "this" is in the stop list and will be ignored.
    Warning: "and" is in the stop list and will be ignored.
    Warning: "he" is in the stop list and will be ignored.
    Warning: "to" is in the stop list and will be ignored.
    Warning: "for" is in the stop list and will be ignored.
    Warning: "t" is in the stop list and will be ignored.
    Your query has multiple valid words, so we will search for them in the inverted index.
    The words were found in files below:
    File name                       Tf-Idf
    measure.2.2.txt                 0.017464   0.015054
    ```
  ]

- *test result*:
  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (8em, 1fr, 1fr), table.header([output],[ticks(ticks/search)],[time(s/search)]))[correct][992.903][9.9e-4]

  The time is averaged from 1000 runs, and `threshold=1`.
  
  #set table(
    fill: (x, y) =>
      if y == 0 {
        gray.lighten(80%)
      },
  )

  #show table.cell.where(y: 0): fakebold  

  #table(columns: (1fr, 1fr, 1fr, 1fr), table.header([threshold],[rank],[tf-idf], [search time]))[0.2][3/7][0.003003][2e-4][0.4][1/1][0.005808][2e-4][0.6][1/1][0.011469][3e-4][0.8][1/1][0.012638][5e-4][1.0][1/1][0.015054][9.9e-4]

  From the test results, we can see that the works can be correctly screened out with a threshold of about 0.4.
  
  I also tested some sentences with similar results, which I will not list here.


= Chapter 4: Analysis and Comments

#v(.5em)

#let render(theme_name) = [
  #state_block_theme.update(theme_name)

  #note(name: [Note])[
    - We only care the memory space of #fakebold[major] data structures
    - It's undeniable that every word has different length, but we set the smallest unit to "word", not "character" for our convenience of analysis 
  ]

  #v(1em)
]
#render("thickness")

== Space Complexity

*Conclusion*: $O(W + D + H + I + M + K)$

- $W$: Word count all articles in _The Works_
- $D$: The number of documents(files, or articles in _The Works_)
- $H$: The size of the hash table which contains all stopwords
- $I$: The size of the inverted index(notice: we have removed the duplicated words, so every two nodes contain distinct words)
- $M$: The number of valid words
- $K$: The number of found documents for a single word

*Analysis*: 

We should analyze the space complexity step by step:

- #fakebold[Word count]: We use C++ STL containers(`pair`, `map` and `set`) to store the essential information about words and files. Specifically, `wordList` records the word count in all files for every word; `wordNumOfDoc` involves the word count for every file; `wordDocs` contains the number of articles where words apppear for every word. Consequently, the memory space in this step depends on both word count in all files ($W$) and the number of files($D$).

- #fakebold[Stop words]: Actually, in the word count program, we also extracted the stop words from the variables mentioned above, then these words will be stored in a hash table for fast finding when building the inverted index. Therefore, the size of hash table($H$) represents the space that stopwords use.

- #fakebold[Inverted index]: The bulk of inverted index is stored in a B+ tree. It's universally acknowledged that the space complexity of B+ tree is $O(N)$, when $N$ is the number of data. However, our program has some uncertain factors, for instance, we can't control the size of the position list for each word, because the specific frequency of words are different. But what we can assure is that the total size of all position list is proportional to unduplicated word count, which is less than $W$. As a consequence, we only care $I$ = $N times$ `ORDER`, where `ORDER` means the order of the B+ tree, and we allocate `ORDER + 1` bytes for data and children in each node.  

- #fakebold[Query]: There are four data structures used in this part: `queryWord` stores up to `m` words, yielding $O(M)$; `posVec` holds document positions, with a maximum size of `k`, contributing $O(K)$; `freqMap` may contain up to `d` documents, leading to $O(D)$.; Temporary hash table `currentDocIdMap` also contributes $O(D)$. Consequently, the overall space complexity is $O(M + K + D)$.

Although our main program calls other functions which are not listed above, but their space complexity overlaps with the above operations or they are too trivial to be considered, so we don't care much about these function.

To sum up, the space complexity of our program is $O(W + D + H + I + M + K)$.


== Time Complexity

*Conclusion*: $O(W + I log I + n + m + m log m + X)$

- $W$: Word count all articles in _The Works_
- $I$: The size of the inverted index
- $n$: The query string of length
- $m$: The number of valid words 
- $X$: Vary in different situation
  - #fakebold[single] valid word: $k log k$, where $k$ is the number of documents
  - #fakebold[multiple] valid words: $m times (p log p)$, where $p$ is the total document results inworst-case scenario

*Analysis*:

- #fakebold[Word count and stop words]: Apparently, we count and handle every word in all files to implement the functions of word count and stop words detection. Therefore, the time complexity of this part is proportional to $W$, which is mentioned above.

- #fakebold[Inverted Index]: The most frequent operations we have run in the inverted index are insertion and finding, so we consider these operations mainly. It's proved that the efficiency is $O(log N)$ for both insertion and finding, and for every node we should execute these operations at least once. Consequently, the whole time complexity of building inverted index is $O(I log I)$. Additionally, in our speed test above, we have drawn the curve chart of it, which can be fitted with $a + b dot N log N$ function, which proves the correctness of our analysis further.

- #fakebold[Query]: We will analyze this part step by step:
  - #fakebold[character iteration]: The function iterates through each character in the query string of length $n$, resulting in $O(n)$.
  - #fakebold[word processing]: Each word is checked against the stop words list and the inverted index. If $m$ is the number of valid words, this step is $O(m)$ since both checks have an average time complexity of $O(1)$.
  - #fakebold[sorting]: The `queryWord` vector containing valid words is sorted, taking $O(m log m)$.
  - #fakebold[ocument Search]: If there is one valid word, it retrieves the positions from the B+ tree (let's assume $k$ documents) and sorts them: $O(k log k)$. If there are multiple valid words, for a worst-case scenario of $p$ total document results, it processes each word, resulting in $O(m times (p log p))$.

  As a consequence:

  - For a #fakebold[single] valid word: $O(n + m + m log m + k log k)$
  - For #fakebold[multiple] valid words: $O(n + m + m log m + m times (p log p))$

For the same reason above, we also ignore the time complexity of some functions which have slight impact on the whole time complexity of our main program.

In a nutshell, the time complexity of our program is $O(W + I log I + n + m + m log m + X)$.

== Further Improvement

#v(.5em)

1. #fakebold[Algorithm refinement]: So far, we have learned few of the efficient algorithms and data structures, which means that our implementation of the mini search engine might not be the best practice. However, it's possible for us to devised more ingenious and efficient procedure to cope with this problem after we systematically learned more excellent algorithms and data structrues.

2. #fakebold[Testing construction]: Although we come up with some testing cases, probably some crucial tests are still lost, and potential bugs may exists in our programs owing to our incomplete consideration. From our standpoint, it's difficult to find all typical cases for a program, but we're fully convinced that by delicate techniques and tricks for testing results, we can come up with tests as complete as possible.

3. #fakebold[Complexity analysis]: As you can see, it's awkward to analyze the complexity of some programs such as the space complexity of position list in nodes of inverted index. As a consequence, our analysis on complexity isn't very accurate. We will study the systematic method of analyzing the complexity and improve the precision of our analysis in the foreseeable future.


= Appendix: Source code

== File Structure

#table(columns: 1fr, align: left)[
  ```
  .
├── README.md
├── code
│   ├── Makefile
│   ├── README.pdf
│   ├── build
│   ├── scripts
│   │   ├── getStopWord
│   │   ├── getStopWord.cpp
│   │   ├── html2txt.py
│   │   ├── iist_diagram.py
│   │   ├── invIndexFunc.cpp
│   │   ├── invIndexHeader.h
│   │   ├── invIndexSearch.cpp
│   │   ├── invIndexTest.cpp
│   │   ├── search_main.cpp
│   │   ├── search_test.cpp
│   │   └── wordStem
│   └── data
│       ├── file_word_count.txt
│       ├── search_test
│       ├── shakespeare-master
│       ├── shakespeare_works
│       ├── stop_words.txt
│       ├── inverted_index_tests
│       ├── txt_title.txt
│       ├── word_count.txt
│       └── word_docs.txt
└── documents
    └── report-p1.pdf


  ```
]

== `getStopWord.cpp`

#importCode("../code/scripts/getStopWord.cpp")

== `invIndexHeader.h`

#importCode("../code/scripts/invIndexHeader.h")

== `invIndexFunc.cpp`

#importCode("../code/scripts/invIndexFunc.cpp")

== `invIndexTest.cpp`

#importCode("../code/scripts/invIndexTest.cpp")

== `invIndexSearch.cpp`

#importCode("../code/scripts/invIndexSearch.cpp")

== `search_test.cpp`

#importCode("../code/scripts/search_test.cpp")

== `search_main.cpp`

#importCode("../code/scripts/search_main.cpp")

= References

#show link: underline
// #show link: text(fill: blue)[]

- #text()[Blake-Madden, OleanderStemmingLibrary,]#text(fill: blue)[ 
#link("https://github.com/Blake-Madden/OleanderStemmingLibrary")]

- #text()[William Shakespeare, “The Complete Works of William Shakespeare”, ]#text(fill: blue)[ 
#link("http://shakespeare.mit.edu/")]

 

= Author list

#v(.5em)

- #fakebold[Huang Xingyao]: Extracted the txt files of _The Complete Work of Shakespeare_, implemented the functions of word count, obtaining stop words, coping with queries and the main program, devised the tests of the threshold of queries. 
- #fakebold[Qian Ziyang]: Accomplished the foundation of the inverted index by B+ trees, designed the tests of inverted index, and wrote the bulk of this report.

= Declaration

#v(.5em)

#fakebold[#fakeitalic[We hereby declare that all the work done in this project titled "Roll Your Own Mini Search Engine" is of our independent effort as a group.]]

= Signatures

#align(center)[
#grid(columns: 2)[
  #image("../template/images/signature_hxy.png", width: 60%)
][#image("../template/images/signature_qzy.jpg", width: 60%)]
]
