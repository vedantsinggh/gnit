#include <iostream>
#include <cstring>
#include <cassert>
#define LOG_IMPLEMENTATION
#include "Log.h"

#define NUMBER_OF_ARGUMENTS 2

char* arguments[NUMBER_OF_ARGUMENTS];
int argument_size = 0;
bool commanded = false;
char* command;
char* files[10];
int file_size = 0;

int add_arguments(char* argument);
int run_command(char* command);
void log_info();
bool validate_file(char* file);

int main(int argc, char* argv[]){
	char* file_name = argv[0];
	
	if(argc == 1){
		log_info();
	}

	for (int i=1; i<argc; ++i){
		assert((void("Invalid number of arguments! ")  , argument_size < NUMBER_OF_ARGUMENTS));
		if (*(argv[i]) == '-') {
			if(add_arguments(argv[i]) != 0) return 1;
		}		
		else {
			if(!commanded){
				if(run_command(argv[i]) != 0){
					return 1;
				}
			}
			else {
				if (validate_file(argv[i])){
					files[file_size] = argv[i];
					++file_size;
				}
				else {
					Log(WARN ,"INVALID INPUT FILE");
					return 1;
				}
			}
		}
	}


	for (int i=0; i<argument_size; ++i){
		Log(arguments[i]);
	}

	for (int i=0; i<file_size; ++i){
		Log(files[i]);
	}

	return 0;
}


bool validate_file(char*  file){
	return true;
}

int add_arguments(char* argument){
	arguments[argument_size] = argument;
	++argument_size;	
	return 0;
}

int run_command(char* command){
	assert((void("Invalid Usage!"), !commanded));
	if (strcmp(command,"add") == 0) {
		commanded = true;
		Log(INFO, "ADDING FILES TO GNIT");
	}
	else if (strcmp(command,"register") == 0){
		commanded = true;
		Log(INFO, "REGISTARING ORIGIN TO SERVER!");
	}
	else if (strcmp(command,"commit") == 0){ 
		Log(INFO, "ADDING COMMITS TO LOCAL");
		commanded = true;
	}
	else if (strcmp(command,"login") == 0){
		Log(INFO, "LOGIN USING USER CREDS");
		commanded = true;
	}
	else {
		Log(ERROR ,"INVALID COMMAND!");
		return 1;
	}
	return 0;
}

void log_info(){
	ColorLog(CYAN, "                     -- GNIT --                   ");
	ColorLog(CYAN, "IT IS A VERSION CONROLLING SYSTEM MADE IN PURE C++");
	ColorLog(CYAN, "     THIS IS DOC ON HOW TO USE THIS SOFTWARE     ");
}


//Usage 
//file_name -[flags] [commands]
//flags --verbose --forced
//commands add commit register login
