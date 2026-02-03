#include <stdio.h>
#include <stdint.h>

int charInArr(char c, char *arr[], int len) {
  for(int i=0; i < len; i++) {
    if(*arr[i] == c) return 1;
  }
  return 0;
}

int getInstructionChars(char outPoint[], char filePath[]) {
  char text[128];
  FILE *filePtr;

  // Windows:
  // fopen_s(&filePtr, filePath, "r");
  // Other:
  filePtr = fopen(filePath, "r");

  int instructionIndex = 0;

  while(fgets(text, sizeof(text), filePtr) != NULL) {
    // Read loop
    
    int i = 0;
    while(1) {
      char *breakArr[] = {"\n", "#", "\0"};
      char *continueArr[] = {">", "<", "+", "-", ".", ",", "[", "]"};

      int breakLen = sizeof(breakArr)/sizeof(breakArr[0]);
      int continueLen = sizeof(continueArr)/sizeof(continueArr[0]);

      if(charInArr(text[i], breakArr, breakLen)) break;
      if(!charInArr(text[i], continueArr, continueLen)) { i++; continue; }

      outPoint[instructionIndex++] = text[i];
      
      i++;
    }
  }

  return instructionIndex;
}

int main() {
  printf("Starting\n\n");

  printf("Parse Tokens\n");
  char instructionChars[128];
  int size = getInstructionChars(instructionChars, "test.txt");

  printf("Readable:\n");
  for(int i=0; i < size; i++) {
    printf("\"%c\", ", instructionChars[i]);
  }

  printf("\n\nChars:\n");
  
  for(int i=0; i < size; i++) {
    printf("\"%u\", ", instructionChars[i]);
  }
  printf("\n");

  // All the execution code and memory will go here later on


  return 0;
}
