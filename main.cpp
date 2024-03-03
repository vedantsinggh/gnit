#include <iostream>
#include <cstring>
#include <cassert>
#define print(x) std::cout<<x<<std::endl

char* arguments[2];
int argument_size = 0;
bool commanded = false;
char* command;

int add_arguments(char* argument);
int run_command(char* command);

int main(int argc, char* argv[]){
	char* file_name = argv[0];

	for (int i=1; i<argc; ++i){
		if (*(argv[i]) == '-') {
			if(add_arguments(argv[i]) != 0) return 1;
		}		
		else {
			if(run_command(argv[i]) != 0) return 1;
		}
	}


	for (int i=0; i<argument_size; ++i)
	{
		print(arguments[i]);
	}

	return 0;
}

int add_arguments(char* argument){
	arguments[argument_size] = argument;
	++argument_size;	
	return 0;
}

int run_command(char* command){
	assert((void("Invalid Usage!"), !commanded));
	if (!commanded){
		if (strcmp(command,"add") == 0) {
			commanded = true;
			print("[INFO] adding files to gnit");
		}
		else if (strcmp(command,"register") == 0){
			commanded = true;
			print("[INFO] registaring origin to server!");
		}
		else if (strcmp(command,"commit") == 0){ 
			print("[INFO] adding commits to local");
			commanded = true;
		}
		else if (strcmp(command,"login") == 0){
			print("[INFO] login using user creds");
			commanded = true;
		}
		else {
			print("[ERROR] invalid command! ");
			return 1;
		}
	}
	return 0;
}

//Usage 
//file_name -[flags] [commands]
//flags --verbose --forced
//commands add commit register login
