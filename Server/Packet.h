#ifndef PACKET_H
#define	PACKET_H

#include "TCPClient.h"
#include "EnryptManager.h"
#include "ErrorManager.h"
#include "LogManager.h"

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
	void Quepack(INT64 p, void *data, int size);
	void pack(PROTOCOL p, void *data, int size);
	void bitpack(PROTOCOL p, void *data, int size);
	void unPack(PROTOCOL *p, void *data);
	bool isSendSuccess();
	bool isRecvSuccess();
	// 비트연산 프로토콜 pack. _existingprotocol : 기존 프로토콜, _additionalprotocol : 추가할 프로토콜
	INT64 BitPackProtocol(INT64 _existingprotocol,INT64 _additionalprotocol);

	// 비트 연산 프로토콜 unpack
	void BitunPack(INT64 *_p, void* _data);

	// 큰틀 결과

	//

	bool isSendQueue();
	bool isSendQueueSending();

	bool IOCP_isSendSuccess(int _sentbyte);
	bool IOCP_isRecvSuccess(int _sentbyte);
	bool IOCP_isRecvSuccess();
	void IOCP_InitializeBuffer();
};

#endif
