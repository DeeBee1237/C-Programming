#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2014-15.
   Uses a modified caeser I & II (in 2015)

   Compile:  gcc -o decode decode.c

   See encode for  examples on using it
 */

char upcase(char ch){
    if(islower(ch))
        ch -= 'a' - 'A';
    return ch;
}


int in(char c, char* s, int pos){
    // Assume everything is already in the same case
    int i;

    for(i = 0; i < pos; i++)
        if(c == s[i]) return 1;

    return 0;
}


// My function: Takes in a string, and replaces all of the duplicate letters with the char '!':
// to be used for the Caeser Cypher:
char* removeDuplicates (char* string) {

    // create a char array from the string that is passed in:
    int length = strlen(string);
    char arr [length + 1];

    for (int i = 0; i < length; i ++) {
        arr[i] = *(string+i);
    }

    int i;
    // now iterate over the char array, and replace all duplicate letters with a '!':
    for (i = 0; i< length; i++) {
        char currentLetter = *(arr+i);

        for (int j = i + 1;j < length; j++) {
            char nextLetter = *(arr+j);
            if (nextLetter == currentLetter)
                *(arr+j) = '!';
        }
    }

    arr[i] = '\0';
    char* result = arr;
    return result;
}


void buildtable (char* key, char* decode){ // this changed from encode

    // create the encode dictionary again ... :
    char* encode = (char*)(malloc(sizeof(char)*26)); //[26];

    int length = strlen(key);


    // some sort of bug in the fixKey method, I will sort that out later...
    // plain will be the new array of chars with the capatlized letters etc:
    int i, j;
    char plain[26];

    for(i = 0, j = 0; i < strlen(key); i++){
        if(isalpha(key[i])) {
            plain[j++] = upcase(key[i]);
        }
    }
    plain[j] = '\0';


    char* keyWithNoDuplicates = removeDuplicates(plain); //ICAME!S!W
    int newLength = strlen(keyWithNoDuplicates);

    char dictionary [26]; // the array of chars that contains the cypher

    // 1. fill up the dictionary with the key first:

    int keyCounter = 0;

    int dicPosition = length - 1; // where we are in the dictionary
    while (keyCounter < newLength) {

        if (*(keyWithNoDuplicates + keyCounter) != '!') {
            dictionary[dicPosition] = *(keyWithNoDuplicates + keyCounter);
            dicPosition++;
        }

        keyCounter++;
    }

    // 2. fill up the rest of the dictionary
    // that is, from the key to the end of the alphabet:

    char currentLetter;

    for (int i = dicPosition; i < 26; i++) {
        if (currentLetter == 'Z')
            currentLetter = 'A';
        else
            currentLetter = dictionary[i-1] + 1;

        // while the current Letter is already in the shortened key:
        while (strchr(keyWithNoDuplicates,currentLetter) != NULL) {
            currentLetter++;
        }

        dictionary[i] = currentLetter;
    }


    // 3. Now fill up the dictionary from the beginning to the the first letter of the key:
    for (int i = 0; i < length - 1; i ++) {
        if (currentLetter == 'Z')
            currentLetter = 'A';
        else
            currentLetter++;

        // while the current Letter is already in the shortened key:
        while (strchr(keyWithNoDuplicates,currentLetter) != NULL) {
            currentLetter++;
        }

        dictionary[i] = currentLetter;
    }

    // 4. Load up encode with the dictionary:
    for (int c = 0; c < 26; c++)
        encode[c] = dictionary[c];

    // Create the decode dictionary:

    char toInsert = 'A';
    for  (int i = 0; i < 26; i++) {
        char currentLetter = encode[i];
        int position = currentLetter - 'A';

        decode[position] = toInsert;
        toInsert++;
    }

    decode[26] = '\0';

}


    int main (void) {

        char* decode = (char*)malloc(sizeof(char)*26);


        char key [80];
        char message [80];


        printf("%s","Enter your key: ");
        scanf("%[^\n]",key);


        buildtable(key,decode);

        printf("%s", "This is the resulting decoding dictionary: ");
        printf("%s",decode);

        getchar();

        printf("%s","\n");
        printf("%s","Enter the message you want to decode: ");
        scanf("%[^\n]",message);

        printf("%s","Your decoded message is: ");

        for (int c = 0; c < strlen(message); c++) {
            char letter = message[c];

            if (isalpha(letter)) {
                if (islower(letter))
                    printf("%c", decode[letter - 'a']);
                else
                    printf("%c", decode[letter - 'A']);
            }

            else
                printf("%c",message[c]);


        }


    }



