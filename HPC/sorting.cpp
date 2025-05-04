#include<bits/stdc++.h>
#include<omp.h>
#include<chrono>

using namespace std;
using namespace chrono;

void bubbleSequential(vector<int> &arr)
{
    int n = arr.size();
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        }
    }
}

void bubbleParallel(vector<int> &arr)\
{
    int n = arr.size();
    for(int i=0;i<n;i++)
    {
        #pragma omp parallel for
        for(int j=(i+1) % 2;j<n-1;j+=2)
        {
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
        }
    }
}

void merge(vector<int> &arr, int l, int m , int r)
{
    vector<int> temp;
    int left = l;
    int right = m + 1;
    while(left<=m and right <= r)
    {
        if(arr[left] <= arr[right])
        {
            temp.push_back(arr[left]);
            left++;
        }
        else
        {
            temp.push_back(arr[right]);
            right++;
        }
    }
    
    while(left <= m) {
        temp.push_back(arr[left]);
        left++;
    }
    
    while(right <= r) {
        temp.push_back(arr[right]);
        right++;
    }
    
    for(int i=l;i<=r;i++) {
        arr[i] = temp[i-l];
    }
}

void mergeSequential(vector<int> &arr, int l, int r)
{
    if(l < r)
    {
        int m = (l + r) / 2;
        mergeSequential(arr, l , m);
        mergeSequential(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void mergeParallel(vector<int> &arr, int l, int r, int depth = 0)
{
    if(l < r)
    {
        int m = (l + r) / 2;
        if(depth <= 4)
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeParallel(arr, l, m, depth + 1);
            #pragma omp section
            mergeParallel(arr, m+1, r, depth + 1);
        }
        else 
        {
            mergeSequential(arr, l, m);
            mergeSequential(arr, m+1, r);
        }
        
        merge(arr,l,m,r);
    }
}


int main()
{
    vector<int> arr(10000), arr_cpy;
    srand(time(0));
    for(int i=0;i<10000;i++)
        arr[i] = rand() % 10000;
    arr_cpy = arr;
    
    
    auto start_time = high_resolution_clock::now();
    bubbleSequential(arr);
    auto end_time = high_resolution_clock::now();
    
    cout<<"Time taken sequential bubble: "<<duration_cast<milliseconds>(end_time - start_time).count()<<" ms\n";
    
    start_time = high_resolution_clock::now();
    bubbleParallel(arr);
    end_time = high_resolution_clock::now();
    cout<<"Time taken parallel bubble: "<<duration_cast<milliseconds>(end_time - start_time).count()<<" ms\n";
    
    start_time = high_resolution_clock::now();
    mergeSequential(arr, 0, 10000);
    end_time = high_resolution_clock::now();
    cout<<"Time taken sequential merge sort: "<<duration_cast<microseconds>(end_time - start_time).count()<<" ms\n";

    start_time = high_resolution_clock::now();
    mergeParallel(arr, 0, 10000);
    end_time = high_resolution_clock::now();
    cout<<"Time taken parallel merge sort: "<<duration_cast<microseconds>(end_time - start_time).count()<<" ms\n";

}


/*
Q1. What is Bubble Sort?
A: Bubble sort is a simple comparison-based algorithm where adjacent elements are swapped if they are in the wrong order. It has a time complexity of O(n²).

Q2. What is Merge Sort?
A: Merge sort is a divide-and-conquer algorithm. It splits the array in halves, sorts each half recursively, and merges the sorted halves. Time complexity is O(n log n).

Q3. How is Bubble Sort parallelized?
A: Using odd-even transposition sort, where we alternate between comparing even-odd and odd-even indexed pairs in parallel.

Q4. How is Merge Sort parallelized?
A: By running the left and right recursive calls of merge sort in parallel sections using OpenMP.

Q5. What does #pragma omp parallel sections mean?
A: It allows different blocks (sections) of code to run concurrently in separate threads.

⚙️ Performance & Analysis
Q6. Why does Merge Sort perform better in parallel?
A: Merge Sort divides the task into independent sub-tasks (divide-and-conquer), making it highly parallelizable with less synchronization.

Q7. Why might Bubble Sort not show much improvement in parallel?
A: It involves frequent data dependencies (adjacent comparisons) and many passes, limiting its parallel performance.

Q8. What is the depth parameter in parallel merge sort for?
A: It limits recursion depth for parallelism to avoid thread explosion.

Q9. What happens if too many threads are created?
A: It may lead to thread contention, high memory usage, and worse performance than the sequential version.

Q10. How do you measure the performance of sorting algorithms?
A: By recording the execution time using high-resolution clocks (chrono library in C++).

*/
