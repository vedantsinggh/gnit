#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string_view>

#include "cmd.h"
#include "log.h"
#include "files.h"

std::vector<std::string> all_dirs;
std::vector<std::string> all_files;
std::vector<std::string> new_files;
std::vector<std::string> updated_files;
std::string config = "";
const char*  config_file_path = "./.gnitconfig";

void add_scan(std::string name, std::ofstream* config_file){
	for (const auto & entry : std::filesystem::directory_iterator(name)){
		if (entry.path().string() == "./.git") continue;
		if (entry.path().string() == config_file_path) continue;
		if(std::filesystem::is_directory(entry.path())){
			all_dirs.push_back(entry.path().string());	
			add_scan(entry.path().string(), config_file);
		}else{
			all_files.push_back(entry.path().string());	
			std::string final_hash;
			std::string line;
			std::ifstream file(config_file_path);
			bool flag = false;
			while(getline(file, line))
			{
				if(line.rfind(entry.path().string(), 0 ) == 0){
					std::string old_hash = line.substr( line.size() - 64, line.size());
					std::string new_hash = getFileHash(entry.path().string().c_str());
					if(old_hash != new_hash){
						final_hash = new_hash;
						updated_files.push_back(entry.path().string());
					}else {
						final_hash = old_hash;
					}
					flag = true;
					break;
				}
			}
			
			file.close();
			if(flag){
				std::string res = entry.path().string() + ": " + final_hash + "\n";
				config += res;				
			}else{
				std::string hash = getFileHash(entry.path().string().c_str());
				std::string res = entry.path().string() + ": " + hash + "\n";
				new_files.push_back(entry.path().string());
				config += res;				
			}

		}
	}
}

std::vector<std::string> oldf;
std::vector<std::string> newf;
std::vector<std::string> updatedf;
void status_scan(std::string name){

	if(fileExists(config_file_path) != 0) {
		Log(ERROR, "NO CONFIG FILE!");
	}else{
		for (const auto & entry : std::filesystem::directory_iterator(name)){
			if (entry.path().string() == "./.git") continue;
			if (entry.path().string() == config_file_path) continue;
			if(std::filesystem::is_directory(entry.path())){
				status_scan(entry.path().string());
			}else{
				std::string line;
				std::ifstream file(config_file_path);
				bool flag = false;
				while(getline(file, line))
				{
					if(line.rfind(entry.path().string(), 0 ) == 0){
						std::string old_hash = line.substr( line.size() - 64, line.size());
						std::string new_hash = getFileHash(entry.path().string().c_str());
						if(old_hash != new_hash){
							updatedf.push_back(entry.path().string());
						}else {
							oldf.push_back(entry.path().string());
						}
						flag = true;
						break;
					}
				}
				
				file.close();
				if(flag){
				}else{
					newf.push_back(entry.path().string());
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	Cmd cmd;
	cmd.docs = [](){
		Log("THIS IS DOCS!");
	};

	add_argument(&cmd, "init", [&](int arc, char* arv[]) -> int{
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

	
	add_argument(&cmd,"status" , [&](int arc, char* arv[]) -> int{
		status_scan("./");		

		if (newf.size() == 0){
			Log(INFO, "NO NEW FILE!");
		}else{
			Log(GREEN, "NEW FILES:");
			for(std::string f : newf){
				Log(GREEN," + " + f);
			}
		}
		
		if (updatedf.size() == 0){
			Log(INFO, "NO FILE MODIFIED!");
		}else{
			Log(VOILET, "MODIFIED FILES:");
			for(std::string f : updatedf){
				Log(VOILET," ~ " + f);
			}
		}

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

		if(fileExists(config_file_path) == 0) {
			std::ofstream file;
			file.open(config_file_path, std::ios::out);
			add_scan("./", &file);
			file << config;
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

