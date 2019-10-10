#ifndef _IOCP_MODEL_H_
#define _IOCP_MODEL_H_

#include "UserManager.h"
#include "ThreadManager.h"
#include "LogManager.h"
#include "InGameManager.h"

/*
	IocpModel
	 - Iocp 모델의 기본 프레임 워크입니다. 

	! 이 클래스를 상속받은 클래스에서는 가상함수를 구현해서 사용해야 합니다. !
*/

class IocpModel
{
protected:
	// 입출력 완료 포트
	HANDLE hcp;
public:
	IocpModel();
	~IocpModel();

	// 소켓 작업의(Accept,Recv,Send) 완료 통보를 받은 구조체로 작업을 하는 스레드
	void IocpProcess();
	// IOCP 초기화 ( 입출력 완료 포트 생성, CPU 개수 만큼 스레드 생성)
	bool Initialize_Iocp();

	HANDLE GetIOCPHandle() { return hcp; }
	// 입출력 완료 포트 생성
	void PortBind(SOCKET _client_socket);

	// 새로 연결된 클라이언트 처리
	virtual bool acceptProcess(User* _user) = 0;
	// Recv 관련 처리
	virtual bool recvProcess(User* _user) = 0;
	// Send 관련 처리
	virtual bool sendProcess(User* _user) = 0;
};

#endif // !IOCPMODEL_H
