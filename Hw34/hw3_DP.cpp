# include <iostream>
# include <fstream>
# include <string>
using namespace std;

int *weight; // read weight for each item
int *value; // read value for each item
int **arr = NULL; // DP array

// parameter:
// n: 第幾個item
// w: 現在的容量
int knapsack(int n,int w)  
{
    if (w < 0) return -1e9; // 容量用完了，不放
    if (n == 0) return 0; // 第0項沒有value

    if (arr[n][w]) return arr[n][w]; // 拿之前找過的解

    // 分兩種case:
    // 1. 有放: 容量足夠大，能放到兩個皆可放
    // 2. 沒放: 容量不夠大，只能維持目前的最佳解
    arr[n][w] = max(
        (knapsack(n-1, w-weight[n]) + value[n]), 
        knapsack(n-1, w)
        );
    return arr[n][w];
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
        // start to read file
        in >> num >> capacity;
        // set up 2 dynamic array to store weight & value
        weight = new int [num+1];
        value = new int [num+1];
        weight[0] = value[0] = 0;
        int idx = 1;
        while(!in.eof()) // read weight and value respectively
        {
            in >> weight[idx] >> value[idx];
            idx++;
        }
        
        // set up DP array
        arr = (int**) calloc(num+1, sizeof(int*));
        for(int i=0; i<num+1; i++)
            arr[i] = (int*) calloc(capacity+1, sizeof(int));

        // define each optimal solution
        for(int i=1; i<num+1; i++)
        {
            for(int j=1; j<capacity+1; j++)
            {
                knapsack(i, j);
            }
        }
        
        // Trace DP array
        string binary(num, '0'); // the selective item string
        int total = arr[num][capacity]; // total is the maximum profit
        int w = capacity; // Start from the maximum capacity
        for (int i = num; i > 0; i--) {
            if (arr[i][w] != arr[i-1][w]) { // if the item is included
                binary[i-1] = '1'; // Include this item in the solution
                w -= weight[i]; // Decrease the remaining capacity
            }
            // If they are equal, move to the next item without changing w
        }
            
        cout << "max profit:" << total << endl;
        cout <<"solution:" << binary << endl;

        // 寫檔案
        ofstream out;
        string filename = "DP_ans_dt0x.txt";
        filename[10] = s[3];
        out.open(filename);
        out  << "max profit:" << total <<endl
             << "solution:" << binary;

        // 關檔 跟 還原memory
        out.close();
        in.close();
        delete [] weight;
        delete [] value;
        for (int i=0;i<num+1;i++)
            free(arr[i]);
        free(arr);
    }
    
    return 0;
}