#ifndef PACKET_H
#define	PACKET_H

#include "TCPClient.h"
#include "EnryptManager.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include <queue>

struct SendPacket
{
	char sendBuf[BUFSIZE];
	int sendSize;

	SendPacket()
	{
		sendSize = 0;
		memset(sendBuf, 0, sizeof(sendBuf));
	}
};

//struct RecvPacket
//{
//	char recvBuf[BUFSIZE];
//	int recvsize;
//
//	RecvPacket()
//	{
//		recvsize = 0;
//		memset(recvBuf, 0, sizeof(recvBuf));
//	}
//};


class Packet : public TCPClient
{
private:
	WSAOVERLAPPEDEx sendEx;
	WSAOVERLAPPEDEx os_sendEx;
	WSAOVERLAPPEDEx recvEx;
	
	queue<SendPacket*>sendQueue;		// send 큐
	//queue<RecvPacket*>recvQueue;		// recv 큐

	char sendBuf[BUFSIZE];
	char recvBuf[BUFSIZE];
	int sendSize;				// 총 보낼 양
	int sentSize;				// 보낸 양
	int recvSize;				// 총 받을 양
	int recvedSize;				// 받은 양
	bool sending;
	WSABUF sendwsabuf;
	WSABUF recvwsabuf;
public:
	Packet();
	Packet(SOCKET _socket, SOCKADDR_IN _addr);
	~Packet() 
	{
		ClearSendQueue();
	}

	int GetSendQueueSize() { return sendQueue.size(); }
	bool isSending() { return sending; }
	bool sendMsg();
	bool recvMsg();

	bool IOCP_SendMsg();
	bool IOCP_RecvMsg();

	void ClearSendQueue();
	bool TakeOutSendPacket();

	void Quepack(PROTOCOL p, void *data, int size);
	void pack(PROTOCOL p, void *data, int size);
	void bitpack(PROTOCOL p, void *data, int size);
	void unPack(PROTOCOL *p, void *data);
	bool isSendSuccess();
	bool isRecvSuccess();

	bool isSendQueue();
	bool isSendQueueSending();

	bool IOCP_isSendSuccess(int _sentbyte);
	bool IOCP_isRecvSuccess(int _sentbyte);
	void IOCP_InitializeBuffer();
};

#endif
