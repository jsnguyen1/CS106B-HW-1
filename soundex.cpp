/*
soundex.cpp
Names: Justin Nguyen, Suhurrith Adhikari

Course: CS 106B

Description: This file implements the Soundex algorithm for encoding surnames
according to their pronunciation. The program contains the main Soundex
function, several helper functions used during the encoding process, and a
search function that finds surnames with matching Soundex codes in the given
Stanford surname database. The file also includes a console program for
searching names and student tests to ensure functionality of the individual
helpers and overall algorithm.
 */

#include "SimpleTest.h"
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include <cctype>
#include <fstream>
#include <string>
using namespace std;

/*
 * Removes all non-letter characters from s while keeping the remaining letters
 * in their original order. The parameter s is a string that may contain letters
 * and non-letter characters, and the function returns a string containing only
 * the letters from s. If s contains no letters, the function returns an empty
 * string.
 */
string lettersOnly(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/*
 * Returns the first character of s as an uppercase letter. The parameter s is
 * a string containing alphabetical characters, and the function returns a
 * string containing the uppercase version of the first character. The function
 * assumes s contains at least one character.
 */
string keepFirst(string s) { return string(1, toupper(s[0])); }

/*
 * Converts each alphabetical character in s into its matching Soundex digit.
 * The parameter s is a string containing only alphabetical characters, and the
 * function returns a string containing the corresponding Soundex digit for each
 * character. The function assumes s contains only alphabetical characters.
 */
string encodeLetters(string s) {
    string result = "";

    s = toUpperCase(s);

    for (char ch : s) {

        if (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U' ||
            ch == 'H' || ch == 'W' || ch == 'Y') {
            result += "0";
        } else if (ch == 'B' || ch == 'F' || ch == 'P' || ch == 'V') {
            result += "1";
        } else if (ch == 'C' || ch == 'G' || ch == 'J' || ch == 'K' || ch == 'Q' ||
                   ch == 'S' || ch == 'X' || ch == 'Z') {
            result += "2";
        } else if (ch == 'D' || ch == 'T') {
            result += "3";
        } else if (ch == 'L') {
            result += "4";
        } else if (ch == 'M' || ch == 'N') {
            result += '5';
        } else if (ch == 'R') {
            result += '6';
        }
    }

    return result;
}

/*
 * Removes consecutive duplicate digits from a Soundex code. The parameter s is
 * a string containing Soundex digits, and the function returns a string where
 * each adjacent group of matching digits is reduced to one digit. If s is empty
 * or contains only one character, the function returns s unchanged.
 */
string removeDuplicates(string s) {
    string result = s;
    int i = 0;

    while (i + 1 < result.length()) {
        if (result[i] == result[i + 1]) {
            result.erase(i, 1);
        } else {
            i++;
        }
    }

    return result;
}

/*
 * Removes all zero characters from a Soundex code while preserving the order
 * of the remaining characters. The parameter s is a string containing Soundex
 * digits, and the function returns a string containing only the nonzero
 * characters from s. If s contains only zeros, the function returns an empty
 * string.
 */
string discardZeros(string s) {
    string result = "";
    for (char ch : s) {
        if (ch != '0') {
            result += ch;
        }
    }
    return result;
}

/*
 * Adjusts a Soundex code to exactly four characters. The parameter s is a
 * string containing a Soundex code, and the function returns a four-character
 * string. Codes shorter than four characters are padded with zeros, while
 * codes longer than four characters are truncated.
 */
string addZeros(string s) {
    string result = s;

    if (result.length() < 4) {
        while (result.length() != 4) {
            result += '0';
        }
    } else if (result.length() > 4) {
        result = result.substr(0, 4);
    }

    return result;
}

/*
 * Computes the Soundex code for a given surname. The parameter s is a string
 * representing the surname to encode, and the function returns the
 * corresponding four-character Soundex code. The function assumes s contains
 * at least one alphabetical character.
 */
string soundex(string s) {
    string letters = lettersOnly(s);
    string firstLetter = keepFirst(letters);
    string encoded = encodeLetters(letters);
    string withoutDuplicates = removeDuplicates(encoded);

    // Replace the first encoded digit with the original first letter.
    string withFirstLetter =
        firstLetter + withoutDuplicates.substr(1, withoutDuplicates.length());

    string withoutZeros = discardZeros(withFirstLetter);
    string finalCode = addZeros(withoutZeros);

    return finalCode;
}

/*
 * Repeatedly searches a surname database for names with the same Soundex code
 * as a surname entered by the user. The parameter filepath is a string
 * representing the path to the surname database file, and the function returns
 * void because it does not return a value. The search ends when the user
 * presses Return without entering a surname, and matching surnames are printed
 * in sorted order.
 */
void soundexSearch(string filepath) {
    ifstream in;
    Vector<string> allNames;
    Vector<string> matchingSoundex;

    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }

    cout << "Read file " << filepath << ", " << allNames.size() << " names found."
         << endl;

    string specificName = getLine("Enter a surname (RETURN to quit): ");

    while (specificName != "") {
        string specificSoundex = soundex(specificName);

        cout << "Soundex code is " << specificSoundex << endl;

        // Collect all surnames whose Soundex code matches the user's surname.
        for (int i = 0; i < allNames.size(); i++) {
            string currentSoundex = soundex(allNames[i]);

            if (specificSoundex == currentSoundex) {
                matchingSoundex.add(allNames[i]);
            }
        }

        matchingSoundex.sort();
        cout << "Matches from database: " << matchingSoundex << endl;

        matchingSoundex.clear();

        cout << endl;
        specificName = getLine("Enter a surname (RETURN to quit): ");
    }

    cout << "All done! " << endl;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);

    EXPECT_EQUAL(result, "OHara");

    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");

    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");

    s = "5Planet";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// Student Test Cases

