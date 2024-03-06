#include <iostream>
#include <cstring>
#include <cassert>

#define print(x) std::cout<<x
#define println(x) std::cout<<x<<std::endl

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
		//assert((void("Invalid Usage!"), !commanded));
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
					println("[WARNING] invalid files");
					return 1;
				}
			}
		}
	}


	for (int i=0; i<argument_size; ++i){
		println(arguments[i]);
	}

	for (int i=0; i<file_size; ++i){
		println(files[i]);
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
		println("[INFO] adding files to gnit");
	}
	else if (strcmp(command,"register") == 0){
		commanded = true;
		println("[INFO] registaring origin to server!");
	}
	else if (strcmp(command,"commit") == 0){ 
		println("[INFO] adding commits to local");
		commanded = true;
	}
	else if (strcmp(command,"login") == 0){
		println("[INFO] login using user creds");
		commanded = true;
	}
	else {
		println("[ERROR] invalid command! ");
		return 1;
	}
	return 0;
}

void log_info(){
	println("-- GNIT -- ");
	println("It is a version conrolling system made in pure c ");
	println("   this is doc on how to use this software ");
}


//Usage 
//file_name -[flags] [commands]
//flags --verbose --forced
//commands add commit register login
