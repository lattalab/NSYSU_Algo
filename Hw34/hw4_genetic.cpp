# include <iostream>
# include <fstream>
# include <string>
# include <cstdlib>
# include <time.h>
# include <map>
# include <vector>
# include <algorithm>
# include <iomanip>
using namespace std;

int *weight; // read weight for each item
int *value; // read value for each item

// 參數們
int evaluation_max;
int population_size;
float Crossover_rate = 0.9; 
float mutation_rate = 0.003; 

// parameter items: the total number of items
// return the binary form of this solution.
string pick_population(int items)
{
    /*
    pick the randomly selected population (任一組解)
    */ 
    string str;
    for (int i=0;i<items;i++)
    {
        // 從一個範圍中選取亂數，並決定要選還是不選
        int temp = rand()% 10;
        if (temp < 9)
        {
            str += '1';
        }
        else
        {
            str += '0';
        }
        
    }
    return str;
}

// parameter: s-> the string that we want to check, capacity->the capacity of packback
void evalution(string& s, int capacity)
{
    /*
    評估各種解的object value
    如果大於背包容量就進行刪減
    */
    int sum =0;
    for(int j=0; j<s.length(); j++)
    {
        if (s[j] == '1') // calculate total weight for this population
        {
            sum += weight[j];
        }    
    }
    while(sum > capacity) // modify when it exceeds maximum capacity
    {
        int randIdx = rand() % s.length();
        int randIdx2 = rand() % s.length();
        if (s[randIdx] == '1' && s[randIdx2] == '1')
        {
            double cp = value[randIdx] / weight[randIdx];
            double cp2 = value[randIdx2] / weight[randIdx2];
            if (cp2 > cp)
            {
                s[randIdx] = '0';
                sum -= weight[randIdx];
            }
            else
            {
                s[randIdx2] = '0';
                sum -= weight[randIdx2];
            }
        }
    }
}

// parameter: the 3 strings that want to compare with
string selection(string a, string b, string c)
{
    /*
        選擇優勢個體
    */

    // total value 較大者為優勢個體
    int sum_a=0, sum_b =0, sum_c =0, len = a.length();
    for(int i=0; i<len; i++)
    {
        if (a[i] == '1')
            sum_a += value[i];
        if (b[i] == '1')
            sum_b += value[i];
        if (c[i] == '1')
            sum_c += value[i];
    }

    if (sum_a > sum_b)
    {
        if (sum_a > sum_c)
            return a;
        else
            return c;
    }
    else
    {
        if (sum_b > sum_c)
            return b;
        else
            return c;
    }
}

// parameter: 想要進行Crossover的兩個個體
void crossover(string& a, string& b)
{   
    // 來測試uniform crossover
    int len = a.length();
    string child_a = "", child_b = "";
    // 同時選Mask跟生成小孩
    for (int i=0;i<len;i++)
    {
        int temp = rand() %2; // 類似選Mask
        if (temp == 0)
        {
            child_a += a[i];
            child_b += b[i];
        }
        else
        {
            child_a += b[i];
            child_b += a[i];
        }
    }
    
    a = child_a;
    b = child_b;
}

// parameter: 需要進行突變的個體
void mutation(string& a)
{
    // 測試均一突變 (Uniform Mutation)
    // 一樣同時選Mask來判斷要不要改變
    for(int i=0;i<a.length();i++)
    {
        int temp = rand()%2;

        if (temp == 1)
        {
            a[i] = char((a[i] - '0' + 1) %2);
        }
    }
}

// parameter: 欲得出object value的字串
// return : 該字串的object value
int calculate_profit(string s)
{
    int len = s.length();
    int sum =0;
    for (int i=0;i<len; i++)
    {
        if (s[i] == '1')
            sum += value[i];
    }
    return sum;
}
int main()
{
    // use for rand number
    srand(time(NULL));

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
        // 寫檔案的部分
        ofstream out;
        string filename = "GeneticAlgo_ans_dt0x.txt";
        filename[19] = s[3];
        out.open(filename);
        
        // start to read file
        in >> num >> capacity;
        // set up 2 dynamic array to store weight & value
        weight = new int [num];
        value = new int [num];
        int idx = 0;
        while(!in.eof()) // read weight and value respectively
        {
            in >> weight[idx] >> value[idx];
            idx++;
        }
        
        // define evaluation_max with different dataset
        if (s[3] == '1')
        {
            population_size = 1000;
            evaluation_max = 1000;
        }
        else if (s[3] == '2')
        {
            population_size = 1000;
            evaluation_max = 10000;
        }
        else if (s[3] == '3')
        {
            population_size = 25000;
            evaluation_max = 500000;
        }

        int epoch = 30 , count =0;
        long long int avg[30]; // 用來計算平均最佳解
        while(count < epoch)
        {
            string sol; // the sequence mapped to max profit
            int profit =0; // final profit

            string p[population_size]; // population
            // select population
            for (int i=0;i<population_size;i++)
            {
                p[i] = pick_population(num);
                evalution(p[i], capacity); // do evalution at the same time
            }

            int eva = 0;
            string child[population_size];
            while(eva < evaluation_max)
            {
                // selection
                for (int i=0; i<population_size; i++)
                {
                    // Tournament selection: 隨機挑選數個解比較，並留下較好的那個
                    int a = rand() % population_size;
                    int b = rand() % population_size;
                    int c = rand() % population_size;
                    child[i] = selection(p[a], p[b] , p[c]);
                }
                // crossover
                for (int i=0; i<population_size; i+=2)
                {
                    double random =  rand() % population_size / double(population_size);
                    // 小於交配率 需進行Crossover
                    if (random < Crossover_rate)
                        crossover(child[i], child[i+1]);
                }
                // mutation
                for (int i=0; i<population_size; i++)
                {
                    double random =  rand() % population_size / double(population_size);
                    // 小於突變率 需進行Mutation
                    if (random < mutation_rate)
                        mutation(child[i]);

                    evalution(child[i], capacity); // do evalution at the same time
                    // find largest object value in this run
                    int temp = calculate_profit(child[i]);
                    if (temp > profit)
                    {
                        sol = child[i];
                        profit = temp;
                    }
                }

                eva += population_size ; // evaluation 控制變數
                
            }   // end of GA loop

            avg[count] = profit; // 紀錄每Run的最佳解
            // terminal output
            cout << "Run " << count+1 << ": " << endl;
            cout << "max profit:" << profit << endl;
            cout <<"solution:" << sol << endl;
            // write to file
            out << "Run " << count+1 << ": " << endl;
            out << "max profit:" << profit << endl;
            out <<"solution:" << sol << endl;

            count ++; // increase epoch

        } // end of each run
        
        // 計算平均最佳解
        long long int sum =0;
        for(auto i:avg)
        {
            sum += i;
        }
        cout << endl << fixed << setprecision(2)
             << "Best object value in 30 runs: " << sum/(double)30 << endl;
        out  << endl << fixed << setprecision(2)
             << "Best object value in 30 runs: " << sum/(double)30 << endl;


        // 關檔 跟 還原memory
        out.close();
        in.close();
        delete [] weight;
        delete [] value;
    }
    
    return 0;
}