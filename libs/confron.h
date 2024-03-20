#include <sys/stat.h>
#include <fstream>
#include <iostream>

#define LOG_IMPLEMENTATION
#include "./Log.h"

#include "./sha256.h"

#ifndef CONFRON_H
#define CONFRON_H

int fileExists(const char* path);
std::string readFile(std::string path);
std::string getFileHash(const char* name);

#endif //CONFRON_H

#ifdef CONFRON_IMPLEMENTATION

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

#endif //CONFRON_IMPLEMENTATION
