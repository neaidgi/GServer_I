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

	sprintf(filepath, "%s/%s", _path, _name);	// ��� �̸� ����
	read->open(filepath, ios::in);				// �б����� ����
	
	bool is_open = read->is_open();

	if (is_open)
	{
		return true;
	}
	else
	{
		char* str = "������ ������������";
		int len = strlen(str);
		memcpy(_msg, str, len);
		return false;
	}
}

bool FileSystem::OpenWriteFile(char * _path, char * _name, char * _msg, int _mode)
{
	char filepath[255];

	sprintf(filepath, "%s/%s", _path, _name);	// ��� �̸� ����

	switch (_mode)
	{
	case 1:
		write->open(filepath, ios::trunc);				// �������� ���� �����ϴ� ���� ����� ���θ���
		break;
	case 0:
		write->open(filepath, ios::out);				// �������� ����
		break;
	}

	bool is_open = read->is_open();

	if (is_open)
	{
		return true;
	}
	else
	{
		char* str = "������ ������������";
		int len = strlen(str);
		memcpy(_msg, str, len);
		return false;
	}
}
