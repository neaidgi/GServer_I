#pragma once
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Global.h"

//���---------------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
class cNode																	//Ŭ���� ���
{
private:
	Type  Data;																//��忡 ����� ������
	cNode<Type> * Link;														//��尡 ����Ű�� �������
public:
	cNode() { memset(&Data, 0, sizeof(Data)), Link = nullptr; }				//�⺻ ������
	cNode(Type _Data) { Data = _Data, Link = nullptr; }						//����� �����͸� �޴� ������
	~cNode() {}																//�Ҹ���

	void SetData(Type _Data) { Data = _Data; }
	Type  GetData() { return Data; }										//��忡 ����� ������ ��ȯ
	cNode<Type> * GetLink() { return Link; }								//��尡 ����Ű�� ������� ��ȯ
	void SetNextLink(cNode<Type>*_Link) { Link = _Link; }					//��尡 ����Ű�� ������� ����
};
//-----------------------------------------------------------------------------------------------------------------------------------------


//������ Ÿ�� ������ ���----------------------------------------------------------------------------------------------------------------------
template <typename Type>													
class cNode<Type*>															//������ Ÿ�� ���� Ư��ȭ ���
{
private:
	Type*  Data;															//��忡 ����� ������Ÿ�� ������
	cNode<Type*> * Link;													//��尡 ����Ű�� �������
public:
	cNode() { memset(&Data, 0, sizeof(Data)), Link = nullptr; }				//�⺻ ������	
	cNode(Type* _Data) { Data = _Data, Link = nullptr; }					//����� �����͸� �޴� ������
	~cNode() { if(Data != NULL) delete Data; }												//������Ÿ�� �������� ��� delete�� ���� �Ҵ�� �޸� ��ȯ

	void SetData(Type* _Data) { Data = _Data; }
	Type*  GetData() { return Data; }										//��忡 ����� ������Ÿ�� ������ ��ȯ
	cNode<Type*> * GetLink() { return Link; }								//��尡 ����Ű�� ������� ��ȯ
	void SetNextLink(cNode<Type*>*_Link) { Link = _Link; }					//��尡 ����Ű�� ������� ����
};


//-----------------------------------------------------------------------------------------------------------------------------------------


//���Ḯ��Ʈ---------------------------------------------------------------------------------------------------------------------------------
template <typename Type>
class cLinkedList															//���Ḯ��Ʈ Ŭ����
{
private:
	cNode<Type> * Headp;													//���� ����Ʈ�� ��� ������
	cNode<Type> * Tailp;													//���� ����Ʈ�� ���� ������
	int Listnum;															//���� ����Ʈ�� ����� ��� ����
public:
	cLinkedList();															//�⺻������
	~cLinkedList();															//�⺻ �Ҹ���

	cNode<Type> * GetHead() { return Headp; }								//���� ����Ʈ�� ��� ������ ��ȯ
	cNode<Type> * GetTail() { return Tailp; }								//���� ����Ʈ�� ���� ������ ��ȯ
	int GetListnum() { return Listnum; }									//���� ����Ʈ�� ����� ��� ���� ��ȯ


	bool Insert(Type);														//���� ����Ʈ�� ��� ����
	bool Except(Type);														//���Ḯ��Ʈ���� ������ ����
	bool Delete(Type);														//���� ����Ʈ�� ��� ����
	void ExceptAll();														//��� ������ ����
	void DeleteAll();
};

template <typename Type>
cLinkedList<Type>::cLinkedList()											//������
{
	Headp = new cNode<Type>;												//��Ʈ������ ���� �Ҵ�
	Tailp = Headp;															//���Ḯ��Ʈ ó�� ������ ���� == ���
	Listnum = 0;															//���Ḯ��Ʈ ����� ��� ���� 0

}

template <typename Type>													
cLinkedList<Type>::~cLinkedList()											//�Ҹ���
{
	cNode<Type> * Deletep = Headp;											//Deletep�� ��� ������ ����
	for (int i = 0; i <= Listnum; i++)										//
	{
		Headp = Headp->GetLink();											//��������͸� ���� ��ũ�� ����
		delete Deletep;														//Deletep ����
		Deletep = Headp;													//Deletep �ٽ� ��������� ����
	}

}

