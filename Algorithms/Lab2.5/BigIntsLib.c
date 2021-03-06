///////////////////////////////////////////////////////////////////////
////////////////////////////////////
////////////////
/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 2.5
* Fibonacci  with BigInts
* COMAND TO RUN: gcc BigIntsLib.c -o BigIntsLib 
***************************************************************************************************/

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

#define ULL unsigned long long int 

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
    
    int numBits = floor(log2(y?y:1)) + 1;
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
        sprintf(tmpStr,"%02X ",(unsigned int) x->digits_b256[digitIndex]); // each character value corresponds to exactly 2 hex digits
        tmpStr = tmpStr + 2; // increment by 2 to point to where we will want the next two hex digits to go
    }
    *tmpStr = 0; // put in the null terminator, because this should be a string!
}

// function to compare two bigints...
// return -1 if x is lesser, 0 if equal, +1 if y is lesser
int compareBigInts(BigInt *x, BigInt *y)
{
    if (x->numDigits < y->numDigits) return -1;
    if (x->numDigits > y->numDigits) return +1;
    int lastIndex = x->numDigits-1; // same for both if we get to this points
    for(int k = lastIndex; k>=0; k--)
    {
        if ( x->digits_b256[ k ] < y->digits_b256[ k ] ) return -1;
        if ( x->digits_b256[ k ] > y->digits_b256[ k ] ) return +1;
    }
    // if we get this far they are equal...
    return 0;
    // I've broken the single-entry-single-exit rule... rules are made to be broken.
}

// because we are using a fixed size struct, we can get away without using pointers here -- the struct will be fully copied both
// when passed as parameter, or when returned. In this way the bigint function can work much more like regulat arithemetic expressions
// Note: this does not affect the big-, and addition is already O(n) and the copying steps only take O(n)
BigInt addBigInts(BigInt x, BigInt y)
{
    // note: my bigints have padded zeros, so no need to swap x & y based on size
    // be sure to zero out sum...
    BigInt sum;
    clearBigInt(&sum);
    ULL maxDigit = x.numDigits > y.numDigits ? x.numDigits : y.numDigits;
    sum.numDigits = maxDigit;
    ULL carry = 0;
    for(ULL digitIndex=0; digitIndex < maxDigit; digitIndex++)
    {
        ULL tmp = x.digits_b256[digitIndex] + y.digits_b256[digitIndex] + carry;
        sum.digits_b256[digitIndex] = tmp % 256;
        carry = tmp / 256; // can  only be 1 or 0
    }
    if (carry)
    {
        sum.numDigits++;
        sum.digits_b256[maxDigit]=1;
    }
    return sum;  // return will provide a copy of sum if used as an assignment

}
// because I am using a fixed size struct, we can do pass by value and get away without using pointers here 
// -- the struct(including the array) will be fully copied both when passed as parameter, or when returned. 
// In this way the bigint function can work much more like regular arithemetic expressions
// Note: this does not affect the big-, and multiplication is already O(n^2) and the copying only take O(n)
BigInt multiplyBigInts(BigInt x, BigInt y)
{
    ULL xDigit, yDigit;
    // zero out product
    BigInt product;
    clearBigInt(&product);

    for(xDigit=0; xDigit < x.numDigits ; xDigit++)
    {
        ULL carry = 0;
        BigInt lineProduct; // this will store the result of one digit of x times all of y
        clearBigInt(&lineProduct); // zero it out first
        for(yDigit=0; yDigit < y.numDigits ; yDigit++)
        {
            ULL digitProduct = x.digits_b256[xDigit] * y.digits_b256[yDigit] + carry; // product of two single digits, + carry
            lineProduct.digits_b256[yDigit+xDigit] = digitProduct % 256; // gets us the lower 8 bits - modulo the base gets you digit in that base
            carry = digitProduct / 256; // can never be larger than 254            
        }
        if (carry) 
        {
            lineProduct.digits_b256[xDigit+yDigit] = carry;
            lineProduct.numDigits= xDigit + yDigit + 1;
        }
        else
        {
            lineProduct.numDigits=xDigit+yDigit;        
        }
        // now add the line product to the accumulating sum...
        product = addBigInts(lineProduct,product);
    }
    return product;
}


