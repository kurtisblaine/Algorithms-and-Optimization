////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 2
* BigInts (Addition and mulitiplication)
* COMAND TO RUN: g++ -std=c++11 BigIntsLib.cpp -o bil 
***************************************************************************************************/

///////////////////////////////////////LIBRARIES/////////////////////////////////////////////////////
//#include "BigInts1M.c"
#include<bits/stdc++.h> 
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <ratio>
#include <chrono>

///////////////////////////////////////LIBRARIES/////////////////////////////////////////////////////

using namespace std; 

///////////////////////////////////////FUNCTION CALLS////////////////////////////////////////////////

string addBigInt(string xInString, string yInString){
    // Make sure the first string isn't smaller than the second...
    if (xInString.length() > yInString.length())
        swap(xInString, yInString);
    //Restul string to store the result of the addition
    string resultString = "";
    // Calculate lenght of both strings being calculated
    int lengthStringX = xInString.length(), lengthStringY = yInString.length();
    // Reverse both of strings to start at first index
    reverse(xInString.begin(), xInString.end());
    reverse(yInString.begin(), yInString.end());
    int carryOver = 0;
    for (int i=0; i<lengthStringX; i++)
    {
    // Do old school mathematics compute the sum and do carryOvers as the digits roll
    int digitSum = ((xInString[i]-'0')+(yInString[i]-'0') + carryOver);
    resultString.push_back(digitSum %10 + '0');
    // Calculate carryOver for the lookahead step
    carryOver = digitSum/10;
    }
    // Add remaining digits of larger number
    for (int i=lengthStringX; i<lengthStringY; i++)
    {
        int digitSum = ((yInString[i]-'0') + carryOver);
        resultString.push_back(digitSum%10 + '0');
        carryOver = digitSum/10;
    }
    // Add the remaining carry after loop terminates
    if (carryOver)
        resultString.push_back(carryOver+'0');
    // reverse resultant string to the original layout to be displayed
    reverse(resultString.begin(), resultString.end());
    return resultString;
}

string multiplyBigInt(string xInString, string yInString) 
{ 
    int xInStringSize = xInString.size(); 
    int yInStringSize = yInString.size(); 
    if (xInStringSize == 0 || yInStringSize == 0) 
       return 0; 
  
    // utilizing a vector of ints to initialize the result
    vector<int> resultsVector(xInStringSize + yInStringSize, 0); 
  
    //Two different indexes to find the result in the loop
    //initialize to zero before iterations 
    int xInStringSizeIndex = 0;  
    int yInStringSizeIndex = 0;  
  
    // Go from the right side to the self side of the string in xInString 
    for (int i=xInStringSize-1; i>=0; i--) 
    { 
        int carryOver = 0; 
        int xInStringSize = xInString[i] - '0'; 
        //shift the position to the left after every multiplication carr
        yInStringSizeIndex = 0;  
          
        // now go from right side to the left side in xInString             
        for (int j=yInStringSize-1; j>=0; j--) 
        { 
            //take the jth digit of the number
            int yInStringSize = yInString[j] - '0'; 
            // Multiply with current digit of first number 
            // and add result to previously stored result 
            // at current position.  
            int digitSum = xInStringSize*yInStringSize + resultsVector[xInStringSizeIndex + yInStringSizeIndex] + carryOver; 
  
            //Carry over the digit sum for the next iteration to be
            //later added to next digit group
            carryOver = digitSum/10; 
  
            // Store result into the resultsVector and
            // access later for the next digit grouping
            resultsVector[xInStringSizeIndex + yInStringSizeIndex] = digitSum % 10; 
            //increment the size index finally,,
            yInStringSizeIndex++; 
        } 
        // store the carryOver into the next memory location in the vector
        if (carryOver > 0) 
            resultsVector[xInStringSizeIndex + yInStringSizeIndex] += carryOver; 
        //shift the position to the left after every iteration on the xInString
        xInStringSizeIndex++; 
    } 
  
    // ignore the following zeros
    //as they are just placeholders and can be deleted from the outputted string
    int vectorIndex = resultsVector.size() - 1; 
    while (vectorIndex>=0 && resultsVector[vectorIndex] == 0) 
       vectorIndex--; 
  
    //handles the case if one of the strings
    //inputted were zeros then we would return 
    if (vectorIndex == -1) 
       return 0; 
  
    //create the string to be outputted to the user for correctness 
    string resultString = ""; 
    while (vectorIndex >= 0) 
        resultString += std::to_string(resultsVector[vectorIndex--]); 
  
    return resultString; 
} 

