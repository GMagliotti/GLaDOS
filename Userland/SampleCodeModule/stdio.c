#include <stdio.h>
#include <user_syscalls.h>

#define BUFFER_SIZE 64
#define STDOUT 1
#define STDERR 2

char buffer[BUFFER_SIZE] = { 0 };
char copyBuffer[BUFFER_SIZE] = { 0 };
uint16_t copyBufferStart = 0;
uint16_t copyBufferEnd = 0;
uint16_t bufferPos = 0;
uint64_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void addToCopyBuffer(uint8_t c);
void getInput();
int baseToSint(uint32_t base, uint8_t * errFlag);
int baseToUint(char* str, uint8_t base);
int vscanf(const char *format, va_list args);

void clearBuffer(){
	for(int i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = 0;
	}
}

void vprintf(char * fmt, va_list ap){
	uint32_t length = 0;
	while(*fmt && length < BUFFER_SIZE){
        if(*fmt == '%'){
            fmt++;
            switch(*fmt){
                case 'd':
                    {
                        int i = va_arg(ap, int);
						length += uintToBase(i, &(buffer[length]), 10);
                    }
                    break;
                case 's':
                    {
                        char * s = va_arg(ap, char *);
                        while(*s!='\0'){
							buffer[length++] = *s;
							s++;
						}
                    }
                    break;
				case 'x':
					{
						int i = va_arg(ap, int);
						length += uintToBase(i, &(buffer[length]), 16);
					}
					break;
				case 'o':
					{
						int i = va_arg(ap, int);
						length += uintToBase(i, &(buffer[length]), 8);
					}
					break;
				case 'b':
					{
						int i = va_arg(ap, int);
						length += uintToBase(i, &(buffer[length]), 2);
					}
					break;
				case 'c':
					{
						char * c = va_arg(ap, char *);
						if (*c <= 0x7F && *c >= 0x10) buffer[length++] = *c;
						else buffer[length++] = ' ';
					}
					break;
                default:
					buffer[length++] = *fmt;
                    break;
            }
        }else{
			buffer[length++] = *fmt;
        }
        fmt++;
    }
	call_to_sys_write(buffer, length, STDOUT);
}

void printf(char * fmt, ...){
	clearBuffer();
	va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

void putc(char character) {
	call_to_sys_write(&character, 1, STDOUT);
}

uint64_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	
	return digits;
}

void addToCopyBuffer(uint8_t c){
    // two cases: it is full, or not
    if((copyBufferEnd+1)%BUFFER_SIZE==copyBufferStart){
        copyBufferStart = (copyBufferStart + 1)%BUFFER_SIZE;
    }
    copyBuffer[copyBufferEnd] = c;
    copyBufferEnd = (copyBufferEnd + 1)%BUFFER_SIZE;
}

char getChar(){
    putc(' ');
    uint8_t c;
    if(copyBufferEnd!=copyBufferStart){ // if it is not empty, returns whatever it has
        c = copyBuffer[copyBufferStart];
        copyBufferStart= (copyBufferStart + 1)%BUFFER_SIZE;
        return c;
    }
    int i=copyBufferEnd;
    while(copyBuffer[i]!='\n'){
        call_to_hlt();
        if(i!=copyBufferEnd){ // i have input
            i = copyBufferEnd; // equivalent to incremente in mod BUFFER_SIZE
        }
        call_to_sys_read((uint8_t) STDIN, &c, (uint8_t)1);// this is OK, as long as
		// the last parameter (amount of chars read) is 1
        if(c != 0 && c!='\b'){
            addToCopyBuffer(c);
        }
        if(c=='\b' && copyBufferEnd!=copyBufferStart){
            copyBufferEnd = (copyBufferEnd - 1) % BUFFER_SIZE;
        }
    }
    c = copyBuffer[copyBufferStart];
    copyBufferStart = (copyBufferStart + 1)%BUFFER_SIZE;
    return c;
}


