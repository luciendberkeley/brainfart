#include <stdio.h>
#include <stdint.h>

int charInArr(char c, char *arr[], int len) {
  for(int i=0; i < len; i++) {
    if(*arr[i] == c) return 1;
  }
  return 0;
}

int getInstructionChars(char outPoint[], char filePath[]) {
  char text[256];
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

int getStrLen(char *s) {
  char *tempPtr = s;
  int len = 0;
  while(*tempPtr != '\0') {
    len++;

    tempPtr++;
  }

  return len;
}

int endLoopIndex(char instructions[], int count, int index) {
  int opensFound = 0;

  for(int i=index; i < count; i++) {
    if(instructions[i] == 91) {
      opensFound++;
    }

    if(instructions[i] == 93) {
      opensFound--;
      if(opensFound == 0) {
        return i;
      }
    }
  }

  return -1;
}

int startLoopIndex(char instructions[], int count, int index) {
  int closesFound = 0;

  for(int i=index; i >= 0; i--) {
    if(instructions[i] == 93) {
      closesFound++;
    }

    if(instructions[i] == 91) {
      closesFound--;
      if(closesFound == 0) {
        return i;
      }
    }
  }

  return -1;
}

void processInstructions(char instructions[], int count, uint8_t memory[], int *pointer, int size, char *input) {
  printf("\n\nPROGRAM:\n");
  printf("---------------------------------\n");

  *pointer = 0;
  for(int i=0; i < size; i++) {
    memory[i] = 0;
  }

  int inputLen = getStrLen(input);
  uint8_t inputBytes[inputLen];
  for(int i=0; i < inputLen; i++) {
    inputBytes[i] = input[i];
  }

  int inputIndex = 0;

  for(int i=0; i < count; i++) {
    switch(instructions[i]) {
      case 62:
        // Right
        if(*pointer < size) {
          (*pointer)++;
        }
        break;
      case 60:
        // Left
        if(*pointer > 0) {
          (*pointer)--;
        }
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
        if(inputIndex >= inputLen) {
          memory[*pointer] = 0;
        } else {
          memory[*pointer] = input[inputIndex];
        }
        inputIndex++;
        break;
      case 91:
        // Loop start
        if(memory[*pointer] == 0) {
          // Move to the ending of this loop + 1
          i = endLoopIndex(instructions, count, i);
        }
        break;
      case 93:
        // Loop end
        if(memory[*pointer] != 0) {
          // Move back to beginning of loop + 1
          i = startLoopIndex(instructions, count, i);
        }
        break;
    }
  }
  
  printf("\n---------------------------------\n");
  printf("END PROGRAM\n\n");
}

void printMemory(uint8_t memory[], int *pointer, int size) {
  printf("Memory:\n[");
  for(int i=0; i < size; i++) {
    printf("%s%i%s", i == *pointer ? "P|> " : "" ,memory[i], i != size - 1 ? ", " : "");
  }
  printf("]. Pointer Pos: %i\n", *pointer);
}

int main(int argc, char *argv[]) {
  if(argc == 1) {
    printf("Wrong usage. Must run with <programPath> and optionally <input> parameters.\n");
    return 1;
  }

  char *programFile = argv[1];
  char *input = "";
  if(argc == 3) {
    input = argv[2];
  }

  printf("Starting\n\n");

  printf("Parse Tokens\n");
  char instructionChars[128];
  int instructionCount = getInstructionChars(instructionChars, programFile);

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
  int memorySize = 50;
	uint8_t memory[memorySize];
  int pointer = 0;
  processInstructions(instructionChars, instructionCount, memory, &pointer, memorySize, input);

  printMemory(memory, &pointer, memorySize);

  return 0;
}
