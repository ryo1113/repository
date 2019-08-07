//======================================================================================================================
//
// �Q�[������ [game.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"

class CScore;

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

	static void AddGameScore(int nScore);

	void SetGameState(GAMESTATE state);
	GAMESTATE GetGameState();

private:
	int random(int min, int max);

	static CScore *m_Score;

	GAMESTATE g_GameState = GAMESTATE_MAX;
	int g_nCounterGameState = 0;
	float nCol��;

	int nCount;
};

#endif