int scanf(const char *format, ...) {
	clearBuffer();
    int ret;
    va_list args;
    va_start(args, format);
    ret = vscanf(format, args);
    va_end(args);
	return ret;
}


int vscanf(const char *format, va_list args) {
    int ret = 0;
	uint8_t flag = 0;
    while (*format != '\0') {
		if(bufferPos==BUFFER_SIZE){
			clearBuffer();
		}
        if (*format == '%') {
			ret++;
            format++;
            switch (*format) {
                case 'd':
                    {
                        int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToSint(10, &flag);
                    }
                    break;
                case 'i':
                    {
                        int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToSint(10, &flag);
                    }
                    break;
                case 'u':
                    {
                        int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToUint(buffer, 10);
						if(*i<0){
							flag = 1;
						}
                    }
                    break;
                case 'x':
                    {
                        int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToUint(buffer, 16);
						if(*i<0){
							flag = 1;
						}
                    }       
                    break;
                case 'X':
                    {
                       	int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToUint(buffer, 16);
						if(*i<0){
							flag = 1;
						}
                    }      
                    break;
                case 'o':
                    {
                        int *i;
						i = va_arg(args, int*);
						getInput();
						*i = baseToUint(buffer, 8);
						if(*i<0){
							flag = 1;
						}
                    }
                    break;
                case 'c':
                    {
                        char *c;
						c = va_arg(args, char *);
                        getInput();
						*c = buffer[bufferPos++];
                    }
                    break;
                default:
                    break;
            }
        } else {
			getInput();
			while(*format!='%' && *format!=0){
				putc(buffer[bufferPos]);
				if (buffer[bufferPos++] != *format) {
					return ret;
				}
				format++;
			}

			while(buffer[bufferPos]=='\n'){
				bufferPos++;
			}

			if(*format==0 || buffer[bufferPos]!=0){ // if buffer is not finished, or if there is no more format to read, return
				return ret;
			}

			
			clearBuffer();
			format--;
        }
		if(flag==1){
			return ret;
		}
        format++;

    }
    return ret;
}

// uses getChar for input and returns the number inputted
void getInput(){
	int i = bufferPos;
	char c;
	do{
		buffer[i] = getChar();
		if(buffer[i]!=0){
			c=buffer[i++];
		}
	}while(c!='\n' && i<BUFFER_SIZE);

	 if(i==BUFFER_SIZE){
		buffer[i-1] = '\0';
		call_to_sys_write("\nMAXIMO DE CARACTERES ALCANZADO\n", 32, STDERR);
	 } else {
		buffer[i] = '\0';
	 }
}

// function that converts a string to a signed integer
int32_t baseToSint(uint32_t base, uint8_t * errFlag){
	char * str = buffer;
	int sign = 1;
	int32_t num = 0;
	if(str[0]=='-'){
		sign = -1;
		str+=1;
	}
	num=baseToUint(str, base);
	if(sign==-1){
		bufferPos++;
	}
	if(num==-1){
		*errFlag=1;
	} else {
		*errFlag=0;
	}
	return num*sign;
}

// function that converts a string to an unsigned integer
int32_t baseToUint(char* str, uint8_t base){
	int32_t ret = 0;
	int i = bufferPos;
	while(str[i]!='\0' && str[i]!='\n'){
		if(str[i]>='0' && str[i]<='9'){
			ret = ret*base + (str[i]-'0');
		} else if(str[i]>='A' && str[i]<='F' && base==16){
			ret = ret*base + (str[i]-'A'+10);
		} else if(str[i]>='a' && str[i]<='f' && base==16){
			ret = ret*base + (str[i]-'a'+10);
		} else {
			call_to_sys_write("\nERROR: CARACTER NO VALIDO\n", 27, STDERR);
			call_to_sleep(3);
			bufferPos=i;
			return ret==0&&i!=0?-1:ret;
		}
		i++;
	}
	bufferPos=i;
	return ret;
}