// a main program to test & act as example of using the bigint functions...
#define MaxIOCharArraySize 5000
int main(int argc, char* argv[])
{
    BigInt x,y,z,w;
    char xInString[MaxIOCharArraySize], yInString[MaxIOCharArraySize], zInString[MaxIOCharArraySize];
    char xOutString[MaxIOCharArraySize], yOutString[MaxIOCharArraySize], zOutString[MaxIOCharArraySize], wOutString[MaxIOCharArraySize];
    printf("Please enter 3 large integers, x y and z, in hexadecimal, up to 1000 digits:\n");
    // read them in as strings then use our function to conver...
    scanf("%1000s %1000s %1000s", xInString, yInString, zInString);
    // convert them...
    printf(">>> Converting to bigints...\n");
    setBigInt_FromHexString(xInString, &x);   
    setBigInt_FromHexString(yInString, &y);    
    setBigInt_FromHexString(zInString, &z);   
    
    
    // now convert them back into strings...
    printf(">>> Converting the bigints back to strings\n");
    hexString_FromBigInt(&x, xOutString);
    hexString_FromBigInt(&y, yOutString);   
    hexString_FromBigInt(&z, zOutString);
    
    
    printf("**************************************************\n");
    printf("Addtion Tests...\n\n");
    w = addBigInts(x,y);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %30s\n+%30s\n",xOutString,yOutString);
    printf("===============================\n");
    printf("%30s\n\n",wOutString);
    w = addBigInts(y,z);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %30s\n+%30s\n",yOutString,zOutString);
    printf("===============================\n");
    printf("%30s\n\n",wOutString);
    w = addBigInts(z,x);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %30s\n+%30s\n",zOutString,xOutString);
    printf("===============================\n");
    printf("%30s\n\n",wOutString);
    printf("**************************************************\n");
    printf("Multiplication Tests...\n\n");
    w = multiplyBigInts(x,y);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %60s\n*%60s\n",xOutString,yOutString);
    printf("=============================================================\n");
    printf(" %60s\n\n",wOutString);
    w = multiplyBigInts(y,z);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %60s\n*%60s\n",yOutString,zOutString);
    printf("=============================================================\n");
    printf(" %60s\n\n",wOutString);
    w = multiplyBigInts(z,x);
    hexString_FromBigInt(&w, wOutString);   
    printf(" %60s\n*%60s\n",zOutString,xOutString);
    printf("=============================================================\n");
    printf(" %60s\n\n",wOutString);
    printf("**************************************************\n");        
    // now print the string back out... if they are correct it is highly like the conversions work both ways...
    printf(">>>Below are the before/after converting & converting back...\n");
    printf("x  input: %s \nx output: %s\n", xInString, xOutString);
    printf("y  input: %s \ny output: %s\n", yInString, yOutString);
    printf("z  input: %s \nz output: %s\n", zInString, zOutString);
    // now let's test the comparison function
    printf(">>>Testing comparisons...");
    if (compareBigInts(&x,&x)==0) {
        printf("x==x true\n");
    } else {
        printf("*****FAILURE: x==x false????? ");
    }
    if (compareBigInts(&y,&y)==0) {
        printf("y==y true\n");
    } else {
        printf("*****FAILURE: y==y false????? ");
    }
    if (compareBigInts(&z,&z)==0) {
        printf("z==z true\n");
    } else {
        printf("*****FAILURE: z==z false????? ");
    }
    // compare x to y
    if (compareBigInts(&x,&y)==-1){
        printf("x < y\n");
    } else if (compareBigInts(&x,&y)==1) {
        printf("x > y\n");
    } else {
        printf("x == y\n");
    }
    
    // compare y to z
    if (compareBigInts(&y,&z)==-1){
        printf("y < z\n");
    } else if (compareBigInts(&y,&z)==1) {
        printf("y > z\n");
    } else {
        printf("y == z\n");
    }
    
    // compare z to x
    if (compareBigInts(&z,&x)==-1){
        printf("z < x\n");
    } else if (compareBigInts(&z,&x)==1) {
        printf("z > x\n");
    } else {
        printf("z == x\n");
    }
    
    // test the number of bits function...
    printf(">>> Testing numBitsInBigInt function...\n");
    printf("bits in x: %u\n", numBitsInBigInt(&x) );
    printf("bits in y: %u\n", numBitsInBigInt(&y) );
    printf("bits in z: %u\n", numBitsInBigInt(&z) );
    
    // test the bigint from int function...
    unsigned int xhex, yhex, zhex;
    printf("Please enter 3 hexadecimal integers (no more than 8 digits each!):\n");
    scanf("%8X %8X %8X", &xhex, &yhex, &zhex);
    // convert to bigints...
    setBigInt_FromInteger(&x, xhex);
    setBigInt_FromInteger(&y, yhex);
    setBigInt_FromInteger(&z, zhex);
    // convert to strings for output
    hexString_FromBigInt(&x, xOutString);
    hexString_FromBigInt(&y, yOutString);   
    hexString_FromBigInt(&z, zOutString);
    // print them back out...
    printf("Here are your numbers after being converted to bigint and then to strings:");
    printf("%s  %s  %s\n", xOutString, yOutString, zOutString);
}



















///////////////////////////////////////////////////////////////////////
////////////////////////////////////
////////////////


