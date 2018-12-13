//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////
////////////////

/*****************************************************************************************************************************

    BigInts1M.c
    
    A simple implementation of BigInt definitions and functions.

    This version of BigInt (parital) Library just makes them arbitrarily 1 Megabyte integers (uses char array of size 2^20).
    On today's computers, a megabyte is tiny compared to multi-gigabyte memory sizes, so we're pretty safe using a whole megabyte 
    for each big integer. 
    
    This simplifies our code: by using fixed size arrays for the digits, we have no need for memory management (malloc, calloc, free...).
    
    It does mean we will be wasting memory on small numbers, but since we have gigabytes to spare, it is fine for experimental purposes.
    
    We are however, limited to 8 megabit integers. But that should be big enough for most test we might want to do.
    
*******************************************************************************************************************************/
#include <math.h>
#include <string.h>
#include <stdio.h>

// define the largest number of base-256 digits (bytes) for our big integers... can increase this if you need to work with larger
// of course multiply this by 8 to get max number of bits
#define MaxBigIntBytes 1048576 // this is 2^20 or "1 Mega-" as in "Megabyte"

typedef struct BigInt {

    unsigned int  numDigits; // number of base-256 digits used in the array (the rest should be "leading zeros") so really the number of bytes
    unsigned char digits_b256[MaxBigIntBytes]; // store our base-256 digits in this array
                                               // fixed array size sets a limit on the size of integer (hopefully bigger than we need)

} BigInt;


// Always do this on a new BigInt before using it in any other function!!
void clearBigInt(BigInt *x) 
{
    x->numDigits = 1; // there's always at least one digit: 0 in the ones column
    for(int k=0;k < MaxBigIntBytes; k++)
    {
        x->digits_b256[k] = 0;
    }
}

unsigned int numBitsInBigInt(BigInt *x)
{
    unsigned char leftmostDigit = x->digits_b256[ x->numDigits - 1 ];
    return 8 * ( x->numDigits-1 )  +  floor(log2(leftmostDigit))+1;
}

// function to compare two bigints...
// return -1 if x is lesser, 0 if equal, +1 if y is lesser
int compareBigInts(BigInt *x, BigInt *y)
{
    if (x->numDigits < y->numDigits) return -1;
    if (x->numDigits > y->numDigits) return +1;
    int lastIndex = x->numDigits-1; // same for both if we get to this point
    for(int k = lastIndex; k>=0; k--)
    {
        if ( x->digits_b256[ k ] < y->digits_b256[ k ] ) return -1;
        if ( x->digits_b256[ k ] > y->digits_b256[ k ] ) return +1;
    }
    // if we get this far they are equal...
    return 0;
    // I've broken the single-entry-single-exit rule... rules are made to be broken.
}

// set a bigint using a string of hex digits... 
// you would want to read such a string with "%s" (read it as a string) then use this function to convert that string to a bigint
void setBigInt_FromHexString(char* s, BigInt *x)
{
    char *tmpStr = s; // use this as temporary pointer into middle of string s
    unsigned int tmpInt; // store numeric value of hex digit(s) read
    int strIndex=0; // we're reading left to right so first hex digit we encounter has highest inex in the array
    int sLength = strlen(s);

    clearBigInt(x); // not &x because in here x is already a pointer...
    
    x->numDigits = (sLength/2) + (sLength % 2);
    int digitIndex = x->numDigits - 1;  // start with highest digit index (left to right here)
    
    // check if string length is odd, so we only read a single hex digit first time (each pair *from the right* corresponds to a byte) 
    if ( sLength % 2 == 1 )
    { // odd number of digits, so read only the first hex digit here...
        tmpStr = & ( s[strIndex] ); // make tmpStr point to kth character in string, so it can be use a string pointer starting at right position for sscanf        
        sscanf(tmpStr,"%1X",&tmpInt); // %1X to read only 1 hex digit instead of all of them  
        strIndex++;
        x->digits_b256[digitIndex] = (unsigned char) tmpInt; 
        digitIndex--;
    }
    // let strIndex keep its value from above, don't re-initialize it (yes, you can have empty initializer in a for loop)
    for( ; strIndex < sLength ; strIndex = strIndex+2 )
    {
        tmpStr = & ( s[strIndex] ); // make tmpStr point to kth character in string, so it can be use a string pointer starting at right position for sscanf
        sscanf(tmpStr,"%2X",&tmpInt); // %1X to read only 1 hex digit instead of all of them
        x->digits_b256[digitIndex] = (unsigned char) tmpInt; 
        digitIndex--;    
    } 
}

