/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 5
* Sort Variations
* COMAND TO RUN: g++ -std=c++11 graph.cpp -o graph
* time ./graph
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
#include <bits/stdc++.h> 
using namespace std;

#define V 500
#define TRIALS 32
#define INTMAX 999999
#define BOUNDARYX 100

int dp[V*V][V];
int graph[V*V][V];
int nodes[V];		    
int cost=999;		    
/////////////////////////////////////////////////////////////////HELPER/////////////////////////////////////////////////////////////////

void GenerateRandomCostMatrix(int n){
    int random;
    
    for(int i=0;i<n;i++){
        printf("#");
        for(int j=0;j<n;j++){
            if(i == j){ 
                graph[i][j] = 0;
                printf(" %d ", graph[i][j]);
            }
            else{
                random = rand() % BOUNDARYX;
                graph[i][j] = random;
                printf(" %d ", graph[i][j]);
           }
        }
        printf("\n");
    }
}

void GenerateRandomEuclideanCostMatrix(int n){
    int randomIndex = rand() % BOUNDARYX;
    int otherRandomIndex = rand() % BOUNDARYX;
}

void GenerateRandomCircularCostMatrix(int n){
    int randomIndex = rand() % BOUNDARYX;
    int otherRandomIndex = rand() % BOUNDARYX;
}

void generateGraph(int whichSort, int length){
  printf("# Generating graph...\n");
  
  switch(whichSort){
            case 0:
                    printf("# Random Cost Matrix\n");
                    GenerateRandomCostMatrix(length);
                    break;
            case 1:
                    printf("# Random Euclidean Cost Matrix");
                    GenerateRandomEuclideanCostMatrix(length);
                    break;
            case 2:
                    printf("# Random Circular Cost Matrix");
                    GenerateRandomCircularCostMatrix(length);
                    break;
            default: printf("# You broke something\n");
  }
  
}

void swap (int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void copy_array(int *nodes, int n){
    int i;
    int sum=0;
    for(i = 0; i <= n-1; i++)
        sum += graph[nodes[i % n]][nodes[(i + 1) % n]];
    
    if (cost > sum)
        cost = sum;
}
void getPath(int n){
    int node, min, j=0;
    
   // printf("0 ");
    for(int i=0;i<n;i++){
        if(i == j) min = 999;
        else{ min = graph[i][0]; printf("-> ");}
        for(j=0;j<n;j++){
           if(i == j) goto label;
           if(min > graph[i][j]) min = graph[i][j]; 
     
           label: continue;
        }

       for(int j=0;j<n;j++) if(min == graph[i][j]) node = j;
        printf("%d ", node);
        
    }
}
/////////////////////////////////////////////////////////////////HELPER/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////ALGORITHMS/////////////////////////////////////////////////////////////////
int TspGreedy(int *nodes, int index, int length){
   int j=0;
   int k=0; 
   if (index == length)
        copy_array(nodes, length+1);
   
   else{
         for(j = index; j <= length; j++){
            swap((nodes + index), (nodes + j));
            TspGreedy(nodes, index + 1, length);
            swap((nodes + index), (nodes + j));
        }
    }
}

int TspBrute(int s, int n) { 
	vector<int> vertex;
	
	for (int i = 0; i < n; i++) 
		if (i != s) 
			vertex.push_back(i); 

	// store minimum weight
	int min_path = INT_MAX; 
	do { 
		int current_pathweight = 0; 
 
		int k = s; 
		for (int i = 0; i < vertex.size(); i++) { 
			current_pathweight += graph[k][vertex[i]]; 
			k = vertex[i]; 
		} 
		current_pathweight += graph[k][s]; 

		min_path = min(min_path, current_pathweight); 
		
	} while (next_permutation(vertex.begin(), vertex.end())); 

	return min_path; 
} 

int TspDynamic(int mask, int pos, int visited, int n){
	if(mask==visited) return graph[pos][0];
	if(dp[mask][pos]!= -1) return dp[mask][pos];
	
	//Now from current node, we will try to go to every other node and take the min ans
	int ans = INTMAX;

	//Visit all the unvisited nodes and take the best route
	for(int node=0;node<n;node++){
		if((mask&(1<<node))==0){
			int newAns = graph[pos][node] + TspDynamic( mask | (1<<node), node, visited, n);
			ans = min(ans, newAns);
		}
	}
	return dp[mask][pos] = ans;
}

/////////////////////////////////////////////////////////////////ALGORITHMS/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////
int main() { 
    time_t start, stop;
    srand(time(NULL)); 
    float timeBrute, timeGreedy, timeDynamic, timeACS, nextInputSize;
    float two = 2.0;
    int visited, whichTest=0;
	
for(int nextInputSize = 2; nextInputSize < TRIALS; nextInputSize=nextInputSize+2){	
///////////////////////////////////////CORRECTNESS TESTS//////////////////////////////////////
    printf("\n# Correctness Tests for TS Algorithms\n");    
    generateGraph(whichTest, nextInputSize);
    
///////////////////////////////////////INITIALIZING//////////////////////////////////////
    //bit wise operation 2^n
    visited = (1 << nextInputSize) - 1;
    for(int i=0;i<(1<<nextInputSize);i++){
        for(int j=0;j<nextInputSize;j++){
            dp[i][j] = -1;
        }
    }
    for(int i = 0; i < nextInputSize; i++) nodes[i]=i;
    cost=999;
///////////////////////////////////////INITIALIZING//////////////////////////////////////  
if(nextInputSize >= 12) goto jumper ;
else{
    printf("# GREEDY\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    cout << "# Cost: " << TspBrute(0, nextInputSize) << endl; 
    stop = clock(); // set endingTimeStamp
    timeBrute += (stop-start)/(double)CLOCKS_PER_SEC;
    cout << "# Path: "; 
    getPath(nextInputSize); 
    cout << endl;
    
    printf("# BRUTE\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
	TspGreedy(nodes, 0, nextInputSize-1);
	stop = clock(); // set endingTimeStamp
    timeGreedy += (stop-start)/(double)CLOCKS_PER_SEC;
	cout << "# Cost: " << cost << endl;
	cout << "# Path: ";
	getPath(nextInputSize);
	cout << endl;
   }
   jumper: 
    printf("# DYNAMIC\n");
    start = clock(); // clock function from time.h lib, (or ctime in C++)
	cout << "# Cost: " << TspDynamic(1, 0, visited, nextInputSize) << endl;
	stop = clock(); // set endingTimeStamp
    timeDynamic += (stop-start)/(double)CLOCKS_PER_SEC;
    cout << "# Path: "; 
    getPath(nextInputSize);
    cout << endl;
    
	
	
///////////////////////////////////////CORRECTNESS TESTS//////////////////////////////////////
	
///////////////////////////////////////TIME TESTS//////////////////////////////////////
	printf("\n# Performance Data for Graph Algorithms\n");
    printf("# With Generated Data\n");
    printf("# Column Headers\n");
    printf("# Input(N)   GREEDY(T)   \tBRUTE(T)\t   DYNAMIC(T)\t\t \n");
	printf("     \t%d    %.12lf    %.12lf     %.12lf  \n\n", nextInputSize, timeBrute, timeGreedy, timeDynamic);
    }
///////////////////////////////////////TIME TESTS//////////////////////////////////////
	return 0; 
} 
/////////////////////////////////////////////////////////////////MAIN////////////////////////////////////////////////////////////////

