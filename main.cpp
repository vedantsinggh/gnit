#include "log.h"
#include <cstring>
#include "cmd.h"


int main(int argc, char* argv[])
{
	Cmd cmd;
	add_argument(&cmd,"add" , [&](int argc, char* argv[]) -> int{
		std::cout << "You said Add, right??\n";
		return 0;
	});
	execute(&cmd, argc, argv);
}

