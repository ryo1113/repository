//======================================================================================================================
//
// ゲーム処理 [game.h]
// Author:RYO KANDA
//
//======================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"

class CWord;

//======================================================================================================================
// マクロ定義
//======================================================================================================================
#define BULLET_TYPE			(2)

//======================================================================================================================
// クラス定義
//======================================================================================================================
class CGame : public CModeBase
{
public:
	CGame() {}
	~CGame() {}

	typedef enum
	{//画面状態
		GAMESTATE_NONE = 0,			//何もしていないとき
		GAMESTATE_NOMAL,			//通常
		GAMESTATE_PAUSE,			//一時停止
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
	float nColα;

	int nCount;
};

#endif