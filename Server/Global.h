#ifndef GLOBAL_H
#define GLOBAL_H

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>
#include <math.h>
#include <queue>
#include <list>
#include <vector>
#include <ctime>
#include <complex.h>
#include "Vector3.h"

// ��Ŷ : [��üũ�� 4Byte][�������� 4Byte][������]
// ���ڿ� ������ [strlen(���ڿ�)+1 4Byte][���ڿ�]

// AND����� ����
#define PROTOCOL_FFFFFFFFFFFFFFFF 0xFFFFFFFFFF000000

// -- ���� -- //

// ūƲ
#define PROTOCOL_80 0x80										// [ūƲ - ����]
#define PROTOCOL_40 0x40										// [ūƲ - ����]
#define PROTOCOL_20 0x20										// [ūƲ - ����]
#define PROTOCOL_10 0x10										// [ūƲ - ����]

#define PROTOCOL_8 0x8											// [ūƲ - ����]
#define PROTOCOL_INGAME 0x4										// [ūƲ - �ΰ���]
#define PROTOCOL_CHRACTER 0x2									// [ūƲ - ĳ���ͼ���ȭ��]
#define PROTOCOL_TITLE 0x1										// [ūƲ - Ÿ��Ʋ]

// Ÿ��Ʋ �߰�Ʋ
#define PROTOCOL_LOGIN 0x100									// [�߰�Ʋ - Ÿ��Ʋ]

// Ÿ��Ʋ ����Ʋ(PROTOCOL_LOGIN)
#define PROTOCOL_SERVER_LOGIN_MENU_COMPART 0x7000000			// [����Ʋ - Ÿ��Ʋ] [�������� �����]

#define PROTOCOL_LOGIN_RESULT 0x4000000							// [����Ʋ - Ÿ��Ʋ] [����->Ŭ��] �α��� ���
#define PROTOCOL_JOIN_RESULT 0x2000000							// [����Ʋ - Ÿ��Ʋ] [����->Ŭ��] ȸ������ ���
#define PROTOCOL_ID_OVERLAP_CHECK 0x1000000						// [����Ʋ - Ÿ��Ʋ] [����->Ŭ��] ID �ߺ�üũ ���

// ĳ���� �߰�Ʋ
#define PROTOCOL_CHARACER_MENU 0x100							// [�߰�Ʋ - ĳ���ͼ���ȭ��]

// ĳ���� ����Ʋ(PROTOCOL_CHARACER_MENU)
#define PROTOCOL_SERVER_CHARACTER_MENU_COMPART 0xF000000		// [����Ʋ - ĳ���ͼ���ȭ��] [�������� �����]

#define PROTOCOL_CHARACTER_SLOT_INFO 0x8000000					// [����Ʋ - ĳ���ͼ���ȭ��] [����->Ŭ��] ĳ���� ���� ����		[��������]...
#define PROTOCOL_CHARACTER_ENTER_RESULT	0x4000000				// [����Ʋ - ĳ���ͼ���ȭ��] [����->Ŭ��] ĳ���� ���Ӱ��		[��������][bool]
#define PROTOCOL_CHARACTER_DELETE_RESULT 0x2000000				// [����Ʋ - ĳ���ͼ���ȭ��] [����->Ŭ��] ĳ���� ���� ���		[��������][bool]
#define PROTOCOL_CHARACTER_RESULT 0x1000000						// [����Ʋ - ĳ���ͼ���ȭ��] [����->Ŭ��] ĳ���� ���� Ȯ�� ���	[��������][bool]

// �ΰ��� �߰�Ʋ				
#define PROTOCOL_INGMAE_MENU 0x4000								// [�߰�Ʋ - �ΰ���] �Ŵ�����
#define PROTOCOL_INGAME_ANIMATION 0x2000						// [�߰�Ʋ - �ΰ���] �ִϸ��̼� ����
#define PROTOCOL_INGAME_DUNGEON 0x1000							// [�߰�Ʋ - �ΰ���] ��������

