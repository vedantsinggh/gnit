#include <string>
#include <cassert>
#include <functional>
#include <iostream>

#include "log.h"
#include "cmd.h"

int count = 0;

int add_argument(Cmd* cmd, std::string arg, std::function<int(int, char**)> callbackfunction){
	argument argu;
	argu.prompt = arg;
	argu.callback_func =  callbackfunction;
	cmd->args[count++] = argu;
	return 0;
}

int execute(Cmd* cmd, int argc ,char* args[]){

	if (cmd->docs == nullptr){
		Log(ERROR,"DOCS WEREN'T PROVIDED");
		return -1;
	}
	
	if (argc == 1)	{
		cmd->docs();
		return 0;
	}

	for(int i = 0; i < count; i++){
		argument ar = cmd->args[i];
		if (args[1] == ar.prompt){
			ar.callback_func(argc, args);
			return 0;
		}
	}
	Log(WARN,"CAN'T YOU TYPE SOME VALID COMMAND, PIECE OF SHIT!");
	return -1;
}
