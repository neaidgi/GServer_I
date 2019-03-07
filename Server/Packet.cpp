#include "Packet.h"
#include "MsgManager.h"
#include "CriticalSectionManager.h"

// 생성자
Packet::Packet()
{
	sendSize = 0;
	sentSize = 0;
	recvSize = 0;
	recvedSize = 0;
	sending = false;
}

// TCPClient 생성자에 인수 넣어줌
Packet::Packet(SOCKET _socket, SOCKADDR_IN _addr): TCPClient(_socket, _addr)
{
	sendSize = 0;
	sentSize = 0;
	recvSize = 0;
	recvedSize = 0;
	sending = false;

	ZeroMemory(&sendEx.overlapped, sizeof(sendEx.overlapped));
	ZeroMemory(&recvEx.overlapped, sizeof(recvEx.overlapped));
	ZeroMemory(&os_sendEx.overlapped, sizeof(os_sendEx.overlapped));
	sendEx.p = this;
	recvEx.p = this;
	os_sendEx.p = this;
	sendEx.type = IOTYPE_SEND;
	recvEx.type = IOTYPE_RECV;
	os_sendEx.type = IOTYPE_ONESIDED_SEND;
}

// 데이터 전송
bool Packet::sendMsg()
{
	char* ptr = sendBuf;
	int retval = send(sock, ptr + sentSize, sendSize - sentSize, 0);
	if (retval == SOCKET_ERROR)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("Packet::sendMsg : ERROR : send() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPClient send()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}
	else
	{
		// 보낸양
		sentSize += retval;
		return true;
	}
}

// 데이터 받기
bool Packet::recvMsg()
{
	if (recvSize == 0)				// 총 받을 양이 0 이라면
	{
		char* ptr = recvBuf;
		int retval = recv(sock, ptr + recvedSize, sizeof(int) - recvedSize, 0);
		if (retval == SOCKET_ERROR)
		{
			LogManager::GetInstance()->SetTime();
			LogManager::GetInstance()->LogWrite("Packet::recvMsg : ERROR : recv() result = SOCKET_ERROR");
			ErrorManager::GetInstance()->err_display("Packet first_recv()");
			return false;
		}
		else if (retval == 0)
		{
			return false;
		}
		else
		{
			// 받은양 더함
			recvedSize += retval;

			// 4바이트 다 받았으면
			if (recvedSize == 4)
			{
				// 총 받을양 세팅 -> 아래 if문으로 나머지 데이터 받음
				memcpy(&recvSize, ptr, sizeof(int));
				memset(ptr, 0, sizeof(recvBuf));
				recvedSize = 0;
			}
		}
	}
	if (recvSize > 0)				// 총 받을 양이 0 초과라면
	{
		char* ptr = recvBuf;
		int retval = recv(sock, ptr + recvedSize, recvSize - recvedSize, 0);
		if (retval == SOCKET_ERROR)
		{
			LogManager::GetInstance()->SetTime();
			LogManager::GetInstance()->LogWrite("Packet::recvMsg : ERROR : recv() result = SOCKET_ERROR");
			ErrorManager::GetInstance()->err_display("Packet second_recv()");
			return false;
		}
		else if (retval == 0)
		{
			return false;
		}
		else
		{
			recvedSize += retval;
		}
	}
	return true;
}


bool Packet::IOCP_SendMsg()
{
	int retval;
	DWORD sendbytes;
	DWORD flags;

	ZeroMemory(&sendEx.overlapped, sizeof(sendEx.overlapped));

	sendwsabuf.buf = sendBuf + sentSize;
	sendwsabuf.len = sendSize - sentSize;

	retval = WSASend(sock, &sendwsabuf, 1, &sendbytes, 0, &sendEx.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		int errorcode = WSAGetLastError();
		LPVOID lpMsgBuf;

		if (errorcode != WSA_IO_PENDING)
		{
			LogManager::GetInstance()->SetTime();
			LogManager::GetInstance()->LogWrite("Packet::IOCP_SendMsg : ERROR : WSASend() result = SOCKET_ERROR");
			char err[BUFSIZE];
			memset(err, 0, sizeof(err));
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, errorcode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL);
			sprintf(err, "Packet second_WSAsend() ERROR CODE : %d MSG : %s", errorcode, lpMsgBuf);
			printf("%s", err);
			//ErrorManager::GetInstance()->err_display(err);

			return false;
		}
	}

	return true;

}

