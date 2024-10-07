#import "../../template/template.typ": *
#import "@preview/algo:0.3.3": algo, i, d, comment, code



#set text(font: ("New Computer Modern", "Times New Roman", "Noto Serif SC"), size: 14pt)


#algo(
  header: [
    #fakebold[Procedure]: InvertedIndex()
  ],
  block-align: left,
  fill: white,
  // strong-keywords: false
)[
  Begin #i\
    _InvIndex_ $<-$ CreateBP() \
    askforFilePos() \
    for _file_ in the diretory do #i\
      extract each _word_ from _file_ \
      if _word_ isn't a stop word then #i\
        _node_ $<-$ FindBP(_word_, _InvIndex_) #d\
      endif \
      if _word_ is already in _InvIndex_ then #i\
        increment the frequency and record the position for the _word_ #d\
      else #i\
        InsertBP(_word_, _node_, _InvIndex_) #d\
      endif #d\
    end #d\
  End
]

#pagebreak()

#algo(
  header: [
    #fakebold[Procedure]: FindBP(_word_: #fakebold[string], _T_: #fakebold[BplusTree])
  ],
  block-align: left,
  fill: white,
  // strong-keywords: false
)[
  Begin #i\
    if _T_ has no children then #i\
      isSameword(_word_, _T_) \
      return _T_ #d\
    endif \
    \
    _pos_ $<-$ -1 \
    for _i_ in range(0, _T_$->$_size_) do #comment[not contains _T_$->$_size_] #i\
      if _word_ has less lexicographical order than _T_$->$_data_[_i_]$->$_word_ then #i\
        _pos_ $<-$ _i_ \
        break #d\
      endif #d\
    end \

    if _pos_ = -1 then #i\
      _pos_ $<-$ _i_ #d\
    endif \

    return FindBP(_word_, _T_$->$_children_[_pos_]) #d\
  End
]


#pagebreak()

#algo(
  header: [
    #fakebold[Procedure]: InsertBP(_word_: #fakebold[string], _node_: #fakebold[NodeBP], _T_: #fakebold[BplusTree])
  ],
  block-align: left,
  fill: white,
  // strong-keywords: false
)[
  Begin #i\ 
    append _word_ as the new data to the _node_\
    sort the data in _node_\
    _T_ $<-$ SplitBP(_node_, _T_) \
    return _T_ #d\
  End
]
\
\
\
\
\
\
\

#algo(
  header: [
    #fakebold[Procedure]: SplitBP(_word_: #fakebold[string], _node_: #fakebold[NodeBP], _T_: #fakebold[BplusTree])
  ],
  block-align: left,
  fill: white,
  // strong-keywords: false
)[
  Begin #i\ 
    if the _node_ isn't full then #i\
      return _T_ #d\
    endif \
    \ 

    if the _node_ has no parent then #i\  
      create a new _parent_ for the _node_\
      let the _parent_ be the root of _T_ #d\
    endif \
    \
    _lnode_ $<-$ CreateBP() \
    _rnode_ $<-$ CreateBP() \
    distribute _node_'s data evenly to these two new nodes \
    if _node_ isn't a child node then #i\
      also distribute _node_'s children evenly to these two new nodes #d\
    endif \
    
    adjust the relationship between _parent_ and two new nodes(_lnode_ and _rnode_) \
    sort _parent_$->$_data_ and _parent_$->$_children_ \
    _T_ $<-$ SplitBP(_parent_, _T_) \
    return _T_ #d\
  End
]
