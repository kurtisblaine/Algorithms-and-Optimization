#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

// suppose we were experimentally studying the performance of an algorithm for search a string...
// this is our function to test
int countCharInString(char searchFor, char *searchIn) {
    int i,count;
    for (i=0;searchIn[i]!=0;i++){
        if(searchFor == searchIn[i]) count++;
    }
    return count;
}

// function to create randomized input to test out "algorithm" function
void generateSampleInput(char *testChar, char *testString, int stringSize) {
    int i;
    // get rand int, module the range size, + the starting number...
    // this give random ints from: 32..127
    *testChar = rand() % 95 + 32; // look up ascii code -- 32 for space, 1st printable char

    for(i=0;i<stringSize; i++) {
        testString[i] = rand() % 95 + 32;
    }
    testString[stringSize] = 0; // sets the null terminator at end of our string
}

#define numberOfSizes 11

/// function to create a string of random char or a given length

int main (int argc, int argv) {

    int sizeIndex,trial,numberFound;
    clock_t start, stop; // clock_t defined time.h lib
    double interval;     // this will be the runtime of our function

    char testSearchString[1000001]; 
    char testSearchChar;

    // set range or specific list of input sizes we want to test
    int sizesOfInput[numberOfSizes] = { 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000 };   
	// set number of trials per input size (to get average results, maybe other statistics)
    int trialsPerSize = 3;
	// For each size N of input we want to test
    for (sizeIndex=0;sizeIndex<numberOfSizes;sizeIndex++) {
	    // For each trial trialNumber=1 to Number of Trials per input size:
        for (trial=0;trial<trialsPerSize;trial++) {
            // **** Generate sampleInput -- create a string of random characters...
            generateSampleInput(&testSearchChar, &testSearchString, sizesOfInput[sizeIndex]);

            // set startingTimeStamp
            start = clock(); // clock funciton from time.h lib, (or ctime in C++)

            // **** Call functionWeAreTesting with sampleInput as input
            numberFound = countCharInString(testSearchChar, testSearchString);

            // set endingTimeStamp
            stop = clock();
            // let runtime = endingTimeStamp - startingTimeStamp
            interval = (stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            // print trial data:  N, trialNumber, runtime 
            printf("Input Size: %d, trial: %d, Run Time: %lf \n",sizesOfInput[sizeIndex],trial,interval); 
            // (possibly other info, such as the input used)
            // Accumulate aggregates, such as…
            //						} totalTime = totalTime+runTime;
            //						} totalTimeSquared = totalTimeSquared + runTime*runTime;
        }
        // Print aggregate results for input size N:
	    //  N
	    //  average run time
	    // total time? number trials?
    }
}

//////////////////////////////////
/////////////////////////////////
