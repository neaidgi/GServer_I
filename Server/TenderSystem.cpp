#include"TenderSystem.h"
#include"UserManager.h"
#include "ThreadManager.h"
#include "EnryptManager.h"

TenderInfo::TenderInfo(const char* _name, const char* _regiuser, int _code, int _count)
{
	strcpy(product_name, _name);
	strcpy(register_id, _regiuser);

	code = _code;
	count = _count;
	end = false;

	user_list = new list<_ParticipateInfo*>;
}

TenderInfo::~TenderInfo()
{
	list<_ParticipateInfo*>::iterator itor = user_list->begin();
	while (user_list->end() != itor)
	{
		delete (*itor);
		itor++;
	}
	user_list->clear();
	delete user_list;
}

int TenderInfo::getCode()
{
	return code;
}

void TenderInfo::PackTenderInfo(char* _buf, int& _size) //입찰 정보 물품코드, 이름, 현재입찰수, 총입찰수
{
	char* ptr = _buf;
	_size = 0;
	int namesize = strlen(product_name) + 1;

	memcpy(ptr, &code, sizeof(int));
	ptr += sizeof(int);
	_size += sizeof(int);

	memcpy(ptr, &namesize, sizeof(int));
	ptr += sizeof(int);
	_size += sizeof(int);

	memcpy(ptr, product_name, namesize);
	ptr += namesize;
	_size += namesize;

	int listsize = user_list->size();
	memcpy(ptr, &listsize, sizeof(int));
	ptr += sizeof(int);
	_size += sizeof(int);

	memcpy(ptr, &count, sizeof(int));
	ptr += sizeof(int);
	_size += sizeof(int);

}


bool TenderInfo::IsParticipate(const char* _id) //이미 참여 했는지 확인
{
	list<_ParticipateInfo*>::iterator itor = user_list->begin();
	while (user_list->end() != itor)
	{
		if (!strcmp((*itor)->user_id, _id))
		{
			return true;
		}
		itor++;
	}
	return false;
}

PARTICIPATERESULT TenderInfo::AddParticipateInfo(const char* _id, int _price) //입찰 참여
{
	if (end) return ENDERROR;
	else if (!strcmp(register_id, _id)) return REGIERROR;
	else if (IsParticipate((_id))) return OVERLAPERROR;

	_ParticipateInfo * empt = new _ParticipateInfo();
	empt->price = _price;
	strcpy(empt->user_id, _id);

	user_list->push_back(empt);

	if (user_list->size() == count)
	{
		end = true;
	}
	return SUCCESS;
}

bool TenderInfo::IsEnd()
{
	return end;
}

