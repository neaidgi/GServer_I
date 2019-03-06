#include "TCPServer.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"

TCPServer::TCPServer()
{
}

// ���� ���� �� ������������ ����� �ش�.
bool TCPServer::start(int port, const char * ip)
{
	// ���� ����
	SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_socket == INVALID_SOCKET)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPServer::start : ERROR : socket() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPServer socket()");
		return false;
	}

	addr.sin_family = AF_INET;
	if (ip == 0)
		addr.sin_addr.s_addr = htonl(INADDR_ANY); // �μ��� ���� ip�� null���̸� �ڵ� �Ҵ�
	else
		addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	int retval;
	retval = bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPServer::start : ERROR : bind() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPServer bind()");
		return false;
	}

	// ������������ ����
	retval = listen(listen_socket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPServer::start : ERROR : listen() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPServer listen()");
		return false;
	}

	// ���� ������������ �ʱ�ȭ
	sock = listen_socket;

	return true;
}

// closesocket
void TCPServer::stop()
{
	closesocket(sock);
}

// accept, ���� ����
SOCKET TCPServer::acceptClient()
{
	SOCKADDR_IN clientaddr;
	ZeroMemory(&clientaddr, sizeof(clientaddr));

	int addrlen = sizeof(clientaddr);
	MsgManager::GetInstance()->DisplayMsg("����", "Ŭ���̾�Ʈ ���� �����...");
	SOCKET socket = accept(sock, (SOCKADDR*)&clientaddr, &addrlen);
	if (socket == INVALID_SOCKET)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPServer::acceptClient : ERROR : accept() result = INVALID_SOCKET");
		ErrorManager::GetInstance()->err_display("TCPServer accept()");
		return socket;
	}

	char temp[BUFSIZE];
	memset(temp, 0, sizeof(temp));

	sprintf(temp, "MainManager::accept : CONNECT : IP = %s PORT = %d", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	LogManager::GetInstance()->SetTime();
	LogManager::GetInstance()->LogWrite(temp);

	printf("[TCP ����] Ŭ���̾�Ʈ ����: Socket=%d IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", socket,
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return socket;
}
