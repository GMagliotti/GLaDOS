#include "include/shell.h"
#include <stdbool.h>

#define MAX_CHARS_PER_COMMAND 128

int textBufferActive = 1;			// no usamos estas dos vars
int kernelBufferPos = 0;

int minCharSize = 1;
int maxCharSize = 6;

static char commandBuffer[256] = {0}; //buffer local en base al buffer 
static int commandBufferPos = 0; 	  //current position de commandBuffer
void saveChar(char c) {
	//si tecla que recibo de kernel es un enter, chequeo lo que ya tenia, si es backspace borro ultimo char, si no lo guardo en buffer local
	if(c == '\n') {
		printf("\n");
		checkBuffer();
	} else if(c == '\b') {
		if(commandBufferPos > 0 || (commandBufferPos == 0 && commandBuffer[commandBufferPos]=='>')) {
			commandBuffer[--commandBufferPos] = 0;
			printf("\b");
		}
	} else {
		commandBuffer[commandBufferPos++] = c;
		putc(c);
	}

	if(commandBufferPos>=MAX_CHARS_PER_COMMAND){
		clearCommandBuffer();
		printf("\nMax command length reached. Type HELP to see available commands\n>");
	}
}

//chequea si lo que esta en el buffer antes del \n es un comando correcto, y ademas limpia el buffer
char* validCommands[16][2] = 	{{"HELP", "Provides a list of available programs"}, 
								{"CLEAR", "Clears the screen"}, 
								{"TIME", "Shows current system time (GMT-3)"}, 
								{"TRON", "Runs Tron game"}, 
								{"REGISTERS", "Prints value of all the registers"}, 
								{"TEST0DIV", "Tests the division by 0 exception"},
								{"TESTINVALIDEXC", "Tests an invalid exception"}
								};
char* validCommands2Args[16][2] = 	{{"MEMORYAT", "Displays the content at the desired location"}, 
									{"SETSIZE", "Sets the letter size (Default 2)"},
									{"SONGS", "Choose from a variety of available songs"}
									};
char* validProcessCommands[16][2] = {{"CREATE", "Creates a new process an sends it to foreground"}, 
									{"GETPID", "Displays the current process ID"}, 
									{"PS", "Prints the list of active processes"},
									{"LOOP", "Loops the current process on a message"},
									};
char* validProcessCommands2Args[16][2] = 	{{"PKILL", "Kills the process"},
											{"NICE", "Sets the process priority to 2"},
											{"BLOCK", "Blocks the process"},
											};

void (*commandFunctions[16])(void) = {help, returnToShell, time, tron, printRegisters, test0Div, testInvalidExc};
void (*commandFunctions2Args[16])(int) = {printMemoryAt, setSize, beeperSongs};
void (*processFunctions[16])(void) = {create_process, getpid, ps, loop_process, /*kill_process, nice_process, block_process */};
void (*processFunctions2Args[16])(int) = {kill_process, nice_process, block_process};



void checkBuffer() {
	int found = 0;
	char * params[5] = {0, 0, 0, 0, 0};
	get_params(commandBuffer, params, 5);

	found = find_command(params[0], validCommands, commandFunctions);

	for (int j = 0; !found && validCommands2Args[j][0]; j++) {
		if (stringEquals(params[0], validCommands2Args[j][0])) {
			found = 1;
			if (params[1] == NULL) printf("Invalid number of arguments, %s <NUMBER>\n", validCommands2Args[j][0]);
			else {
				int argnum = string_to_int(params[1]);		
				if (argnum >= 0) { (*commandFunctions2Args[j])(argnum);
				} else if (argnum == -1) printf("Invalid argument, please enter a valid positive integer\n");
			}
		}
	}

	if(!found) {
		found = find_command(params[0], validProcessCommands, processFunctions);
	}

	for (int j = 0; !found && validProcessCommands2Args[j][0]; j++) {
		if (stringEquals(params[0], validProcessCommands2Args[j][0])) {
			found = 1;
			if (params[1] == NULL) printf("Invalid number of arguments, %s <NUMBER>\n", validProcessCommands2Args[j][0]);
			else {
				int argnum = string_to_int(params[1]);		
				if (argnum >= 0) { (*processFunctions2Args[j])(argnum);
				} else if (argnum == -1) printf("Invalid argument, please enter a valid positive integer\n");
			}
		}
	}	

	if(!found && !is_only_space(commandBuffer)) {
		printf("Invalid option. Type HELP for more information.\n");
	}

	printf(">");
	clearCommandBuffer();  //limpio el buffer local y seteo posicion de contador en 0
	call_to_clearbuffer(); //limpio el buffer de kernel mediante syscall
}

