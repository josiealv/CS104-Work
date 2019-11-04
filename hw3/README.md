
1) to run/compile permutations when in permutations folder, just compile it normally with whatever main function is used: 

    g++ -g -Wall -std=c++11 permutations.cpp -o permutations
    
2) to run and compile cave, type make cave and then run the executable with a file name:

    make cave
    
    ./cave *some input file name*.txt (or whatever text file type it is)
    
3) depending on how you write the main testing code for Startup Companies, the typical way to run 
   and compile the code when in company folder is:
   
    g++ -g -Wall -std=c++11 -c company.cpp -o company.o
    
    g++ -g -Wall -std=c++11 company.o *testing code name*.cpp -o *whatever the executable is, not named company*
