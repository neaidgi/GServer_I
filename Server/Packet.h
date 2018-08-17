#ifndef PACKET_H
#define	PACKET_H

#include "TCPClient.h"
#include "EnryptManager.h"
#include "ErrorManager.h"
#include "LogManager.h"

class Packet : public TCPClient
{
private:
	WSAOVERLAPPEDEx sendEx;
	WSAOVERLAPPEDEx recvEx;
	char sendBuf[BUFSIZE];
	char recvBuf[BUFSIZE];
	int sendSize;				// 총 보낼 양
	int sentSize;				// 보낸 양
	int recvSize;				// 총 받을 양
	int recvedSize;				// 받은 양
	WSABUF sendwsabuf;
	WSABUF recvwsabuf;
public:
	Packet();
	Packet(SOCKET _socket, SOCKADDR_IN _addr);
	bool sendMsg();
	bool recvMsg();

	bool IOCP_SendMsg();
	bool IOCP_RecvMsg();
	void pack(PROTOCOL p, void *data, int size);
	void bitpack(PROTOCOL p, void *data, int size);
	void unPack(PROTOCOL *p, void *data);
	bool isSendSuccess();
	bool isRecvSuccess();
	bool IOCP_isSendSuccess(int _sentbyte);
	bool IOCP_isRecvSuccess(int _sentbyte);
};

#endif
