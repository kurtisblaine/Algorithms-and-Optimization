/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 4
* Sort Variations
* COMAND TO RUN: g++ -std=c++11 sort.cpp -o sort
*time ./sort
***************************************************************************************************/
#include<cstdio>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<ratio>
#include<chrono>
#include<string.h>
#include<iostream> 
#include<cstring> 
#include<cstdlib>
#include<stdlib.h> 
#include<stdio.h>

#define MAXINT 100001
#define TRIALS 18
using namespace std;

/////////////////////////////////////////////////////////////////HELPERFUNCTIONS/////////////////////////////////////////////////////////////////
/* Function to print an array */
void printArray(int A[], int size) 
{ 
    int i; 
     printf("#\t");
    for (i=0; i < size; i++) 
        printf("%d ", A[i]); 
    printf("\n"); 
} 

void swap(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
}
/////////////////////////////////////////////////////////////////HELPERFUNCTIONS/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////INSERTIONSORT/////////////////////////////////////////////////////////////////
void insertionSort(int A[], int size) { 
   int pivot, j; 
   for (int i = 1; i < size; i++) 
   { 
       pivot = A[i]; 
       j = i-1; 

       while (j >= 0 && A[j] > pivot) 
       { 
           A[j+1] = A[j]; 
           j = j-1; 
       } 
       A[j+1] = pivot; 
   } 
}
/////////////////////////////////////////////////////////////////INSERTIONSORT/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////QUICKSORT/////////////////////////////////////////////////////////////////
int partition(int A[], int lowHalf, int highHalf) { 
    int pivot = A[highHalf];    // pivot 
    int i = (lowHalf - 1);  // Index of smaller element 
  
    for (int j = lowHalf; j <= highHalf- 1; j++) { 
        if (A[j] <= pivot) { 
            i++;    
            swap(&A[i], &A[j]); 
        } 
    } 
    swap(&A[i + 1], &A[highHalf]); 
    return (i + 1); 
} 

int partitionRandom(int A[], int lowHalf, int highHalf) { 
    // Generate a random number in between 
    // low .. high 
    srand(time(NULL)); 
    int random = lowHalf + rand() % (highHalf - lowHalf); 
  
    // Swap A[random] with A[high] 
    swap(A[random], A[highHalf]); 
  
    return partition(A, lowHalf, highHalf); 
}

void quickSort(int A[], int lowHalf, int highHalf, bool wise) { 

    if (wise == true) partitionRandom(A, lowHalf, highHalf);
    else{
        if (lowHalf < highHalf){ 
            int temp = partition(A, lowHalf, highHalf); 
            bool defaults = false;
            
            quickSort(A, lowHalf, temp - 1, defaults); 
            quickSort(A, temp + 1, highHalf, defaults); 
        } 
   }
}

/////////////////////////////////////////////////////////////////QUICKSORT/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////HEAPSORT/////////////////////////////////////////////////////////////////
void heapify(int A[], int n, int i) { 
    int largest = i; // Initialize largest as root 
    int l = 2*i + 1; // left = 2*i + 1 
    int r = 2*i + 2; // right = 2*i + 2 
  
    // If left child is larger than root 
    if (l < n && A[l] > A[largest]) 
        largest = l; 
  
    // If right child is larger than largest so far 
    if (r < n && A[r] > A[largest]) 
        largest = r; 
  
    // If largest is not root 
    if (largest != i) 
    { 
        swap(A[i], A[largest]); 
  
        // Recursively heapify the affected sub-tree 
        heapify(A, n, largest); 
    } 
}

void heapSort(int A[], int n) { 
    // Build heap (reAange Aay) 
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(A, n, i); 
  
    // One by one extract an element from heap 
    for (int i=n-1; i>=0; i--) 
    { 
        // Move current root to end 
        swap(A[0], A[i]); 
  
        // call max heapify on the reduced heap 
        heapify(A, i, 0); 
    } 
} 
/////////////////////////////////////////////////////////////////HEAPSORT/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////MERGESORT/////////////////////////////////////////////////////////////////
void merge(int A[], int l, int m, int r) { 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp Aays */
    int L[n1], R[n2]; 
    /* Copy data to temp Aays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = A[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = A[m + 1+ j]; 
  
    /* Merge the temp Aays back into A[l..r]*/
    i = 0; // Initial index of first subAay 
    j = 0; // Initial index of second subAay 
    k = l; // Initial index of merged subAay 
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            A[k] = L[i]; 
            i++; 
        } 
        else{ 
            A[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) { 
        A[k] = L[i]; 
        i++; 
        k++; 
    } 
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) { 
        A[k] = R[j]; 
        j++; 
        k++; 
    } 
} 
  
/* l is for left index and r is right index of the 
   sub-Aay of A to be sorted */