bool Packet::IOCP_RecvMsg()
{
	int retval;
	DWORD recvbytes;
	DWORD flags = 0;

	ZeroMemory(&recvEx.overlapped, sizeof(recvEx.overlapped));

	recvwsabuf.buf = recvBuf + recvedSize;
	recvwsabuf.len = BUFSIZE - recvedSize;

	retval = WSARecv(sock, &recvwsabuf, 1, &recvbytes, &flags, &recvEx.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LogManager::GetInstance()->SetTime();
			LogManager::GetInstance()->LogWrite("Packet::IOCP_RecvMsg : ERROR : WSARecv() result = SOCKET_ERROR");
			ErrorManager::GetInstance()->err_display("Packet second_WSArecv()");
			return false;
		}
	}

	return true;
}


void Packet::ClearSendQueue()
{
	CriticalSectionManager::GetInstance()->Enter();
	while (sendQueue.empty() == false)
	{
		SendPacket* temp = sendQueue.front();
		delete temp;
		sendQueue.pop();
	}
	CriticalSectionManager::GetInstance()->Leave();
}

// send 큐에서 패킷 꺼내오기
bool Packet::TakeOutSendPacket()
{
	CriticalSectionManager::GetInstance()->Enter();
	char msg[BUFSIZE];
	sprintf(msg, "꺼내기 전 소켓: [%d] SendQueue Size: [%d]", sock, GetSendQueueSize());
	MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	if (isSendQueue() && isSending() == false)
	{
		SendPacket* sendpacket = sendQueue.front();
		memcpy(sendBuf, sendpacket->sendBuf, sendpacket->sendSize);
		sendSize = sendpacket->sendSize;

		delete sendpacket;
		sendQueue.pop();
		sending = true;
		CriticalSectionManager::GetInstance()->Leave();
		char msg[BUFSIZE];
		sprintf(msg, "꺼낸 후 소켓: [%d] SendQueue Size: [%d]", sock, GetSendQueueSize());
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}
	else
	{
		CriticalSectionManager::GetInstance()->Leave();
		return false;
	}
}

// Data 패킹
void Packet::pack(PROTOCOL p, void * data, int size)
{
	CriticalSectionManager::GetInstance()->Enter();
	memset(sendBuf, 0, sizeof(sendBuf));

	char* ptr = sendBuf;
	sendSize = sizeof(PROTOCOL) + size;

	memcpy(ptr, &sendSize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &p, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, data, size);

	sendSize += sizeof(size);			// 보낼 사이즈
	sentSize = 0;						// 보낸 사이즈(초기화)

	// 암호화
	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
	CriticalSectionManager::GetInstance()->Leave();
}

// Data 패킹 Send 큐사용
void Packet::Quepack(PROTOCOL p, void * data, int size)
{
	CriticalSectionManager::GetInstance()->Enter();

	SendPacket* temp = new SendPacket();

	char* ptr = temp->sendBuf;

	temp->sendSize = sizeof(PROTOCOL) + size;

	memcpy(ptr, &temp->sendSize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &p, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, data, size);

	temp->sendSize += sizeof(size);		// 보낼 사이즈
	
	// 암호화
	EncryptManager::GetInstance()->encoding(temp->sendBuf + sizeof(PROTOCOL), temp->sendSize);
	// 큐에 넣기

	sendQueue.push(temp);

	CriticalSectionManager::GetInstance()->Leave();
}

