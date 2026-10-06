/*
 * perfect.cpp
 * Names: Justin Nguyen, Suhurrith Adhikari
 * Course: CS 106B
 *
 * Description: This file explores multiple approaches for finding perfect
 * numbers. It begins with a brute-force method that checks every possible
 * divisor, then uses more efficient techniques to reduce the amount of
 * computation. The final approach uses Euclid's method and Mersenne primes to
 * generate perfect numbers much more efficiently.
 *
 * Something interesting: Small changes to an algorithm can have a huge effect
 * on the run time. The original approach checked every possible divisor, which
 * becomes very slow as the input grows. By avoiding unnecessary checks and
 * using mathematical properties of perfect numbers (Euclid), we were able to
 * search much larger ranges in much less time.
 *
 */
#include "SimpleTest.h"
#include "console.h"
#include <cmath>
#include <iostream>
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
bool isPerfect(long n) { return (n != 0) && (n == divisorSum(n)); }

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush; // progress bar
        }
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*
 * Calculates the sum of the proper divisors of n by checking only divisors up
 * to the square root of n and adding each matching divisor pair. The parameter
 * n is a long representing the number whose proper divisors are summed, and
 * the function returns a long representing the sum of all proper divisors of
 * n. The function assumes n is nonnegative. If n is a perfect
 * square, its square root is added only once, and if n is 1, there are no
 * proper divisors and the function returns 0.
 */
long smarterSum(long n) {
    long total = 0;

    if (n == 1) {
        return total;
    }

    for (long divisor = 1; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            total += divisor;

            // Add the paired divisor, avoiding n itself and double-counting a square
            // root.
            if (divisor != 1 && divisor != sqrt(n)) {
                total += n / divisor;
            }
        }
    }

    return total;
}

/*
 * Determines whether a number is perfect using smarterSum. The parameter n is
 * a long representing the number to test, and the function returns a bool that
 * is true if n is a perfect number and false otherwise. The function assumes n
 * is nonnegative. As a special case, 0 is not considered a
 * perfect number.
 */
bool isPerfectSmarter(long n) { return (n != 0) && (n == smarterSum(n)); }

/*
 * Searches for all perfect numbers below stop using isPerfectSmarter and
 * prints each perfect number found. The parameter stop is a long representing
 * the upper bound of the range to search, and the function returns void because
 * it does not return a value. The function assumes stop is a positive integer.
 * If there are no perfect numbers below stop, no perfect numbers are printed.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found smarter perfect number: " << num << endl;
        }
        if (num % 10000 == 0) {
            cout << "." << flush; // progress bar
        }
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/*
 * Finds the nth perfect number using the Euclid formula and Mersenne numbers of
 * the form 2^k - 1. The parameter n is a long representing the position of the
 * perfect number to find, and the function returns a long representing the nth
 * perfect number. The function assumes n is a positive integer and therefore
 * does not handle values of n that are 0 or negative.
 */
long findNthPerfectEuclid(long n) {
    long count = 0;
    long k = 1;

    while (count < n) {
        long m = (pow(2, k)) - 1;

        // A number with divisor sum 1 is prime.
        if (divisorSum(m) == 1) {
            count++;
        }

        if (count < n) {
            k++;
        }
    }

    // Use Euclid's formula to construct the perfect number from k.
    return (pow(2, k - 1)) * (pow(2, k) - 1);
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

PROVIDED_TEST("Confirm 33550336 is perfect") { EXPECT(isPerfect(33550336)); }

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

/*STUDENT_TEST("Create time trials") {

    TIME_OPERATION(62500, findPerfects(62500));
    TIME_OPERATION(125000, findPerfects(125000));
    TIME_OPERATION(250000, findPerfects(250000));
    TIME_OPERATION(500000, findPerfects(500000));
}*/

STUDENT_TEST("testing isPerfect(n) on negative numbers") {
    EXPECT(!isPerfect(-1));
    EXPECT(!isPerfect(-100));
    EXPECT(!isPerfect(-10000));
}

STUDENT_TEST("smarterSum matches divisorSum for a normal number") {
    EXPECT_EQUAL(smarterSum(6), divisorSum(6));
}

STUDENT_TEST("smarterSum handles a perfect square") {
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
}

STUDENT_TEST("smarterSum handles zero") {
    EXPECT_EQUAL(smarterSum(0), divisorSum(0));
}

STUDENT_TEST("smarterSum handles one") {
    EXPECT_EQUAL(smarterSum(1), divisorSum(1));
}

/*STUDENT_TEST(
    "Multiple time trials of findPerfectsSmarter on increasing input sizes") {
    TIME_OPERATION(1875000, findPerfectsSmarter(1875000));
    TIME_OPERATION(3750000, findPerfectsSmarter(3750000));
    TIME_OPERATION(7500000, findPerfectsSmarter(7500000));
    TIME_OPERATION(15000000, findPerfectsSmarter(15000000));
}*/

STUDENT_TEST("findNthPerfectEuclid finds the first perfect number") {
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
}

STUDENT_TEST("findNthPerfectEuclid finds the third perfect number") {
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
}

STUDENT_TEST("findNthPerfectEuclid finds the fifth perfect number") {
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}

STUDENT_TEST("findNthPerfectEuclid returns a perfect number") {
    EXPECT(isPerfect(findNthPerfectEuclid(3)));
}
