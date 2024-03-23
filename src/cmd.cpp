#include <string>
#include <cassert>
#include <functional>
#include <iostream>
#include "cmd.h"
/*
{

	add_argument(&Cmd, "command", callback_function(){}); add_argument(&Cmd, "command", callback_function(){});	
	add_flag(&Cmd, "flag", "short", callback_function(){});
	execute(&Cmd, arguments);

}
*/

int count = 0;

int add_argument(Cmd* cmd, std::string arg, std::function<int(int, char**)> callbackfunction){
	argument argu;
	argu.prompt = arg;
	argu.callback_func =  callbackfunction;
	cmd->args[count++] = argu;
	return 0;
}

int execute(Cmd* cmd, int argc ,char* args[]){
	char* fileName = args[0];
	
	if (argc == 1)	{
		//cmd.docs();
		return 0;
	}

	for(int i = 0; i < count; i++){
		argument ar = cmd->args[i];
		if (args[1] == ar.prompt){
			ar.callback_func(argc, args);
			return 0;
		}
	}
	assert("CAN'T YOU TYPE SOME VALID COMMAND, PIECE OF SHIT!");
	return 0;
}
/*
int main(int argc, char* argv[])
{
	Cmd cmd;
	add_argument(&cmd,"add" , [&](int argc, char* argv[]) -> int{
		std::cout << "You said Add, right??\n";
		return 0;
	});
	execute(&cmd, argc, argv);
}
*/