void TenderInfo::EndTender() //입찰 종료
{
	list<_ParticipateInfo*>::iterator itor = user_list->begin();
	_ParticipateInfo* empt = nullptr; //입찰 성공자 찾기

	while (user_list->end() != itor)
	{
		if (empt == nullptr)
		{
			empt = *itor;
		}
		else if (empt->price < (*itor)->price)
		{
			empt = *itor;
		}
		itor++;
	}

	itor = user_list->begin();

	char buf[BUFSIZE];
	char* ptr;
	int size;
	char msg[256];
	int msgsize;
	PROTOCOL protocol = SERVER_TENDER_END_RESULT;

	User* user = UserManager::GetInstance()->getUser(register_id);
	if (user != nullptr) //등록자에게 메세지 전송
	{

		sprintf(msg, "등록하신 %s물품이 %s님에게 낙찰되었습니다. 낙찰가는 %d원입니다.\n", product_name, empt->user_id, empt->price);
		msgsize = strlen(msg) + 1;
		ptr = buf;

		size = sizeof(PROTOCOL) + sizeof(int) + msgsize;

		memcpy(ptr, &size, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr, &protocol, sizeof(PROTOCOL));
		ptr += sizeof(PROTOCOL);

		memcpy(ptr, &msgsize, sizeof(int));
		ptr += sizeof(int);

		memcpy(ptr, msg, msgsize);


		EncryptManager::GetInstance()->encoding(buf + sizeof(int), size);
		send(user->getSocket(), buf, sizeof(int) + size, 0);
	}

	while (user_list->end() != itor)
	{
		if (empt == *itor)//입찰 성공자일 경우
		{
			user = UserManager::GetInstance()->getUser(empt->user_id);
			if (user != nullptr)
			{

				sprintf(msg, "%s물품에 대한 입찰에 성공했습니다. 낙찰가는 %d원입니다.\n", product_name, empt->price);
				msgsize = strlen(msg) + 1;
				ptr = buf;

				size = sizeof(PROTOCOL) + sizeof(int) + msgsize;

				memcpy(ptr, &size, sizeof(int));
				ptr += sizeof(int);

				memcpy(ptr, &protocol, sizeof(PROTOCOL));
				ptr += sizeof(PROTOCOL);

				memcpy(ptr, &msgsize, sizeof(int));
				ptr += sizeof(int);

				memcpy(ptr, msg, msgsize);

				EncryptManager::GetInstance()->encoding(buf + sizeof(int), size);

				send(user->getSocket(), buf, sizeof(int) + size, 0);
			}
			else
			{
				user = UserManager::GetInstance()->getUser((*itor)->user_id);
				if (user != nullptr)//입찰한 실패한 유저
				{
					user->setmoney(empt->price);
					sprintf(msg, "%s물품에 대한 입찰에 실패했습니다. %s님에게 낙찰되었으며 낙찰가는 %d원입니다.\n", product_name, empt->user_id, empt->price);
					msgsize = strlen(msg) + 1;
					ptr = buf;

					size = sizeof(PROTOCOL) + sizeof(int) + msgsize;

					memcpy(ptr, &size, sizeof(int));
					ptr += sizeof(int);

					memcpy(ptr, &protocol, sizeof(PROTOCOL));
					ptr += sizeof(PROTOCOL);

					memcpy(ptr, &msgsize, sizeof(int));
					ptr += sizeof(int);

					memcpy(ptr, msg, msgsize);

					EncryptManager::GetInstance()->encoding(buf + sizeof(int), size);

					send(user->getSocket(), buf, sizeof(int) + size, 0);
				}
			}
		}
		itor++;
	}
}

TenderManager::TenderManager()
{
	tender_list = new list<TenderInfo*>;
	end_tender_list = new queue<TenderInfo*>;
	tender_end = CreateEvent(NULL, FALSE, FALSE, NULL);
	poc = ThreadManager::GetInstance()->addThread(ThreadManager::TenderProcess, 0, this);
	poc = ThreadManager::GetInstance()->addThread(ThreadManager::TenderProcess, 0, this);
}

TenderManager::~TenderManager()
{
	ThreadManager::GetInstance()->removeThread(poc);
	list<TenderInfo*>::iterator itor = tender_list->begin();
	while (tender_list->end() != itor)
	{
		delete (*itor);
		itor++;
	}
	tender_list->clear();
	delete tender_list;

	while (!end_tender_list->empty())
	{
		TenderInfo* info = end_tender_list->front();
		delete info;
		end_tender_list->pop();
	}
	delete end_tender_list;

	CloseHandle(tender_end);
}

TenderManager* TenderManager::Instance = nullptr;

void TenderManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new TenderManager();
	}
}
TenderManager* TenderManager::GetInstance()
{
	return Instance;
}
void TenderManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		delete Instance;
		Instance = nullptr;
	}
}

bool TenderManager::InitializeManager()
{
	return true;
}

void TenderManager::EndManager()
{

}

