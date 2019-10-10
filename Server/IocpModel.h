#ifndef _IOCP_MODEL_H_
#define _IOCP_MODEL_H_

#include "UserManager.h"
#include "ThreadManager.h"
#include "LogManager.h"
#include "InGameManager.h"

/*
	IocpModel
	 - Iocp ���� �⺻ ������ ��ũ�Դϴ�. 

	! �� Ŭ������ ��ӹ��� Ŭ���������� �����Լ��� �����ؼ� ����ؾ� �մϴ�. !
*/

class IocpModel
{
protected:
	// ����� �Ϸ� ��Ʈ
	HANDLE hcp;
public:
	IocpModel();
	~IocpModel();

	// ���� �۾���(Accept,Recv,Send) �Ϸ� �뺸�� ���� ����ü�� �۾��� �ϴ� ������
	void IocpProcess();
	// IOCP �ʱ�ȭ ( ����� �Ϸ� ��Ʈ ����, CPU ���� ��ŭ ������ ����)
	bool Initialize_Iocp();

	HANDLE GetIOCPHandle() { return hcp; }
	// ����� �Ϸ� ��Ʈ ����
	void PortBind(SOCKET _client_socket);

	// ���� ����� Ŭ���̾�Ʈ ó��
	virtual bool acceptProcess(User* _user) = 0;
	// Recv ���� ó��
	virtual bool recvProcess(User* _user) = 0;
	// Send ���� ó��
	virtual bool sendProcess(User* _user) = 0;
};

#endif // !IOCPMODEL_H
