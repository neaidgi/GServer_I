#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "User.h"

// TCPServer家南, 积己->start()->acceptClient()
class TCPServer : public Socket
{
public:
	TCPServer();
	bool start(int port, const char *ip = 0);		// 家南 积己, 府郊家南拳
	void stop();								// closesocket()
	SOCKET acceptClient();						// accept() 蜡历家南 积己
};

#endif