void TenderManager::reqCreate(User* user, char* buf)
{
	static int codemaker = 0;

	char name[PRODUCT_NAMESIZE];
	int namesize;
	int count;
	char* ptr = buf;

	memcpy(&namesize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(name, ptr, namesize);
	ptr += namesize;

	memcpy(&count, ptr, sizeof(int));

	TenderInfo * tenderinfo = new TenderInfo(name, user->getID(), codemaker++, count);
	tender_list->push_back(tenderinfo);
	bool check = true;

	user->pack(SERVER_TENDER_CREATE_RESULT, &check, sizeof(bool));
	user->include_wset = true;
}

void TenderManager::reqParticipate(User* user, char* buf)
{
	int code;
	int price;
	char* ptr = buf;

	memcpy(&code, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(&price, ptr, sizeof(int));
	ptr += sizeof(int);

	PARTICIPATERESULT result;
	list<TenderInfo*>::iterator itor;
	if (user->checkmoney(price)) //돈확인
	{
		bool find = false; //코드가 있는지 확인
		user->setmoney(-price);
		itor = tender_list->begin();
		while (tender_list->end() != itor)
		{
			if ((*itor)->getCode() == code)
			{
				result = (*itor)->AddParticipateInfo(user->getID(), price);//결과 저장
				find = true;
				break;
			}
			itor++;
		}
		if (!find) result = CODEERROR; //코드 없을시
	}
	else
	{
		result = NOMONEY; //돈없을시
	}

	ptr = buf;

	int msgsize = strlen(PARTICIPATERESULTMSG[result]) + 1;

	memcpy(ptr, &msgsize, sizeof(int));
	ptr += sizeof(int);

	memcpy(ptr, PARTICIPATERESULTMSG[result], msgsize);

	user->pack(SERVER_TENDER_PARTICIPATE_RESULT, buf, sizeof(int) + msgsize);//결과에 맞는 문자열 전송
	user->include_wset = true;
	if ((*itor)->IsEnd())//입찰이 종료 됬는지 확인
	{
		end_tender_list->push(*itor);
		tender_list->erase(itor);
		SetEvent(tender_end);
	}

}

void TenderManager::reqTenderList(User* user, char* buf)
{
	char* ptr = buf;
	int size = 0;

	int listcount = tender_list->size();
	memcpy(ptr, &listcount, sizeof(int));
	ptr += sizeof(int);
	size += sizeof(int);

	int infosize;
	list<TenderInfo*>::iterator itor = tender_list->begin();
	while (tender_list->end() != itor)
	{
		(*itor)->PackTenderInfo(ptr, infosize);
		ptr += infosize;
		size += infosize;
		itor++;
	}

	user->pack(SERVER_TENDER_LIST_RESULT, buf, size);
	user->include_wset = true;
}

RESULT TenderManager::TenderRecvProcess(User * _user)
{
	PROTOCOL protocol;
	char buf[BUFSIZE];
	RESULT result;

	_user->unPack(&protocol, &buf);
	switch (protocol)
	{
	case CLIENT_REQ_TENDER_CREATE:
		reqCreate(_user, buf);
		result = RT_TENDERCREATE;
		break;
	case CLIENT_REQ_TENDER_PARTICIPATE:
		reqParticipate(_user, buf);
		result = RT_TENDERPARTICIPATE;
		break;
	case CLIENT_REQ_TENDER_LIST:
		reqTenderList(_user, buf);
		result = RT_TENDERLIST;
		break;
	}
	return result;
}

RESULT TenderManager::tenderMenuChoice(User* _user)
{
	int choice;

	PROTOCOL protocol;
	char buf[BUFSIZE];
	RESULT result;
	PROTOCOL SendProtocl;

	_user->unPack(&protocol, &buf);
	memcpy(&choice, buf, sizeof(int));
	switch (protocol)
	{
	case CLIENT_TENDER_MENU_CHOICE:
		switch (choice)
		{
		case TenderMenu.CREATE:
			SendProtocl = SERVER_TENDER_CREATE;
			result = RT_MENUCREATE;
			break;
		case TenderMenu.PARTICIPATE:
			SendProtocl = SERVER_TENDER_PARTICIPATE;
			result = RT_MENUPARTICIPATE;
			break;
		case TenderMenu.BACK:
			SendProtocl = SERVER_BACK;
			result = RT_MENUBACK;
			break;
		}
		break;
	}

	_user->pack(SendProtocl, buf, 0);
	_user->include_wset = true;

	return result;
}

void TenderManager::TenderProcess()
{
	while (1)
	{
		WaitForSingleObject(tender_end, INFINITE); //입찰 종료가 된게 있을때 까지 대기
		TenderInfo* empt = end_tender_list->front(); //큐에서 데이터 가져옴
		empt->EndTender(); //메세지 전송
		delete empt; //삭제
		end_tender_list->pop();//큐에서 삭제
	}
}

