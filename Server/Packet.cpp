#include "Packet.h"
#include "MsgManager.h"
#include "CriticalSection.h"

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
Packet::Packet(SOCKET _socket, SOCKADDR_IN _addr) : TCPClient(_socket, _addr)
{
	sendSize = 0;
	sentSize = 0;
	recvSize = 0;
	recvedSize = 0;
	sending = false;

	memset(sendBuf, 0, sizeof(sendBuf));
	memset(recvBuf, 0, sizeof(recvBuf));

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
	CThreadSync cs;
	while (sendQueue.empty() == false)
	{
		SendPacket* temp = sendQueue.front();
		delete temp;
		sendQueue.pop();
	}
}

// send 큐에서 패킷 꺼내오기
bool Packet::TakeOutSendPacket()
{
	CThreadSync cs;

	//char msg[BUFSIZE];
	//sprintf(msg, "꺼내기 전 소켓: [%d] SendQueue Size: [%d]", sock, GetSendQueueSize());
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	if (isSendQueue() && isSending() == false)
	{
		SendPacket* sendpacket = sendQueue.front();
		memcpy(sendBuf, sendpacket->sendBuf, sendpacket->sendSize);
		sendSize = sendpacket->sendSize;

		delete sendpacket;
		sendQueue.pop();
		sending = true;
		//char msg[BUFSIZE];
		//sprintf(msg, "꺼낸 후 소켓: [%d] SendQueue Size: [%d]", sock, GetSendQueueSize());
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return true;
	}
	else
	{
		return false;
	}
}

// Data 패킹 Send 큐사용. BitProtocol 추가
void Packet::Quepack(UINT64 p, void * data, int size)
{
	CThreadSync cs;

	SendPacket* temp = new SendPacket();

	char* ptr = temp->sendBuf;

	temp->sendSize = sizeof(UINT64) + size;

	memcpy(ptr, &temp->sendSize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &p, sizeof(UINT64));
	ptr += sizeof(UINT64);

	memcpy(ptr, data, size);

	temp->sendSize += sizeof(size);		// 보낼 사이즈

	// 암호화
	EncryptManager::GetInstance()->encoding(temp->sendBuf + sizeof(int), temp->sendSize);
	// 큐에 넣기

	sendQueue.push(temp);
}

// 비트연산 프로토콜 pack. _existingprotocol : 기존 프로토콜, _additionalprotocol : 추가할 프로토콜
UINT64 Packet::BitPackProtocol(UINT64 _existingprotocol, UINT64 _additionalprotocol)
{
	UINT64 protocol = 0;
	protocol = _existingprotocol | _additionalprotocol;

	return protocol;
}

// 비트연산 프로토콜 pack. 기존프로토콜, 큰틀프로토콜, 중간틀프로토콜, 세부프로토콜
UINT64 Packet::BitPackProtocol(UINT64 _existingprotocol, UINT64 _first_additionalprotocol, UINT64 _second_additionalprotocol, UINT64 _third_additionalprotocol)
{
	UINT64 protocol = 0;
	protocol = _existingprotocol | _first_additionalprotocol;
	protocol = protocol | _second_additionalprotocol;
	protocol = protocol | _third_additionalprotocol;

	return protocol;
}

// 비트 연산 프로토콜 unpack
void Packet::BitunPack(UINT64 & _p, void * _data)
{
	CThreadSync cs;

	char* ptr = recvBuf;
	UINT64 protocol = 0;

	EncryptManager::GetInstance()->decoding(recvBuf, recvSize);

	memcpy(&protocol, ptr, sizeof(protocol));
	ptr += sizeof(protocol);

	int int64size = sizeof(protocol);

	memcpy(_data, ptr, recvSize - sizeof(protocol));
	_p = protocol;

	IOCP_InitializeBuffer();
}

bool Packet::isSendQueue()
{
	CThreadSync cs;
	if (sendQueue.empty() == false)
	{
		return true;
	}
	return false;
}

bool Packet::isSendQueueSending()
{
	CThreadSync cs;
	if (sendQueue.empty() == false)
	{
		return true;

	}
	return false;
}

bool Packet::IOCP_isSendSuccess(int _sentbyte)
{
	CThreadSync cs;
	sentSize += _sentbyte;
	if (sentSize == sendSize)
	{
		sentSize = 0;
		sendSize = 0;
		sending = false;
		return true;
	}
	return false;
}

