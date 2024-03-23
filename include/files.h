#include <sys/stat.h>
#include <fstream>
#include <iostream>

#include "log.h"
#include "sha256.h"

#ifndef FILES_H
#define FILES_H

int fileExists(const char* path);
std::string readFile(std::string path);
std::string getFileHash(const char* name);

#endif //FILES_H

