#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_SIZE 200000  // Note, the longer the text the more likely you will get a good 'decode' from the start.
#define ALEN 26         // Number of chars in ENGLISH alphabet
#define CHFREQ "ETAONRISHDLFCMUGYPWBVKJXQZ" // Characters in order of appearance in English documents.
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer, this extended version (c) 2015
   LAB: 2

   This program applies a basic frequency analysis on a cyphertext.  It has been extened over the 2014 version to
   solve polyalphabetic cyphers - by brute force.  In this case, it applies the frequency analysis for different 
   numbers of n keys (polyalphabetic Caeser).  Obviously it will need a cypher of about n times
   the typical length for a monoalphabetic cypher.

   Program is used like this:

   Compile:  gcc -o crack crack.c

   Test file (ctext): JWRLS, XSSH PZK JH HES BJFV, UZU (this is not a realistic length piece of cypher text)

   crack n

   Argument:

   n number of keys to try

   ---

   % cat ctext | crack 1
   ALICE, MEET YOU AT THE PARK, BOB   <-- of course it won't be this correct.  Don't worry about that for the -d option.
   AMFDE, UEET LNH AT TIE RASC, ONO   <-- this is what it really looks like, a larger sample is better, this is short.


 */


// my code starts here:
// returns 1 if all values in array are set to 0 and 0 if not all are 0
int allOnes (int* array) {
   for (int i = 0; i < 26; i ++) {
       if (array[i] != 0)
           return 0;
   }
    return 1;
}


// compute the frequency table of a String:
int* computeFrequencyTable (char* string) {

    int* result  = (int*) (calloc(sizeof(int),26));

    int i = 0;
    while (*(string + i) != '\0') {
        char currentLetter = *(string+i);
        int position = currentLetter - 'A';

        *(result+position) = *(result+position) + 1;
        i++;
    }

    return result;
}




// I will use this function for decoding one subtext only. So if I pass in n = 4
// the function will generate 4 separate subtexts and then decode each of them.
// I will use this function in a loop somewhere, to try i = 1 .. n keys.
void outPutDecodedSubtext (int n, char* encryptedText) {

        int columns = (strlen(encryptedText) + n - 1)/n; // number of subtexts essentially
        char subtext [n][columns]; // the 2d array of all the subtext strings
        char decodedSubtext [n][columns]; // the 2d array where I will put the decoded subtexts

        int row = 0;
        int col = 0;

        int index = 0; // iterate over the encryptedText

        // split the text into subtexts:
        while (*(encryptedText + index) != '\0') {
            char currentChar = *(encryptedText + index);

            // check for white space:
            if (isspace(currentChar) || !isalpha(currentChar)) {
                index++;
                continue;
            }

            // add the current letter at the appropriate 'split position' (ie row col in the 2d array)
            // so for three subtexts
            // first char goes in row 1, second in row 2 etc ..
            subtext[row][col] = currentChar;
            row++;
            index++;
            if (row == n) {
                col++;
                row = 0;
            }

        }


        // perform frequency analysis for those subtexts:
        for (int row = 0; row < n; row ++) {
            // get the current subtext from that row:
            char currentSubtext [columns];
            for (int col = 0; col < columns; col ++) {
               currentSubtext[col] = subtext[row][col];
            }

            // perform frequency analysis and generate a possibly decoded value:
            // one subtext:
            int translationIndex = 0; // starts with E, then moves down to letters with less frequency
            int counter = 0;
            char* decoded = (char*) calloc(sizeof(char),26);
            int *frequencyTable = computeFrequencyTable(currentSubtext);


            while (counter < strlen(currentSubtext)) {

                // stop looping if we have translated all the letters:
                if (allOnes(frequencyTable) == 1)
                    break;

                // find the letter from the current subtext with the highest frequency:
                int maxFrequencyPosition = 0;
                for (int i = 0; i < 26; i++) {
                    if (frequencyTable[i] > frequencyTable[maxFrequencyPosition]) {
                        maxFrequencyPosition = i;
                    }
                }

                decoded[maxFrequencyPosition] = CHFREQ[translationIndex];
                frequencyTable[maxFrequencyPosition] = 0; // reset the max frequency, so we don't see it again
                translationIndex++;
                counter++;
            }



            // load up the decodedSubtext 2d array with the appropriate row:
            for (int col = 0; col < columns; col ++) {
                char currentLetter = currentSubtext[col];
                decodedSubtext [row][col] =  decoded[currentLetter - 'A'];
            }

        }

    // now recombine the decoded subtexts to give a possibly decoded result:
    char possiblyDecodedResult [n*columns];
    int position = 0; // where to insert in the possibly decoded text

    for (int col = 0; col < columns; col++) {
        for (int row = 0; row < n; row++) {
            possiblyDecodedResult[position] = decodedSubtext[row][col];
            position++;
        }
    }


    // now print the possibly decoded result:
    int i = 0;
    int decodedPosition = 0; // index of decoded text/

    while (i < strlen(encryptedText)) {
        // if the current character is a letter, then print out the decoded result:
        if (isalpha(encryptedText[i])) {
            printf("%c",possiblyDecodedResult[decodedPosition]);
            decodedPosition++;
        }
        // otherwise, print whatever char is there, like ',' or ' ' for example"
        else {
            printf("%c",encryptedText[i]);
        }
        i++;
    }




}

