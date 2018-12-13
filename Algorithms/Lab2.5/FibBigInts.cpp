///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////
/**************************************************************************************************
* KURTIS WALDNER
* ALGORITHMS AND OPTIMIZATION
* CSC-482
* LAB 2.5
* Fibonacci  with BigInts
* COMAND TO RUN: g++ -std=c++11 FibBigInts.cpp -o FibBigInts
***************************************************************************************************/
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <ratio>
#include <chrono>
#include <string.h>

//BIG INT IMPLEMENTATION FUNCTIONS/////////////////////////////////////////////////////////////////
#define MaxBigIntBytes 1048576 // this is 2^20 or "1 Mega-" as in "Megabyte"
#define RULL unsigned long long int 
#define ULL BigInt 
#define MaxIOCharArraySize 5000

typedef struct BigInt {

    unsigned int  numDigits; // number of base-256 digits used in the array (the rest should be "leading zeros") so really the number of bytes
    unsigned char digits_b256[MaxBigIntBytes]; // store our base-256 digits in this array
                                               // fixed array size sets a limit on the size of integer (hopefully bigger than we need)

} BigInt;

// Always do this on a new BigInt before using it in any other function!!
void clearBigInt(BigInt *x) 
{
    x->numDigits = 1; // there's always at least charOne digit: 0 in the charOnes column
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

// because we are using a fixed size struct, we can get away without using pointers here -- the struct will be fully copied both
// when passed as parameter, or when returned. In this way the bigint function can work much more like regulat arithemetic expressions
// Note: this does not affect the big-, and addition is already O(n) and the copying steps only take O(n)
BigInt addBigInts(BigInt x, BigInt y)
{
    // note: my bigints have padded zeros, so no need to swap x & y based on size
    // be sure to zero out sum...
    BigInt sum;
    clearBigInt(&sum);
    RULL maxDigit = x.numDigits > y.numDigits ? x.numDigits : y.numDigits;
    sum.numDigits = maxDigit;
    RULL carry = 0;
    for(RULL digitIndex=0; digitIndex < maxDigit; digitIndex++)
    {
        RULL tmp = x.digits_b256[digitIndex] + y.digits_b256[digitIndex] + carry;
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
    RULL xDigit, yDigit;
    // zero out product
    BigInt product;
    clearBigInt(&product);

    for(xDigit=0; xDigit < x.numDigits ; xDigit++)
    {
        RULL carry = 0;
        BigInt lineProduct; // this will store the result of charOne digit of x times all of y
        clearBigInt(&lineProduct); // zero it out first
        for(yDigit=0; yDigit < y.numDigits ; yDigit++)
        {
            RULL digitProduct = x.digits_b256[xDigit] * y.digits_b256[yDigit] + carry; // product of two single digits, + carry
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

//BIG INT IMPLEMENTATION FUNCTIONS/////////////////////////////////////////////////////////////////


// functions we want to test...
/**********************************************************************************************************************

    fibLoop
/
***********************************************************************************************************************/
ULL BigIntOne, BigIntZero;
char charOne[2] = {'1','\0'};
char charZero[2] = {'0','\0'};
ULL x,y,z,w;
ULL mat[2][2], F[2][2];
char xInString[MaxIOCharArraySize+1], xOutString[MaxIOCharArraySize], yInString[MaxIOCharArraySize], zInString[MaxIOCharArraySize];
 char yOutString[MaxIOCharArraySize], zOutString[MaxIOCharArraySize], wOutString[MaxIOCharArraySize];

BigInt fibLoop(BigInt x)
{
        ULL i,a,b,result;
        setBigInt_FromHexString(charZero, &i);
        setBigInt_FromHexString(charOne, &a);
        setBigInt_FromHexString(charZero, &b);
        setBigInt_FromHexString(charOne, &BigIntOne);

        while(compareBigInts(&x , &i)) 
        {
            result = addBigInts(a , b);
            b =  a;
            a = result;
            i = addBigInts(i ,BigIntOne );
        }
    return b ;

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

void powerMatrix(BigInt F[2][2], BigInt n);
BigInt fibMatrix(BigInt n)
{

  F[0][0] = BigIntOne;
  F[0][1] = BigIntOne;
  F[1][0] = BigIntOne;
  F[1][1] = BigIntZero;

  if (compareBigInts(&n , &BigIntOne)==0)
      return BigIntZero;
    BigInt i  = BigIntZero;
    while(1)
    {
        BigInt tmp = addBigInts(i , BigIntOne);
        if (compareBigInts(&tmp , &n)==0)
            break;
        i  = addBigInts(i , BigIntOne);

    }
  powerMatrix(F, i);

  return F[0][0];
}

void multiplyMatrix(BigInt MatrixcharOne[2][2], BigInt M[2][2])
{
   x =  addBigInts(multiplyBigInts(MatrixcharOne[0][0],M[0][0]) , multiplyBigInts( MatrixcharOne[0][1],M[1][0]));
   y =  addBigInts(multiplyBigInts(MatrixcharOne[0][0],M[0][1]) , multiplyBigInts(MatrixcharOne[0][1],M[1][1]));
   z =  addBigInts(multiplyBigInts(MatrixcharOne[1][0],M[0][0]) , multiplyBigInts(MatrixcharOne[1][1],M[1][0]));
   w =  addBigInts(multiplyBigInts(MatrixcharOne[1][0],M[0][1]) , multiplyBigInts(MatrixcharOne[1][1],M[1][1]));

  MatrixcharOne[0][0] = x;
  MatrixcharOne[0][1] = y;
  MatrixcharOne[1][0] = z;
  MatrixcharOne[1][1] = w;
}

void powerMatrix(BigInt MatrixcharOne[2][2], BigInt n)
{

  BigInt i;

  mat[0][0] = BigIntOne;
  mat[0][1] = BigIntOne;
  mat[1][0] = BigIntOne;
  mat[1][1] = BigIntZero;

  // n - 1 times multiply the matrix to {{1,0},{0,1}}
  i = BigIntOne;
  i = addBigInts(i, BigIntOne);
  for (; compareBigInts(&i,&n) < 1; i = addBigInts(i  , BigIntOne))
      multiplyMatrix(MatrixcharOne, mat);
}


int main (int argc, char* argv[]) 
{
    char Val;
    BigInt x;
    setBigInt_FromHexString(charOne, &BigIntOne);
    setBigInt_FromHexString(charZero, &BigIntZero);
    time_t start, stop;
    double total_time1, total_time2;
    FILE * fp;
    printf("#Matrix Multipliation: \t\t\t\t #FibLoop:\n\n");

        for(int r = 100; r <= 200; r=r+25 )
        {
            sprintf(xInString, "%x", r+1 );
            setBigInt_FromHexString(xInString, &x);

            start = clock();
            x= fibMatrix(x);
            stop = clock();

            total_time1 += (stop-start)/(double)CLOCKS_PER_SEC;

            hexString_FromBigInt(&x, xOutString);

            printf("%i\t%s\t%f",r, xOutString, total_time1);
            
            
            //start fib loop test function
            sprintf(xInString, "%x", r+1 );
            setBigInt_FromHexString(xInString, &x);

            start = clock();
            x= fibLoop(x);
            stop = clock();

            total_time2 += (stop-start)/(double)CLOCKS_PER_SEC;

            hexString_FromBigInt(&x, xOutString);

            printf("\t\t\t%s\t%f\n", xOutString, (total_time2));
           
        }
        
       return 0;
}


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////

