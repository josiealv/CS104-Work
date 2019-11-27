#include <vector>
#include <stdexcept>
template <typename T>
class MinHeap {
  struct Node
  {
    Node (T v, int pri, int id)
    {
      node=v;
      priorities = pri;
      node_num=id;
    }
    T node; 
    int priorities;
    int node_num;
  };
     public:
       MinHeap (int d);
       /* Constructor that builds a d-ary Min Heap
          This should work for any d >= 2,
          but doesn't have to do anything for smaller d.*/

       ~MinHeap ();

       int add (T item, int priority);
         /* adds the item to the heap, with the given priority. 
            multiple identical items can be in the heap simultaneously. 
            Returns the number of times add has been called prior to this
            call (for use with the update function).*/

       const T & peek () const;
         /* returns the element with smallest priority.  
            If two elements have the same priority, use operator< on the 
            T data, and return the one with smaller data.*/

       void remove ();
         /* removes the element with smallest priority, with the same tie-breaker
            as peek. */

       void update (int nth, int priority);
         /* finds the nth item (where nth is 0-based) that has ever been added 
            to the heap (the node that was created on the nth call to add), 
            and updates its priority accordingly. */

       bool isEmpty ();
         /* returns true iff there are no elements on the heap. */

   private:
      // whatever you need to naturally store things.
      // You may also add helper functions here.
      int d_ary; //tells us what kind of d-ary heap we have
      int nth_add; //tells us how many times add has been called
      std::vector <Node*> nodes; //stores nodes
      std::vector <int> loc; //locations of each node
      void bubbleUp(int pos); 
      void trickleDown(int pos);
      void swap (int pos1, int pos2); //traditional swap functions that also swaps locations
  };
template <typename T> 
MinHeap<T>::MinHeap(int d)
{
  if (d<2)
  {
    throw std::invalid_argument("Cannot make heap for d<2!");
  }
  d_ary=d;
  nth_add=0;
}
template <typename T>
MinHeap<T>::~MinHeap()
{
  for (int i=0; i<(int)nodes.size(); i++)
  {
    delete nodes[i];
  }
  nodes.clear();
}
template <typename T>
int MinHeap<T>::add(T item, int priority) //returns # of times add has been called prior to this call
{
  if((int)nodes.size()>=1 || nth_add!=0) //if not the first call to add, increment nth_add
  {
    ++nth_add;
    int curr_node_size = nth_add;
    nodes.push_back(new Node (item, priority, curr_node_size));
  }
  else
  {
    int curr_node_size = nth_add;
    nodes.push_back(new Node (item, priority, curr_node_size));
  }
  loc.push_back((int)nodes.size()-1); //have the location be the end of the nodes vector (for now)
  bubbleUp((int)nodes.size()-1);
  return nth_add;
}
template <typename T>
void MinHeap<T>::remove()
{
  if (nodes.empty())
  {
    throw std::out_of_range("Can't remove from empty heap");
  }
  swap(0, (int)nodes.size()-1); //move the node to the bottom of the heap
  loc[nodes[(int)nodes.size()-1]->node_num]= -1; //update location with -1
  delete nodes[(int)nodes.size()-1]; //deallocate it 
  nodes.pop_back(); //pop from both nodes vector
  trickleDown(0); //balance the heap
}
template <typename T>
void MinHeap<T>::bubbleUp(int pos)
{
  if(pos>0 && 
  nodes[pos]->priorities < nodes[(pos-1)/d_ary]->priorities)
  {
    swap(pos, (pos-1)/d_ary);
    bubbleUp((pos-1)/d_ary);
  }
  //compare actual node values if nodes are of the same priority
  else if (pos>0 && nodes[pos]->priorities==nodes[(pos-1)/d_ary]->priorities)
  {
    if(nodes[pos]->node < nodes[(pos-1)/d_ary]->node)
    {
      swap(pos, (pos-1)/d_ary);
      bubbleUp((pos-1)/d_ary);
    }
  }
}
template <typename T>
void MinHeap<T>::trickleDown(int pos)
{
  int child = d_ary*pos+1;
  if(child<(int)nodes.size())
  {
    if(child+1 < (int)nodes.size()&&
    nodes[child]->priorities > nodes[child+1]->priorities)
    {
      child++;
      if(nodes[child]->priorities < nodes[pos]->priorities)
      {
        swap(child, pos);
        trickleDown(child);
      }
      else if (nodes[pos]->priorities==nodes[child]->priorities)
      {
        if(nodes[child]->node < nodes[pos]->node)
        {
          swap(pos, child);
          trickleDown(child);
        }
      }
    }
    else if (nodes[child]->priorities < nodes[pos]->priorities)
    {
      swap(pos, child);
      trickleDown(child);
    }
     //compare actual node values if nodes are of the same priority
    else if (nodes[pos]->priorities==nodes[child]->priorities)
    {
      if(nodes[child]->node < nodes[pos]->node)
      {
        swap(pos, child);
        trickleDown(child);
      }
    }
  }
} 
template <typename T> 
void MinHeap<T>::swap(int pos1, int pos2)
{
  int temp_nth1 = nodes[pos1]->node_num; //store the original id, to be consistent
  int temp_nth2 = nodes[pos2]->node_num;
  Node* temp = nodes[pos1];
  nodes[pos1] = nodes[pos2];
  nodes[pos2] = temp;

  nodes[pos2]->node_num = temp_nth1;
  nodes[pos1]->node_num = temp_nth2;
  loc[temp_nth2] = pos1; //update location
  loc[temp_nth1] = pos2;
}
template <typename T>
void MinHeap<T>::update(int nth, int priority)
{
  if (nodes.empty())
  {
    throw std::out_of_range("Can't update empty heap!");
  }
  int location = loc[nth];
  if (location == -1)
  {
     throw std::out_of_range("Node no longer exists");
  }
  if(nodes[location]->priorities < priority) //if new priority > current one, call trickelDown
  {
    nodes[location]->priorities = priority;
    trickleDown(location);
  }
  else if(nodes[location]-> priorities > priority) //if new priority < current one, call bubbleUp
  {
    nodes[location]->priorities = priority;
    bubbleUp(location);
  }
}
template <typename T>
const T& MinHeap<T>::peek() const
 {
   //if the nodes vector is empty, can't return first node bc DNE, so throw error
   if(nodes.empty()) 
   {
     throw std::out_of_range("Node doesn't exist!");
   }
   return (nodes[0]->node);
 }
template <typename T>
bool MinHeap<T>::isEmpty()
{
  if (nodes.empty())
  {
    return true;
  }
  return false;
}
