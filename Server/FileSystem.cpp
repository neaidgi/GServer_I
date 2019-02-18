#include "FileSystem.h"

FileSystem::FileSystem()
{
	read = new ifstream();
	write = new ofstream();
}

FileSystem::~FileSystem()
{
	delete read;
	delete write;
}

bool FileSystem::OpenReadFile(char* _path, char * _name, char* _msg)
{
	char filepath[255];

	sprintf(filepath, "%s/%s", _path, _name);	// 경로 이름 연결
	read->open(filepath, ios::in);				// 읽기전용 오픈
	
	bool is_open = read->is_open();

	if (is_open)
	{
		return true;
	}
	else
	{
		char* str = "파일이 존재하지않음";
		int len = strlen(str);
		memcpy(_msg, str, len);
		return false;
	}
}

bool FileSystem::OpenWriteFile(char * _path, char * _name, char * _msg, int _mode)
{
	char filepath[255];

	sprintf(filepath, "%s/%s", _path, _name);	// 경로 이름 연결

	switch (_mode)
	{
	case 1:
		write->open(filepath, ios::trunc);				// 쓰기전용 파일 존재하는 파일 지우고 새로만듬
		break;
	case 0:
		write->open(filepath, ios::out);				// 쓰기전용 파일
		break;
	}

	bool is_open = read->is_open();

	if (is_open)
	{
		return true;
	}
	else
	{
		char* str = "파일이 존재하지않음";
		int len = strlen(str);
		memcpy(_msg, str, len);
		return false;
	}
}