#define PROTOCOL_INGAME_PARTY 0x800								// [�߰�Ʋ - �ΰ���] ��Ƽ����
#define PROTOCOL_INGAME_CHANNEL 0x400							// [�߰�Ʋ - �ΰ���] ä�ΰ���
#define PROTOCOL_INGMAE_MONSTER 0x200							// [�߰�Ʋ - �ΰ���] ���Ͱ���
#define PROTOCOL_INGAME_CHARACER 0x100							// [�߰�Ʋ - �ΰ���] ĳ���Ͱ���

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_CHARACER) - ĳ���� ����
#define PROTOCOL_SERVER_INGAME_CHARACTER_COMPART 0x7FF0000000	// [����Ʋ - ĳ���Ͱ���] [�������� �����]

#define PROTOCOL_INGAME_OTHERPLAYER_ATTACK_SUCCESS 0x4000000000	// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ���ݼ����ߴ�
#define PROTOCOL_INGAME_ATTACK_RESULT 0x2000000000				// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] ���� ���� ���
#define PROTOCOL_INGAME_OTHERPLAYER_ATTACK 0x1000000000			// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� �����ߴ�

#define PROTOCOL_INGAME_MOVE_OTHERPLAYER_END_JUMP 0x800000000	// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ���� �����ٴ� ����(����)
#define PROTOCOL_INGAME_MOVE_OTHERPLAYER_START_JUMP 0x400000000	// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ���� �ߴٴ� ����
#define PROTOCOL_INGAME_OTHERPLAYER_LEAVE 0x200000000			// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ��������
#define PROTOCOL_INGAME_OTHERPLAYER_CONNECT 0x100000000			// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ��������

#define PROTOCOL_INGAME_MOVE_ROTATION 0x80000000				// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� ȸ�� ����
#define PROTOCOL_INGAME_MOVE_OTHERPLAYERINFO 0x40000000			// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] �ٸ� ���� �̵� ����
#define PROTOCOL_INGAME_MOVE_RESULT 0x20000000					// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] ���� �̵� ���
#define PROTOCOL_INGAME_OTHERPLAYERLIST_INFO 0x10000000			// [����Ʋ - ĳ���Ͱ���] [����->Ŭ��] ������ ����

// �ΰ��� ����Ʋ(PROTOCOL_INGMAE_MONSTER) - ���� ����
#define PROTOCOL_SERVER_INGAME_MONSTER_COMPART 0xF0000000		// [����Ʋ - ��������] [�������� �����]

#define PROTOCOL_INGAME_MONSTER_OTHER_ATTACK_RESULT 0x80000000	// [����Ʋ - ���Ͱ���] [����->Ŭ��] ���� �ٸ����� ���ݰ��
#define PROTOCOL_INGAME_MONSTER_ATTACK_RESULT 0x40000000		// [����Ʋ - ���Ͱ���] [����->Ŭ��] ���� ���� ���ݰ��
#define PROTOCOL_MONSTER_INFO 0x20000000						// [����Ʋ - ���Ͱ���] [����->Ŭ��] ������ �ִ� ��������
#define PROTOCOL_MONSTER_MOVE_RESULT 0x10000000					// [����Ʋ - ���Ͱ���] [����->Ŭ��] ���� �̵�����

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_CHANNEL) - ä�� ����
#define PROTOCOL_SERVER_INGAME_CHANNEL_COMPART 0x7000000		// [����Ʋ - ä�ΰ���] [�������� �����]

#define PROTOCOL_CHANNLE_USER_CHANGE 0x4000000					// [����Ʋ - ä�ΰ���] [����->Ŭ��] ä�� �̵��� ���� ����
#define PROTOCOL_CHANNLE_CHANGE_RESULT 0x2000000				// [����Ʋ - ä�ΰ���] [����->Ŭ��] ä�� ���� ���
#define PROTOCOL_CHANNLE_INFO 0x1000000							// [����Ʋ - ä�ΰ���] [����->Ŭ��] ä�� ����

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_PARTY) - ��Ƽ ����
#define PROTOCOL_SERVER_INGAME_PARTY_COMPART 0x1FFF0000000		// [����Ʋ - ��Ƽ����] [�������� �����]