void Packet::bitpack(PROTOCOL p, void * data, int size)
{
	memset(sendBuf, 0, sizeof(sendBuf));

	char* ptr = sendBuf;
	sendSize = sizeof(PROTOCOL) + size;

	memcpy(ptr, &sendSize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &p, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, data, size);

	sendSize += sizeof(size);			// 보낼 사이즈
	sentSize = 0;						// 보낸 사이즈(초기화)

	// 암호화
	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
}

// Data 언패킹
void Packet::unPack(PROTOCOL * p, void * data)
{
	CriticalSectionManager::GetInstance()->Enter();

	char* ptr = recvBuf;
	PROTOCOL protocol;

	EncryptManager::GetInstance()->decoding(recvBuf, recvSize);

	memcpy(&protocol, ptr, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(data, ptr, recvSize - sizeof(PROTOCOL));
	*p = protocol;

	IOCP_InitializeBuffer();

	CriticalSectionManager::GetInstance()->Leave();
}

bool Packet::isSendQueue()
{
	CriticalSectionManager::GetInstance()->Enter();
	if (sendQueue.empty() == false)
	{
		CriticalSectionManager::GetInstance()->Leave();
		return true;
	}
	CriticalSectionManager::GetInstance()->Leave();
	return false;
}

bool Packet::isSendQueueSending()
{
	CriticalSectionManager::GetInstance()->Enter();
	if (sendQueue.empty() == false)
	{
		CriticalSectionManager::GetInstance()->Leave();
		return true;

	}
	CriticalSectionManager::GetInstance()->Leave();
	return false;
}

// sendSize만큼 보냈는지
bool Packet::isSendSuccess()
{
	//CriticalSectionManager::GetInstance()->Enter();
	if (sendSize == sentSize)
		return true;
	else
		return false;
	//CriticalSectionManager::GetInstance()->Leave();
}

// recvSize만큼 받았는지
bool Packet::isRecvSuccess()
{
	//CriticalSectionManager::GetInstance()->Enter();
	if (recvSize == 0)
	{
		if (recvedSize == 0)
			return true;
		else
			return false;
	}
	else
	{
		if (recvSize == recvedSize)
			return true;
		else
			return false;
	}
	//CriticalSectionManager::GetInstance()->Leave();
}

bool Packet::IOCP_isSendSuccess(int _sentbyte)
{
	CriticalSectionManager::GetInstance()->Enter();
	sentSize += _sentbyte;
	if (sentSize == sendSize)
	{
		sentSize = 0;
		sendSize = 0;
		sending = false;
		CriticalSectionManager::GetInstance()->Leave();
		return true;
	}
	CriticalSectionManager::GetInstance()->Leave();
	return false;
}

bool Packet::IOCP_isRecvSuccess(int _recvedSize)
{
	if (recvSize < sizeof(int))
	{
		recvedSize += _recvedSize;

		if (recvedSize >= sizeof(int))
		{
			memcpy(&recvSize, recvBuf, sizeof(int));

			recvedSize = recvedSize - sizeof(int);

			memcpy(&recvBuf, recvBuf + sizeof(int), recvedSize);

			if (recvedSize >= recvSize)
			{
				recvedSize -= recvSize;
				return true;
			}
			else
			{
				if (!IOCP_RecvMsg())
				{
					return false;
				}

				return false;
			}
		}
		else
		{
			if (!IOCP_RecvMsg())
			{
				return false;
			}
		}
	}

	recvedSize += _recvedSize;

	if (recvedSize >= recvSize)
	{
		recvedSize -= recvSize;

		return true;
	}
	else
	{
		if (!IOCP_RecvMsg())
		{
			return false;
		}

		return false;
	}
	
}

void Packet::IOCP_InitializeBuffer()
{
	CriticalSectionManager::GetInstance()->Enter();
	memcpy(recvBuf, recvBuf + recvSize, recvedSize);
	recvSize = 0;
	CriticalSectionManager::GetInstance()->Leave();
}