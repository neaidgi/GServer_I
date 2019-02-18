#pragma once

#include "Global.h"
#include <fstream>
#include <iostream>

using namespace std;

class FileSystem {
private:
	ifstream* read;
	ofstream* write;
public:
	FileSystem();
	~FileSystem();

	bool OpenReadFile(char* _path, char* _name, char* _msg);
	bool OpenWriteFile(char* _path, char* _name, char* _msg, int _mode);
};