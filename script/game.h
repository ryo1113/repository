//======================================================================================================================
//
// �Q�[������ [game.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"

class CWord;

//======================================================================================================================
// �}�N����`
//======================================================================================================================
#define BULLET_TYPE			(2)

//======================================================================================================================
// �N���X��`
//======================================================================================================================
class CGame : public CModeBase
{
public:
	CGame() {}
	~CGame() {}

	typedef enum
	{//��ʏ��
		GAMESTATE_NONE = 0,			//�������Ă��Ȃ��Ƃ�
		GAMESTATE_NOMAL,			//�ʏ�
		GAMESTATE_PAUSE,			//�ꎞ��~
		GAMESTATE_END,
		GAMESTATE_MAX
	} GAMESTATE;

	static CGame *Create();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetGameState(GAMESTATE state);
	GAMESTATE GetGameState();

private:

	GAMESTATE g_GameState = GAMESTATE_MAX;

	CWord *m_pWord[BULLET_TYPE];

	int g_nCounterGameState = 0;
	float nCol��;

	int nCount;
};

#endif