void clearCommandBuffer() {
	for(int i=0; i<256;i++)
		commandBuffer[i]=0;
	commandBufferPos=0;
}

void shell() {
	call_to_setSize(DEFAULT_TEXT_SIZE);
	printf("Welcome to the command line! Type HELP for more information.\n");
    putc('>');
    while(1) {
		call_to_hlt();
		char c = call_to_getchar();
		if(c!=0) {
			saveChar(c);
		}
    }
}

void setBash() {
    call_to_setptrx(0);
    call_to_setptry(0);
}

void hvdClear() {
	call_to_fillrectangle(0, 0, 0, SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT);
	setBash();
}

void time() {
	printCurrentTime();
}

void print_valid_array(char * array[16][2]) {
	if (array == NULL) return;
	int i = 0;
	while (array[i][0] != 0 && array[i][1] != 0) {
		printf("\t%s - %s\n", array[i][0], array[i][1]);
		i++;
	}
}

void help() {
	printf("The available commands are:\n\n");

	printf("Functions that receive 1 arg, format: COMMAND\n");
	print_valid_array(validCommands);

	printf("\nFunctions that receive 2 args, format: COMMAND <NUMBER>\n");
	print_valid_array(validCommands2Args);

	printf("\nProcess related functions, format: COMMAND\n");
	print_valid_array(validProcessCommands);

	printf("\nProcessess that receive 2 args, format: COMMAND <PID>\n");
	print_valid_array(validProcessCommands2Args);
}

void returnToShell() {
    hvdClear();				 //limpio pantalla
}

void printRegisters() {
	call_to_printRegisters();
}

void setSize(int newSize) {
	if (newSize < minCharSize || newSize > maxCharSize) {
		printf("size must be between %d and %d\n", minCharSize, maxCharSize);	
	} else {
		call_to_setSize(newSize);
		hvdClear();
		printf("New screen size: %d\n", newSize);
	}
}


bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

int is_only_space(char * str) {
	for (int i = 0; str[i] != 0 ;i++) {
		if (!is_space(str[i])) {
			return 0;
		}
	}
	return 1;
}

void get_params(char* str, char* params[], int max_params) {
    if (str == NULL || params == NULL || max_params <= 0) {
        return;
    }

    int count = 0;
    int i = 0;
    int param_index = 0;
    bool inside_param = false;

    while (str[i] != '\0') {
        if (!inside_param && !is_space(str[i])) {
            // Start of a new parameter
            inside_param = true;
            params[param_index] = str + i;
            param_index++;

            if (param_index == max_params) {
                break; // Reached the maximum number of parameters
            }
        }
        else if (inside_param && is_space(str[i])) {
            // End of a parameter
            inside_param = false;
            str[i] = '\0';
            count++;
        }

        i++;
    }

    if (param_index < max_params) {
        // Fill the remaining slots with NULL
        for (int j = param_index; j < max_params; j++) {
            params[j] = NULL;
        }
    }
}

int string_to_int(char * str) {
	int num = -1;
	if ( str[0] == '0' ) {
		if ( str[1] == 'X' ) {
			num = toNumWithBase(str, 2, 16);
		} else if ( str[1] == 'B' ) {
			num = toNumWithBase(str, 2, 2);
		} else {
			num = toNumWithBase(str, 0, 10);
		}
	} else {
		num = toNumWithBase(str, 0, 10);
	}
	return num;
}

int find_command(char * command, char * validCommands[16][2], void(*commandFunctions[16])(void)) {
	for(int i = 0; validCommands[i][0] != 0; i++) {
		if(stringEquals(command, validCommands[i][0])) {
			(*commandFunctions[i])();	 //en array paralelo tengo los punteros a las funciones, ejecuto funcion que ingresó user
			return 1;
		}
	}
	return 0;
}
