# include <iostream>
# include <fstream>
# include <string>
# include <algorithm>
# include <cmath>
using namespace std;
// goal: select the most valuable item to package until full

// function to compare pair<int,double> 's second operand
bool cmp(pair<int,double> &x, pair<int,double> &y)
{
    return x.second > y.second;
}
int main()
{
    // input the specific file and then read
    string s;
    cout << "Which dataset file you want to read (eg: dt01./item.txt): ";
    cin >> s;

    ifstream in;
    int num; // number of items
    int capacity; // capacity of backage

    // './' means go to next file location
    string root = "./dataset./dataset./"; 
    in.open(root + s);
    if (!in.is_open()) //check if file is open or not
    {
        cout << "Failed to open file.\n";
        return 1; // EXIT_FAILURE
    }
    else
    {
        in >> num >> capacity;
        // set up 2 dynamic array to store weight & value
        int* weight = new int [num];
        int* value = new int [num];
        // create a dynamic array to store cp value
        pair<int, double>* cp = new pair<int,double> [num];
        int idx = 0;
        while(!in.eof()) // read weight and value respectively
        {
            in >> weight[idx] >> value[idx];
            // calculate Cp value at every position
            cp[idx] = {idx, value[idx]/(double)weight[idx]};
            idx++;
        }
        sort(cp,cp+num,cmp); //sorting
        
        int profit =0;
        string sol(num, '0'); // initialize
        for(int i=0;i<num;i++)
        {
            int idx = cp[i].first;
            if (weight[idx] > capacity) // 背包不夠放
                continue;
            else
            {
                capacity -= weight[idx]; // 減去背包空間
                sol[idx] = '1'; // mean for select this item
                profit += value[idx]; //accumlate the profit
            }
        }

        // write terminal output and file output
            cout << "max profit:" << profit <<endl
                 << "solution:" << sol << endl;

        ofstream out;
        string filename = "Greedy_ans_dt0x.txt";
        filename[14] = s[3];
        out.open(filename);
        out  << "max profit:" << profit <<endl
             << "solution:" << sol;

        // 關檔 跟 還原memory
        out.close();
        in.close();
        delete [] weight;
        delete [] value;
    }
    return 0;
}