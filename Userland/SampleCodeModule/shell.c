#include <shell.h>

#define MAX_CHARS_PER_COMMAND 128

int textBufferActive = 1;			// no usamos estas dos vars
int kernelBufferPos = 0;

int minCharSize = 1;
int maxCharSize = 6;

static char commandBuffer[256] = {0}; //buffer local en base al buffer 
static int commandBufferPos = 0; 	  //current position de commandBuffer
void saveChar(uint8_t c){
	//si tecla que recibo de kernel es un enter, chequeo lo que ya tenia, si es backspace borro ultimo char, si no lo guardo en buffer local
	if(c == '\n'){
		putc(c);
		checkBuffer();
	} else if(c == '\b'){
		if(commandBufferPos > 0 || (commandBufferPos == 0 && commandBuffer[commandBufferPos]=='>')) {
			commandBuffer[--commandBufferPos] = 0;
			putc(c);
		}
	} else {
		commandBuffer[commandBufferPos++] = c;
		putc(c);
	}

	if(commandBufferPos>=MAX_CHARS_PER_COMMAND){
		clearCommandBuffer();
		putc('\n');
		printf("Max command length reached. Type HELP to see available commands\n");
		putc('>');
	}
}

//chequea si lo que esta en el buffer antes del \n es un comando correcto, y ademas limpia el buffer
char* validCommands[16][2] = {{"HELP", "Provides a list of available programs"}, {"CLEAR", "Clears the screen"}, 
							{"TIME", "Shows current system time (GMT-3)"}, {"TRON", "Runs Tron game"}, 
							{"REGISTERS", "Prints value of all the registers"}, {"TEST0DIV", "Tests the division by 0 exception"},
							{"TESTINVALIDEXC", "Tests an invalid exception"}};
char* validCommands2Args[16][2] = {{"MEMORYAT", "Displays the content at the desired location"}, {"SETSIZE", "Sets the letter size (Default 2)"},
							{"SONGS", "Choose from a variety of available songs"}};
void (*commandFunctions[16])(void) = {help, returnToShell, time, tron, printRegisters, test0Div, testInvalidExc};
void (*commandFunctions2Args[16])(int) = {printMemoryAt, setSize, beeperSongs};
void checkBuffer(){
	int found = 0;
	for(int i = 0; validCommands[i][0] != 0 && !found; i++) {
		if(stringEquals(commandBuffer, validCommands[i][0])) {
			(*commandFunctions[i])();	 //en array paralelo tengo los punteros a las funciones, ejecuto funcion que ingresó user
			found = 1;
		}
	}
	if (!found) {			// busco en las funciones con 1 arg extra
		for (int j = 0; validCommands2Args[j][0] && !found; j++) {
			// beginsWith
			int pos = beginsWith(commandBuffer, validCommands2Args[j][0]);
			if (pos != -1) {
				found = 1;
				if (commandBuffer[pos] == 0) {
					printf("Invalid number of arguments, %s <NUMBER>\n", validCommands2Args[j][0]);
				} else if (commandBuffer[pos] == ' ') {				// tiene que haber por lo menos un espacio desp del arg
					while (commandBuffer[pos] == ' ') {
						pos++;
					}

					int argnum = -1;						
					if (commandBuffer[pos] != 0 && checkNumWithBase(commandBuffer[pos], 10) != -1) {	// chequeo si es un arg valido (numero)
						if (commandBuffer[pos] == '0' && commandBuffer[pos+1] == 'X'){	
							argnum = toNumWithBase(commandBuffer, pos+2, 16);
						} else if (commandBuffer[pos] == '0' && commandBuffer[pos+1] == 'B') {
							argnum = toNumWithBase(commandBuffer, pos+2, 2);
						} else {
							argnum = toNumWithBase(commandBuffer, pos, 10);
						}
					}
					if (argnum >= 0) {			// lo que quedaba era un numero, ejecuto la funcion
						(*commandFunctions2Args[j])(argnum);	 //en array paralelo tengo los punteros a las funciones, ejecuto funcion que ingresó user
					} else if (argnum == -2) {
						printf("Invalid number of arguments, %s <NUMBER>\n", validCommands2Args[j][0]);
					} else {
						printf("Invalid argument, please enter a valid positive integer\n");
					}
				} else {
					printf("No spaces between arguments\n");
				}
				putc('>');
			}
		}
	}
	if(!found){
		if (!commandBufferIsOnlySpaces()) {									// si el buffer no es espacios o vacio aclaramos
			printf("Invalid option. Type HELP for more information.\n");
		} 
		putc('>');
	}
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
			//putc(c);
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

void time() {
	printCurrentTime();
	putc('>');
}

void help() {
	printf("The available commands are:\n\n");
	printf("Functions that receive 1 arg, format: COMMAND\n");
	int i = 1;
	while (validCommands[i][0] != 0 && validCommands[i][1] != 0) {
		printf("    %s - %s\n", validCommands[i][0], validCommands[i][1]);
		i++;
	}
	printf("\nFunctions that receive 2 args, format: COMMAND <NUMBER>\n");
	i = 0;
	while (validCommands2Args[i][0] != 0 && validCommands2Args[i][1] != 0) {
		printf("    %s - %s\n", validCommands2Args[i][0], validCommands2Args[i][1]);
		i++;
	}
	putc('>');
}

void returnToShell(){
    hvdClear();				 //limpio pantalla
    putc('>');
}

void printRegisters() {
	call_to_printRegisters();
	putc('>');
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

int commandBufferIsOnlySpaces() {
	for (int i = 0; commandBuffer[i] != 0 ;i++) {
		if (commandBuffer[i] != ' ') {
			return 0;
		}
	}
	return 1;
}