template <typename Type>
bool cLinkedList<Type>::Insert(Type  _Data)									//���Ḯ��Ʈ�� ��� ����
{
	cNode<Type> * Empt = new cNode<Type>(_Data);							//�����͸� ������ ��带 ������
	Tailp->SetNextLink(Empt);												//������� ���� ��ũ�� ������ ��� ����
	Tailp = Empt;															//������带 ������ ���� ����
	Listnum++;																//���Ḯ��Ʈ�� ��尹�� ++
	return true;															//��� ���� �Ϸ� �Լ� ����
}

template <typename Type>
bool cLinkedList<Type>::Delete(Type  _Data)									//���Ḯ��Ʈ�� ��� ����
{
	cNode<Type> * Connectp = Headp;											//�����ϱ� �� ���, ������ ��� ������ũ�� �������� ���
	cNode<Type> * Deletep = Connectp->GetLink();							//������ ���
	bool flag = false;
	for (int i = 0; i < Listnum; i++)										//��尹����ŭ �ݺ� 
	{
		if (Deletep->GetData() == _Data)									//������ �����͸� Ž��
		{
			flag = true;													//�����͸� ã�����
			Connectp->SetNextLink(Deletep->GetLink());						//Connectp�� ������带 Deletep�� �������� ����
			if (Deletep == Tailp)											//��������ϰ�� 
			{
				Tailp = Connectp;											//������带 Connectp�� ����
			}
			delete Deletep;													//Deletep ����
			Listnum--;														//���Ḯ��Ʈ�� ��尹�� --
			return flag;													//��� ���� �Ϸ� �Լ� ����									
			break;
		}																	//������ Ž�� ���н�
		Connectp = Deletep;													//Connectp�� Deletep��
		Deletep = Deletep->GetLink();										//Deletep�� Deletep ���� ���� ����						
	}

	return flag;															//��� ���� ���� �Լ� ���� 

}


template <typename Type>
bool cLinkedList<Type>::Except(Type  _Data)									//���Ḯ��Ʈ�� ��� ����
{
	cNode<Type> * Connectp = Headp;											//�����ϱ� �� ���, ������ ��� ������ũ�� �������� ���
	cNode<Type> * Deletep = Connectp->GetLink();							//������ ���
	bool flag = false;
	for (int i = 0; i < Listnum; i++)										//��尹����ŭ �ݺ� 
	{
		if (Deletep->GetData() == _Data)									//������ �����͸� Ž��
		{
			flag = true;													//�����͸� ã�����
			Connectp->SetNextLink(Deletep->GetLink());						//Connectp�� ������带 Deletep�� �������� ����
			if (Deletep == Tailp)											//��������ϰ�� 
			{
				Tailp = Connectp;											//������带 Connectp�� ����
			}
			Deletep->SetData(NULL);
			delete Deletep;													//Deletep ����
			Listnum--;														//���Ḯ��Ʈ�� ��尹�� --
			return flag;													//��� ���� �Ϸ� �Լ� ����									
			break;
		}																	//������ Ž�� ���н�
		Connectp = Deletep;													//Connectp�� Deletep��
		Deletep = Deletep->GetLink();										//Deletep�� Deletep ���� ���� ����						
	}

	return flag;															//��� ���� ���� �Լ� ���� 

}

template <typename Type>
void cLinkedList<Type>::ExceptAll()// ���� ����Ʈ �ʱ�ȭ
{
	cNode<Type> * Connectp = Headp;										
	cNode<Type> * Deletep = Connectp->GetLink();					
	for (int i = 0; i < Listnum; i++)									
	{
		Connectp->SetNextLink(Deletep->GetLink());				
		if (Deletep == Tailp)										
		{
			Tailp = Connectp;										
		}
		Deletep->SetData(NULL);
		delete Deletep;																		
	}
	Listnum = 0;
}

template <typename Type>
void cLinkedList<Type>::DeleteAll()// ���� ����Ʈ �ʱ�ȭ
{
	cNode<Type> * Connectp = Headp;
	cNode<Type> * Deletep = Connectp->GetLink();
	for (int i = 0; i < Listnum; i++)
	{
		Connectp->SetNextLink(Deletep->GetLink());
		if (Deletep == Tailp)
		{
			Tailp = Connectp;
		}
		delete Deletep;
	}
	Listnum = 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------

#endif // !LINKEDLIST_H