#define PROTOCOL_PARTY_USER_INFO 0x10000000000					// [����Ʋ - ��Ƽ����] [����->Ŭ��] ��Ƽ������

#define PROTOCOL_PARTY_LEADER_DELEGATE_RESULT 0x8000000000		// [����Ʋ - ��Ƽ����] [����->Ŭ��] ���� ���� ���
#define PROTOCOL_PARTY_LEADER_DELEGATE 0x4000000000				// [����Ʋ - ��Ƽ����] [����->Ŭ��] ���� ���� ���� ���� ����
#define PROTOCOL_PARTY_ROOM_REMOVE_RESULT 0x2000000000			// [����Ʋ - ��Ƽ����] [����->Ŭ��] ��Ƽ�� ��������� ������ ��������
#define PROTOCOL_PARTY_USER_LEAVE_RESULT 0x1000000000			// [����Ʋ - ��Ƽ����] [����->Ŭ��] Ż����

#define PROTOCOL_PARTY_USER_LEAVE_INFO 0x800000000				// [����Ʋ - ��Ƽ����] [����->Ŭ��] Ż���ϴ� ���� ����
#define PROTOCOL_PARTY_USER_KICK_RESULT 0x400000000				// [����Ʋ - ��Ƽ����] [����->Ŭ��] ������
#define PROTOCOL_PARTY_USER_KICK_INFO 0x200000000				// [����Ʋ - ��Ƽ����] [����->Ŭ��] ������� ���� ����
#define PROTOCOL_PARTY_USER_KICK 0x100000000					// [����Ʋ - ��Ƽ����] [����->Ŭ��] ������ѻ������ ������ ��������

#define PROTOCOL_PARTY_ROOM_ADD_USER 0x80000000					// [����Ʋ - ��Ƽ����] [����->Ŭ��] ���ο� ��Ƽ���� ����
#define PROTOCOL_PARTY_ROOM_JOIN_RESULT 0x40000000				// [����Ʋ - ��Ƽ����] [����->Ŭ��] �������
#define PROTOCOL_PARTY_ROOM_INVITE_RESULT 0x20000000			// [����Ʋ - ��Ƽ����] [����->Ŭ��] ��Ƽ �ʴ� ���
#define PROTOCOL_PARTY_ROOM_INVITE 0x10000000					// [����Ʋ - ��Ƽ����] [����->Ŭ��] ��Ƽ �ʴ�(Ư����������)

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_DUNGEON) - ���� ����
#define PROTOCOL_SERVER_INGAME_DUNGEON_COMPART 0x70000000		// [����Ʋ - ��������] [�������� �����]

#define PROTOCOL_DUNGEON_STAGE_IN_RESULT 0x40000000				// [����Ʋ - ��������] [����->Ŭ��] ���������� ������
#define PROTOCOL_DUNGEON_LEAVE_RESULT 0x20000000				// [����Ʋ - ��������] [����->Ŭ��] ���� ���� ���
#define PROTOCOL_DUNGEON_ENTER_RESULT 0x10000000				// [����Ʋ - ��������] [����->Ŭ��] ���� ���� ���

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_ANIMATION) - �ִϸ��̼� ����
#define PROTOCOL_SERVER_INGAME_ANIMATION_COMPART 0x7000000		// [����Ʋ - �ִϸ��̼ǰ���] [�������� �����]

#define PROTOCOL_MONSTER_ANIMATION_ATTACK 0x4000000				// [����Ʋ - �ִϸ��̼ǰ���] [����->Ŭ��] ���� �ִϸ��̼�(����)
#define PROTOCOL_MONSTER_ANIMATION 0x2000000					// [����Ʋ - �ִϸ��̼ǰ���] [����->Ŭ��] ���� �ִϸ��̼�
#define PROTOCOL_CHARACER_ANIMATION 0x1000000					// [����Ʋ - �ִϸ��̼ǰ���] [����->Ŭ��] ĳ���� �ִϸ��̼�

// �ΰ��� ����Ʋ(PROTOCOL_INGMAE_MENU) - �Ŵ� ����
#define PROTOCOL_SERVER_INGAME_MENU_COMPART 0x7000000				// [����Ʋ - ���Ͱ���] [�������� �����]

#define PROTOCOL_OTHERPLAYER_LEAVE 0x4000000					// [����Ʋ - ���Ͱ���] [����->Ŭ��] �ٸ� ������ ä�ο��� ��������
#define PROTOCOL_MENU_RESULT_LOGOUT 0x2000000					// [����Ʋ - ���Ͱ���] [����->Ŭ��] �α׾ƿ� ���
#define PROTOCOL_MENU_RESULT_CHARACTER 0x1000000				// [����Ʋ - ���Ͱ���] [����->Ŭ��] ĳ���� ����ȭ�� ���


// -- ���� END -- //


// -- Ŭ�� -- //

// ūƲ
#define PROTOCOL_80 0x80										// [ūƲ - ����]
#define PROTOCOL_40 0x40										// [ūƲ - ����]
#define PROTOCOL_20 0x20										// [ūƲ - ����]
#define PROTOCOL_10 0x10										// [ūƲ - ����]

#define PROTOCOL_8 0x8											// [ūƲ - ����]
#define PROTOCOL_INGAME 0x4										// [ūƲ - �ΰ���]
#define PROTOCOL_CHRACTER 0x2									// [ūƲ - ĳ���ͼ���ȭ��]
#define PROTOCOL_TITLE 0x1										// [ūƲ - Ÿ��Ʋ]

// Ÿ��Ʋ �߰�Ʋ
#define PROTOCOL_LOGIN 0x100									// [�߰�Ʋ - Ÿ��Ʋ]

// Ÿ��Ʋ ����Ʋ(PROTOCOL_LOGIN)
#define PROTOCOL_CLIENT_LOGIN_MENU_COMPART 0x7000000			// [����Ʋ - Ÿ��Ʋ] [�������� �����]

#define PROTOCOL_REQ_LOGIN 0x4000000							// [����Ʋ - Ÿ��Ʋ] [Ŭ��->����] �α��� ��û
#define PROTOCOL_REQ_JOIN 0x2000000								// [����Ʋ - Ÿ��Ʋ] [Ŭ��->����] ȸ������ ��û
#define PROTOCOL_REQ_ID_OVERLAP_CHECK 0x1000000					// [����Ʋ - Ÿ��Ʋ] [Ŭ��->����] ID �ߺ�üũ ��û

// ĳ���� �߰�Ʋ
#define PROTOCOL_CHARACER_MENU 0x100							// [�߰�Ʋ - ĳ���ͼ���ȭ��]

// ĳ���� ����Ʋ(PROTOCOL_CHARACER_MENU)
#define PROTOCOL_CLIENT_CHARACTER_MENU_COMPART 0xF000000		// [����Ʋ - ĳ���ͼ���ȭ��] [�������� �����]

#define PROTOCOL_REQ_CHARACTER_CREATE 0x8000000					// [����Ʋ - ĳ���ͼ���ȭ��] [Ŭ��->����] ĳ���� ���� ��û
#define PROTOCOL_CHARACTER_DELETE 0x4000000					    // [����Ʋ - ĳ���ͼ���ȭ��] [Ŭ��->����] ĳ���� ���� ��û
#define PROTOCOL_CHARACTER_ENTER 0x2000000					    // [����Ʋ - ĳ���ͼ���ȭ��] [Ŭ��->����] ĳ���� ���� ��û
#define PROTOCOL_REQ_CHARACTER_SLOT 0x1000000				    // [����Ʋ - ĳ���ͼ���ȭ��] [Ŭ��->����] ĳ���� ���� ���� ����


// �ΰ��� �߰�Ʋ				
#define PROTOCOL_INGMAE_MENU 0x4000								// [�߰�Ʋ - �ΰ���] �Ŵ�����
#define PROTOCOL_INGAME_ANIMATION 0x2000						// [�߰�Ʋ - �ΰ���] �ִϸ��̼� ����
#define PROTOCOL_INGAME_DUNGEON 0x1000							// [�߰�Ʋ - �ΰ���] ��������

#define PROTOCOL_INGAME_PARTY 0x800								// [�߰�Ʋ - �ΰ���] ��Ƽ����
#define PROTOCOL_INGAME_CHANNEL 0x400							// [�߰�Ʋ - �ΰ���] ä�ΰ���
#define PROTOCOL_INGMAE_MONSTER 0x200							// [�߰�Ʋ - �ΰ���] ���Ͱ���
#define PROTOCOL_INGAME_CHARACER 0x100							// [�߰�Ʋ - �ΰ���] ĳ���Ͱ���


// �ΰ��� ����Ʋ(PROTOCOL_INGAME_CHARACER) - ĳ���� ����
#define PROTOCOL_CLIENT_INGAME_CHARACTER_COMPART 0x7F0000000	// [����Ʋ - ĳ���Ͱ���] [�������� �����]

#define PROTOCOL_INGAME_ATTACK_SUCCESS 0x400000000				// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] �����ߴµ� ���Ͷ� �ǰ������̳��Ͱ���.
#define PROTOCOL_INGAME_ATTACK 0x200000000						// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] �����ߴٴ� ����
#define PROTOCOL_INGAME_MOVE_END_JUMP 0x100000000				// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] ���� �����ٴ� ����(����)

#define PROTOCOL_INGAME_MOVE_START_JUMP 0x80000000				// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] ���� �����ߴٴ� ����
#define PROTOCOL_MOVE_ROTATION 0x40000000						// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] ȸ�� ����
#define PROTOCOL_MOVE_REPORT 0x20000000							// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] �̵� ����
#define PROTOCOL_REQ_OTHERPLAYERLIST 0x10000000					// [����Ʋ - ĳ���Ͱ���] [Ŭ��->����] �ٸ� ���� ��û

// �ΰ��� ����Ʋ(PROTOCOL_INGMAE_MONSTER) - ���� ����
#define PROTOCOL_CLIENT_INGAME_MONSTER_COMPART 0x70000000		// [����Ʋ - ���Ͱ���] [�������� �����]

#define PROTOCOL_REQ_MONSTER_ATTACK 0x40000000					// [����Ʋ - ���Ͱ���] [Ŭ��->����] ���� ������ �����ߴ�
#define PROTOCOL_MONSTER_MOVE 0x20000000						// [����Ʋ - ���Ͱ���] [Ŭ��->����] ���� �̵�
#define PROTOCOL_REQ_MONSTER_INFO 0x10000000					// [����Ʋ - ���Ͱ���] [Ŭ��->����] ���� ���� ��û

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_CHANNEL) - ä�� ����
#define PROTOCOL_CLIENT_INGAME_CHANNEL_COMPART 0x3000000		// [����Ʋ - ä�ΰ���] [�������� �����]

#define PROTOCOL_REQ_CHANNEL_CHANGE 0x2000000					// [����Ʋ - ä�ΰ���] [Ŭ��->����] ä�� �̵� ��û
#define PROTOCOL_REQ_CHANNEL_INFO 0x1000000						// [����Ʋ - ä�ΰ���] [Ŭ��->����] ä�� ���� ��û

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_PARTY) - ��Ƽ ����
#define PROTOCOL_CLIENT_INGAME_PARTY_COMPART 0x1F0000000		// [����Ʋ - ��Ƽ����] [�������� �����]

