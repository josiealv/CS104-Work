#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
double max_learn(double max_work, vector<pair <double, double> >& stats, int index, 
double sum, double curr_max, double max_learned)
{
    if(max_work==0.0)
    {
        return 0;
    }
    if (curr_max <= max_work && index==(int)stats.size()) //if at the end of vector and maximum work isn't exceeded
    {
        max_learned=sum;
        return max_learned;
    }
    for (int i=index; i<(int)stats.size(); i++)
    {
        //if the current work < maximum work, keep going thru this class' branch 
        //(i.e. add the learned to sum and work to curr_max)
        if (curr_max+stats[i].first <= max_work) 
        {
            //take the maximum between class branch we're goin' down and the current maximum learned
            max_learned = max(max_learn(max_work, stats, i+1, sum+stats[i].second, 
            curr_max+stats[i].first, max_learned), max_learned);
        }
       else if (curr_max+stats[i].first > max_work) //if curr_max > maximum work, go to the next class branch
        {
            //take the maximum between class branch we're goin' down and the current maximum learned
            max_learned = max(max_learn(max_work, stats, i+1, sum, 
            curr_max, max_learned), max_learned);
        }
    }
    return max_learned;
}
int main(int argc, char* argv[])
{
    if (argc < 2) 
    {
        cout << "Please provide an input file." << endl;
        return 1;
    }

    ifstream ifile (argv[1]);
    if(ifile.fail())
    {
        cout << "File "<<argv[1]<< " cannot be opened." << endl;
    }
    else
    {
        int classes_num;
        double max_work;
        ifile>>classes_num;
        ifile>>max_work;
        vector<pair <double, double> > class_stats; //vector for our class stats
        for (int i=0; i<classes_num; i++)
        {
            double work;
            double learn;
            string name;
            ifile>>name;
            ifile>>work;
            ifile>>learn;
            class_stats.push_back(make_pair(work, learn));
        }
        double max_l=0;
        double learned = max_learn (max_work, class_stats, 0, 0, 0, max_l);
        cout << learned << endl;
    }
    return 0;
}
