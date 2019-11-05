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
class CNumber;

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

	static CGame *Create();

	void Init();
	void Uninit();
	void Update();
	void Draw();

private:

	enum GAMESTATE
	{
		GAMESTATE_PREPARATION,		// ����
		GAMESTATE_NORMAL,
		GAMESTATE_MAX
	};

	void BulletTypeDisplay();
	void EnemySpring();

	CWord *m_pWord[BULLET_TYPE];

	CWord *m_pBg;
	CNumber *m_pNumber;

	GAMESTATE m_State;

	int nCount;
};

#endif