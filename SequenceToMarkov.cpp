#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

unsigned const int C = 10; // Number of colors
unsigned const int S = 10000; // Length of sequence

double* freq(int *arr, double *temp);
double* mMult (double arr[][C], double *Larr, double *ptr);
int nTerm(double *arr, int n);
int getSize(double *arr);
void Gen(double *count1, double tran[][C], int);



//-------------------------------------------//
int main() 
{
    int p = 0;
    int value;
    int nums[S];         // Pointer to array of random nums
    int temp[C];       // To be passed into count()
    double *count;     // Pointer to array of rel. freq
    double tran[C][C]; // Transition matrix/passed into mMult()
    double count1[C];
    double temp1[C];
    int gen = 0;
    ifstream birds;
    birds.open("dog.txt");

    // Initialize arrays
    for (int i = 0; i < C; i++)
    {
        temp[i] = 0;
    }
    for (int i = 0; i < C; i++)
    {
        temp1[i] = 0.00;
        for (int j = 0; j < C; j++)
        {
            tran[i][j] = 0.00;
        }
    }

    // Read numbers from .txt into array
    for (int i = 0; i < S; i++)
    {
        birds >> nums[i];
    }
    birds.close();

    count = freq(nums, temp1); 

    for (int i = 0; i < C; i++)
    {
        count1[i] = count[i];
    }

    for (int i = 0; i < S-1; i++) 
    {
        tran[(nums[i]) % C][(nums[i+1]) % C] += 1.00;
        temp[(nums[i])]++;
    }

    // Sets disribution of above pairwise occurance count.
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < C; j++)
        {
            if (tran[i][j] != 0)
                tran[i][j] = tran[i][j] / (temp[i] + 0.0);
        }
    }

    cout << "\nEnter number of generations: ";
    cin >> gen;
    
    
    Gen(count, tran, gen);

    return 0;
}
//-------------------------------------------//

//-------------------------------------------//
double * freq(int *arr, double *temp)
{
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < S; j++)
        {
            if (arr[j] == i)
                temp[i] = temp[i]+1.00;
        }
    }
    
    for (int i = 0; i < C; i++)
    {
        temp[i] = temp[i]/(S+0.00);
    }
    return temp;
}
//-------------------------------------------//

//-------------------------------------------//
double *mMult(double arr[][C], double *Larr, double *ptr)
{
    for (int i = 0; i < C; i++)
    {
        ptr[i] = 0.0;
    }
    
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < C; j++)
        {
            ptr[i] += Larr[j] * arr[j][i];
        }
    }

    return ptr; 
}
//-------------------------------------------//

//-------------------------------------------//
int nTerm(double *arr, int n)
{
    int ret = 0;
    int l = 0;
    int u = 0;
    int val[C];
    int sum[C];
    const int size = getSize(arr);
    int temp[size];

    for (int i = 0; i < C; i++)
    {
        sum[i] = 0;
    }
    for (int i = 0; i < C; i++)
    {
        val[i] = floor(arr[i]*1000.0);
    }

    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < i+1; j++)
        {
            sum[i] += val[j];
        }
    }
    
    for (int i = 0; i < C; i++)
    {
        u += sum[i];
        for (int j = l; j < u; j++)
        {
            temp[j] = i;
        }
        l += sum[i];
    }
    ret = temp[n];

    return ret;
}

int getSize(double *arr)
{
    int sum[C];
    int val[C];
    int size = 0;

    for (int i = 0; i < C; i++)
    {
        sum[i] = 0;
    }
    for (int i = 0; i < C; i++)
    {
        val[i] = floor(arr[i]*1000.0);
    }
    for (int i = 0; i < C; i++)
    {
        for (int j = 0; j < i+1; j++)
        {
            sum[i] += val[j];
        }
        size += sum[i];
    }
    
    return size;
}

void Gen(double *count, double tran[][C], int gen)
{
    int recArr[S];
    int val[C];
    double *newM;
    double *ptr = new double[C];
    int sum[C];
    double temp[C];
    int k = 0;
    srand(time(NULL));
    ofstream seqs;
    ofstream dist; 
    ofstream matrx;
    seqs.open("seqs.txt");
    dist.open("dist.txt");
    matrx.open("matrx.txt");

    // Initializing arrays
    for (int i = 0; i < C; i++)
    {
        temp[i] = 0.0;
    }
    
    // Initializing array
    for (int i = 0; i < S; i++)
    {
        recArr[i] = 0;
    }

    for (int a = 0; a < gen; a++)
    {
        // Create new sequence based off of stats from count[] and tran[][]
        for (int i = 0; i < S; i++)
        {
            int size = getSize(count);
            k = rand() % size;
            recArr[i] = nTerm(count, k);
            newM = mMult(tran, count, ptr);
            for (int j = 0; j < C; j++)
            {
                count[j] = newM[j];
            }
        }

        // Display new sequence
        seqs << "\nGeneration " << a+1 << ":";
        for (int j = 0; j < S; ++j)
        {
            if (j % 100 == 0)
            {
                seqs << "\n";
                seqs << recArr[j] << " ";
            }
            else
            {
                seqs << recArr[j] << " ";
            }
        }

        // Get distribution from new sequence
        count = freq(recArr, temp); 

        // Display distribution of new sequence
        dist << "\n\nGeneration " << a+1 << " Distribution:\n\n    ";
        for (int j = 0; j < C; j++)
        {
            dist << fixed << setprecision(5) << count[j] << "    ";
        }

        // Initialize new transition matrix and distribution vector
        for (int i = 0; i < C; i++)
        {
            temp[i] = 0.0;
            for (int j = 0; j < C; j++)
            {
                tran[i][j] = 0.00;
            }
        }   

        // Counts occurances of pairs, i.e., how many 0s go to 1. etc.
        for (int i = 0; i < S-1; i++) 
        {
            tran[(recArr[i]) % C][(recArr[i+1]) % C] += 1.00;
            temp[(recArr[i])]++;
        }

        // Sets disribution of above pairwise occurance count.
        for (int i = 0; i < C; i++)
        {
            for (int j = 0; j < C; j++)
            {
                if (tran[i][j] != 0)
                    tran[i][j] = tran[i][j] / (temp[i] + 0.0);
            }
        }

        // Display new transition matrix
        matrx << "\n\nGeneration " << a+1 <<  " Transition Matrix:\n\n    ";
        for (int i = 0; i < C; i++)
        {
            for (int j = 0; j < C; j++)
            {
                matrx << fixed << setprecision(5) << tran[i][j] << "    ";
            }
            matrx << "\n\n    ";
        }

        for (int i = 0; i < C; i++)
        {
            count[i] = count[i] / (S+0.0);
        }
        // Reset array
        for (int i = 0; i < S; i++)
        {
            recArr[i] = 0;
        }
    }
    delete [] ptr;
    ptr = nullptr;
    seqs.close();
    dist.close();
    matrx.close();
    cout << "\n\nDONE\n\n";
}
