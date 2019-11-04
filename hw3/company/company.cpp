#include "company.h"
#include <vector>
#include <iostream>
using namespace std;

CompanyTracker::CompanyTracker (int n)
  // initializes the tracker with n students and their 1-person companies
{
  numCompanies = n;
  companies = new Company* [numCompanies];
  for (int i = 0; i < numCompanies; ++i)
      companies[i] = new Company ();
}

CompanyTracker::~CompanyTracker ()
  // deallocates all dynamically allocated memory
{
  // your implementation goes here
  for (int i=0; i < (int)map.size(); i++)
  {
    delete map[i];
  }
  
  for (int j=0; j<numCompanies; j++)
  {
    delete companies[j];
  }
  delete [] companies;
}
void CompanyTracker::find_largest(int p, Company*& curr_comp)
{
  Company* parent_p = companies[p]->parent;

  while (parent_p != nullptr)
  {
    curr_comp=parent_p;  //store the current parent before parent_p is nullptr, 
                          //so we know what parent comp to merge
    parent_p = parent_p -> parent; //keep traversing thru parent companies
  }
  
}
void CompanyTracker::merge (int i, int j)
  /* Merges the largest companies containing students i and j,
     according to the rules described above.
     That is, it generates a new Company object which will
     become the parent company of the largest companies currently
     containing students i and j.
     If i and j already belong to the same company (or are the same),
     merge doesn't do anything.
     If either i or j are out of range, merge doesn't do anything. */
{
  // your implementation goes here
  bool same_company = inSameCompany(i, j);

  if (same_company)
  {
    return;
  }
  //return if invlaid range
  if (i>=numCompanies || j>=numCompanies||
      i<= -1 || j<= -1)
  {
    return;
  }

  Company* i_parent = nullptr; //used to find parent companies 
  Company* j_parent = nullptr;
  find_largest(i, i_parent);
  find_largest(j, j_parent);

  //if they are both 1 person companies, push back their respect companies elements
  if (i_parent == nullptr && j_parent == nullptr) 
  {
    map.push_back(new Company(companies[i], companies [j]));
    companies[i]->parent = map[map.size()-1];
    companies[j]->parent = map[map.size()-1];
  }
  else
  {
    int index_parent_i=-1; //variables to keep track of where parent companies are in map (if any)
    int index_parent_j=-1;

    for (int k=0; k<(int)map.size(); k++) //loop thru to find where in vector largest parent is
    {
      if (i_parent==map[k])
      {
        index_parent_i = k;
      }
      else if (j_parent==map[k])
      {        
        index_parent_j = k;
      }
    }
    if (index_parent_i != -1 && index_parent_j != -1) //if both have parent companies, push back map[parent index]
    {
      map.push_back(new Company(map[index_parent_i], map[index_parent_j]));
      map[index_parent_i]-> parent = map[map.size()-1];
      map[index_parent_j]-> parent = map[map.size()-1];
    }
    else if (index_parent_i != -1) //if only i has a parent company, push back map[parent index]
    {
      map.push_back(new Company(map[index_parent_i], companies[j]));
      map[index_parent_i]->parent = map[map.size()-1];
      companies[j]->parent = map[map.size()-1];
    }
    else if (index_parent_j != -1) //same as above but for j
    {
      map.push_back(new Company(companies[i], map[index_parent_j]));
      map[index_parent_j]->parent = map[map.size()-1];
      companies[i]->parent = map[map.size()-1];
    }
  }

}

void CompanyTracker::split (int i)
  /* Splits the largest company that student i belongs to,
     according to the rules described above.
     That is, it deletes that Company object, and makes sure that
     the two subcompanies have no parent afterwards.
     If i's largest company is a 1-person company, split doesn't do anything.
     If i is out of range, split doesn't do anything. */
{
    Company* i_largest_comp = nullptr; //pointer to find largest company i is part off
    find_largest(i, i_largest_comp);

    for (int m=0; m<(int) map.size(); m++) //loop thru to find where in vector largest parent is
    {
      if(i_largest_comp==map[m])
      {
        map[m]->merge1->parent = nullptr; //1st want the subcompanies' parents to point to nullptr
        map[m]->merge2->parent = nullptr;
        map.erase(map.begin()+m); //delete and break out loop since no need to keep looping thru
        break;
      }
    }
    delete i_largest_comp; //pointer now trying to point to deleted memory, so we delete it too
}

bool CompanyTracker::inSameCompany (int i, int j)
  /* Returns whether students i and j are currently in the same company.
     Returns true if i==j.
     Returns false if i or j (or both) is out of range. */
{
  if (i==j)
  {
    return true;
  }
  //return false if invlaid range
  else if (i>=numCompanies || j>=numCompanies||
          i<= -1 || j<= -1)
  {
    return false;
  }

  //used to keep track of what the final company is before we hit nullptr
  Company* i_parent_comp=nullptr; 
  Company* j_parent_comp=nullptr;

  find_largest (i, i_parent_comp);
  find_largest (j, j_parent_comp);
  //if both i & j's final parent companies are the same and not nullptr
  //they are in the same company
  if (i_parent_comp==j_parent_comp && i_parent_comp != nullptr)
  {
    return true;
  }
  
  return false;
}

