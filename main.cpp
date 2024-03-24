#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <vector>

#include "cmd.h"
#include "log.h"
#include "files.h"


std::vector<std::string> all_dirs;
std::vector<std::string> all_files;

void scan_dir(std::string name){
	for (const auto & entry : std::filesystem::directory_iterator(name)){
		if (entry.path().string() == "./.git") continue;
		std::cout << entry.path();
		if(std::filesystem::is_directory(entry.path())){
			all_dirs.push_back(entry.path().string());	
			scan_dir(entry.path().string());
			std::cout << " is a folder!\n";
		}else{
			all_files.push_back(entry.path().string());	
			std::cout << " is a file!\n";
		}
	}
}

int main(int argc, char* argv[])
{
	Cmd cmd;

	add_argument(&cmd, "init", [&](int arc, char* arv[]) -> int{
		const char*  config_file_path = ".gnitconfig";
		if(fileExists(config_file_path) != 0) {
			std::ofstream file;
			file.open(config_file_path);
			file << ".gnitconfig";
			file.close();
			Log("config file initialized at current directory");
			return 0;
		}
		Log("config file already exist!");
		return 0;
	});


	add_argument(&cmd,"add" , [&](int arc, char* arv[]) -> int{
		std::string files[arc - 2];
		int count = 2;

		while( count < arc ){
			files[count - 2] = arv[count];
			count++;
		}
		
		for (std::string file : files){
			Log(file);	
		}

		scan_dir("./");
		
		return 0;
	});

	execute(&cmd, argc, argv);
}
/*
.gnitconfig format
	FILE.NAME: HASHHHHHHH;
*/
	
