#include "IocpModel.h"

IocpModel::IocpModel()
{

}

IocpModel::~IocpModel()
{

}

void IocpModel::IocpProcess()
{
	int retval;
	while (1)
	{
		DWORD cbTransferred;
		SOCKET client_sock;
		WSAOVERLAPPEDEx* overlappedex;
		retval = GetQueuedCompletionStatus(hcp, &cbTransferred,(PULONG_PTR)&client_sock, (LPOVERLAPPED*)&overlappedex, INFINITE);

		User* user = (User*)overlappedex->p;

		if (retval == 0 || cbTransferred == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(user->getSocket(), &overlappedex->overlapped,
					&temp1, FALSE, &temp2);
				
				LogManager::GetInstance()->SetTime();
				LogManager::GetInstance()->LogWrite("IocpModel::IocpProcess : ERROR : GetQueuedCompletionStatus() failed ");
				
				user->stop();
				UserManager::GetInstance()->removeUser(user);
			}

			continue;
		}

		// 	IOTYPE_ACCEPT = 200, IOTYPE_RECV, IOTYPE_SEND
		switch (overlappedex->type)
		{
		case IOTYPE_ACCEPT:
			if (acceptProcess(user) == false)
			{
				user->stop();
				UserManager::GetInstance()->removeUser(user);
			}			
			break;
		case IOTYPE_RECV:
			if (user->IOCP_isRecvSuccess(cbTransferred) == false)
			{
				if (user->IOCP_RecvMsg() == false)
				{
					user->stop();
					UserManager::GetInstance()->removeUser(user);
					break;
				}
				continue;
			}

			if (recvProcess(user))
			{
				if (user->IOCP_SendMsg() == false)
				{
					user->stop();
					UserManager::GetInstance()->removeUser(user);
					break;
				}
			}
			break;
		case IOTYPE_SEND:

			// 다 보냈는지 확인
			if (user->IOCP_isSendSuccess(cbTransferred) == false)
			{
				// send : false를 return하면 소켓 종료. 다보내면 
				if (user->IOCP_SendMsg() == false)
				{
					user->stop();
					UserManager::GetInstance()->removeUser(user);
					break;
				}
				continue;
			}

			if (sendProcess(user))
			{
				if (user->IOCP_RecvMsg() == false)
				{
					user->stop();
					UserManager::GetInstance()->removeUser(user);
					break;
				}
			}
			break;
		}
	} // while end
}

bool IocpModel::Initialize_Iocp()
{
	hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
	{
		LogManager::GetInstance()->SetTime();
		LogManager::GetInstance()->LogWrite("IocpModel::Initialize_Iocp : ERROR : CreateIoCompletionPort() failed ");
		return false;
	}

	SYSTEM_INFO sys;
	GetSystemInfo(&sys);

	HANDLE hThread;
	for (int i = 0; i<(int)sys.dwNumberOfProcessors * 2; i++)
	{
		hThread = ThreadManager::GetInstance()->addThread(ThreadManager::CommunicationProcess, 0, this);
		CloseHandle(hThread);
	}

	return true;
}

void IocpModel::PortBind(SOCKET _client_socket)
{
	CreateIoCompletionPort((HANDLE)_client_socket, hcp, _client_socket, 0);
}