#define PROTOCOL_REQ_LEADER_DELEGATE 0x100000000				// [����Ʋ - ��Ƽ����] [Ŭ��->����] ��Ƽ�� ���� ��û

#define PROTOCOL_REQ_USER_LEAVE 0x80000000						// [����Ʋ - ��Ƽ����] [Ŭ��->����] ��Ƽ Ż�� ��û
#define PROTOCOL_REQ_USER_KICK 0x40000000						// [����Ʋ - ��Ƽ����] [Ŭ��->����] ��Ƽ�� ���� ��û
#define PROTOCOL_PARTY_ROOM_ANSWER_INVITE 0x20000000			// [����Ʋ - ��Ƽ����] [Ŭ��->����] ��Ƽ �ʴ� ����
#define PROTOCOL_REQ_PARTY_ROOM_INVITE 0x10000000				// [����Ʋ - ��Ƽ����] [Ŭ��->����] ��Ƽ �ʴ� ��û

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_DUNGEON) - ���� ����
#define PROTOCOL_CLIENT_INGAME_DUNGEON_COMPART 0x70000000		// [����Ʋ - ��������] [�������� �����]

#define PROTOCOL_DUNGEON_STAGE_IN 0x40000000					// [����Ʋ - ��������] [Ŭ��->����] �������� ���� ��û
#define PROTOCOL_REQ_DUNGEON_LEAVE 0x20000000					// [����Ʋ - ��������] [Ŭ��->����] ���� ������ ��û
#define PROTOCOL_REQ_DUNGEON_ENTER 0x10000000					// [����Ʋ - ��������] [Ŭ��->����] ���� ���� ��û

// �ΰ��� ����Ʋ(PROTOCOL_INGAME_ANIMATION) - �ִϸ��̼� ����
#define PROTOCOL_CLIENT_INGAME_ANIMATION_COMPART 0x7000000		// [����Ʋ - �ִϸ��̼ǰ���] [�������� �����]

#define PROTOCOL_CLIENT_MONSTER_ANIMATION_ATTACK 0x4000000		// [����Ʋ - �ִϸ��̼ǰ���] [Ŭ��->����] ���� �ִϸ��̼�(����)
#define PROTOCOL_CLIENT_MONSTER_ANIMATION 0x2000000			    // [����Ʋ - �ִϸ��̼ǰ���] [Ŭ��->����] ���� �ִϸ��̼�
#define PROTOCOL_CLIENT_CHARACER_ANIMATION 0x1000000		    // [����Ʋ - �ִϸ��̼ǰ���] [Ŭ��->����] ĳ���� �ִϸ��̼�

// �ΰ��� ����Ʋ(PROTOCOL_INGMAE_MENU) - �Ŵ� ����
#define PROTOCOL_CLIENT_INGAME_MENU_COMPART 0x7000000		// [����Ʋ - �Ŵ�����] [�������� �����]

#define PROTOCOL_MENU_EXIT 0x4000000						    // [����Ʋ - �Ŵ�����] [Ŭ��->����] ����
#define PROTOCOL_MENU_REQ_LOGOUT 0x2000000					    // [����Ʋ - �Ŵ�����] [Ŭ��->����] �α׾ƿ� ��û
#define PROTOCOL_MENU_REQ_CHARACTER 0x1000000				    // [����Ʋ - �Ŵ�����] [Ŭ��->����] ĳ���� ����ȭ������ ��û

// -- Ŭ�� END -- //

enum STATE {
	INIT_STATE, LOGIN_STATE, TENDER_STATE, JOIN_STATE, EXIT_STATE,
	LOG_IN,
	LOG_OUT,
	TENDER_INTRO,

