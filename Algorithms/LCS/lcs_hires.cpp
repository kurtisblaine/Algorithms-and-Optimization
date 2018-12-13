////////////////////////////////
////////////////how to run
/////g++ -std=c++11 lcs_hires.cpp -o lcs_hires

#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <ratio>
#include <chrono>
#include <unistd.h>

#define MAXSTRLEN 1000001
#define numberOfSizes 17

// fucntion we want to test... LCS implementation
// int lcs(char *s1, char *s2, int *s1index, int *s2index)
//                             ^^^^pseudo return values for the start indice  of the LCS
int lcs(char s1[MAXSTRLEN], char s2[MAXSTRLEN]) {
    int i,j,k, lcsLen;
    for(i=0; s1[i]!=0; i++){
       for(j=0; s2[j]!=0; j++){
          for(k=0; s1[i+k]!=0 && s2[j+k]!=0 ; k++){
              if ( s1[i+k] != s2[i+k] ) break;
          }
          if ( k > lcsLen ) {
              lcsLen = k;
          }
       }
    }
    return lcsLen;
}


// function to create randomized input to test out "algorithm" function
void generateRandomSampleInput(char testString1[MAXSTRLEN], char testString2[MAXSTRLEN], int stringSize) {
    int i;
    for(i=0;i<stringSize; i++) {
        testString1[i] = rand() % 95 + 32;
        testString2[i] = rand() % 95 + 32;
    }
    testString1[stringSize] = 0; // sets the null terminator at end of our string
    testString2[stringSize] = 0; // sets the null terminator at end of our string
}

// function to create randomized input to test out "algorithm" function
void generateWorstCaseSampleInput(char testString1[MAXSTRLEN], char testString2[MAXSTRLEN], int stringSize) {
    int i;
    for(i=0;i<stringSize; i++) {
        testString1[i] = 'A';
        testString2[i] = 'A';
    }
    testString1[stringSize] = 0; // sets the null terminator at end of our string
    testString2[stringSize] = 0; // sets the null terminator at end of our string
}




/// function to create a string of random char or a given length

int main (int argc, char* argv[]) {

    int sizeIndex,trial,lengthOfCommonSubStr;
    // clock_t start, stop;   // clock_t defined time.h lib
    std::chrono::high_resolution_clock::time_point   start,stop; // high resolution start/stop (from chrono)

    double timeIntervalInSeconds;     // this will be the runtime of our function

    double totalTimeForInputSize, averageTimeForInputSize;

    char testSearchStringA[MAXSTRLEN]; 
    char testSearchStringB[MAXSTRLEN];
    char testSearchChar;

    printf("# Performance Data for LCS implementation\n");
    printf("# Column Headers\n");
    printf("# Input Size(N)   Time(T)\n");

    // set range or specific list of input sizes we want to test
    int sizesOfInput[numberOfSizes] = { 1, 5, 10, 15, 20, 30, 40, 60, 80, 120, 160, 240, 320, 480, 640, 960, 1280 };   
	// set number of trials per input size (to get average results, maybe other statistics)
    int trialsPerSize = 4;
	// For each size N of input we want to test
    for (sizeIndex=0;sizeIndex<numberOfSizes;sizeIndex++) {
	    // For each trial trialNumber=1 to Number of Trials per input size:
        for (trial=0;trial<trialsPerSize;trial++) {
            // **** Generate sampleInput -- create a string of random characters...
            generateWorstCaseSampleInput(testSearchStringA, testSearchStringB, sizesOfInput[sizeIndex]);

            // set startingTimeStamp
            //start = clock(); // clock function from time.h lib, (or ctime in C++)
            start = std::chrono::high_resolution_clock::now();

            // **** Call functionWeAreTesting with sampleInput as input
            lengthOfCommonSubStr = lcs(testSearchStringA, testSearchStringB);

            // set endingTimeStamp
            // stop = clock();
            stop = std::chrono::high_resolution_clock::now();

            // let runtime = endingTimeStamp - startingTimeStamp
            // timeInterval = (stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            timeIntervalInSeconds =(double) std::chrono::duration_cast<std::chrono::duration<double>>(stop-start).count();

            // print per trial data:  N, trialNumber, runtime 
            // printf("Input Size: %d, trial: %d, Run Time: %lf \n",sizesOfInput[sizeIndex],trial,interval); 
            // Accumulate aggregates, such as…
            totalTimeForInputSize = totalTimeForInputSize + timeIntervalInSeconds;
        }
        averageTimeForInputSize = totalTimeForInputSize / trialsPerSize;
        printf("%d      %.12lf\n", sizesOfInput[sizeIndex], averageTimeForInputSize);

        // Print aggregate results for input size N:
	    //  N
	    //  average run time
	    // total time? number trials?
    }
}


///////
//////////////////////
/////////////////////////////////////