void mergeSort(int A[], int l, int r) 
{ 
    if (l < r) { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        mergeSort(A, l, m); 
        mergeSort(A, m+1, r); 
  
        merge(A, l, m, r); 
    } 
}
/////////////////////////////////////////////////////////////////MERGESORT/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////TESTINGFRAMEWORK/////////////////////////////////////////////////////////////////
void generateArray(int A[], int length, int whichSort){
    int randomIndex, otherRandomIndex;
    int B[MAXINT];
    switch (whichSort){
        case 0:
            printf("# Already sorted list\n");
            for(int i = 0; i < length; i++)
                A[i] = i;
            break;
        case 1:
            printf("# Reverse sorted list\n");
            for(int i = 0; i < length; i++){
                int reverse = length - 1 - i;
                A[i] = reverse;
            }
            break;
        case 2:
            printf("# Randomly generated list\n");
             for(int i = 0; i < length; i++){
                randomIndex = rand() % MAXINT;
                A[i] = randomIndex;
             }
            break;
        case 3:
            printf("# Partially sorted list\n");
            for(int i = 0; i < length; i++){
                int reverse = length - 1 - i;
                A[i] = reverse;
            }
            for(int p = 0; p < MAXINT/10; p++){
                randomIndex = rand() % MAXINT;
                otherRandomIndex = rand() % MAXINT;
                swap(A[randomIndex], A[otherRandomIndex]); 
            }
            break;
        default: 
            printf("#####Error:404\n");
    }
}

/////////////////////////////////////////////////////////////////TESTINGFRAMEWORK/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////
int main() { 
    bool wise = false;
    float two = 2.0;
    float nextInputSize;
    int X[MAXINT], Y[MAXINT];
    int sizeOfX,sizeOfY, n; 
    double timeInsertion, timeQuick, timeQuickWise, timeHeap, timeMerge;
    int whichSort = 0;
    time_t start, stop;
    int A[MAXINT];
    n = sizeof(A) / sizeof(A[0]); 
    
for(float i = 6.0; i < TRIALS; i++){   
//for(whichSort = 0; whichSort < 4; whichSort++){
    whichSort = 3;
    generateArray(A, nextInputSize, whichSort);
//}  
///////////////////////////////////////CORRECTNESS TESTS//////////////////////////////////////
    printf("# Correctness Tests for Sort Algorithms\n");    
    printf("# Unsorted array of integers before sorting algorithms\n");
    //printArray(A, nextInputSize);
    printf("\n");

///////////////////////////////////////INSERSION//////////////////////////////////////
    printf("# INSERSION\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    insertionSort(A,nextInputSize);
    stop = clock(); // set endingTimeStamp
    timeInsertion += (stop-start)/(double)CLOCKS_PER_SEC;
    //printArray(A, nextInputSize);
///////////////////////////////////////QUICKSORT//////////////////////////////////////  
    printf("# QUICK\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    quickSort(A, 0, nextInputSize-1, wise);  
    stop = clock(); // set endingTimeStamp
    timeQuick += (stop-start)/(double)CLOCKS_PER_SEC;
    //printArray(A, nextInputSize); 
///////////////////////////////////////QUCIKSORTWISE//////////////////////////////////////  
    printf("# QUCIKWISE\n");  
    wise = true;
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    quickSort(A, 0, nextInputSize-1, wise);
    stop = clock(); // set endingTimeStamp
    timeQuickWise += (stop-start)/(double)CLOCKS_PER_SEC;
    //printArray(A, nextInputSize); 
///////////////////////////////////////HEAP//////////////////////////////////////    
    printf("# HEAP\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    heapSort(A, nextInputSize - 1); 
    stop = clock(); // set endingTimeStamp
    timeHeap += (stop-start)/(double)CLOCKS_PER_SEC;
    //printArray(A, nextInputSize);
///////////////////////////////////////MERGE//////////////////////////////////////
    printf("# MERGE\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    mergeSort(A, 0, nextInputSize-1); 
    stop = clock(); // set endingTimeStamp
    timeMerge += (stop-start)/(double)CLOCKS_PER_SEC;
    //printArray(A, nextInputSize); 
///////////////////////////////////////CORRECTNESS TESTS//////////////////////////////////////
//}

///////////////////////////////////////TIME TESTS//////////////////////////////////////
    nextInputSize = pow(two,i);
    printf("\n# Performance Data for Sorting implementations\n");
    printf("# With Generated Data\n");
    printf("# Column Headers\n");
    printf("# Input(N)   INSERSION(T)   \tQUICK(T)\t   QUCIKWISE(T)\t\tHEAP(T)   \tMERGE(T)\n");
    printf("     \t%f    %.12lf    %.12lf     %.12lf   %.12lf  %.12lf\n\n\n", nextInputSize, timeInsertion, timeQuick, timeQuickWise, timeHeap, timeMerge);
    }
///////////////////////////////////////TIME TESTS////////////////////////////////////// 
    return 0; 
}
/////////////////////////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////
