/******************************************************************************
 *                           Cipher-O-Matic 1.0                               *
 *                   Copyright © 2014 Ben Goldsworthy (rumps)                 *
 *                                                                            *
 * A program to encipher text with a maximum line length of 80 characters in  *
 * one of three ways: a Caesar shift of ±n spaces; a reverse cipher; and a    *
 * Vigenère cipher.                                                           *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation, either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.      *
 ******************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TITLEWIDTH 80

#define CAESARSHIFT1 0
#define CAESARSHIFTN 1
#define REVERSE 2
#define VIGENERE 3

#define LINELENGTH 80

char* CaesarShiftN(int n, char* lineToEncode, char* encodedLine);
char* Reverse(char* lineToEncode, char* encodedLine);
char* Vigenere(char *lineToEncode, char *encodedLine, char *keyWord);
void PrintRules(void);

int main() {
  FILE *fileToEncode;
  int currChar, optionEntered = 0, n, lines, newLineChar;
  bool validEntry;
  char lineToEncode[80], encodedLine[80] = {0}, keyWord[5], showEntered;
  
  // Prints the title of the program
  printf("\n");
  for (int i = 0; i <= TITLEWIDTH; i++)  printf("*");
  printf("**\n*\t\t\t\tSCC.110 Cipher-O-Matic\t\t\t\t\t*"
         "\n*\t\t\tCopyright © 2014 Ben Goldsworthy (rumps)\t\t\t*"
         "\n*\t\t\t\t\t\t\t\t\t\t\t*"
         "\n*\tThis program comes with ABSOLUTELY NO WARRANTY; for details type" 
         "'show w'.\t*"
         "\n*\tThis is free software, and you are welcome to redistribute it\t"
         "\t\t*\n*\tunder certain conditions; type 'show c' for details."
         "\t\t\t\t*\n");
  for (int i = 0; i <= TITLEWIDTH; i++)  printf("*");
  printf("**\n");
   
  // The default state, where the user is asked for input
  // goto might be frowned upon, but it has its uses
initialState:
  printf("\nType 'show o' to view cipher options, anything else to quit\n> ");
  showEntered = 0;
  scanf(" show %c", &showEntered);
  switch(showEntered) {
  case 'w':
    printf("\nTHERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY "
           "APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE "
           "COPYRIGHT HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM “AS IS” "
           "WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, "
           "INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF "
           "MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE ENTIRE "
           "RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. "
           "SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL "
           "NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
    goto initialState;
  case 'c':
    printf("\nThis program is free software: you can redistribute it and/or "
           "modify it under the terms of the GNU General Public License as "
           "published by the Free Software Foundation, either version 3 of the "
           "License, or (at your option) any later version.\n\nYou should have "
           "received a copy of the GNU General Public License along with this "
           "program.  If not, see <http://www.gnu.org/licenses/>.\n");
    goto initialState;
  case 'o':      
    PrintRules();
    scanf("%d", &optionEntered);
  
    fileToEncode = fopen("entry.txt", "r");
    if (fileToEncode != NULL) {
      do {
        switch(optionEntered) {
        // A results of Caesar shift +1 can be allowed to run into the code for
        // Caesar shift +n, so no break; is needed
        case CAESARSHIFT1:
          n = 1;
        case CAESARSHIFTN:
          validEntry = true;
          
          if (optionEntered == CAESARSHIFTN) {
            printf("Enter a value for n\n> ");
            scanf(" %d", &n);
          }
            
          printf("\n");
          while(fgets(lineToEncode, LINELENGTH, fileToEncode) != NULL) {
            memset(encodedLine, 0, sizeof(encodedLine));
            printf("%s", CaesarShiftN(n, lineToEncode, encodedLine));
          }

          break;
        case REVERSE:  
          validEntry = true;
               
          while(fgets(lineToEncode, LINELENGTH, fileToEncode) != NULL) {
            memset(encodedLine, 0, sizeof(encodedLine));
            printf("%s", Reverse(lineToEncode, encodedLine));
          }  
          printf("\n");
          break;
        case VIGENERE:
          validEntry = true;

          printf("Enter a five-character lowercase keyword\n> ");
          scanf(" %c%c%c%c%c", &keyWord[0], &keyWord[1], &keyWord[2], 
                               &keyWord[3], &keyWord[4]);
             
          printf("\n");
          while(fgets(lineToEncode, LINELENGTH, fileToEncode) != NULL) {
            memset(encodedLine, 0, sizeof(encodedLine));
            printf("%s", Vigenere(lineToEncode, encodedLine, keyWord));
          }
       
          break;
        default:
          printf("\nInvalid entry, try again.\n");
          goto initialState;
        }
      } while (!validEntry);
      fclose(fileToEncode);
    } else {
      if (fileToEncode == NULL) perror("Error opening file");
    }
    
    goto initialState;
  default:
    return 0;
  }
}

// This sub copies the line to encode into the encoded line, then runs through 
// char-by-char, incrementing the value in the encoded line n times, and 
// wrapping back if 'z' is passed; a value of -n can also be used
char* CaesarShiftN(int n, char *lineToEncode, char *encodedLine) {
  strcpy(encodedLine, lineToEncode);

  for (int currChar = 0; currChar < 80; currChar++) {
    if (lineToEncode[currChar] >= 97 && lineToEncode[currChar] <= 122) {
      if (n > 0) {
        for (int i = n;i > 0;i--) {
          encodedLine[currChar]++;
          if (encodedLine[currChar] == 123) encodedLine[currChar] = 97;
        }
      } else {
        for (int i = n;i < 0;i++) {
          encodedLine[currChar]--;
          if (encodedLine[currChar] == 96) encodedLine[currChar] = 122;
        }
      }
    }
  }
   
  return encodedLine;
}

// This sub gets the length of the line to encode, then starts from that char
// (-1 to avoid starting the encoded line off with a line break) and works
// its way backwards, placing the chars into the encoded line in the reverse
// order
char* Reverse(char *lineToEncode, char *encodedLine) {
  int length = 0, currRevChar = 0;
  length = strlen(lineToEncode);

  for (int currChar = --length; currChar >= 0; currChar--, currRevChar++) {
    encodedLine[currRevChar] = lineToEncode[currChar];
  }
  
  return encodedLine;
}

// This sub encodes any values between 'a' and 'z' by shifting them along 
// by the number of shifts on the keyword-specified row of the tabula recta; 
// the row is determined by subtracting 98 from the keyword character, which
// makes 'a' = 0, 'b' = 1, etc.
char* Vigenere(char *lineToEncode, char *encodedLine, char *keyWord) {
  int currChar = 0, currKeyWordChar = 0, tabulaRecta = 0;

  for (currChar = 0; currChar < 80; currChar++, currKeyWordChar++) {   
    if (currKeyWordChar == 5) currKeyWordChar = 0;
     
    encodedLine[currChar] = lineToEncode[currChar]; 
      
    tabulaRecta = keyWord[currKeyWordChar] - 98;
      
    if (lineToEncode[currChar] >= 97 && lineToEncode[currChar] <= 122) {
      for (int i = 0;i <= tabulaRecta;i++) {
        encodedLine[currChar]++;
        if (encodedLine[currChar] == 123) encodedLine[currChar] = 97;
      }
    }
  }

  return encodedLine;
}

// This sub prints the commands used for interacting with the program
void PrintRules(void) {
  printf("\nChoose cipher\n-------------\n");
  printf("0\tCaesar cipher shift 1\n1\tCaesar cipher shift n\n2\tReverse\n3"
         "\tVigenère cipher\n\n> ");
}
