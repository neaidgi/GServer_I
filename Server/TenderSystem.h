#pragma once
#ifndef TENDER_SYSTEM_H
#define TENDER_SYSTEM_H
#include "Global.h"
#include <list>
#include <queue>
#include "User.h"
#include "UserState.h"

using std::list;
using std::queue;

enum PARTICIPATERESULT {SUCCESS= 0, NOMONEY, REGIERROR, ENDERROR, OVERLAPERROR, CODEERROR };

const struct
{
	enum {
		CREATE=1,
		PARTICIPATE,
		BACK
	};
}TenderMenu = {};

class TenderInfo
{
	struct _ParticipateInfo //������ ����
	{
		char user_id[IDSIZE]; //����
		int price; //����
	};

	char product_name[PRODUCT_NAMESIZE]; //��ǰ �̸�

	int	code; //��ǰ �ڵ�
	int	count; //���� ��
	bool end; //���� ����

	char register_id[IDSIZE];//����� 
	list<_ParticipateInfo*> *user_list;//���� ����
	bool IsParticipate(const char* _id); //�̹� �����ߴ��� Ȯ��

public:
	TenderInfo(const char* _name, const char* _regiuser, int _code, int _count);
	~TenderInfo();
	void PackTenderInfo(char* _buf,int& _size);

	int getCode();
	PARTICIPATERESULT AddParticipateInfo(const char* _id, int _price); //����
	bool IsEnd(); //���Ῡ��
	void EndTender(); //���� ����
};

class TenderManager
{
	char* PARTICIPATERESULTMSG[6] = { "���� �Ϸ�Ǿ����ϴ�.",
		"������ �ʿ��� ���� �����մϴ�.",
		"��ǰ ����ڴ� �����Ҽ� �����ϴ�.",
		"����� �����Դϴ�.",
		"�̹� ������ ��ǰ�Դϴ�.",
		"�߸��� �ڵ� �Դϴ�." };
	TenderManager();
	~TenderManager();
	static TenderManager* Instance;
	HANDLE poc;
	list<TenderInfo*>* tender_list; //���� ����Ʈ
	queue<TenderInfo*>* end_tender_list; //���� ���� ť
	HANDLE tender_end;//���� ���� ��ȣ
	UserState* state;	// ����

	void reqCreate(User* user, char* buf);//��ǰ ����
	void reqParticipate(User* user, char* buf); //����
	void reqTenderList(User* user, char* buf); //��ǰ ����Ʈ
public:
	static void CreateInstance();
	static TenderManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void SetState(UserState* _state) { state = _state; }

	RESULT TenderRecvProcess(User* _user);

	// Tender �޴�
	RESULT tenderMenuChoice(User* _user);

	void TenderProcess(); //���� ������
};



#endif // !TENDER_SYSTEM_H
