#include "CharacterVerification.h"

// �ʱ�ȭ
void CharacterVerification::Initialize(float _CommuTime, float _Move_MarginofError)
{
	CommuTime = _CommuTime;
	Move_MarginofError = _Move_MarginofError;
	m_collision = new Collision();
}

// �̵� ���� �Լ�
bool CharacterVerification::CharacterMoveVerificate(Vector3 _curPos, Vector3 _prePos, int _distance)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	int range = m_collision->Distance(_curPos, _prePos);

	if (m_collision->Distance(_curPos, _prePos) > _distance)
	{
		sprintf(msg, "[�̵� ���� : �̵� ����]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return false;
	}
	sprintf(msg, "[�̵� ���� : �̵� ����]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);
	return true;
}

// ���� ����(�������� ��ġ, ���ݹ���, Ÿ�� ��ġ, Ÿ�� ũ��)
bool CharacterVerification::AttackVerificate(Vector3 _AttackerPos, float _attackerRange, Vector3 _targetPos, float _targetRange)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// ���� �߽ɰ��� �Ÿ� �浹
	if (m_collision->Circle_IsCollision(_AttackerPos, _attackerRange, _targetPos, _targetRange) == false)
	{
		/*sprintf(msg, "[���� ���� : ������ �Ÿ����� �浹��������]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/

		return false;
	}
	return true;
}

// ���� ����(�����ϴ��� ��ġ, ���� ����, ���ݹ���, ���� ���� ����, Ÿ�� ��ġ,Ÿ�� ũ��)
bool CharacterVerification::AttackVerificate(Vector3 _AttackerPos, Vector3 _attackerDir, float _attackerRange, int _attackerAngle, Vector3 _targetPos, float _targetRange)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	// ���� �߽ɰ��� �Ÿ� �浹
	if (m_collision->Circle_IsCollision(_AttackerPos, _attackerRange, _targetPos, _targetRange) == false)
	{
		/*sprintf(msg, "[���� ���� : ������ �Ÿ����� �浹��������]");
		MsgManager::GetInstance()->DisplayMsg("INFO", msg);*/
		return false;
	}

	//sprintf(msg, "[���� ���� : ĳ������ġ [x : %f] [y : %f] [z : %f], ����������ġ [x : %f] [y : %f] [z : %f], ������ġ [x : %f] [y : %f] [z : %f]]"
	//, _AttackerPos.x, _AttackerPos.y, _AttackerPos.z, _attackerDir.x, _attackerDir.y, _attackerDir.z, _targetPos.x, _targetPos.y, _targetPos.z);
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	// �����ڿ��� Ÿ���� ���� ���� ���ϱ�
	Vector3 attackerPos = Vector3(_AttackerPos.x, _AttackerPos.y, 0);
	Vector3 targetPos = Vector3(_targetPos.x, _targetPos.y,0);
	Vector3 attackerDir = Vector3(_attackerDir.x, _attackerDir.y, 0);
	// ��ǥ�� ĳ���� �߽����� �ű��
	targetPos = Vector3(targetPos.x - attackerPos.x, targetPos.y - attackerPos.y, 0);
	attackerDir = Vector3(attackerDir.x - attackerPos.x, attackerDir.y - attackerPos.y, 0);
	attackerPos = Vector3(attackerPos.x - attackerPos.x, attackerPos.y - attackerPos.y, 0);

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "[���� ���� : ĳ������ġ [x : %f] [y : %f] [z : %f], ����������ġ [x : %f] [y : %f] [z : %f], ������ġ [x : %f] [y : %f] [z : %f]]"
	//	, attackerPos.x, attackerPos.y, attackerPos.z, attackerDir.x, attackerDir.y, attackerDir.z, targetPos.x, targetPos.y, targetPos.z);

	if (m_collision->FanShape_IsCollision(attackerDir, targetPos, _attackerAngle) == false)
	{
		//memset(msg, 0, sizeof(msg));
		//sprintf(msg, "[���� ���� : ���ݻ�Ÿ��� �浹��������]");
		//MsgManager::GetInstance()->DisplayMsg("INFO", msg);
		return false;
	}

	//memset(msg, 0, sizeof(msg));
	//sprintf(msg, "[���� ���� : ���� ����]");
	//MsgManager::GetInstance()->DisplayMsg("INFO", msg);

	return true;
}