STUDENT_TEST("lettersOnly removes all non-letter characters") {
    EXPECT_EQUAL(lettersOnly("123-Mc'Donald!"), "McDonald");
}

STUDENT_TEST("keepFirst capitalizes the first letter") {
    EXPECT_EQUAL(keepFirst("nguyen"), "N");
}

STUDENT_TEST("encodeLetters handles several different Soundex groups") {
    EXPECT_EQUAL(encodeLetters("BCLMR"), "12456");
}

STUDENT_TEST("removeDuplicates collapses repeated adjacent digits") {
    EXPECT_EQUAL(removeDuplicates("1112233005"), "12305");
}

STUDENT_TEST("removeDuplicates handles a one-character string") {
    EXPECT_EQUAL(removeDuplicates("1"), "1");
}

STUDENT_TEST("removeDuplicates handles an empty string") {
    EXPECT_EQUAL(removeDuplicates(""), "");
}

STUDENT_TEST("discardZeros removes zeros but preserves other digits") {
    EXPECT_EQUAL(discardZeros("102030405"), "12345");
}

STUDENT_TEST("addZeros pads a short code to length four") {
    EXPECT_EQUAL(addZeros("S2"), "S200");
}

STUDENT_TEST("addZeros truncates a long code to length four") {
    EXPECT_EQUAL(addZeros("S23456"), "S234");
}

STUDENT_TEST("soundex is case insensitive") {
    EXPECT_EQUAL(soundex("nGuYeN"), "N250");
}

STUDENT_TEST("soundex ignores punctuation and digits") {
    EXPECT_EQUAL(soundex("Ng-uy3en!"), "N250");
}

STUDENT_TEST("soundex ignores spaces") {
    EXPECT_EQUAL(soundex("Su hur rith"), "S630");
}

STUDENT_TEST("soundex pads short codes") {
    EXPECT_EQUAL(soundex("Ng"), "N200");
}

STUDENT_TEST("soundex handles repeated encoded digits") {
    EXPECT_EQUAL(soundex("Nguyen"), "N250");
}

STUDENT_TEST("soundex handles a normal mixed-code name") {
    EXPECT_EQUAL(soundex("Justin"), "J235");
}
