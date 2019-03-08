#ifndef _IOCP_MODEL_H_
#define _IOCP_MODEL_H_

#include "UserManager.h"
#include "ThreadManager.h"
#include "LogManager.h"
#include "InGameManager.h"

//using namespace std;

class IocpModel
{
protected:
	HANDLE hcp;

public:
	IocpModel();
	~IocpModel();

	void IocpProcess();
	bool Initialize_Iocp();

	HANDLE GetIOCPHandle() { return hcp; }

	void PortBind(SOCKET _client_socket);

	virtual bool acceptProcess(User* _user) = 0;
	virtual bool recvProcess(User* _user) = 0;
	virtual bool sendProcess(User* _user) = 0;
};

#endif // !IOCPMODEL_H
