#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include "heap.h"
using namespace std;
struct Vertex
{
    Vertex (string name, int heuristic)
    {
        word=name;
        h=heuristic;
    }
    string word;
    int h; //heuristic
    bool visited; //used to mark if visited
    int nth_node;  //used to keep track of what index it is in the heap
    int prioridad;  //store priority, prioridad is priority 
    vector <Vertex*> neighbors; //works like an adjacency list
};
void findNeighbors (map<string, Vertex*>& graph)
{
    map<string, Vertex*>:: iterator it;
    for (it=graph.begin(); it != graph.end(); it++) //change each character in each word and see if it's in dictionary
    {
        string current = it->first;
        for (int j=0; j<(int) current.size(); j++)
        {
            for(char k='a'; k<='z'; k++)
            {
                char temp = current[j];
                current[j]=k;
                if (graph.find(current)!=graph.end() && current!=it->first)
                {
                    map<string, Vertex*>:: iterator it2 = graph.find(current);
                    it->second->neighbors.push_back(it2->second); //update the node's neighbor vector
                }
                current[j]=temp;
            }
        }
    }
}
int calc_pri(map<string, Vertex*>& word_map, string word, int g) //calculates priority
{
    int f = g + word_map[word]->h;
    return (f*(word.size()+1)+word_map[word]->h);
}
void Astar(map<string, Vertex*>& word_map, string& start, string& end)
{
    findNeighbors(word_map); //find all the edges of each word
    map<string, int> g_val; //calculates distance
    
    MinHeap<string> pq(2);
    word_map[start]->visited = true; 
    g_val[start]=0; //set start's distance as 0

    word_map[start]->prioridad=calc_pri(word_map, start, 0); 
    word_map[start]->nth_node = pq.add(start,  word_map[start]->prioridad); //add start to heap

    int expansions=0;
    bool path_found = false; //used to tell if a path was found
    try
    {
        while(!pq.isEmpty())
        {
            string v = pq.peek(); //get node with smallest priority
            pq.remove(); //pop it from the heap
            if (v==end) //if we are at the end, then we mark path_found as true, get the final distance & break
            {
                path_found = true;
                g_val[end]=g_val[v];
                break;
            }
            int num_edges = word_map[v]->neighbors.size(); //get the the num of edges for current node
            expansions++; //update expansions
            for (int i=0; i<num_edges; i++)
            {
                string vecino = word_map[v]->neighbors[i]->word; //vecino is neighbor in spanish

                //check if visited or if new dist from current node 
                //is less than the current neighbor's distance
                if (!word_map[vecino]->visited || 
                    g_val[vecino] > g_val[v]+1)
                {
                    g_val[vecino] = g_val[v]+1; //update distance if either condition is triggered 
                    if(!word_map[vecino]->visited)
                    {
                        word_map[v]->neighbors[i]->visited = true; 

                        word_map[vecino]->prioridad=
                        calc_pri(word_map, vecino, g_val[vecino]); //calculate priority 

                        word_map[vecino]->nth_node =
                        pq.add(vecino, 
                        word_map[vecino]->prioridad); //add to heap
                    }
                    else
                    {
                        word_map[vecino]->prioridad=
                        calc_pri(word_map, vecino, g_val[vecino]); //calculate priority
                        pq.update(word_map[vecino]->nth_node, 
                        word_map[vecino]->prioridad); //update heap
                    }
                }
            }
        }
        if (!path_found) 
        {
            cout << "No transformation" << endl;
            cout << expansions << endl;
        }
        else
        {
            cout << g_val[end] << endl;
            cout << expansions << endl;
        }
    }
    catch (exception& e) //print error if any found from using heap
    {
        cout << "No transformation" << endl;
        cout << expansions << endl;
        cout << e.what() << endl;
        return;
    }
}
int getHeuristic (string word, string end) //calculates heuristic
{
    int h=0;
    for (int i=0; i < (int) end.size(); i++)
    {
        if (word[i]!=end[i])
        {
            h++;
        }
    }
    return h;
}
void lowercase(string& word) //converts the word to all lowercase letters
{
    for (int i=0; i<(int)word.size(); i++)
    {
        if (word[i]>='A'&& word[i]<='Z')
        {
            word[i] = word[i] + 32;
        }
    }
}
void destruct(map<string, Vertex*>& dest) //destructor for the word map
{
    map<string, Vertex*>:: iterator it;
    for (it=dest.begin(); it != dest.end(); it++)
    {
        delete (it->second);
    }
    dest.clear();
}
int main(int argc, char* argv[])
{
    if (argc < 4) 
    {
        cout << "Please provide an input file." << endl;
        return 1;
    }
    else
    {
        string start=argv[1];
        string end=argv[2];
        lowercase(start);
        lowercase(end);
        if(end.size()!=start.size() || start==end)
        {
            cout << "No transformation"<<endl;
            cout << 0 << endl;
            return 1;
        }
        map <string, Vertex*> word_map; //used to store the vertices/nodes
        int start_h = getHeuristic(start, end); //heuristic of start word
        word_map[start] = new Vertex(start, start_h);
        word_map[start]->visited=false; //initialize all words being read in visited variable as false
        
        word_map[end]= new Vertex(end, 0);
        word_map[end]->visited=false;

        ifstream ifile (argv[3]);
        if(ifile.fail())
        {
            cout << "File "<<argv[3]<< " cannot be opened." << endl;
        }
        else
        {
            int num_words;
            ifile >> num_words;
            for(int i=0; i<num_words; i++) //loop thru and read in all the words in text file
            {
                string word;
                ifile >> word;
                lowercase(word);
                //if the current word is the start or end word or if it is a duplicate of another word
                //go to next iteration
                if (word==start || word==end || 
                word_map.find(word)!=word_map.end())
                {
                    continue;
                }
                else if(word.size()==start.size())
                {
                    int h = getHeuristic(word, end);
                    word_map[word] = new Vertex (word, h);
                    word_map[word]->visited=false;
                }
            }
            Astar(word_map, start, end); //runs the actual A* search
        }
        destruct(word_map); //deallocates memory

    }
    return 0;
}
