/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long smarterSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;
            if (divisor != 1 && divisor != sqrt(n)){
                total += n/divisor;
            }
        }
    }
    return total;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found smarter perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
long findNthPerfectEuclid(long n) {

    int count = 0;
    int k = 1;

    while(count <= n){
        int m = (pow(2,k)) - 1; //calculate mersenne number

        if(divisorSum(m) == 1){ // if mersenne number is prime, increment the count of perfect numbers.
            count++;
            if (count == n){ // if the count eventually equals the nth perfect number, return the value
                break;
            }
            k++;
        }
        else{ // if the mersenne number isnt prime, increment k to move on to the next power.
            k++;
        }
    }
    return (pow(2,k-1)) * (pow(2,k) - 1);
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}




// TODO: add your student test cases here


/*STUDENT_TEST("Create time trials") {

    TIME_OPERATION(62500, findPerfects(62500));
    TIME_OPERATION(125000, findPerfects(125000));
    TIME_OPERATION(250000, findPerfects(250000));
    TIME_OPERATION(500000, findPerfects(500000));

}*/

/*STUDENT_TEST("testing isPerfect(n) on negative numbers"){
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-100));
    EXPECT(!isPerfect(-10000));
}*/

/*STUDENT_TEST("testing smarterSum(n)"){
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
    EXPECT_EQUAL(smarterSum(-1), divisorSum(-1));
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
    EXPECT_EQUAL(smarterSum(0), divisorSum(0));
}*/

/*STUDENT_TEST("Multiple time trials of findPerfectsSmarter on increasing input sizes") {
    TIME_OPERATION(1875000, findPerfectsSmarter(1875000));
    TIME_OPERATION(3750000, findPerfectsSmarter(3750000));
    TIME_OPERATION(7500000, findPerfectsSmarter(7500000));
    TIME_OPERATION(15000000, findPerfectsSmarter(15000000));
}*/

STUDENT_TEST("Test findNthPerfectEuclid(n)"){

    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);

    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);

    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);

    EXPECT(isPerfect(findNthPerfectEuclid(3)));


}

/*
 * Below is a suggestion of how to use a loop to set the input sizes
 * for a sequence of time trials.
 *
 *
STUDENT_TEST("Multiple time trials of findPerfects on increasing input sizes") {

    int smallest = 1000, largest = 8000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}

*/
