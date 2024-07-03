# include <iostream>
# include <fstream>
# include <string>
using namespace std;
// goal : try all possible solution

// return the binary form number in string
// parameter: num-> the binary number, bits -> how many bits to represent.
string getBinary(int num, int bits) 
{
    string b(bits,'0'); // initialize
    int idx=0, array[bits]={0};
    // math calculation
    while(num)
    {
        array[idx] = (num%2);
        num/=2;
        b[b.length()-1-idx] = char(array[idx]+'0'); // change if necessary
        idx++;
    }
    return b;
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
        int *weight = new int [num];
        int *value = new int [num];
        int idx = 0;
        while(!in.eof()) // read weight and value respectively
        {
            in >> weight[idx] >> value[idx];
            idx++;
        }
        
        // Note: (1<<num) = 2^num
        int profit=0, sol;
        for(int i=0;i<(1<<num);i++) // 窮舉所有的可能性
        {
            string bin = getBinary(i,num);
            int wt = 0 , vt = 0;
            for (int j=0; j<bin.length(); j++)
            {
                if (bin[j] == '1') // if this item is selected.
                {
                    wt += weight[j];
                    vt += value[j];
                }
            }
            if (wt > capacity) // out of package capacity
                vt =0;
            else // update the maximum and occur number
            {
                int orig = profit;
                profit = max(profit, vt);
                if (profit != orig)
                    sol = i;
            }
        }

        // write terminal output and file output
        cout << "max profit:" << profit <<endl
             << "solution:" << getBinary(sol,num) << endl;

        ofstream out;
        string filename = "Exhaustive_ans_dt0x.txt";
        filename[18] = s[3];
        out.open(filename);
        out  << "max profit:" << profit <<endl
             << "solution:" << getBinary(sol,num);

        // 關檔 跟 還原memory
        out.close();
        in.close();
        delete [] weight;
        delete [] value;
    }
    return 0;
}