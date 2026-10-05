/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
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
Function description: Removes all non-letter characters from the given string.

Parameters:
- string s: The original input string, which may contain letters and non-letter characters.

Returns:
- string: A new string that contains only the letters from string s in their original order.

Errors/special cases:
- If s contains no letters, the function returns an empty string.
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
Function description: Returns the first character of the given string as an uppercase letter.

Parameters:
- string s: A string containing only alphabetical characters.

Returns:
- string: The first character of s converted to uppercase.

Preconditions/assumptions:
- s contains at least one character.
*/
string keepFirst(string s) {
    return string(1, toupper(s[0]));
}

/*
Function description: Converts each alphabetical character in the input string into its matching Soundex digit.

Parameters:
- string s: A string containing only alphabetical characters.

Returns:
- string: A string containing the matching Soundex digit for each character in s.

Preconditions/assumptions:
- s contains only alphabetical characters.
*/
string encodeLetters(string s) {
    string result = "";

    for (char &c : s) {
        c = toupper(c);
    }

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
Function description: Removes consecutive duplicate digits from a Soundex code.

Parameters:
- string s: A string that contains Soundex digits.

Returns:
- string: A new string where each adjacent group of the same digits are reduced to one digit.
*/

string removeDuplicates(string s) {

    string result = s;

    int i = 0;

    while (i < result.length() - 1) {

        if (result[i] == result[i + 1]) {
            result.erase(i, 1);
        }

        else {
            i++;
        }
    }

    return result;
}

/*
Function description: Removes all zero characters from a Soundex code.

Parameters:
- string s: A string that contains Soundex digits.

Returns:
- string: A new string containing all nonzero characters from s in their original order.

Errors/special cases:
- If s contains only zeros, the function returns an empty string.
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
Function description: Adjusts a Soundex code to exactly four characters by
padding shorter codes with zeros and truncating longer codes.

Parameters:
- string s: The Soundex code to adjust.

Returns:
- string: A four-character Soundex code.
*/

string addZeros(string s) {

    string result = s;

    if (result.length() < 4) {
        while (result.length() != 4) {
            result += '0';
        }
    }

    else if (result.length() > 4) {
        result = result.substr(0, 4);
    }

    return result;
}

/*
Function description: Computes the Soundex code for a given surname.

Parameters:
- string s: The surname to convert into a Soundex code.

Returns:
- string: The Soundex code corresponding to s.

Preconditions/assumptions:
- s contains at least one alphabetical character.
*/

string soundex(string s) {

    string letters = lettersOnly(s);

    string firstLetter = keepFirst(letters);

    string encoded = encodeLetters(letters);

    string withoutDuplicates = removeDuplicates(encoded);

    // Replace the first encoded digit with the original first letter.

    string withFirstLetter = firstLetter + withoutDuplicates.substr(1, withoutDuplicates.length());

    string withoutZeros = discardZeros(withFirstLetter);

    string finalCode = addZeros(withoutZeros);

    return finalCode;
}

/*
Function description: Repeatedly searches a surname database for names that have the same Soundex code as a surname entered by the user.

Parameters:
- string filepath: The path to the file with the surname database.

Returns:
- void: This function does not return a value.

Errors/special cases:
- The search ends when the user presses Return without entering a surname.
- Matching surnames are printed in sorted order.
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

//Student Test Cases

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
