#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "User.h"

// TCPServer����, ����->start()->acceptClient()
class TCPServer : public Socket
{
public:
	TCPServer();
	bool start(int port, const char *ip = 0);		// ���� ����, ��������ȭ
	void stop();								// closesocket()
	SOCKET acceptClient();						// accept() �������� ����
};

#endif
