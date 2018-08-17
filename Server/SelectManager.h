#ifndef SELECT_MANAGER_H
#define SELECT_MANAGER_H

#include "TCPServer.h"
#include "LinkedList.h"
#include <list>

using namespace std;

struct SelectGroup
{
	HANDLE thread;
	list<Socket*> sock;
};

class SelectManager
{
public:
	void SelectProcess(void* arg);

	/*virtual void addSelect(Socket* sock) = 0;
	virtual void removeSelect(Socket* sock) = 0;*/

	virtual void accept() = 0;
	virtual void set(FD_SET *read, FD_SET *write) = 0;

	virtual bool recvProcess(FD_SET& _rset) = 0;	// SelectGroup에 변동사항이 생길 때 false를 리턴해야함
	virtual bool sendProcess(FD_SET& _wset) = 0;	// SelectGroup에 변동사항이 생길 때 false를 리턴해야함
};

#endif // !SELECTMANAGER_H
