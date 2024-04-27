#include <iostream>
#include <vector>
#include <climits>
#include <chrono>
#include <omp.h>
#include <bits/stdc++.h>

using namespace std;
using namespace std::chrono;

// Function to perform sequential reduction for minimum value
int sequentialMin(const vector<int> &arr)
{
    int minVal = INT_MAX;
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i] < minVal)
        {
            minVal = arr[i];
        }
    }
    return minVal;
}

// Function to perform sequential reduction for maximum value
int sequentialMax(const vector<int> &arr)
{
    int maxVal = INT_MIN;
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i] > maxVal)
        {
            maxVal = arr[i];
        }
    }
    return maxVal;
}

// Function to perform sequential reduction for sum
int sequentialSum(const vector<int> &arr)
{
    int sum = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        sum += arr[i];
    }
    return sum;
}

// Function to perform sequential reduction for average
double sequentialAverage(const vector<int> &arr)
{
    int sum = sequentialSum(arr);
    return sum / double(arr.size());
}

int parallelMin(const vector<int> &arr)
{
    int minVal = INT_MAX;
#pragma omp parallel for reduction(min : minVal)
    for (int i = 0; i < arr.size(); ++i)
    {
        minVal = min(minVal, arr[i]);
    }
    return minVal;
}

// Function to perform parallel reduction for maximum value
int parallelMax(const vector<int> &arr)
{
    int maxVal = INT_MIN;
#pragma omp parallel for reduction(max : maxVal)
    for (int i = 0; i < arr.size(); ++i)
    {
        maxVal = max(maxVal, arr[i]);
    }
    return maxVal;
}

// Function to perform parallel reduction for sum
int parallelSum(const vector<int> &arr)
{
    int sum = 0;
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < arr.size(); ++i)
    {
        sum += arr[i];
    }
    return sum;
}

// Function to perform parallel reduction for average
double parallelAverage(const vector<int> &arr)
{
    int sum = parallelSum(arr);
    return sum / double(arr.size());
}

int main()
{
    srand(time(NULL)); // Seed the random number generator
    const int size = 10000;
    vector<int> arr(size);

    // Initialize the array with random values
    for (int i = 0; i < size; ++i)
    {
        arr[i] = rand() % 1000;
        cout << arr[i] << " ";
    }
    cout << endl;

    // Sequential calculations
    auto start = high_resolution_clock::now();
    int seqMinVal = sequentialMin(arr);
    int seqMaxVal = sequentialMax(arr);
    int seqSum = sequentialSum(arr);
    double seqAverage = sequentialAverage(arr);
    auto stop = high_resolution_clock::now();
    auto durationSeq = duration_cast<microseconds>(stop - start);

    // Parallel calculations
    start = high_resolution_clock::now();
    int parMinVal = parallelMin(arr);
    int parMaxVal = parallelMax(arr);
    int parSum = parallelSum(arr);
    double parAverage = parallelAverage(arr);
    stop = high_resolution_clock::now();
    auto durationPar = duration_cast<microseconds>(stop - start);

    cout << "Sequential results:" << endl;
    cout << "Minimum value: " << seqMinVal << endl;
    cout << "Maximum value: " << seqMaxVal << endl;
    cout << "Sum: " << seqSum << endl;
    cout << "Average: " << seqAverage << endl;
    cout << "Time taken by sequential operations: " << durationSeq.count() << " microseconds" << endl;

    cout << "Parallel results:" << endl;
    cout << "Minimum value: " << parMinVal << endl;
    cout << "Maximum value: " << parMaxVal << endl;
    cout << "Sum: " << parSum << endl;
    cout << "Average: " << parAverage << endl;
    cout << "Time taken by parallel operations: " << durationPar.count() << " microseconds" << endl;

    return 0;
}
