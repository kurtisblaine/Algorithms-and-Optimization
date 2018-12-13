////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 1
* FIBONACCI VARIATIONS
***************************************************************************************************/
////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////LIBRARIES/////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

///////////////////////////////////////LIBRARIES/////////////////////////////////////////////////////



///////////////////////////////////////FUNCTION CALLS////////////////////////////////////////////////


//basic straigt up implementation of fibonacci algorithm
int FibLoop(unsigned long long int x){
//base bases for the fibonacci sequence
    if (x < 2) return 1;
    else{
//variable initiation for local function
        int a=1;
        int b=1;
        int c=0;
        
//loop adding last two digits added up
        for(int k=2; k<=x; k++){
            c = a+b;
            a = b;
            b = c;
        }
        return c;
    }
}

//recursively called fibonacci function
int FibRecur(unsigned long long int x){
    if(x<2) return 1;
//calls the same function and adds the previous two recursively
    else return FibRecur(x-1) + FibRecur(x-2);
}

//helper function for fibrecurdp funciton
int FibRecursiveWithCache(unsigned long long int x, int array[]){
    bool isInArray = false;  
    int returnValue = 0;  
    
//if the element is found in the array return true
    for(int i = 0; i <= x; i++) if(x == array[i]) isInArray = true; 
    
    if(x < 2) return 1;
    else if(isInArray == true) return array[x];
    else{
        returnValue = FibRecursiveWithCache(x-1, array) + FibRecursiveWithCache(x-2, array);
//stores the return value at index x in the array to be used in future recursions
        array[x] = returnValue;
        return returnValue;
    }
}

//recursively called fibonacci function (wrapper function)
int FibRecurDP(unsigned long long int x){
    int FibResultsCache[x];
    int result=0;
    
//initalized cache array
    for(int i = 0; i <= x; i++) FibResultsCache[i] = -1;
    
//calls function with the cache array 
    result = FibRecursiveWithCache(x, FibResultsCache);

    return result; 
}

void MultiplyMatrix(unsigned long long int matrix1[2][2],unsigned long long int matrix2[2][2]){
//matrix multiplication
    int x = matrix1[0][0]*matrix2[0][0] + matrix1[0][1]* matrix2[1][0];
    int y = matrix1[0][0]*matrix2[0][1] + matrix1[0][1]* matrix2[1][1];
    int a = matrix1[1][0]*matrix2[0][0] + matrix1[1][1]* matrix2[1][0];
    int b = matrix1[1][0]*matrix2[0][1] + matrix1[1][1]* matrix2[1][1];
    
//save results in local variables
    matrix1[0][0] = x;
    matrix1[0][1] = y;
    matrix1[1][0] = a;
    matrix1[1][1] = b;    
}

void FindPowerMatrix(unsigned long long int matrix[2][2], unsigned long long int x){
    unsigned long long int matrix2[2][2] = {{1,1},{1,0}};
    
//calculates power of the matrix by calling helper function multiplymatrix
    for(int i = 2; i <= x; i++) MultiplyMatrix(matrix, matrix2);
  
}

int FibMatrix(unsigned long long int x){
    unsigned long long int matrix[2][2] = {{1,1},{1,0}};
    if(x == 0) return 0;
//calls a helper function that calculates the power of the matrix
    FindPowerMatrix(matrix, x);

    return matrix[0][0]; 
}


void error(void){printf("You entered a something other than an integer when I expected an integer, c'mon\n");}



///////////////////////////////////////FUNCTION CALLS////////////////////////////////////////////////


//////////////////////////////////////MAIN///////////////////////////////////////////////////////////

#define numberOfSizes 10

int main (int argc, char *argv[]) { 
      
//while(1){
//local vairiables
    clock_t start=0;
    clock_t stop=0;
    double interval=0;// this will be the runtime of our function  
    int userChoice=0;
    unsigned long long int userInput=0;
    unsigned long long int fibAnswer=0;

//printing optionz for the user
    printf("#******************************\n");
    printf("#Please Enter 1 for FibLoop\n");
    printf("#Please Enter 2 for FibRecur\n");
    printf("#Please Enter 3 for FibRecurDP\n");
    printf("#Please Enter 4 for FibMatrix\n");
    printf("#Control + C to quit\n");
    printf("#******************************\n");
    
//get the user choice
    scanf("%d", &userChoice); 
    
//if user choice is not a digit send error
    if(isdigit(userChoice)) error();
    else{

        printf("#    Now enter a value to send to the fibonacci function\n");
        scanf("%llu", &userInput);
    
//algorithm options with trial information
    for (unsigned long long int sizeIndex=1;sizeIndex<userInput;sizeIndex++) {

        switch(userChoice){
            start = clock();
            case 1:fibAnswer=FibLoop(sizeIndex); break; 
            case 2:fibAnswer=FibRecur(sizeIndex); break;
            case 3:fibAnswer=FibRecurDP(sizeIndex); break;   
            case 4:fibAnswer=FibMatrix(sizeIndex); break;
            default: error(); exit(1);
        }
        stop = clock();
        
            // let runtime = endingTimeStamp - startingTimeStamp
            interval = (double)(stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h
            //totalTime += interval; 
            // print trial data:  N, runtime 
            
            printf("%llu     %lf \n",fibAnswer, interval); 
            
        }
    
//display results
        printf("#Input: %llu\n#Final Answer: %llu\n#Total Time: %lf \n#Algorithm: %d\n\n", userInput, fibAnswer, interval, userChoice);
    }
  //}
}


//////////////////////////////////////MAIN///////////////////////////////////////////////////////////