string addBigInt(string str1, string str2){
        // Before proceeding further, make sure length
        // of str2 is larger.
        if (str1.length() > str2.length())
            swap(str1, str2);
        // Take an empty string for storing result
        string str = "";
        // Calculate lenght of both string
        int n1 = str1.length(), n2 = str2.length();
        // Reverse both of strings
        reverse(str1.begin(), str1.end());
        reverse(str2.begin(), str2.end());
        int carry = 0;
        for (int i=0; i<n1; i++)
        {
        // Do school mathematics, compute sum of
        // current digits and carry
        int sum = ((str1[i]-'0')+(str2[i]-'0')+carry);
        str.push_back(sum%10 + '0');
        // Calculate carry for next step
        carry = sum/10;
        }
        // Add remaining digits of larger number
        for (int i=n1; i<n2; i++)
        {
            int sum = ((str2[i]-'0')+carry);
            str.push_back(sum%10 + '0');
            carry = sum/10;
        }
        // Add remaining carry
        if (carry)
            str.push_back(carry+'0');
        // reverse resultant string
        reverse(str.begin(), str.end());
        return str;
}

string multiplyBigInt(string num1, string num2) 
{ 
    int n1 = num1.size(); 
    int n2 = num2.size(); 
    if (n1 == 0 || n2 == 0) 
       return "0"; 
  
    // will keep the result number in vector 
    // in reverse order 
    vector<int> result(n1 + n2, 0); 
  
    // Below two indexes are used to find positions 
    // in result.  
    int i_n1 = 0;  
    int i_n2 = 0;  
  
    // Go from right to left in num1 
    for (int i=n1-1; i>=0; i--) 
    { 
        int carry = 0; 
        int n1 = num1[i] - '0'; 
  
        // To shift position to left after every 
        // multiplication of a digit in num2 
        i_n2 = 0;  
          
        // Go from right to left in num2              
        for (int j=n2-1; j>=0; j--) 
        { 
            // Take current digit of second number 
            int n2 = num2[j] - '0'; 
  
            // Multiply with current digit of first number 
            // and add result to previously stored result 
            // at current position.  
            int sum = n1*n2 + result[i_n1 + i_n2] + carry; 
  
            // Carry for next iteration 
            carry = sum/10; 
  
            // Store result 
            result[i_n1 + i_n2] = sum % 10; 
  
            i_n2++; 
        } 
  
        // store carry in next cell 
        if (carry > 0) 
            result[i_n1 + i_n2] += carry; 
  
        // To shift position to left after every 
        // multiplication of a digit in num1. 
        i_n1++; 
    } 
  
    // ignore '0's from the right 
    int i = result.size() - 1; 
    while (i>=0 && result[i] == 0) 
       i--; 
  
    // If all were '0's - means either both or 
    // one of num1 or num2 were '0' 
    if (i == -1) 
       return "0"; 
  
    // generate the result string 
    string s = ""; 
    while (i >= 0) 
        s += std::to_string(result[i--]); 
  
    return s; 
} 


}

// a main program to test & act as example of using the bigint functions...
#define MaxIOCharArraySize 5000
int main(int argc, char* argv[])
{
    BigInt x,y,z;
    char xInString[MaxIOCharArraySize], yInString[MaxIOCharArraySize];
    char xOutString[MaxIOCharArraySize], yOutString[MaxIOCharArraySize];
    string additionStr, multiplyStr;
    printf("Please enter 2 large integers, x y in hexadecimal, up to 1000 digits:\n");
    // read them in as strings then use our function to conver...
    scanf("%1000s %1000s", xInString, yInString,);
    //convert to bigints
    //setBigInt_FromHexString(xInString, &x);   
    //setBigInt_FromHexString(yInString, &y); 
    //start = clock();
    additionStr = addBigInt(xInString, yInString);
    //stop = clock();
    
   // printf("# Correctness Check for BigInts (Addition)\n");
   // printf("# Column Headers\n");
   // printf("# Input Size(N)  Time(T) \n");
    // let runtime = endingTimeStamp - startingTimeStamp
    //interval = (double)(stop-start) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h
   // printf("%lf \n" interval);
    
    //start = clock();
    multiplyStr = multiplyBigInt(xInString, yInString);
    //stop = clock();
    
   printf("# Addition: \n");
   
    
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////
////////////////




