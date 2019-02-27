#include "Packet.h"


// ������
Packet::Packet()
{
	sendSize = 0;
	sentSize = 0;
	recvSize = 0;
	recvedSize = 0;
}

// TCPClient �����ڿ� �μ� �־���
Packet::Packet(SOCKET _socket, SOCKADDR_IN _addr): TCPClient(_socket, _addr)
{
	sendSize = 0;
	sentSize = 0;
	recvSize = 0;
	recvedSize = 0;

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

// ������ ����
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
		// ������
		sentSize += retval;
		return true;
	}
}

// ������ �ޱ�
bool Packet::recvMsg()
{
	if (recvSize == 0)				// �� ���� ���� 0 �̶��
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
			// ������ ����
			recvedSize += retval;

			// 4����Ʈ �� �޾�����
			if (recvedSize == 4)
			{
				// �� ������ ���� -> �Ʒ� if������ ������ ������ ����
				memcpy(&recvSize, ptr, sizeof(int));
				memset(ptr, 0, sizeof(recvBuf));
				recvedSize = 0;
			}
		}
	}
	if (recvSize > 0)				// �� ���� ���� 0 �ʰ����
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

bool Packet::IOCP_OneSided_SendMsg()
{
	int retval;
	DWORD sendbytes;
	DWORD flags;

	ZeroMemory(&os_sendEx.overlapped, sizeof(os_sendEx.overlapped));

	sendwsabuf.buf = sendBuf + sentSize;
	sendwsabuf.len = sendSize - sentSize;
	retval = WSASend(sock, &sendwsabuf, 1, &sendbytes, 0, &os_sendEx.overlapped, NULL);
	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LogManager::GetInstance()->SetTime();
			LogManager::GetInstance()->LogWrite("Packet::IOCP_SendMsg : ERROR : WSASend() result = SOCKET_ERROR");
			ErrorManager::GetInstance()->err_display("Packet OneSided_WSAsend()");
			return false;
		}
	}
	return true;
}

// Data ��ŷ
void Packet::pack(PROTOCOL p, void * data, int size)
{
	memset(sendBuf, 0, sizeof(sendBuf));

	char* ptr = sendBuf;
	sendSize = sizeof(PROTOCOL) + size;

	memcpy(ptr, &sendSize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, &p, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, data, size);

	sendSize += sizeof(size);			// ���� ������
	sentSize = 0;						// ���� ������(�ʱ�ȭ)

	// ��ȣȭ
	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
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

	sendSize += sizeof(size);			// ���� ������
	sentSize = 0;						// ���� ������(�ʱ�ȭ)

	// ��ȣȭ
	EncryptManager::GetInstance()->encoding(sendBuf + sizeof(PROTOCOL), sendSize);
}

// Data ����ŷ
void Packet::unPack(PROTOCOL * p, void * data)
{
	char* ptr = recvBuf;
	PROTOCOL protocol;

	EncryptManager::GetInstance()->decoding(recvBuf, recvSize);

	memcpy(&protocol, ptr, sizeof(PROTOCOL));
	ptr += sizeof(PROTOCOL);

	memcpy(data, ptr, recvSize - sizeof(PROTOCOL));
	*p = protocol;

	recvSize = 0;						// �ʱ�ȭ
	recvedSize = 0;						// �ʱ�ȭ
}

// sendSize��ŭ ���´���
bool Packet::isSendSuccess()
{
	if (sendSize == sentSize)
		return true;
	else
		return false;
}

// recvSize��ŭ �޾Ҵ���
bool Packet::isRecvSuccess()
{
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
}

bool Packet::IOCP_isSendSuccess(int _sentbyte)
{
	sentSize += _sentbyte;
	if (sentSize == sendSize)
	{
		sentSize = 0;
		sendSize = 0;

		return true;
	}
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