unsigned int numBitsInBigInt(char* strUnderInvestigation)
{
    int stringLength = strlen(strUnderInvestigation);
    return pow(2,stringLength);
}

// function to compare two bigints...
// return -1 if x is lesser, 0 if equal, +1 if y is lesser
int compareBigInts(char* x, char* y)
{
    if (strlen(x) < strlen(y)) 
        return 1;
    else
        return 0;
}

///////////////////////////////////////FUNCTION CALLS////////////////////////////////////////////////

// a main program to test & act as example of using the bigint functions...
#define MaxIOCharArraySize 5000

//////////////////////////////////////MAIN///////////////////////////////////////////////////////////

int main()
{
    char xInString[MaxIOCharArraySize], yInString[MaxIOCharArraySize];
    string bigIntAnswer;
    int userChoice;
    int trial,j;
    clock_t start, stop;   // clock_t defined time.h lib
    // std::chrono::high_resolution_clock::time_point   start,stop; // high resolution start/stop (from chrono)
    double timeIntervalInSeconds;     // this will be the runtime of our function
    double totalTimePerX, averageTime_nanosecs;
    totalTimePerX = 0.0;
    // set startingTimeStamp:  we will measure time interval for all trials, the take an average.  
    // clock() only returns time used by our program fro more accurate results
    // back to the old clock() function (which returns milliseconds) 
    // must do enough trials to get non-zero results
    
    //give options
    printf("#******************************\n");
    printf("#Please Enter 1 for BigInt Addition\n");
    printf("#Please Enter 2 for BigInt Multiplication\n");
    printf("#Control + C to quit\n");
    printf("#******************************\n");
    
    //get the user choice
    scanf("%d", &userChoice); 
    
     printf("#Please enter 2 large integers up to 1000 digits:\n");
    // read them in as strings then use our function to conver...
    scanf("%1000s", xInString);
      if(userChoice == 1)
        cout << "# + " << endl;
    else
        cout << "# * " << endl;
    scanf("%1000s", yInString); 
    int numBitsX = numBitsInBigInt(xInString);
    int numBitsY = numBitsInBigInt(yInString);
    int comparedBigInts = compareBigInts(xInString,yInString); 
    
    printf("# Correctness Check for BigInts Variant\n");
    printf("# Column Headers\n");
    printf("# N     Time(T)\n");  
    
    j=1;
    start = clock(); // clock function from time.h lib, (or ctime in C++)
    // start = std::chrono::high_resolution_clock::now();
    for(int i = 1; i < MaxIOCharArraySize; i=i+100){
        //switch statement for userChoice
        switch(userChoice){
                case 1:bigIntAnswer=addBigInt(xInString, yInString); break; 
                case 2:bigIntAnswer=multiplyBigInt(xInString, yInString); break;
                default: exit(1);
            }
     // set endingTimeStamp
    stop = clock();
    //stop = std::chrono::high_resolution_clock::now();
    timeIntervalInSeconds = (stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
    //timeIntervalInSeconds =(double) std::chrono::duration_cast<std::chrono::duration<double>>(stop-start).count();    
    
    cout << 4*j << "    " << timeIntervalInSeconds << endl;
    j++;
  }    
    
}
//////////////////////////////////////MAIN///////////////////////////////////////////////////////////

