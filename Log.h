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

#ifdef LOG_IMPLEMENTATION

void Log(int level, std::string  message){
	// Color text format "\033[COLORm TEXT \033[m
	if (level == SUCCESS)
	{
		std::cout<< "\033[32m" <<"[SUCCESS] " << "\033[m";
	}
	if (level == INFO)
	{
		std::cout<< "\033[36m" <<"[INFO]    " << "\033[m";
	}
	if (level == WARN)
	{
		std::cout<< "\033[33m" <<"[WARN]    " << "\033[m";
	}
	if (level == ERROR)
	{
		std::cout<< "\033[31m" <<"[ERROR]   " << "\033[m";
	}
	std::cout<<message<<std::endl;
}


void Log(std::string  message){
	std::cout<<message<<std::endl;
}

void ColorLog(int color, std::string  message){
	std::cout<< "\033["<< color << "m" << message << "\033[m" << std::endl;
}

#endif //LOG_IMPLEMENTATION
