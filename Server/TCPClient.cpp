#include "TCPClient.h"
#include "ErrorManager.h"
#include "LogManager.h"

// ������
TCPClient::TCPClient()
{
}

// ������
TCPClient::TCPClient(SOCKET _socket, SOCKADDR_IN _addr) : Socket(_socket, _addr)
{
}

// TCP���� ����(Ŭ���̾�Ʈ��)
bool TCPClient::createSocket()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPClient::createSocket : ERROR : socket() result = INVALID_SOCKET");
		ErrorManager::GetInstance()->err_display("createSocket socket()");
		return false;
	}

	return true;
}

// connect 
bool TCPClient::connect_client(const char * ip, int port)
{
	SOCKADDR_IN serveraddr;							// ���� �ּ� ����ü �غ�. �����û�� ������ ������ ����.
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;				//IP4Ÿ��
	serveraddr.sin_addr.s_addr = inet_addr(ip);	    //IP�ּ�
	serveraddr.sin_port = htons(port);			    //��Ʈ��ȣ

	int retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)						// ���� ó��
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPClient::connect_client : ERROR : connect() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPClient connect()");
		return false;
	}
	else
	{
		return true;
	}
}

// ���� ����
void TCPClient::stop()
{
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	closesocket(sock);
}