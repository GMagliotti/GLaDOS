#include "include/shell.h"
#include "include/user_syscalls.h"

#define MAX_CHARS_PER_COMMAND 128

int textBufferActive = 1;			// no usamos estas dos vars
int kernelBufferPos = 0;

int minCharSize = 1;
int maxCharSize = 6;

static char commandBuffer[256] = {0}; //buffer local en base al buffer 
static int commandBufferPos = 0; 	  //current position de commandBuffer
void saveChar(char c){
	//si tecla que recibo de kernel es un enter, chequeo lo que ya tenia, si es backspace borro ultimo char, si no lo guardo en buffer local
	if(c == '\n'){
		printf("\n");
		checkBuffer();
	} else if(c == '\b'){
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
char* validCommands[32][2] = 	{{"HELP", "Provides a list of available programs"}, 
								{"CLEAR", "Clears the screen"}, 
								{"TIME", "Shows current system time (GMT-3)"}, 
								{"TRON", "Runs Tron game"}, 
								{"REGISTERS", "Prints value of all the registers (SHIFT + TAB)"}, 
								{"TEST0DIV", "Tests the division by 0 exception"},
								{"TESTINVALIDEXC", "Tests an invalid exception"},
								{"MEMORYAT", "Displays the content at the desired location"}, 
								{"SETSIZE", "Sets the letter size (Default 2)"},
								{"SONGS", "Choose from a variety of available songs"},
								{"GETPID", "Displays the current process ID"}, 
								{"PS", "Prints the list of active processes"},
								{"LOOP", "Loops a new process printing its pid"},
								{"PKILL", "Kills a valid process"},
								{"NICE", "Changes the priority of a process"},
								{"BLOCK", "Block / Unblock a process (YMMV)"},
								{"SHELL", "Creates a new shell!! (Limit testing)"},
								{"CAT", "Prints the stdin it recieves"},
								{"WC", "Counts the amount of words in input"},
								{"FILTER", "Filters the vocals in the input"},
								{"PRINTSCHEDULER", "Sets the scheduler on print mode"}
								};

void (*commandFunctions[32])(int, char **) = {	help, returnToShell, time, tron, printRegisters, test0Div, testInvalidExc, printMemoryAt, setSize, beeperSongs, 
												getpid, ps, loop_process, kill_process, nice_process, block_process, shell, cat, wc, filter, call_to_set_print_mode};


int find_pipe(char * params[], int argc){
	for(int i = 0; i < argc; i++){
		if(stringEquals(params[i], "|")){
			return i;
		}
	}
	return -1;
}

int is_valid_command(char * command) {
	for(int i = 0; validCommands[i][0] != 0; i++) {
		if(stringEquals(command, validCommands[i][0])) {
			return i;
		}
	}
	return -1;
}

void checkBuffer(){
	int found = 0;
	int command_pos = -1;
    int pid1 = -1;

	char * params[MAX_PARAMS] = {NULL};

	int argc = get_params(commandBuffer, params, MAX_PARAMS);
 
 	int we_piping = find_pipe(params, argc);

	if (we_piping == 0 || ( we_piping == argc -1 && argc > 1 )) {
		printf("Invalid pipe position, %d, argc: %d\n", we_piping, argc);
		found = 1;
	} 

	if (we_piping > 0 && we_piping < argc - 1) {
		int found2 = 0;

		int argc2 = argc - we_piping - 1;
		argc = we_piping;

		char * params2[MAX_PARAMS] = {NULL};

		for (int i = 0; i < argc2; i++) {
			params2[i] = params[we_piping + i + 1];
		}

        // int pipe_id = pipe_open("|");
        // if (pipe_id == -1) printf("Error opening pipe");
        // int fd[2] = {pipe_id, -1};		// -1 hereda del padre
        // int pid1;

		if ((command_pos = is_valid_command(params[we_piping+1])) != -1) {
    			call_to_create_process(argc2, params2, commandFunctions[command_pos]);
				found2 = 1;
		}

    	// fd[0] = -1;	// hereda del padre
    	// fd[1] = pipe_id;

		if ((command_pos = is_valid_command(params[0])) != -1 && found2) {
    			call_to_create_process(argc, params, commandFunctions[command_pos]);
				found = 1;
		}

		if (!found) {
			// kill al proceso 2.
			printf("Somos chicken\n");
		}

		// pipe close cuando??

	} else {
		if ((command_pos = is_valid_command(params[0])) != -1) {
    			pid1 = call_to_create_process(argc, params, commandFunctions[command_pos]);
				found = 1;
		}
	}

	if (pid1 > 0) {
		int ret = call_to_waitpid(pid1);
		printf("hola!! espere a mi hijo, devolvio %d\n", ret);
	}

	if(!found && !is_only_space(commandBuffer)){
		printf("Invalid option. Type HELP for more information.\n");
	}

	// TODO waitpid !!

	printf(">");

	clearCommandBuffer();  //limpio el buffer local y seteo posicion de contador en 0
	call_to_clearbuffer(); //limpio el buffer de kernel mediante syscall
}

void clearCommandBuffer(){
	for(int i=0; i<256;i++)
		commandBuffer[i]=0;
	commandBufferPos=0;
}

void shell(){
	call_to_setSize(DEFAULT_TEXT_SIZE);
	printf("Welcome to the command line! Type HELP for more information.\n");
    putc('>');
    while(1){
		call_to_hlt();
		char c = call_to_getchar();
		if(c!=0){
			saveChar(c);
		}
    }
}

void setBash(){
    call_to_setptrx(0);
    call_to_setptry(0);
}

void hvdClear() {
	call_to_fillrectangle(0, 0, 0, SCREEN_PIXEL_WIDTH, SCREEN_PIXEL_HEIGHT);
	setBash();
}


void help() {
	
	printf("The available commands are:\n\n");

	for (int i = 0; validCommands[i][0] != 0 && validCommands[i][1] != 0; i++) {
		printf("\t%s - %s\n", validCommands[i][0], validCommands[i][1]);
		if (i == 9) printf("\n");
	}
}

void returnToShell(){
    hvdClear();				 //limpio pantalla
}

void printRegisters() {
	call_to_printRegisters();
}

void setSize(int argc, char ** argv) {

	if (argc < 2) {
		printf("Usage: SETSIZE <size>\n");
		return;
	}

	int newSize = string_to_int(argv[1]);

	if (newSize < minCharSize || newSize > maxCharSize) {
		printf("Size must be between %d and %d\n", minCharSize, maxCharSize);	
	} else {
		call_to_setSize(newSize);
		hvdClear();
		printf("New screen size: %d\n", newSize);
	}
}

int get_params(char* str, char* params[], int max_params) {
    if (str == NULL || params == NULL || max_params <= 0) {
        return 0;
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

	return param_index;
}

