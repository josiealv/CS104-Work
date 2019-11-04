#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

void cave (string& direcciones) //direcciones is directions in Spanish
{
    stack <char> dir; //used to pop and push our directions
    for (int i=0; i < (int)direcciones.size(); i++)
    {
        if (!dir.empty()) //if the stack isn't empty, we keep looping thru our directions
        {
            //if we hit a W or E, and at the top of the stack is a W or E
            //then it means we're going back a step, so we pop from the stack
            if ((direcciones[i]== 'W' && dir.top() == 'E')
                ||(direcciones[i]=='E' && dir.top()== 'W'))
            {
                dir.pop();
            }
            //same as above, but for N and S
            else if ((direcciones[i]=='N'&& dir.top()=='S')
                    ||(direcciones[i]=='S'&& dir.top()=='N'))
            {
                dir.pop();
            }       
            else //if a 'pair' is not found, then push to stack
            {
                dir.push(direcciones[i]);
            }
        }
        else //if stack is empty, push direction to stack
        {
            dir.push(direcciones[i]);
        }
    }
    cout << dir.size() << endl; //print out how size of stack i.e. how much string is unrolled
}
int fileread (const char* filename)
{
    ifstream ifile (filename); 
    if (ifile.fail()) 
    {
        return -1;
    }
    string directions = ""; //empty string to store the contents of the input file
    string read_line; 

    //loop until we hit the end of the file and store the contents of the line in
    //directions 
    while (getline(ifile, read_line))
    {
        directions += read_line;
    }
    //call cave function with our string of directions
    cave (directions);
    
    ifile.close();
    return 0;
}
int main (int argc, char* argv[])
{
    int file_read = fileread(argv[1]);
    if (file_read==-1)
    {
        cout << "Error trying to read file!" << endl;
    }
    return 0;
}