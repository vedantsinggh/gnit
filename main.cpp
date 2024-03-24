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

void scan_dir(std::string name, std::ofstream* config_file){
	for (const auto & entry : std::filesystem::directory_iterator(name)){
		if (entry.path().string() == "./.git") continue;
		if(std::filesystem::is_directory(entry.path())){
			all_dirs.push_back(entry.path().string());	
			scan_dir(entry.path().string(), config_file);
		}else{
			all_files.push_back(entry.path().string());	
			std::string hash = getFileHash(entry.path().string().c_str());
			std::string res = entry.path().string() + ": " + hash + "\n";
			*config_file << res;
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
			file << ".gnitconfig\n";
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

		const char*  config_file_path = ".gnitconfig";
		if(fileExists(config_file_path) == 0) {
			std::ofstream file;
			file.open(config_file_path, std::ios::app);
			scan_dir("./", &file);
			file.close();
		}else {
			Log("NO GNIT CONFIG!");
		}
		
		return 0;
	});

	execute(&cmd, argc, argv);
}
/*
.gnitconfig format
	FILE.NAME: HASHHHHHHH;
*/
	
