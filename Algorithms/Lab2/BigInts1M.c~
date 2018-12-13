
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

void setBigInt_FromInteger(BigInt *x, unsigned int y)
{
    // break y into its 4 bytes... (only as many as needed)
    
    int numBits = floor(log2(y)) + 1;
    int numBytes = (int) ceil( numBits / (double) 8.0 ); // forced to use double floating point division before applyin ceilign function
    
    clearBigInt(x); // set all digits to zero, numDigits=1
                    // not &x because in here x is already a pointer...

    x->numDigits = numBytes;
    unsigned int tmp = y; // use tmp instead of y -- good practice to not modify parameter
    for (int k = 0; k < numBytes; k++ )
    {
        x->digits_b256[k] = tmp % 256; // get each base-256 digit by modulo 256
        tmp = tmp/256;                  // equivalent to shifting 8 bits to the right (dropping the 8 rightmost bits)
    }   
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

// write the hexadecimal form of bigint into a string
// *** no bulletproofing -- make sure the string has 2x plu 1 as many characters as x.numDigits before you call this!!
// 
// if youdon't understand what's going on with tmpStr:
//    google: c pointer arithmetic
//
void hexString_FromBigInt(BigInt *x, char* s)
{
    int digitIndex = x->numDigits-1; // start with the rightmost big digit
    char* tmpStr = s;  // tmpStr will point into the middle of s, so sprintf can write to the correct place inside s each time
    for ( ; digitIndex>=0; digitIndex-- )
    {
        sprintf(tmpStr,"%02X",(unsigned int) x->digits_b256[digitIndex]); // each character value corresponds to exactly 2 hex digits
        tmpStr = tmpStr + 2; // increment address pointer by 2 bytes to point to where we will want the next two hex digits to go
    }
    *tmpStr = 0; // put in the null terminator, because this should be a string!
}
// same as above, but puts a space between each pair of hex digits
// *** make sure the string has 3x (plus 1) as many elements as their thare b256 digits in x when you call this!!!
void hexStringSpaced_FromBigInt(BigInt *x, char* s)
{
    int digitIndex = x->numDigits-1; // start with the rightmost big digit
    char* tmpStr = s;  // tmpStr will point into the middle of s, so sprintf can write to the correct place inside s each time
    for ( ; digitIndex>=0; digitIndex-- )
    {
        sprintf(s,"%02X ",(unsigned int) x->digits_b256[digitIndex]); // each character value corresponds to exactly 2 hex digits
        tmpStr = tmpStr + 3; // increment by 2 to point to where we will want the next two hex digits to go
    }
    *tmpStr = 0; // put in the null terminator, because this should be a string!
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


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////
////////////////



