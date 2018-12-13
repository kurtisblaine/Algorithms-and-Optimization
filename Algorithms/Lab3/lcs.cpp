/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 3
* Least Common Substring (LCS)
* COMAND TO RUN: g++ -std=c++11 lcs.cpp -o lcs
*time ./lcs
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
#define MAXSTRLEN 1000001
#define numberOfSizes 21
using namespace std;

/////////////////////////////////////////////////////////////////PSUEDOCODE FOR BRUTE FORCE LCS ALGORITHM/////////////////////////////////////////////////////////////////
	/*LcsBrute(S1, S2)  returns lcs
		let l1 = length(S1), l2 = length(S2)
		let lcsLength = 0
		for i = 0 to l1   // loop iterating over characters of first string
			 for j= 0 to l2  // loop interating over characters of 2nd string
				 for k=0 to min(l1-i-1, l2-j-1) // loop over characters of a matching substring
					} if ( S1[i+k] != S2[j+k] ) break;
				 if (k > maxSoFar)
					} lcsLength = k
					} lcsStartIndexInS1= i
					} lcsStartIndexInS2= j
		return: lcsLength , lcsStartIndexInS1, lcsStartIndexInS2
*/
/////////////////////////////////////////////////////////////////PSUEDOCODE FOR BRUTE FORCE LCS ALGORITHM/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////DYNAMIC PROGRAMMING LCS/////////////////////////////////////////////////////////////////
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int lcsWithDynamicProgramming( char *X, char *Y, int m, int n ) 
{ 
   int L[m+1][n+1]; 
  
   /* Following steps build L[m+1][n+1] in bottom up fashion. Note 
      that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
   for (int k=0; k<=m; k++) 
   { 
     for (int j=0; j<=n; j++) 
     { 
       if (k == 0 || j == 0) 
         L[k][j] = 0; 
       else if (X[k-1] == Y[j-1]) 
         L[k][j] = L[k-1][j-1] + 1; 
       else
         L[k][j] = max(L[k-1][j], L[k][j-1]); 
     } 
   }
   // Following code is used to print LCS 
   int index = L[m][n]; 
  
   // Create a character array to store the lcs string 
   char lcs[index+1]; 
   lcs[index] = '\0'; // Set the terminating character 
  
   // Start from the right-most-bottom-most corner and 
   // one by one store characters in lcs[] 
   int i = m, j = n; 
   while (i > 0 && j > 0) 
   { 
      // If current character in X[] and Y are same, then 
      // current character is part of LCS 
      if (X[i-1] == Y[j-1]) 
      { 
          lcs[index-1] = X[i-1]; // Put current character in result 
          i--; j--; index--;     // reduce values of i, j and index 
      } 
  
      // If not same, then find the larger of two and 
      // go in the direction of larger value 
      else if (L[i-1][j] > L[i][j-1]) 
         i--; 
      else
         j--; 
   } 
   return strlen(lcs);
} 
/////////////////////////////////////////////////////////////////DYNAMIC PROGRAMMING LCS/////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////IMPLEMENTATION////////////////////////////////////////////////////////////////////////////
int lcsOwnImplement(char *X, char *Y, int lengthX, int lengthY)
{
    int LCS[lengthX + 1][lengthY + 1];
    int result = 0;
    for (int i = 0; i <= lengthX; i++)
    {
        for (int j=0; j<=lengthY; j++)
        {
            if (i == 0 || j == 0)
                LCS[i][j] = 0;
            else if (X[i-1] == Y[j-1])
            {
                LCS[i][j] = LCS[i-1][j-1] + 1;
                result = max(result, LCS[i][j]);
            }
            else LCS[i][j] = 0;
        }
    }
    return result;
}
///////////////////////////////////////////////////////////////IMPLEMENTATION////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////BRUTE FORCE ALGORITHM/////////////////////////////////////////////////////////////////
// fucntion we want to test... LCS implementation
// int lcs(char *s1, char *s2, int *s1index, int *s2index)
//                             ^^^^pseudo return values for the start indice  of the LCS
int lcsBruteForce(char s1[MAXSTRLEN], char s2[MAXSTRLEN]){
    int i,j,k;
    int lcsLen = 0;
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

/////////////////////////////////////////////////////////////////BRUTE FORCE ALGORITHM/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////TEST FUNCTIONS/////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////TEST FUNCTIONS/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////////
int main(){
    int lengthOfX, lengthOfY, lengthOfCommonSub,sizeIndex,trial;
    char X[MAXSTRLEN], Y[MAXSTRLEN];
    double total_time_brute_force, total_time_dynamic_programming, 
        timeIntervalInSeconds,totalTimeForInputSizeBF, totalTimeForInputSizeDP,total_time_own,averageTimeForInputSizeDP, averageTimeForInputSizeBF,totalTimeForInputSizeMY, averageTimeForInputSizeMY;
    time_t start, stop;
    int sizesOfInput[numberOfSizes] = { 1, 5, 10, 15, 20, 30, 40, 60, 80, 120, 160, 240, 320, 480, 640, 960, 1280, 1400, 1620, 1840, 1960};
    int trialsPerSize = 4;
    
    
    printf("#Enter a string, x, for testing...\n#");
    scanf("%s", X);
    printf("#Enter a string, y, for testing...\n#");
    scanf("%s", Y);
     
    lengthOfX = strlen(X);
    lengthOfY = strlen(Y);

///////////////////////////////////////CORRECTNESS TESTS//////////////////////////////////////
    printf("\n\n# Correctness Tests for Input Strings\n");
    printf("# Brute Force\tDynamic   \tMy algo\n");
    lengthOfCommonSub = lcsBruteForce(X,Y);
    printf("# %d\t\t ", lengthOfCommonSub);
    lengthOfCommonSub = lcsWithDynamicProgramming(X,Y,lengthOfX, lengthOfY);  
    printf("%d\t\t", lengthOfCommonSub); 
    lengthOfCommonSub = lcsOwnImplement(X,Y,lengthOfX, lengthOfY);
    printf("%d\n", lengthOfCommonSub);   
    
    printf("\n\n# Performance Data for LCS implementation\n");
    printf("# With Generated Data\n");
    printf("# Column Headers\n");
    printf("# Input(N)   TimeBF(T)   TimeDP(T)   TimeOwn(T)\n");
///////////////////////////////////////CORRECTNESS TESTS///////////////////////////////////////
    
     for (sizeIndex=0;sizeIndex<numberOfSizes;sizeIndex++) {
	    // For each trial trialNumber=1 to Number of Trials per input size:
        for (trial=0;trial<trialsPerSize;trial++) {
            // **** Generate sampleInput -- create a string of random characters...
            generateWorstCaseSampleInput(X, Y, sizesOfInput[sizeIndex]);
            
////////////////////////////////////////TIME TEST FOR BRUTE FORCE ALGORITHM////////////////////////////////////////
            start = clock(); // clock function from time.h lib, (or ctime in C++)
            lcsBruteForce(X,Y);
            stop = clock(); // set endingTimeStamp
            total_time_brute_force += (stop-start)/(double)CLOCKS_PER_SEC;
////////////////////////////////////////TIME TEST FOR BRUTE FORCE ALGORITHM////////////////////////////////////////

////////////////////////////////////TIME TEST FOR DYNAMIC PROGRAMMING ALGORITHM////////////////////////////////////
            start = clock(); // clock function from time.h lib, (or ctime in C++)
            lcsWithDynamicProgramming(X,Y,lengthOfX, lengthOfY);
            stop = clock(); // set endingTimeStamp
            total_time_dynamic_programming += (stop-start)/(double)CLOCKS_PER_SEC;
////////////////////////////////////TIME TEST FOR DYNAMIC PROGRAMMING ALGORITHM////////////////////////////////////

////////////////////////////////////TIME TEST FOR MY ALGORITHM////////////////////////////////////////////////////
            start = clock(); // clock function from time.h lib, (or ctime in C++)
            lcsOwnImplement(X,Y,lengthOfX, lengthOfY);
            stop = clock(); // set endingTimeStamp
            total_time_own +=(stop-start)/(double)CLOCKS_PER_SEC;
////////////////////////////////////TIME TEST FOR MY ALGORITHM////////////////////////////////////////////////////

            timeIntervalInSeconds += (stop-start)/(double)CLOCKS_PER_SEC;

            totalTimeForInputSizeBF = totalTimeForInputSizeBF + total_time_brute_force;
            totalTimeForInputSizeDP = totalTimeForInputSizeDP + total_time_dynamic_programming;
            totalTimeForInputSizeMY= totalTimeForInputSizeMY + total_time_own;
        }
        averageTimeForInputSizeBF = totalTimeForInputSizeBF / trialsPerSize;
        averageTimeForInputSizeDP = totalTimeForInputSizeDP / trialsPerSize;
        averageTimeForInputSizeMY =totalTimeForInputSizeMY / trialsPerSize;
        printf("%d      %.12lf      %.12lf      %.12lf\n", sizesOfInput[sizeIndex], averageTimeForInputSizeBF, averageTimeForInputSizeDP, averageTimeForInputSizeMY);
    }
    return 0;
}
/////////////////////////////////////////////////////////////////MAIN/////////////////////////////////////////////////////////////////