// the code that will try i = 1 ... n keys:
// loop through i  = 1 ... n subtexts, and print the decoded set of subtexts for each i:
void loopAndDecode (int n, char* encryptedText) {

	
  for (int i = 1; i <= n; i++) {
     outPutDecodedSubtext(i,encryptedText);
     printf("%s","\n");
  }
}


// my code ends here^^^

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

int main(int argc, char **argv){

  // first allocate some space for our input text (we will read from stdin).

  char* text = (char*)malloc(sizeof(char)*TEXT_SIZE+1);
  char ch;
  int n, i;

  if(argc > 1 && (n = atoi(argv[1])) > 0); else{ fprintf(stderr,"Malformed argument, use: crack [n], n > 0\n"); exit(-1);} // get the command line argument n
  
  // Now read TEXT_SIZE or feof worth of characters (whichever is smaller) and convert to uppercase as we do it.
  // Added: changed to count frequencies as we read it in

  for(i = 0, ch = fgetc(stdin); i < TEXT_SIZE && !feof(stdin); i++, ch = fgetc(stdin)){
    text[i] = (ch = (isalpha(ch)?upcase(ch):ch));
  }
  text[i] = '\0'; // terminate the string properly.

  /* At this point we have two things,
   *   1. The input cyphertext in "text"
   *   2. The maximum number of keys to try (n) - we'll be trying 1..n keys.
   *
   * What you need to do is as follows:
   *   1. create a for-loop that will check key lengths from 1..n
   *   2. for each i <= n, spit the cypher text into i sub-texts.  For i = 1, 1 subtext, for i = 2, 2 subtexts, of alternating characters etc.
   *   3. for each subtext: 
   *          a. count the occurance of each letter 
   *          b. then map this onto the CHFREQ, to create a map between the sub-text and english
   *          c. apply the new map to the subtext 
   *   4. merge the subtexts
   *   5. output the 'possibly' partially decoded text to stdout.  This will only look OK if i was the correct number of keys
   *
   * what you need to output (sample will be provided) - exactly:
   * i maps -> stderr
   * i 'possible' translations
   *
   * You would be wise to make seperate functions that perform various sub-tasks, and test them incrementally.  Any other approach will likely
   * make your brain revolt.  This isn't a long program, mine is 160 lines, with comments (and written in a very verbose style) - if yours is
   * getting too long, double check you're on the right track.
   *
   */
  
  // Your code here...

  loopAndDecode(n,text);

}
