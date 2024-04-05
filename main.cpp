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

std::vector<std::string> old_files;
std::vector<std::string> new_files;
std::vector<std::string> updated_files;
std::vector<std::string> deleted_files;
std::vector<std::string> removed_files;

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
					updated_files.push_back(name);
				}else {
					old_files.push_back(name);
					final_hash = old_hash;
				}
				isFileFound = true;
				break;
			}
		}
		
		config_file.close();

		if(isFileFound){

			std::string res = name + ": " + final_hash + "\n";
			config += res; // add line to config

		}else{

			std::string hash = getFileHash(name.c_str());
			std::string res = name + ": " + hash + "\n";
			new_files.push_back(name);
			config += res;	//add line to config

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
							updated_files.push_back(entry.path().string());
						}else {
							old_files.push_back(entry.path().string());
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
					new_files.push_back(entry.path().string());
					config += res;				

				}
			}
		}
	}
}

void scan_files(std::string name){

	if(fileExists(config_file_path) != 0) {
		Log(ERROR, "NO CONFIG FILE!");
	}
	else{
		for (const auto & entry : std::filesystem::directory_iterator(name)){

			if (entry.path().string() == "./.git") continue;
			if (entry.path().string() == " ") continue;
			if (entry.path().string() == "\n") continue;
			if (entry.path().string() == config_file_path) continue;
			
			if(std::filesystem::is_directory(entry.path())){
				all_dirs.push_back(entry.path().string());
				scan_files(entry.path().string());
			}else{
				all_files.push_back(entry.path().string());
			}
		}
	}
}

void scan_status(){
	if(fileExists(config_file_path) != 0) {
		Log(ERROR, "NO CONFIG FILE!");
	}
	else{
		
		std::string line;
		std::ifstream config_file(config_file_path);

		while(getline(config_file, line))
		{
			
			if (line.size() < 10) continue;

			int pos = line.find(":");
			std::string name  = line.substr(0 , pos);
			
			if(fileExists(name.c_str()) != 0){
				removed_files.push_back(name);
			}else{
				for (std::string item : all_files){
					if(item == name){
						std::string old_hash = line.substr( line.size() - 64, line.size());
						std::string new_hash = getFileHash(item.c_str());
		
						if(old_hash != new_hash){
							updated_files.push_back(item);
						}else {
							old_files.push_back(item);
						}
						break;
					}
				}
			}
		}
		config_file.close();
	}
}

void scan_new_files(){
	std::vector<std::string> temp_f;
	std::string line;
	std::ifstream config_file(config_file_path);

	while(getline(config_file, line))
	{
		int pos = line.find(":");
		std::string name  = line.substr(0 , pos);
		temp_f.push_back(name);
	}
	config_file.close();

	for(std::string item : all_files){
		bool found = false;
		for(std::string t : temp_f){
			if(t == item)
			{
				found = true;
				break;
			}
		}
		if(!found){
			new_files.push_back(item);
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
			file << "\n";
			file.close();
			Log("config file initialized at current directory");
			return 0;
		}
		Log("config file already exist!");
		return 0;
	});

	
	add_argument(&cmd,"status" , [&](int arc, char* arv[]) -> int{

		scan_files("./");	
		scan_status();		
		scan_new_files();

		if (new_files.size() == 0){
			Log(INFO, "NO NEW FILE!");
		}else{
			Log(GREEN, "NEW FILES:");
			for(std::string f : new_files){
				Log(GREEN," + " + f);
			}
		}
		
		if (updated_files.size() == 0){

			Log(INFO, "NO FILE MODIFIED!");
		}else{
			Log(VOILET, "MODIFIED FILES:");
			for(std::string f : updated_files){
				Log(VOILET," ~ " + f);
			}
		}
		
		if (removed_files.size() == 0){
			Log(INFO, "NO DELETED FILE!");
		}else{
			Log(GREEN, "DELETED FILES:");
			for(std::string f : removed_files){
				Log(RED," + " + f);
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

