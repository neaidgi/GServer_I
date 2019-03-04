#include "TCPServer.h"
#include "ErrorManager.h"
#include "LogManager.h"
#include "MsgManager.h"

TCPServer::TCPServer()
{
}

// 소켓 생성 후 리슨소켓으로 만들어 준다.
bool TCPServer::start(int port, const char * ip)
{
	// 소켓 생성
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
		addr.sin_addr.s_addr = htonl(INADDR_ANY); // 인수로 받은 ip가 null값이면 자동 할당
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

	// 리슨소켓으로 만듬
	retval = listen(listen_socket, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("TCPServer::start : ERROR : listen() result = SOCKET_ERROR");
		ErrorManager::GetInstance()->err_display("TCPServer listen()");
		return false;
	}

	// 소켓 리슨소켓으로 초기화
	sock = listen_socket;

	return true;
}

// closesocket
void TCPServer::stop()
{
	closesocket(sock);
}

// accept, 유저 생성
SOCKET TCPServer::acceptClient()
{
	SOCKADDR_IN clientaddr;
	ZeroMemory(&clientaddr, sizeof(clientaddr));

	int addrlen = sizeof(clientaddr);
	MsgManager::GetInstance()->DisplayMsg("메인", "클라이언트 연결 대기중...");
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

	printf("[TCP 서버] 클라이언트 접속: Socket=%d IP 주소=%s, 포트 번호=%d\n", socket,
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return socket;
}
