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
  fopen_s(&filePtr, filePath, "r");
  // Other:
  // filePtr = fopen(filePath, "r");

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

void processInstructions(char instructions[], int count, uint8_t memory[], int *pointer, int size) {
  printf("\n\nPROGRAM:\n");
  printf("---------------------------------\n\n");

  *pointer = 0;
  for(int i=0; i < size; i++) {
    memory[i] = 0;
  }
  

  for(int i=0; i < count; i++) {
    switch(instructions[i]) {
      case 62:
        // Right
        (*pointer)++;
        break;
      case 60:
        // Left
        (*pointer)--;
        break;
      case 43:
        // Incr
        memory[*pointer]++;
        break;
      case 45:
        // Decr
        memory[*pointer]--;
        break;
      case 46:
        // Output
        printf("%c", memory[*pointer]);
        break;
      case 44:
        // Input byte
        break;
      case 91:
        // Loop start
        break;
      case 93:
        // Loop end
        break;
    }
  }
  
  printf("\n\n---------------------------------\n");
  printf("END PROGRAM\n\n");
}

void printMemory(uint8_t memory[], int *pointer, int size) {
  printf("Memory:\n[");
  for(int i=0; i < size; i++) {
    printf("%s%i%s", i == *pointer ? "P|> " : "" ,memory[i], i != size - 1 ? ", " : "");
  }
  printf("]. Pointer Pos: %i\n", *pointer);
}

int main() {
  printf("Starting\n\n");

  printf("Parse Tokens\n");
  char instructionChars[128];
  int instructionCount = getInstructionChars(instructionChars, "test.bf");

  printf("Readable:\n[");
  for(int i=0; i < instructionCount; i++) {
    printf("\"%c\"%s", instructionChars[i], i != instructionCount - 1 ? ", " : "");
  }
  printf("]\n\n");

  printf("Chars:\n[");
  
  for(int i=0; i < instructionCount; i++) {
    printf("%u%s", instructionChars[i], i != instructionCount - 1 ? ", " : "");
  }
  printf("]\n\n");

  // All the execution code and memory will go here later on
  int memorySize = 10;
	uint8_t memory[memorySize];
  int pointer = 0;
  processInstructions(instructionChars, instructionCount, memory, &pointer, memorySize);

  printMemory(memory, &pointer, memorySize);

  return 0;
}