bool Packet::IOCP_isRecvSuccess(int _recvedSize)
{
	CThreadSync cs;

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

				//char msg[BUFSIZE];
				//sprintf(msg, "IOCP_isRecvSuccess: sock : [%d] recvedSize : [%d]", sock, recvedSize);
				//MsgManager::GetInstance()->DisplayMsg("[recvedSize] INFO ", msg);
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

bool Packet::IOCP_isRecvSuccess()
{
	CThreadSync cs;

	if (recvSize < sizeof(int)) // recvSize를 받아야한다면
	{
		// 받은크기가 받아야할 크기 보다 크면
		if (recvedSize >= sizeof(int))
		{
			// 받아야할 크기 저장
			memcpy(&recvSize, recvBuf, sizeof(int));
			// 받은크기에서 받아야할크기(sizeof(int)) 빼기
			recvedSize = recvedSize - sizeof(int);

			memcpy(&recvBuf, recvBuf + sizeof(int), recvedSize);

			if (recvedSize >= recvSize)
			{
				recvedSize -= recvSize;

				return true;
			}
			else // 받아야할 크기보다 받은크기가 작을때
			{
				return false;
			}
		}
		else // 받은 크기가 받아야할 크기 보다 작으면
		{
			return false;
		}
	}

	// 크기는 알고있을때
	if (recvedSize >= recvSize)
	{
		memcpy(&recvBuf, recvBuf, recvedSize);

		recvedSize -= recvSize;
		return true;
	}
	else
	{
		return false;
	}
}

void Packet::IOCP_InitializeBuffer()
{
	CThreadSync cs;
	memcpy(recvBuf, recvBuf + recvSize, recvedSize);
	recvSize = 0;
}


//// Data 패킹
//void Packet::pack(PROTOCOL p, void * data, int size)
//{
//	CThreadSync cs;
//	memset(sendBuf, 0, sizeof(sendBuf));
//
//	char* ptr = sendBuf;
//	sendSize = sizeof(PROTOCOL) + size;
//
//	memcpy(ptr, &sendSize, sizeof(int));
//	ptr += sizeof(int);
//
//	memcpy(ptr, &p, sizeof(PROTOCOL));
//	ptr += sizeof(PROTOCOL);
//
//	memcpy(ptr, data, size);
//
//	sendSize += sizeof(size);			// 보낼 사이즈
//	sentSize = 0;						// 보낸 사이즈(초기화)
//
//	// 암호화
//	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
//}

//// Data 패킹 Send 큐사용
//void Packet::Quepack(PROTOCOL p, void * data, int size)
//{
//	CThreadSync cs;
//
//	SendPacket* temp = new SendPacket();
//
//	char* ptr = temp->sendBuf;
//
//	temp->sendSize = sizeof(PROTOCOL) + size;
//
//	memcpy(ptr, &temp->sendSize, sizeof(int));
//	ptr += sizeof(int);
//
//	memcpy(ptr, &p, sizeof(PROTOCOL));
//	ptr += sizeof(PROTOCOL);
//
//	memcpy(ptr, data, size);
//
//	temp->sendSize += sizeof(size);		// 보낼 사이즈
//
//	// 암호화
//	EncryptManager::GetInstance()->encoding(temp->sendBuf + sizeof(PROTOCOL), temp->sendSize);
//	// 큐에 넣기
//
//	sendQueue.push(temp);
//}

//void Packet::bitpack(PROTOCOL p, void * data, int size)
//{
//	memset(sendBuf, 0, sizeof(sendBuf));
//
//	char* ptr = sendBuf;
//	sendSize = sizeof(PROTOCOL) + size;
//
//	memcpy(ptr, &sendSize, sizeof(int));
//	ptr += sizeof(int);
//
//	memcpy(ptr, &p, sizeof(PROTOCOL));
//	ptr += sizeof(PROTOCOL);
//
//	memcpy(ptr, data, size);
//
//	sendSize += sizeof(size);			// 보낼 사이즈
//	sentSize = 0;						// 보낸 사이즈(초기화)
//
//	// 암호화
//	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
//}

//// Data 언패킹
//void Packet::unPack(PROTOCOL * p, void * data)
//{
//	CThreadSync cs;
//
//	char* ptr = recvBuf;
//	PROTOCOL protocol;
//
//	EncryptManager::GetInstance()->decoding(recvBuf, recvSize);
//
//	memcpy(&protocol, ptr, sizeof(PROTOCOL));
//	ptr += sizeof(PROTOCOL);
//
//	memcpy(data, ptr, recvSize - sizeof(PROTOCOL));
//	*p = protocol;
//
//	IOCP_InitializeBuffer();
//}

//// sendSize만큼 보냈는지
//bool Packet::isSendSuccess()
//{
//	// Enter();
//	if (sendSize == sentSize)
//		return true;
//	else
//		return false;
//	// Leave();
//}

//// recvSize만큼 받았는지
//bool Packet::isRecvSuccess()
//{
//	// Enter();
//	if (recvSize == 0)
//	{
//		if (recvedSize == 0)
//			return true;
//		else
//			return false;
//	}
//	else
//	{
//		if (recvSize == recvedSize)
//			return true;
//		else
//			return false;
//	}
//	// Leave();
//}