	CLOSE = -1,
	RECV_LOGOUT_MENU_CHOICE = 0,
	RECV_LOGIN_MENU_CHOICE,
	SEND_JOIN,
	RECV_REQ_ID_OVERLAP_CHECK,
	SEND_ID_OVERLAP_CHECK,
	RECV_REQ_JOIN,
	SEND_JOIN_SUCCESS,
	SEND_LOGIN,
	RECV_REQ_LOGIN,
	SEND_LOGIN_SUCCESS,
	SEND_LOGIN_FAILED,
	SEND_LEAVE,
	SEND_LOGOUT,

	SEND_TENDER, //���� ����
	RECV_TENDER_MENU_CHOICE, //�����޴� ���� Ȯ��
	SEND_TENDER_CREATE, //������ǰ ���� ���� 
	SEND_TENDER_PARTICIPATE, //���� ���� ����
	SEND_BACK, //�ڷΰ���
	RECV_REQ_TENDER_CREATE,	 //������ǰ ���� ��û	 [��������][char* ��ǰ�̸�][int ����][int �����ڼ�]
	SEND_TENDER_CREATE_RESULT, //������ǰ ���� ��û ���
	RECV_REQ_TENDER_LIST, //���� ����Ʈ ��û
	SEND_TENDER_LIST_RESULT, //���� ����Ʈ ��û ���
	RECV_REQ_TENDER_PARTICIPATE, //���� ��û
	SEND_TENDER_PARTICIPATE_RESULT, //���� ��û ���
	SEND_TENDER_END_RESULT //���� ���� ��� [��������][char* �޼���]
};

enum RESULT {
	RT_DEFAULT = 0,
	// �α��θŴ��� ���
	RT_LOGINMENU = 100, RT_JOINMENU, RT_TENDER, RT_MEMBER_LEAVE, RT_LOGOUT, RT_USER_DISCONNECT,
	RT_ID_OVERLAP, RT_JOIN, RT_EXIT_JOIN, RT_LOGIN, RT_LOGINFAIL,

	RT_CHARACTER_SLOTRESULT = 200,
	RT_CHARACTER_ENTERGAME,					// ���� ����
	RT_CHARACTER_ENTERGAMEFAIL,				// ���� ���� ����
	RT_CHARACTER_DELETE,					// ĳ���� ����
	RT_CHARACTER_ENTERCREATE, 				// ĳ���� ���� ȭ������ �̵�
	RT_CHARACTER_CREATE,					// ĳ���� ���� 
	RT_CHARACTER_NICKOVERLAP_TRUE,			// �г��� �ߺ�
	RT_CHARACTER_EXIT,						// ���� ���

	RT_INGAME_MOVE = 300,					// �̵�
	RT_INGAME_OTHERPLAYER_INFO,				// �ٸ� �÷��̾� ����
	RT_INGAME_OTHERPLAYER_LIST,				// �ٸ� �÷��̾� ����Ʈ
	RT_INGAME_ATTACK,						// ����
	RT_INGAME_ATTACK_RESULT,				// ���ݼ���
	RT_INGAME_CHANNEL_INFO,					// ä�� ����
	RT_INGAME_CHANNEL_CHANGE,				// ä�� �̵�
	RT_INGAME_MENU_CHARACTER,				// ĳ���� ����ȭ��
	RT_INGAME_MENU_LOGOUT,					// �α׾ƿ�
	RT_INGAME_MENU_EXIT,					// ����
	RT_INGAME_PARTY_INVITE,					// ��Ƽ �ʴ�
	RT_INGAME_PARTY_INVITE_RESULT,			// ��Ƽ �ʴ� ���
	RT_INGAME_PARTY_USER_KICK_RESULT,		// ���� ���
	RT_INGAME_PARTY_USER_LEAVE_RESULT,		// Ż�� ���
	RT_INGAME_PARTY_LEADER_DELEGATE_RESULT,	// ��Ƽ�� ���� ���
	RT_INGAME_DUNGEON_ENTER_RESULT,			// ���� ���� ���
	RT_INGAME_DUNGEON_LEAVE_RESULT,			// ���� ���� ���
	RT_INGAME_DUNGEON_MONSTER_INFO_RESULT,	// ���� ���� ���� ���
	RT_INGAME_DUNGEON_STAGE_IN_RESULT,		// ���� �������� ���� ���
	RT_INGMAE_MONSTER_MOVE_RESULT,			// ���� �̵� ���
	RT_INGAME_MONSTER_ATTACK				// ���� ����
};

