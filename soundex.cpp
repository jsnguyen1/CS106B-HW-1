/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* This function is intended to return a string which
 * includes only the letter characters from the original
 * (all non-letter characters are excluded)
 *
 * WARNING: The provided code is buggy!
 *
 * Use test cases to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
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

string keepFirst(string s){
    return string(1, toupper(s[0]));
}

string encodeLetters(string s) {
    string result = "";

    for (char &c : s) {
        c = toupper(c);
    }

    for (char ch : s) {

        if(ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U' || ch == 'H' || ch == 'W' || ch == 'Y'){
            result += "0";
        }
        else if(ch == 'B' || ch == 'F' || ch == 'P' || ch == 'V'){
            result += "1";
        }

        else if(ch == 'C' || ch == 'G' || ch == 'J' || ch == 'K' || ch == 'Q' || ch == 'S' || ch == 'X' || ch == 'Z'){
            result += "2";
        }

        else if(ch == 'D' || ch == 'T'){
            result += "3";
        }

        else if(ch == 'L'){
            result += "4";
        }

        else if(ch == 'M' || ch == 'N'){
            result += '5';
        }

        else if(ch == 'R'){
            result += '6';
        }
    }

    return result;
}

string removeDuplicates(string s){
    //22205

    string result = s;

    int i = 0;

    while(i < result.length() - 1){

        if(result[i] == result[i+1]){
            result.erase(i,1);
        }

        else{
            i++;
        }
    }

    return result;
}

string discardZeros(string s){
    string result = "";
    for(char ch : s){
        if (ch != '0'){
            result += ch;
        }
    }
    return result;
}

string addZeros(string s){

    string result = s;

    if(result.length() < 4){
        while(result.length() != 4){
            result += '0';
        }
    }

    else if(result.length() > 4){
        result = result.substr(0,4);
    }

    return result;
}



/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    /* TODO: Fill in this function. */
    string keepFirstLetter = keepFirst(s);

    string letters = lettersOnly(s);

    string encode = encodeLetters(letters);

    string duplicates = removeDuplicates(encode);

    string total = keepFirstLetter + duplicates.substr(1,duplicates.length());

    string discard = discardZeros(total);

    string add = addZeros(discard);

    return add;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // This provided code opens the specified file
    // and reads the lines into a vector of strings
    ifstream in;
    Vector<string> allNames;

    Vector<string> matchingSoundex;


    if (openFile(in, filepath)) {
        allNames = readLines(in);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;

    // The names read from file are now stored in Vector allNames

    /* TODO: Fill in the remainder of this function. */


    string userChoice = "Y";

    while(userChoice == "Y"){

        string specificName = getLine("Enter a surname (RETURN to quit):");

        cout << "Soundex code is " << soundex(specificName) << endl;

        /*for(int i = 0; i < allNames.size(); i++){

            string currentSound = soundex(allNames[i]);

            if(currentSound == specificName){
                matchingSoundex.add("Soundex of " + allNames[i] + ": " + soundex(allNames[i]));
            }
        } */

        cout << soundex(allNames[2]) << endl;

        matchingSoundex.sort();

        cout << "Matches from database: " << matchingSoundex << endl;

        userChoice = getLine("Would you like to enter another surname? Enter Y for Yes or N for No");

    }

    cout << "All done!";

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
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}


// TODO: add your test cases here
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




