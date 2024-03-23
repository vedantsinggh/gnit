#include <iostream>
#include <string>
#ifndef LOG_H
#define LOG_H

void Log(std::string  message);
void Log(int level, std::string  message);
void ColorLog(int color, std::string message);

enum LOG_LEVEL{
	SUCCESS, 
	INFO,
	ERROR,
	WARN,
};

enum LOG_COLOR{
	BLACK=30,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	VOILET,
	CYAN,
	WHITE
};

#endif //LOG_H

