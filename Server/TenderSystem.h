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
	struct _ParticipateInfo //참여자 정보
	{
		char user_id[IDSIZE]; //유저
		int price; //가격
	};

	char product_name[PRODUCT_NAMESIZE]; //물품 이름

	int	code; //물품 코드
	int	count; //입찰 수
	bool end; //종료 여부

	char register_id[IDSIZE];//등록자 
	list<_ParticipateInfo*> *user_list;//참여 유저
	bool IsParticipate(const char* _id); //이미 참여했는지 확인

public:
	TenderInfo(const char* _name, const char* _regiuser, int _code, int _count);
	~TenderInfo();
	void PackTenderInfo(char* _buf,int& _size);

	int getCode();
	PARTICIPATERESULT AddParticipateInfo(const char* _id, int _price); //입찰
	bool IsEnd(); //종료여부
	void EndTender(); //입찰 종료
};

class TenderManager
{
	char* PARTICIPATERESULTMSG[6] = { "입찰 완료되었습니다.",
		"입찰에 필요한 돈이 부족합니다.",
		"물품 등록자는 입찰할수 없습니다.",
		"종료된 입찰입니다.",
		"이미 입찰한 물품입니다.",
		"잘못된 코드 입니다." };
	TenderManager();
	~TenderManager();
	static TenderManager* Instance;
	HANDLE poc;
	list<TenderInfo*>* tender_list; //입찰 리스트
	queue<TenderInfo*>* end_tender_list; //입찰 종료 큐
	HANDLE tender_end;//입찰 종료 신호
	UserState* state;	// 상태

	void reqCreate(User* user, char* buf);//물품 생성
	void reqParticipate(User* user, char* buf); //참여
	void reqTenderList(User* user, char* buf); //물품 리스트
public:
	static void CreateInstance();
	static TenderManager* GetInstance();
	static void DestroyInstance();

	bool InitializeManager();
	void EndManager();

	void SetState(UserState* _state) { state = _state; }

	RESULT TenderRecvProcess(User* _user);

	// Tender 메뉴
	RESULT tenderMenuChoice(User* _user);

	void TenderProcess(); //입찰 스레드
};



#endif // !TENDER_SYSTEM_H
