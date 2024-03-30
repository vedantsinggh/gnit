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

std::vector<std::string> old_file;
std::vector<std::string> new_file;
std::vector<std::string> updated_file;
std::vector<std::string> deleted_files;

std::string config = "";
const char* config_file_path = "./.gnitconfig";

void add_scan(std::string name){

	if(!std::filesystem::is_directory(name)){
		all_files.push_back(name);	
		std::string final_hash;
		std::string line;
		std::ifstream config_file(config_file_path);
		bool isFileFound = false;
		while(getline(config_file, line))
		{
			if(line.rfind(name, 0 ) == 0){
				std::string old_hash = line.substr( line.size() - 64, line.size());
				std::string new_hash = getFileHash(name.c_str());
				if(old_hash != new_hash){
					final_hash = new_hash;
					updated_file.push_back(name);
				}else {
					old_file.push_back(name);
					final_hash = old_hash;
				}
				isFileFound = true;
				break;
			}
		}
		
		config_file.close();

		if(isFileFound){

			std::string res = name + ": " + final_hash + "\n";
			config += res;				

		}else{

			std::string hash = getFileHash(name.c_str());
			std::string res = name + ": " + hash + "\n";
			new_file.push_back(name);
			config += res;				

		}
		
	}
	else{
		for (const auto & entry : std::filesystem::directory_iterator(name)){
			if (entry.path().string() == "./.git") continue;
			if (entry.path().string() == config_file_path) continue;
			if(std::filesystem::is_directory(entry.path())){
				all_dirs.push_back(entry.path().string());	
				add_scan(entry.path().string());
			}else{
				all_files.push_back(entry.path().string());	
				std::string final_hash;
				std::string line;
				std::ifstream config_file(config_file_path);
				bool isFileFound = false;
				while(getline(config_file, line))
				{
					if(line.rfind(entry.path().string(), 0 ) == 0){
						std::string old_hash = line.substr( line.size() - 64, line.size());
						std::string new_hash = getFileHash(entry.path().string().c_str());
						if(old_hash != new_hash){
							final_hash = new_hash;
							updated_file.push_back(entry.path().string());
						}else {
							old_file.push_back(entry.path().string());
							final_hash = old_hash;
						}
						isFileFound = true;
						break;
					}
				}
				
				config_file.close();

				if(isFileFound){

					std::string res = entry.path().string() + ": " + final_hash + "\n";
					config += res;				

				}else{

					std::string hash = getFileHash(entry.path().string().c_str());
					std::string res = entry.path().string() + ": " + hash + "\n";
					new_file.push_back(entry.path().string());
					config += res;				

				}
			}
		}
	}
}

void status_scan(std::string name){

	if(fileExists(config_file_path) != 0) {
		Log(ERROR, "NO CONFIG FILE!");
	}
	else{
		for (const auto & entry : std::filesystem::directory_iterator(name)){

			if (entry.path().string() == "./.git") continue;
			if (entry.path().string() == config_file_path) continue;

			if(std::filesystem::is_directory(entry.path())){
				status_scan(entry.path().string());
			}else{

				std::string line;
				std::ifstream config_file(config_file_path);
				bool isFileFound = false;

				while(getline(config_file, line))
				{
					if(line.rfind(entry.path().string(), 0 ) == 0){

						std::string old_hash = line.substr( line.size() - 64, line.size());
						std::string new_hash = getFileHash(entry.path().string().c_str());

						if(old_hash != new_hash){
							updated_file.push_back(entry.path().string());
						}else {
							old_file.push_back(entry.path().string());
						}

						isFileFound = true;
						break;
					}
				}
				
				config_file.close();

				if(!isFileFound){
					new_file.push_back(entry.path().string());
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

		if (new_file.size() == 0){
			Log(INFO, "NO NEW FILE!");
		}else{
			Log(GREEN, "NEW FILES:");
			for(std::string f : new_file){
				Log(GREEN," + " + f);
			}
		}
		
		if (updated_file.size() == 0){
			Log(INFO, "NO FILE MODIFIED!");
		}else{
			Log(VOILET, "MODIFIED FILES:");
			for(std::string f : updated_file){
				Log(VOILET," ~ " + f);
			}
		}

		return 0;


	});

	add_argument(&cmd,"add" , [&](int arc, char* arv[]) -> int{
		std::string items[arc - 2];
		int count = 2;

		while( count < arc ){
			items[count - 2] = arv[count];
			count++;
		}
		
		
		if(arc - 2 <=  0){
			Log(ERROR, "COM'ON NIGGA, GIVE US SOME FILES!");
			return -1;
		}

		if(fileExists(config_file_path) == 0) {
			std::ofstream config_file;
			config_file.open(config_file_path, std::ios::app);
			for (std::string item : items){
				Log(item);
				if(item == "."){
					item = "./";
				}else if (item.rfind("/", 0) == 0){
					item = "." + item;
				}else if(item.rfind("./", 0 ) != 0){
					item = "./" + item;
				}
				add_scan(item);
			}
			config_file << config;
			config_file.close();
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