// �����ڵ�
enum CHARACTER_JOB
{
	TANKER = 1000,
	WARRIOR = 2000,
	MAGICIAN = 3000,
	GUNNER = 4000
};

// �����ڵ�
enum MONSTER_ORIGINCODE
{
	SPIDER = 10001,
	WORM = 10002,
	BOSS_SPIDER = 10003,
	DOG = 20001,
	ORCCANNONSOLDIER = 20002,
	KING_OF_THEAXE = 20003,
	BEAR = 30001,
	DINOSAUR = 30002,
	DRAGON = 30003
};
// ���� ���� �ڵ�
enum MONSTER_ATTACK_CODE
{
	SPIDER_FIRST_ATTACK = 10011,
	SPIDER_SECOND_ATTACK = 10021,
	WORM_FIRST_ATTACK = 10012,
	BOSS_SPIDER_FIRST_ATTACK = 10013,
	DOG_FIRST_ATTACK = 20011,
	ORCCANNONSOLDIER_FIRST_ATTACK = 20012,
	BEAR_FIRST_ATTACK = 30011,
	DINOSAUR_FIRST_ATTACK = 30012
};

enum IOTYPE {
	IOTYPE_ACCEPT = 200, IOTYPE_RECV, IOTYPE_SEND, IOTYPE_ONESIDED_SEND
};

enum DUNGEON_STAGE
{
	DEFAULT_STAGE = 0,
	DUNGEON_STAGE_NORMAL_1 = 10,
	DUNGEON_STAGE_BOSS_1,
	DUNGEON_STAGE_NORMAL_2 = 20,
	DUNGEON_STAGE_BOSS_2,
	DUNGEON_STAGE_NORMAL_3 = 30,
	DUNGEON_STAGE_BOSS_3,
	DUNGEON_STAGE_BOSS_4 = 40
};

struct WSAOVERLAPPEDEx {
	WSAOVERLAPPED overlapped;
	void* p;
	IOTYPE type;
};

#define USERCOUNT 200
#define CLIENTCOUNT 255
#define SERVERPORT 12345
#define BUFSIZE 512
#define JOBCODE_SIZE 4
#define NICKNAME_CODESIZE 6
#define IDSIZE 30
#define NICKNAMESIZE 20
#define CHARACTERCODESIZE 30
#define PRODUCT_NAMESIZE 50
#define CHARACTER_SPAWNPOS_MAXCOUNT 6
#define DUNGEON_SPAWNPOS_MAXCOUNT 4
#define MONSTER_SPAWNPOS_MAXCOUNT 3
#define NORMAL_MONSTER_SPAWNPOS_COUNT 2
#define BOSS_MONSTER_SPWANPOS_NUM 2
#define ENCRYPT_KEY 951324896

#define MAXCHARACTERORIGIN 4

// ���� ��������
#define MAXMONSTERORIGIN 8
// ���� ���� �ð�
#define MONSTERSPAWNTIME 1.5
// �׽�Ʈ �������� ���� ����
#define TESTSTAGE_MONSTER 1
// ù��° �������� ���� ����
#define FIRSTSTAGE_NORMALMONSTER_1 1
// �������� ����
#define STAGE_COUNT 4
// �������� �Ϲݸ� ����
#define STAGE_MONSTER_NUM 2
#define MOVETIME 0.02

// ���� �ð�
#define MONSTERTIME 0.5

// 360��
#define CIRCLE_THETA 360
//PI����
#define PI 3.14159265359
// ��������
#define DEG2RAD PI/180
// ��׸���
#define RAD2DEG 180/PI
// ��������
#define MARGIN_OF_ERROR 0.05


#endif