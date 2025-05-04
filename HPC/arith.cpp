#include<bits/stdc++.h>
#include<chrono>
#include<omp.h>

using namespace std;
using namespace chrono;

int min_sequential(vector<int> &arr)
{
    int mini = arr[0];
    for(int i=0;i<arr.size();i++)
    {
        if(mini > arr[i]) mini = arr[i];
    }
    return mini;
}

int max_sequential(vector<int> &arr)
{
    int maxi = arr[0];
    for(int i=0;i<arr.size();i++)
    {
        if(maxi < arr[i]) maxi = arr[i];
    }
    return maxi;
}

int sum_sequential(vector<int> &arr)
{
    int sum = 0;
    for(int i=0;i<arr.size();i++)
    {
        sum += arr[i];
    }
    return sum;
}

int average_sequential(vector<int> &arr)
{
    int average = sum_sequential(arr) / arr.size();
    return average;
}

int min_parallel(vector<int> &arr)
{
    int mini = arr[0];
    #pragma omp parallel for reduction(min : mini)
    for(int i=0;i<arr.size();i++)
    {
        if(mini > arr[i]) mini = arr[i];
    }
    return mini;
}

int max_parallel(vector<int> &arr)
{
    int maxi = arr[0];
    #pragma omp parallel for reduction(max : maxi)
    for(int i=0;i<arr.size();i++)
    {
        if(maxi < arr[i]) maxi = arr[i];
    }
    return maxi;
}

int sum_parallel(vector<int> &arr)
{
    int sum = 0;
    #pragma omp parallel for reduction(+ : sum)
    for(int i=0;i<arr.size();i++)
    {
        sum += arr[i];
    }
    return sum;
}

int average_parallel(vector<int> &arr)
{
    int average = sum_parallel(arr) / arr.size();
    return average;
}

int main()
{
    int n = 10000;
    vector<int> v(n);
    for(int i=0;i<n;i++)
    {
        v[i] = rand() % n;    
    }
    
    auto start_time = high_resolution_clock::now();
    min_sequential(v);
    auto end_time = high_resolution_clock::now();
    cout<<"Time for min sequential: "<<duration_cast<microseconds>(end_time- start_time).count()<<" ms\n";
    
    start_time = high_resolution_clock::now();
    min_parallel(v);
    end_time = high_resolution_clock::now();
    cout<<"Time for min parallel: "<<duration_cast<microseconds>(end_time- start_time).count()<<" ms\n";
    
    start_time = high_resolution_clock::now();
    max_sequential(v);
    end_time = high_resolution_clock::now();
    cout<<"Time for max sequential: "<<duration_cast<microseconds>(end_time- start_time).count()<<" ms\n";
    
    start_time = high_resolution_clock::now();
    max_parallel(v);
    end_time = high_resolution_clock::now();
    cout<<"Time for max parallel: "<<duration_cast<microseconds>(end_time- start_time).count()<<" ms\n";
    
}