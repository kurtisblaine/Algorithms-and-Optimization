///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////g++ -std=c++11 fib.cpp -o fib
//    ./fib N T 40 1
//    ./fib X T 40 100

#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <ratio>
#include <chrono>

// learn how to use #defines... save youself some typing!
#define ULL unsigned long long int

// functions we want to test...

/**********************************************************************************************************************

    fibRecur

***********************************************************************************************************************/
ULL  fibRecur(ULL x)
{
    ULL result=1; // default for base cases
    if (x>=2) result = fibRecur(x-1) + fibRecur(x-2);
    return result;  // single entry single exit for good style
}

/**********************************************************************************************************************

    fibRecurDp
        (note: there are two versions: 2nd one is fibRecurDpAlt

***********************************************************************************************************************/

// helper functino to do the actual recursion after wrapper function sets up the DP table
ULL fibRecurDpHelper(ULL x, ULL *dpTable) // define helper here, since only wrapper uses it
{
    ULL result;
    if (dpTable[x] != 0 ) // no fib#'s are 0, so we can just check for 0 to see if table already has value
        result = dpTable[x];
    else
    {
        // note: dpTable is passed by reference (a pointer to the table) so it is always the same table
        result = fibRecurDpHelper(x-1, dpTable) + fibRecurDpHelper(x-2, dpTable);
        dpTable[x] = result; // store result in the table 
    }
    return result;
}

// wrapper function - this is what is called elsewhere (such as in main) when you want to calculate a fibonacci number
ULL fibRecurDp(ULL x) // wrapper
{
    ULL result;
    if (x<2) result = 1;
    else 
    {
        ULL *dpTable = (ULL *) calloc(x+1,sizeof(ULL)); // using x+1, since we need entries for 0..X
        dpTable[0]=1;
        dpTable[1]=1;
        result = fibRecurDpHelper(x,dpTable);
        free(dpTable);
    } 
    return result;
}

// alternate DP recursive function that avoids need for wrapper by using a static variable
ULL fibRecurDpAlt(ULL x)
{
    ULL result=1; // default value of result for base cases x=0,1
    // calloc sets allt to 0, and no fib #'s are 0, so can indicate not-yet-set
    static ULL *dpTable = NULL; // static variables are shared by all calls to the function (not with static will only be set to NULL first time)

    int freeTable = 0; // we'll use this to know if we are in the *first* call, so we know to free the allocated table later
    while (dpTable==NULL) // dpTable will be *initially* NULL in the first call, so allocate it, clearing all data to zeros
    {
        freeTable = 1;  // remember we are in the first call, so we know to free the table later
        dpTable = (ULL *) calloc(x+1,sizeof(ULL)); // allocate table, all set to 0's,  (x+1 because we need entries for 0..x)
        dpTable[0]=1;
        dpTable[1]=1;
    }
    if ( dpTable[x] != 0 ) // if table entry already set
    {
        result = dpTable[x]; // then use the table entry
    }
    else
    {
        result = fibRecurDpAlt(x-1) + fibRecurDpAlt(x-2);
        dpTable[x]=result;
    }
    if (freeTable) // if we're in the first call, we need to free the table when we're done
    {
        free(dpTable);
        dpTable=NULL;  // must reset the static pointer variable to NULL, so next "first" call will have a clean slate
    }
    
    return result; // single-entry/single-exit is good style. Hence only one "return" at the end.
}

/**********************************************************************************************************************

    fibLoop

***********************************************************************************************************************/
ULL fibLoop(ULL x)
{
    ULL result=1; //default for base cases
    if (x>=2) 
    {
        ULL last=1; 
        ULL secondLast=1;
        for(ULL k=2;k<=x;k++)
        {  
            result = last + secondLast;
            secondLast=last;
            last=result;
        }
    }
    return result;
}
/**********************************************************************************************************************

    fibMatrix

***********************************************************************************************************************/

// helper function to calculate powers of a 2x2 matrix, used by fibMatrix
// you could generalize to powers of any matrix (also writing a matrixMultiplication function) but not necessary
//
// How this works:
//     Suppose we want to calculate M^27
//     27 in binary is 11011b
//     note that 11011b modulo 2 is 1, and 11011b / 2 is 1101 (integer division, equivalen to right shitf 1 bit)
//     repeating the above process gets us the bits of 27 in least to greatest order
//     keep in mind the binary representation of 27 just means that 27 = 1*2^4 + 1*2^3 + 0*2^2 + 1*2^1 + 1*2^0
//     
//     Now consider that:  M^27 = M^16 * M^8 * M^2 * M^1  (because 27 = 16+8+2+1)
//          notice we use all the powers of 2 that correspond to 1 bits in the binary rep of 27
//          also notice that:  M^2 is M^1 squared
//                             M^4 is M^2 squared
//                             M^8 is M^4 sqaured ... and so on
// so all we need is a loop that determines the binary bits of the power, repeatedly squares our base matrix,
// and multiplies that into the result matrix when the binary bits are 1

