### Assignment Descriptions:

  **Learning.cpp**:
  
  Your source file should be learning.cpp, and compile into a file learning. That program should take just one command line     parameter, the name of the input file. The input file will have two numbers on the first line: n is the total number of       classes from which you can choose (we promise that this number will never exceed 25), and the second number W is a double,     capturing the total amount of work you can do for the semester. Next come n lines, each describing a class. The class i is     described by first the class name s(i) (a string that does not contain any white space), followed by one or more               whitespaces. Next come two non-negative doubles w(i) and L(i). The first of these is the amount of total work you need to do   for class i, and the second is the total amount you learn from class i. Here is an example of what this may look like:

  * 4 3.14159
  * CSCI104 3.001 10.0
  * CSCI170 2.41       8.0
  * ENGR102  0.10 0.36
  * CSCI280    0.66 2.15
  
  Your goal is to output the maximum total amount you can learn from any combination of classes whose total work does not       exceed W. In the example above, the correct output would be 10.36, by combining CSCI104 with ENGR102. You cannot combine       CSCI104 with any of the other classes (it would exceed your workload capacity), and you don't learn as much if you take       CSCI170 with CSCI280; also, CSCI170 and ENGR102 and CSCI280 together exceed your workload capacity. The output above would     simply look as follows:

  10.36
  
  **d-ary Heap**
   
  Build your own templated d-ary MinHeap class with the interface given below. Put your entire implementation in heap.h         (because the class is templated). You learned in class how to build a binary MinHeap, where each node had 2 children. For a   d-ary MinHeap, each node will have d children.
  In order to build it, you may use internally the vector container (if you so choose). You can not use anything else from the   STL.
  
  **Doublet.cpp/A Star Search**
  
  The word game "Doublet" was invented by Lewis Carroll, and is a word transformation puzzle. Two words of identical length     are given. The objective is to transform the first word into the second word by forming successive words of the same length,   changing only one letter at a time. Here is an example from HEAD to TAIL:

  * HEAD
  * HEAL
  * TEAL
  * TELL
  * TALL
  * TAIL
  
  The challenge is to do the transformation in the least number of words.

  Your source file should be doublet.cpp, and the executable should be called doublet. It takes three command line parameters.   The first indicates the starting word, the second indicates the ending word, and the third is a file which contains a list     of valid words. So you might run the program as follows:

  `./doublet head tail words.txt`
  
  Everything should be case-insensitive, so there is no difference if the starting word is HEad or heAD.

  The word file will be formatted as follows:

  * 7
  * head
  * heAl
  * hem
  * Tail
  * tell
  * taLL
  * teal
  
  The first row contains a number n, indicating the number of words in the file. There will be n more rows, each with a single   word, each possibly followed by some whitespace. There may be blank lines at the end of the file. You may assume the file is   formatted properly. We may give your program very large word files (around 1 million words).

  The word file will not contain duplicates, and the start and end word will always be in the word file. But, as mentioned       above, there may sometimes be no way to get from the starting word to the ending word.

  You will implement the A* search algorithm to quickly find the shortest transformation. You can think of each word in the     word file as a node, and there is an edge between two words of the same length if they differ by exactly one letter.
  
  An Example
  Starting from AAAAA, ending at BBBBB, with the following dictionary:

  * AAAAA
  * AAAAB
  * AAABB
  * AABAA
  * AABBA
  * AABBB
  * ABBBA
  * BAAAA
  * BBBBA
  * BBBBB
  
  * Expanding AAAAA, we would add AAAAB, AABAA, and BAAAA to the heap.
  * Expanding AAAAB (because it comes first according to operator<), we would add AAABB.
  * Expanding AAABB, we would add AABBB.
  * Expanding AABBB (because it has the smaller h-value), we would add AABBA.
  * Expanding AABAA (because it has the smaller f-value), we would update AABBA.
  * Expanding AABBA, we would add ABBBA.
  * Expanding ABBBA, we would add BBBBA.
  * Expanding BBBBA, we would add BBBBB.
  * BBBBB would be the next expansion, so we're done with a total of 8 expansions (we never searched BAAAA).
  
  It would output:
~~~~
  5
  8
~~~~
