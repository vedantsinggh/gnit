#include <string>
#include <cassert>
#include <functional>
#include <iostream>

#ifndef CMD_H
#define CMD_H

#ifndef NUMBER_OF_ARGUMENTS 
#define NUMBER_OF_ARGUMENTS 5
#endif

struct argument{
	std::string prompt;
	std::function<int(int, char**)> callback_func;
};

struct Cmd{
	argument args[NUMBER_OF_ARGUMENTS];
	std::function<void()> docs;
	
};

int add_argument(Cmd* cmd, std::string arg, std::function<int(int, char**)> callbackfunction);

int execute(Cmd* cmd, int argc ,char* args[]);

#endif //CMD_H
