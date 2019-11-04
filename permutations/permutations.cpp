#include <iostream>

void swap (char& i, char& j) //normal swap function
{
    char temp = i;
    i = j;
    j = temp;
}
void print_p (std::string in, int start, int len)
{
    if(in.empty()) //check if our string is empty first
    {
        return;
    }
    if (start == len) //base case (if we reach the end of string, print and return)
    {
        std::cout << in << '\n';
        return;
    }
    for (int i=start; i <len; i++)
    { 
        swap (in[start], in[i]); //based on my tree, I need to swap the characters
        print_p (in, start+1, len); //ofc, we need to move the starting index
        swap (in[start], in [i]); //undo swap to backtrack
    }
}
void permutations(std::string in)
{
    print_p(in, 0, (int)in.size()); //call our helper function
}
