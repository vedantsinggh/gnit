#include "files.h"

int fileExists(const char* path){
	struct stat buf;
	if (stat(path, &buf) == 0) return 0;
	return -1;
}

std::string readFile(const char* path){
	std::string line;
	std::string text;
	std::ifstream file(path);
	while(getline(file, line))
	{
		line += "\n";
		text += line;
	}
	file.close();
	return text;	
}

std::string getFileHash(const char* name){
	if(fileExists(name) != 0) return "X";
	std::string out = sha256(readFile(name));
	return out;
}