void matrixPower2x2(ULL matrix[2][2], ULL power, ULL resultMatrix[2][2]) // arrays are passed by reference, so resultMatrix lets us get the result back out
                                                                         // we could put the result in matrix, but separating "input" from "output" is cleaner
{
    ULL tmp;
    // start with tmpMatrix as our base matrix we want to take powers of
    ULL tmpMatrix[2][2] = { { matrix[0][0], matrix[0][1] }, 
                            { matrix[1][0], matrix[1][1] } };

    // initialize result matrix to the identity matrix
    resultMatrix[0][0]=1;resultMatrix[0][1]=0;
    resultMatrix[1][0]=0;resultMatrix[1][1]=1;

    ULL a,b,c,d;
    for(tmp=power; tmp>0; tmp = tmp/2) // tmp=tmp/2 effectively shifts the bits of tmp left by 1 (dropping the leftmost bit)
    {
        // check if the leftmost bit of tmp is a 1
        if (tmp % 2 == 1)
        {
            // multiply resultMatrix by tmpMatrix (resultMatrix is a *product* accumulator, similar to sum accumulators
            a = resultMatrix[0][0]*tmpMatrix[0][0] + resultMatrix[0][1]*tmpMatrix[1][0];
            b = resultMatrix[0][0]*tmpMatrix[0][1] + resultMatrix[0][1]*tmpMatrix[1][1];
            c = resultMatrix[1][0]*tmpMatrix[0][0] + resultMatrix[1][1]*tmpMatrix[1][0];
            d = resultMatrix[1][0]*tmpMatrix[0][1] + resultMatrix[1][1]*tmpMatrix[1][1];
            resultMatrix[0][0] = a;
            resultMatrix[0][1] = b;
            resultMatrix[1][0] = c;
            resultMatrix[1][1] = d;
        }
        // square the tmp matrix for the next time around
        a = tmpMatrix[0][0]*tmpMatrix[0][0] + tmpMatrix[0][1]*tmpMatrix[1][0];
        b = tmpMatrix[0][0]*tmpMatrix[0][1] + tmpMatrix[0][1]*tmpMatrix[1][1];
        c = tmpMatrix[1][0]*tmpMatrix[0][0] + tmpMatrix[1][1]*tmpMatrix[1][0];
        d = tmpMatrix[1][0]*tmpMatrix[0][1] + tmpMatrix[1][1]*tmpMatrix[1][1];
        tmpMatrix[0][0] = a;
        tmpMatrix[0][1] = b;
        tmpMatrix[1][0] = c;
        tmpMatrix[1][1] = d;
    }
}   

ULL fibMatrix(ULL x) 
{
    ULL result;
    ULL baseMatrix[2][2] = { { 1, 1 },
                             { 1, 0 } };
    ULL finalMatrix[2][2];

    matrixPower2x2(baseMatrix, x, finalMatrix); // aarrays passed by reference, so finalMatrix is modified
    result = finalMatrix[0][0];  // result is in top right corner of final matrix:
    return result;
}

/***************************************************************************************************************************

    Correctness Tests

***************************************************************************************************************************/
void correctnessTests (char XorN, ULL MaxFibRecurX  )
{ 
    ULL X, N, tmpResult;

    printf("# Correctness Check for all Fibonacci variant functions\n");
    printf("# Column Headers\n");
    printf("# Size(N)  Value(X)        FibRecur(X)       FibRecurDp(X)    FibRecurDpAlt(X)          FibLoop(X)        FibMatrix(X)\n");
    // Loop over values of x because it is more straightforward
    // use n = floor(log(x)) + 1 (number of bits needed for x) to get the sizes
    for (X=1;X<=92;X++) 
    {
        N = floor(log2(X)) + 1; // number of bits needed for value of X in binary
        // check XorN which plot do we need data for?
        // if 'N' then we only want to do worst case (i.e. when X = (2^N)-1
        if ( XorN=='X' || X == (ULL) pow(2.0,(double)N)-1 )
        {
            printf("%6lld %6lld ",N,X);
            // ******** fibRecur **********
            if (X<=MaxFibRecurX)  // from experiementation, we know we don't have time for fibRecur beyong about X=50, so skip it for higher X
            {
                tmpResult = fibRecur(X);
                printf("%20lld", tmpResult);
            } 
            else printf("%20c",'?'); // "?" means "undefined" to gnuplot, lets us maintain the fibRecur column
            // ******** fibRecurDp **********
            tmpResult = fibRecurDp(X);
            printf("%20lld", tmpResult);
            // ******** fibRecurDpAlt **********
            tmpResult = fibRecurDpAlt(X);
            printf("%20lld", tmpResult);
            // ******** fibLoop **********
            tmpResult = fibLoop(X);
            printf("%20lld", tmpResult);           
            // ******** fibMatrix *********
            tmpResult = fibMatrix(X);
            printf("%20lld", tmpResult);       
            // ****************************
            printf("\n"); // drop to next line after all data printed for this value of X
        }
    }
}

