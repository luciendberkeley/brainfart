#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
  
}

void printMemory(uint8_t memory[], int *pointer, int size) {
  printf("Memory:\n[");
  for(int i=0; i < size; i++) {
    printf("%s%i%s", i == *pointer ? "P|> " : "" ,memory[i], i != size - 1 ? ", " : "");
  }
  printf("]. Pointer Pos: %i\n", *pointer);
}

int main(int argc, char *argv[]) {
	int debug = 0;
  char *input = "";

	int opt;
	while((opt = getopt(argc, argv, "hdi:")) != -1) {
		switch(opt) {
			case 'h':
				printf("Usage: brainfart [options] file\n  -h  Show this Page\n  -d  Debug Mode\n  -i [input]  Takes in input and gives it to program\n");
				exit(EXIT_SUCCESS);
			case 'd':
				debug = 1;
				break;
			case 'i':
				if(optarg != NULL) {
					input = optarg;
				} else {
					fprintf(stderr, "-i takes in input, try -h\n");
					exit(EXIT_FAILURE);
				}
				break;
			case '?':
				fprintf(stderr, "Invalid option, try -h\n");
				exit(EXIT_FAILURE);
		}
	}
	
	if(optind >= argc) {
		fprintf(stderr, "Invalid option, try -h\n");
		exit(EXIT_FAILURE);
	}

  char *programFile = argv[optind];

	if(debug) {
		printf("Starting\n\n");

		printf("Parse Tokens\n");
	}
  char instructionChars[128];
  int instructionCount = getInstructionChars(instructionChars, programFile);

	if(debug) {
		printf("Program Arr:\n[");
		for(int i=0; i < instructionCount; i++) {
			printf("\"%c\"%s", instructionChars[i], i != instructionCount - 1 ? ", " : "");
		}
		printf("]\n\n");
		
		printf("Chars:\n[");
		
		for(int i=0; i < instructionCount; i++) {
			printf("%u%s", instructionChars[i], i != instructionCount - 1 ? ", " : "");
		}
		printf("]\n\n\n");
		
		printf("Program Simplified:\n");
		for(int i=0; i < instructionCount; i++) {
			printf("%c", instructionChars[i]);
		}
		printf("\nLength: %d\n", instructionCount);
	}

  // All the execution code and memory will go here later on
  int memorySize = 50;
	uint8_t memory[memorySize];
  int pointer = 0;
	if(debug) {
		printf("\n\nPROGRAM:\n");
		printf("---------------------------------\n");
	}
  processInstructions(instructionChars, instructionCount, memory, &pointer, memorySize, input);
	if(debug) {
		printf("\n---------------------------------\n");
		printf("END PROGRAM\n\n");
	}

	if(debug) {
		printMemory(memory, &pointer, memorySize);
	}

  return 0;
}