/********************************************************************************************

   Function that runs time test for a single function for a single input value (it runs all trials for a single X)
   Note that it takes a function pointer as a parameter, which lets us pass in the function to be tested

********************************************************************************************/
double timeTestOneFunction(ULL (*functionToTest)(ULL), ULL X, int nTrials)
{
    int trial;
    ULL tmpResult;
    clock_t start, stop;   // clock_t defined time.h lib
    // std::chrono::high_resolution_clock::time_point   start,stop; // high resolution start/stop (from chrono)
    double timeIntervalInSeconds;     // this will be the runtime of our function
    double totalTimePerX, averageTime_nanosecs;
        
    totalTimePerX = 0.0;
    // set startingTimeStamp:  we will measure time interval for all trials, the take an average.  
    // clock() only returns time used by our program fro more accurate results
    // back to the old clock() function (which returns milliseconds) 
    // must do enough trials to get non-zero results
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    // start = std::chrono::high_resolution_clock::now();
    for (trial=0;trial<nTrials;trial++) 
    {
        // **** Call function we are testing with X as input
        tmpResult = (*functionToTest)(X);  // this is how to call a function using a pointer to the function
    }
    // set endingTimeStamp
    stop = clock();
    //stop = std::chrono::high_resolution_clock::now();
    timeIntervalInSeconds = (stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
    //timeIntervalInSeconds =(double) std::chrono::duration_cast<std::chrono::duration<double>>(stop-start).count();    
    averageTime_nanosecs = 1000000000.0 * timeIntervalInSeconds / nTrials;
    return averageTime_nanosecs;
}


void timeTests(char XorN, ULL MaxFibRecurX, int nTrials )
{
    /***************************************************************************************************************************

        Time Tests

    ***************************************************************************************************************************/
    int trial;
    ULL X, N, tmpResult;
    double averageTime_nanosecs;


    printf("# Performance Data for all Fibonacci function variants\n");
    printf("# Column Headers\n");
    printf("#                    +---------------------- Time (Nanoseconds)-------------------------+\n");
    printf("# Size(N)  Value(X)   FibRecur      FibRecurDp   FibRecurDpAlt         FibLoop      FibMatrix\n");
    // Loop over values of x because it is more straightforward
    // use n = floor(log(x)) + 1 (number of bits needed for x) to get the sizes
    for (X=1;X<=92;X++) 
    {
        N = floor(log2(X)) + 1; // number of bits needed for value of X in binary
        // check XorN which plot do we need data for?
        // if 'N' then we only want to do worst case (i.e. when X = (2^N)-1
        if ( XorN=='X' || X == (ULL) pow(2,N)-1 )
        {
            printf("%6lld %6lld ",N,X);
            // ******** fibRecur **********
            // from experiementation, we know we don't have time for fibRecur beyond about X=48, so skip it for higher X
            if (X<=MaxFibRecurX) 
            {
                // call teimTestOneFunction, pass in the function we want to test 
                averageTime_nanosecs = timeTestOneFunction(fibRecur,X,nTrials);
                printf("%16.2lf", averageTime_nanosecs);
            } 
            else
            {
                printf("%16c",'?'); // "?" means "undefined" to gnuplot, lets us maintain the fibRecur column
            }
                
            // ******** fibRecurDp **********
            averageTime_nanosecs = timeTestOneFunction(fibRecurDp,X,nTrials);
            printf("%16.2lf", averageTime_nanosecs);

            // ******** fibRecurDpAlt **********
            averageTime_nanosecs = timeTestOneFunction(fibRecurDpAlt,X,nTrials);
            printf("%16.2lf", averageTime_nanosecs);

            // ******** fibLoop **********
            averageTime_nanosecs = timeTestOneFunction(fibLoop,X,nTrials);
            printf("%16.2lf", averageTime_nanosecs);
            
            // ******** fibMatrix *********
            averageTime_nanosecs = timeTestOneFunction(fibMatrix,X,nTrials);
            printf("%16.2lf", averageTime_nanosecs);

            // ****************************
            printf("\n"); // drop to next line after all data printed for this value of X
        }
    }
}

int main (int argc, char* argv[]) 
{
    int nTrials;
    ULL MaxFibRecurX;
    // use argv[1] to tell us whether to show all X values or just the worst case for each N
    // I highly recommend learning how to use argc & argv to utilize command line arguments to your C programs.
    char XorN = argv[1][0]; // expecting string argv[1] to be just one letter: "X" or "N"
    // use 2nd arg to indicate whether we want to rum time tests or correctness tests  "T" or "C"
    char TimeOrCorrectness = argv[2][0];
    // use 3rd arg to specify how high to go with fibRecur
    sscanf(argv[3],"%llu",&MaxFibRecurX);  // convert string argument to unsigned long long integer value  
    // use 4th arg to specify number of trials (only if doing time test)
    if ( TimeOrCorrectness == 'T' ) sscanf(argv[4],"%d",&nTrials);

    // run selected tests...
    if ( TimeOrCorrectness == 'C' ) 
    {    
        correctnessTests(XorN, MaxFibRecurX);
    }
    else // assume TimeOrCorrectness == 'T'
    {
        timeTests(XorN, MaxFibRecurX, nTrials);
    }
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////
